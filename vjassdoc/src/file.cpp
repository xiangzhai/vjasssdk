/***************************************************************************
 *   Copyright (C) 2008, 2009 by Tamino Dauth                              *
 *   tamino@cdauth.de                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <string>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <list>

#include "file.hpp"
#include "vjassdoc.hpp"
#include "objects.hpp"
#include "parser.hpp"
#include "internationalisation.hpp"
#include "syntaxerror.hpp"
#include "utilities.hpp"

namespace vjassdoc
{

const char *File::expressionText[] =
{
	//Jass stand-alone expressions
	"///", //This tool
	"//",
	"/**",
	"*/",
	"type",
	"constant",
	"native",
	"function",
	"endfunction",
	"globals",
	"endglobals",
	"local",
	"set",
	"call",
	"if",
	"else",
	"elseif",
	"endif",
	"return",
	"loop",
	"endloop",
	"exitwhen",
	//vJass stand-alone expressions
	"//!",
	"method", //Operator is not required because the syntax is "method operator..."
	"endmethod", //Required for function blocks, there can not be custom types
	"private",
	"public",
	"static",
	"debug",
	"keyword",
	"library",
	"library_once",
	"endlibrary",
	"scope",
	"endscope",
	"struct",
	"endstruct",
	"interface",
	"endinterface",
	"delegate",
	"stub",
	"module",
	"endmodule",
	"implement",
	"hook",
	"/*",
	"*/",
	"key",
	//Jass none-start-line expressions
	"nothing",
	"takes",
	"returns",
	"extends",
	"array",
	//vJass none-start-line expressions
	"import",
	"dovjassinit",
	"inject",
	"endinject",
	"novjass",
	"endnovjass",
	"loaddata",
	"external",
	"textmacro",
	"textmacro_once",
	"endtextmacro",
	"runtextmacro", //An instance of the macro
	"requires",
	"needs",
	"uses",
	"initializer",
	"defaults",
	"optional",
	"super",
	"thistype",
	"operator"
};

File::File(const std::string &filePath) : filePath(filePath), notRequiredSpace(File::InvalidExpression), isInGlobals(false), isInLibrary(false), isInScope(false), isInInterface(false), isInStruct(false), isInModule(false), isInBlockComment(false), isInBlockDocComment(false), currentLine(0), currentDocComment(0), currentLibrary(0), currentScope(0), currentInterface(0), currentStruct(0), currentModule(0), currentFunction(0), gotDocComment(false)
{
	fin.open(filePath.c_str());

	if (!fin.good())
	{
		fin.close();
		fprintf(stderr, _("Was unable to open file %s.\n"), filePath.c_str());
		
		return;
	}

	std::string line, token, output;
	unsigned int index;
	File::Expression expression;
	
	//ignore spaces and tabs before expressions!
	while (std::getline(this->fin, line))
	{
		++this->currentLine;
		
		if (Vjassdoc::optionVerbose())
			printf(_("Reading line %d of file %s.\n"), this->currentLine, filePath.c_str());

		index = 0; //reset index!
		expression = this->getFirstLineExpression(line, index);

		if (expression == File::NoExpression || expression == File::InvalidExpression || expression == File::DebugExpression || expression == File::CustomExpression)
			continue;
	
		switch (expression)
		{
			//optional
			case DocCommentExpression:
				this->getDocComment(line, index);
				break;

			case CommentExpression:
			{
				this->truncateComments(line, index);
				std::string identifier = getToken(line, index, true);
			
				if (!identifier.empty())
					Vjassdoc::parser()->add(new Comment(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment));
				
				break;
			}

			case BlockCommentExpression:
			{
				std::string::size_type position = std::string::npos; 

				if (index < line.length())
					position = line.find(expressionText[EndBlockCommentExpression], index);

				if (position == std::string::npos)
				{
					this->isInBlockComment = true;
					
					if (index < line.length())
						this->currentBlockComment = line.substr(index);
				}
				else
				{
					Vjassdoc::parser()->add(new class Comment(line.substr(index), Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment));
					this->clearDocComment();
				}

				break;
			}

			case BlockDocCommentExpression:
			{
				std::string::size_type position = std::string::npos; 

				if (index < line.length())
					position = line.find(expressionText[EndBlockDocCommentExpression], index);

				if (position == std::string::npos)
				{
					this->isInBlockDocComment = true;
					
					if (index < line.length())
						this->currentBlockDocComment = line.substr(index);
				}
				else
				{
					if (this->currentDocComment == 0)
					{
						this->gotDocComment = true;
						this->currentDocComment = new class DocComment(line.substr(index), Vjassdoc::parser()->currentSourceFile(), this->currentLine);
						Vjassdoc::parser()->add(this->currentDocComment);
					}
					else
						this->currentDocComment->addIdentifier(line.substr(index));
				}

				break;
			}

			case KeywordExpression:
				this->truncateComments(line, index);
				this->getKeyword(line, index, false);
				break;

			case KeyExpression:
				this->truncateComments(line, index);
				this->getKey(line, index, false);
				break;

			case PreprocessorExpression:
			{
				this->truncateComments(line, index);
				std::string preprocessor = getToken(line, index);
				
				if (preprocessor == expressionText[ImportExpression])
				{
					std::string filePath = getToken(line, index, true);
					filePath = filePath.substr(1, filePath.length() - 2);
					std::string openPath;
					std::ifstream fin;
					bool result = false;
					std::list<std::string> list = Vjassdoc::optionImport();
					std::list<std::string>::const_iterator iterator = list.begin();
					
					while (iterator != list.end())
					{
						openPath = *iterator;
						std::size_t length = strlen(dirSeparator);
						
						if (strcmp(openPath.substr(openPath.length() - length, length).c_str(), dirSeparator) != 0)
							openPath += dirSeparator;
						
						openPath += filePath;
						fin.open(openPath.c_str(), std::ios_base::ate);

						if (fin.good())
						{
							result = true;
							fin.close(); //TODO Just close if good?
							break;
						}

						++iterator;
					}

					if (!result)
					{
						fin.open(filePath.c_str(), std::ios_base::ate);

						if (fin.good())
							fin.close(); /// @todo Just close if good?
						else
						{
							fprintf(stderr, _("Was unable to import file %s.\n"), filePath.c_str());
							continue;
						}
					}

					if (result)
					{
						if (Vjassdoc::optionVerbose())
							printf(_("Importing file %s.\n"), openPath.c_str());
						
						Vjassdoc::parser()->add(new SourceFile(filePath, openPath));
						this->clearDocComment();
					}
					else
					{
						if (Vjassdoc::optionVerbose())
							printf(_("Importing file %s.\n"), filePath.c_str());
						
						Vjassdoc::parser()->add(new SourceFile(filePath, filePath));
						this->clearDocComment();
					}
				}
				else if (preprocessor == expressionText[TextmacroExpression])
					this->getTextMacro(line, index, false);
				else if (preprocessor == expressionText[TextmacroonceExpression])
					this->getTextMacro(line, index, true);
				else if (preprocessor == expressionText[RuntextmacroExpression])
					this->getTextMacroInstance(line, index);
				//NOTE Syntax checker!
				else if (Vjassdoc::optionSyntax() && preprocessor != expressionText[DovjassinitExpression] && preprocessor != expressionText[InjectExpression] && preprocessor != expressionText[EndinjectExpression] && preprocessor != expressionText[NovjassExpression] && preprocessor != expressionText[EndnovjassExpression] && preprocessor != expressionText[LoaddataExpression] && preprocessor != expressionText[ExternalExpression])
				{
					char message[255];

					if (!preprocessor.empty())
						sprintf(message, _("Unknown preprocessor statement: \"%s\"."), preprocessor.c_str());
					else
						sprintf(message, _("Missing preprocessor statement."));

					Vjassdoc::parser()->add(new SyntaxError(Vjassdoc::parser()->currentSourceFile(), this->currentLine, message));
				}
				
				break;
			}

			//Example: type MyType extends integer array[10]
			//Output: Typ: MyType - Enthaltener Typ: integer - Größe: array[10]
			case TypeExpression:
			{
				this->truncateComments(line, index);
				std::string identifier = getToken(line, index);

				if (Vjassdoc::optionSyntax() && identifier.empty())
				{
					char message[255];
					sprintf(message, _("Missing type identifier."));
					Vjassdoc::parser()->add(new SyntaxError(Vjassdoc::parser()->currentSourceFile(), this->currentLine, message));
				}

				std::string parentTypeIdentifier;
				parentTypeIdentifier = getToken(line, index);
				
				if (parentTypeIdentifier == expressionText[ExtendsExpression])
					parentTypeIdentifier = getToken(line, index);
				else if (Vjassdoc::optionSyntax() && !parentTypeIdentifier.empty())
				{
					char message[255];
					sprintf(message, _("Unknown expression: \"%s\". Expected \"extends\"."), parentTypeIdentifier.c_str());
					Vjassdoc::parser()->add(new SyntaxError(Vjassdoc::parser()->currentSourceFile(), this->currentLine, message));
				}
				
				std::string size = getToken(line, index);
				
				if (!size.empty())
				{
					bool getSize = true;

					if (size == File::expressionText[File::ArrayExpression]) //expression: type TypeName extends array [10]
						size = getToken(line, index);
	
					std::string::size_type start = size.find('[') + 1;

					if (Vjassdoc::optionSyntax() && start == std::string::npos)
					{
						char message[255];
						sprintf(message, _("Missing \"[\" expression."));
						Vjassdoc::parser()->add(new SyntaxError(Vjassdoc::parser()->currentSourceFile(), this->currentLine, message));
						getSize = false;
					}

					std::string::size_type length = size.find(']') - start;

					if (Vjassdoc::optionSyntax() && length == std::string::npos)
					{
						char message[255];
						sprintf(message, _("Missing \"]\" expression."));
						Vjassdoc::parser()->add(new SyntaxError(Vjassdoc::parser()->currentSourceFile(), this->currentLine, message));
						getSize = false;
					}

					if (getSize) //can only be false when syntax checker is enabled
						size = size.substr(start, length);
				}

				Vjassdoc::parser()->add(new Type(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, parentTypeIdentifier, size));
				this->clearDocComment();
				break;
			}

			case ConstantExpression:
			{
				this->truncateComments(line, index);
				unsigned int lastIndex = index;
				
				if (!this->getFunction(line, index, false, false, true, false, false, false))
					this->getGlobal(line, lastIndex, false, false, true, false, false);

				break;
			}

			case NativeExpression:
				this->truncateComments(line, index);
				this->getFunction(line, index, false, false, false, true, false, false);
				break;

			case FunctionExpression:
				this->truncateComments(line, index);
				this->getFunction(line, index, false, false, false, false, false, false);
				break;

			case EndfunctionExpression:
				this->currentFunction = 0;
				break;

			case MethodExpression:
				this->truncateComments(line, index);
				this->getFunction(line, index, false, false, false, false, false, false);
				break;

			case EndmethodExpression:
				this->currentFunction = 0;
				break;

			/// @todo Add static if support, should be evaluated during parse process (ignore code between static ifs, if variable is false).
			case StaticExpression:
			{
				this->truncateComments(line, index);
				unsigned int lastIndex = index;
				
				if (!this->getFunction(line, index, false, false, false, false, true, false))
					this->getGlobal(line, lastIndex, false, false, false, true, false);

				break;
			}

			case PublicExpression:
			{
				this->truncateComments(line, index);
				unsigned int lastIndex = index;
				std::string token = getToken(line, index);

				if (token == expressionText[KeyExpression])
				{
					this->getKey(line, index, false);
					break;
				}

				index = lastIndex;

				if (!this->getFunction(line, index, false, true, false, false, false, false))
					this->getGlobal(line, lastIndex, false, true, false, false, false);

				break;
			}

			//Private objects won't be parsed
			case PrivateExpression:
			{
				unsigned int lastIndex = index;
				token = getToken(line, index);

				if (Vjassdoc::optionSyntax() && token.empty())
				{
					char message[255];
					sprintf(message, _("Missing expression after \"private\" keyword."));
					Vjassdoc::parser()->add(new SyntaxError(Vjassdoc::parser()->currentSourceFile(), this->currentLine, message));
				}

				if (!Vjassdoc::optionPrivate())
				{
					//just possible if it is a method
					if (this->isInStruct && token == expressionText[StaticExpression])
						token = getToken(line, index);
				
					if (token == expressionText[FunctionExpression])
						this->notRequiredSpace = FunctionExpression;
					else if (token == expressionText[MethodExpression])
						this->notRequiredSpace = MethodExpression;
					else if (token == expressionText[ScopeExpression])
						this->notRequiredSpace = ScopeExpression;
					else if (token == expressionText[InterfaceExpression])
						this->notRequiredSpace = InterfaceExpression;
					else if (token == expressionText[StructExpression])
						this->notRequiredSpace = StructExpression;
					
					if (Vjassdoc::optionVerbose())
						printf(_("Starting with not required space. Reason: %d.\n"), this->notRequiredSpace);
					
					//as there will be an object let the program clear the current doc comment
					this->clearDocComment();
				}
				else
				{
					this->truncateComments(line, index);
					
					if (token == expressionText[KeywordExpression])
						this->getKeyword(line, index, true);
					if (token == expressionText[KeyExpression])
						this->getKey(line, index, true);
					else if (token == expressionText[ScopeExpression])
						this->getScope(line, index, true);
					else if (token == expressionText[InterfaceExpression])
						this->getInterface(line, index, true);
					else if (token == expressionText[StructExpression])
						this->getStruct(line, index, true);
					else if (token == expressionText[ModuleExpression])
						this->getModule(line, index, true);
					else
					{
						unsigned int secondLastIndex = lastIndex;
				
						if (!this->getFunction(line, lastIndex, true, false, false, false, false, false))
							this->getGlobal(line, secondLastIndex, true, false, false, false, false);
					}
				}
				
				break;
			}

			case GlobalsExpression:
				this->isInGlobals = true;
				break;
	
			case EndglobalsExpression:
				this->isInGlobals = false;
				break;

			case LocalExpression:
				this->truncateComments(line, index);
				this->getLocal(line, index);
				break;

			case SetExpression:
				std::cout << "Set expression" << std::endl;
				break;

			case CallExpression:
				std::cout << "call expression" << std::endl;
				break;

			case IfExpression:
				std::cout << "if expression" << std::endl;
				break;

			case ElseExpression:
				std::cout << "else expression" << std::endl;
				break;

			case ElseifExpression:
				std::cout << "elseif expression" << std::endl;
				break;

			case EndifExpression:
				std::cout << "endif expression" << std::endl;
				break;

			case ReturnExpression:
				std::cout << "return expression" << std::endl;
				break;

			case LoopExpression:
				std::cout << "loop expression" << std::endl;
				break;

			case EndloopExpression:
				std::cout << "endloop expression" << std::endl;
				break;

			case ExitwhenExpression:
				std::cout << "exitwhen expression" << std::endl;
				break;

			case LibraryExpression:
				this->truncateComments(line, index);
				this->getLibrary(line, index, false);
				break;

			case Library_onceExpression:
				this->truncateComments(line, index);
				this->getLibrary(line, index, true);
				break;

			case EndlibraryExpression:
				this->isInLibrary = false;
				this->currentLibrary = 0;
				break;
	
			case ScopeExpression:
				this->truncateComments(line, index);
				this->getScope(line, index, false);
				break;

			case EndscopeExpression:
				this->isInScope = false;
				this->currentScope = 0;
				break;

			case InterfaceExpression:
				this->truncateComments(line, index);
				this->getInterface(line, index, false);
				break;

			case EndinterfaceExpression:
				this->isInInterface = false;
				this->currentInterface = 0;
				break;

			case StructExpression:
				this->truncateComments(line, index);
				this->getStruct(line, index, false);
				break;

			case EndstructExpression:
				this->isInStruct = false;
				this->currentStruct = 0;
				break;

			//new
			case ModuleExpression:
				this->truncateComments(line, index);
				this->getModule(line, index, false);
				break;

			//new
			case EndmoduleExpression:
				this->isInModule = false;
				this->currentModule = 0;
				break;

			//new
			//Has to be a struct/interface member.
			//Should be handled as member.
			case DelegateExpression:
				this->truncateComments(line, index);
				this->getGlobal(line, index, false, false, false, false, true);
				break;

			case StubExpression:
				this->truncateComments(line, index);
				this->getFunction(line, index, false, false, false, false, false, true);
				break;

			case ImplementExpression:
				this->truncateComments(line, index);
				this->getImplementation(line, index);
				break;

			case HookExpression:
				this->truncateComments(line, index),
				this->getHook(line, index);
		}
	}

	Vjassdoc::addLines(this->currentLine);
	Vjassdoc::addFile();
	printf(_("Parsed file %s successfully (number %d, %d lines).\n"), filePath.c_str(), Vjassdoc::files(), this->currentLine);
	fin.close();
}

File::Expression File::getFirstLineExpression(std::string &line, unsigned int &index)
{
	if (this->isInBlockComment)
	{
		std::string::size_type position = line.find(expressionText[EndBlockCommentExpression]);

		if (position == std::string::npos)
		{
			this->currentBlockComment += line;
			return File::NoExpression; //stop parsing
		}

		this->currentBlockComment += line.substr(0, position);
		Vjassdoc::parser()->add(new class Comment(this->currentBlockComment, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment));
		this->currentBlockComment.clear();
		this->isInBlockComment = false;
		index = position + 2;
	}
	else if (this->isInBlockDocComment)
	{
		std::string::size_type position = line.find(expressionText[EndBlockDocCommentExpression]);
		std::string firstToken = getToken(line, index);

		if (position == std::string::npos)
		{
			if (firstToken == "*") // * character is truncated.
				this->currentBlockDocComment += line.erase(0, index);
			else
				this->currentBlockDocComment += line;

			return File::NoExpression; //stop parsing
		}

		if (firstToken == "*" && index <= position) // * character is truncated.
			this->currentBlockDocComment += line.erase(0, index).substr(0, position);
		else
			this->currentBlockDocComment += line.substr(0, position);

		if (this->currentDocComment == 0)
		{
			this->gotDocComment = true;
			this->currentDocComment = new class DocComment(this->currentBlockDocComment, Vjassdoc::parser()->currentSourceFile(), this->currentLine);
			Vjassdoc::parser()->add(this->currentDocComment);
		}
		else
			this->currentDocComment->addIdentifier(this->currentBlockDocComment);

		this->currentBlockDocComment.clear();
		this->isInBlockDocComment = false;
		index = position + 2;
	}

	std::string token = getToken(line, index);

	if (token.empty())
	{
		if (Vjassdoc::optionVerbose())
			std::cout << _("Empty line, ignoring line.") << std::endl;
			
		return File::NoExpression;
	}

	if (!Vjassdoc::optionJass() && token == expressionText[File::DebugExpression])
	{
		if (Vjassdoc::optionDebug())
			getToken(line, index); //cut the keyword
		else
		{
			if (Vjassdoc::optionVerbose())
				std::cout << _("Debug statement, ignoring line.") << std::endl;
			
			return File::DebugExpression;
		}
	}

	//Function and text macro blocks won't be parsed
	if (this->notRequiredSpace != File::InvalidExpression)
	{
		if ((this->notRequiredSpace == File::FunctionExpression && token == expressionText[File::EndfunctionExpression]) ||
			(this->notRequiredSpace == File::MethodExpression && token == expressionText[File::EndmethodExpression]) ||
			(this->notRequiredSpace == File::TextmacroExpression && token == expressionText[File::PreprocessorExpression] && getToken(line, index) == expressionText[File::EndtextmacroExpression]) || /// @todo Truncate the //! prefix
			(this->notRequiredSpace == File::ScopeExpression && token == expressionText[File::EndscopeExpression]) ||
			(this->notRequiredSpace == File::InterfaceExpression && token == expressionText[File::EndinterfaceExpression]) ||
			(this->notRequiredSpace == File::StructExpression && token == expressionText[File::EndstructExpression]) || (this->notRequiredSpace == File::ModuleExpression && token == File::expressionText[File::EndmoduleExpression]))
		{
			if (Vjassdoc::optionVerbose())
				printf(_("Not required space. Reason: %d.\n"), this->notRequiredSpace);

			this->notRequiredSpace = File::InvalidExpression;
		}
		
		return File::NoExpression; //stop parsing, ignore comments and documentation comments because it is not required space
	}

	unsigned int maxExpressions = NothingExpression; //maximum stand-alone vJass expression

	if (Vjassdoc::optionJass())
		maxExpressions = PreprocessorExpression; // Spart Speicher, da die Schleife nur alle Jass Schlüsselwörter durchläuft.
	
	for (unsigned int expression = File::DocCommentExpression; expression < maxExpressions; ++expression)
	{
		if (token == expressionText[expression])
			return File::Expression(expression);
	}

	if (token.substr(0, 3) == expressionText[File::DocCommentExpression])
	{
		this->getDocComment(line, index - token.length() + 3);
		return File::DocCommentExpression;
	}
	
	if (token.substr(0, 2) == expressionText[CommentExpression])
	{
		index = 0;
		//token = this->getIdentifier(getToken(line, index, true).substr(2, line.length()));
		token = getToken(line, index, true).substr(2, line.length());
		Vjassdoc::parser()->add(new class Comment(token, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment));
		this->clearDocComment();
		return File::CommentExpression;
	}
	
	this->truncateComments(line, index);
	index = 0; //Restart, the getGlobal method searches for the type

	if (this->getGlobal(line, index, false, false, false, false, false))
	{
		this->clearDocComment();
		return File::CustomExpression;
	}

	char message[255];
	sprintf(message, _("Unknown expression: \"%s\"."), line.substr(index).c_str());

	if (Vjassdoc::optionVerbose())
		std::cerr << message << std::endl;

	if (Vjassdoc::optionSyntax())
	{
		Vjassdoc::parser()->add(new SyntaxError(Vjassdoc::parser()->currentSourceFile(), this->currentLine, message));
	}

	return File::InvalidExpression;
}

void File::truncateComments(std::string &line, unsigned int index)
{
	bool isInString = false;
	
	while (index < line.length())
	{
		if (line[index] == '\"')
		{
			isInString = !isInString;
			
			if (Vjassdoc::optionVerbose())
			{
				if (isInString)
					std::cout << _("Detected \" character. Ignoring // and /// expressions.") << std::endl;
				else
					std::cout << _("Detected \" character. Do not ignore // and /// expressions anymore.") << std::endl;
			}
		}
		else if (!isInString)
		{
			if (index < line.length() - 3 && line.substr(index, 3) == expressionText[DocCommentExpression])
			{
				index += 3;
				this->getDocComment(line, index);
				
				break;
			}
			else if (index < line.length() - 2 && line.substr(index, 2) == expressionText[CommentExpression])
			{
				std::string token = line.substr(index + 2, line.length() - (index + 2));
			
				if (!token.empty())
				{
					Vjassdoc::parser()->add(new Comment(token, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment));
					line.erase(index - 1);
				}
				else if (Vjassdoc::optionVerbose())
					std::cout << _("Detected empty comment.") << std::endl;
				
				break;
			}
			else if (index < line.length() - 3 && line.substr(index, 3) == expressionText[BlockDocCommentExpression] && !(index < line.length() - 4 && line[index + 3] == '/')) //exception: /**/
			{
				bool result = false;
				std::size_t i;

				for (i = index + 2; i < line.length() - 1; ++i)
				{
					if (line.substr(i, 2) == expressionText[EndBlockDocCommentExpression])
					{
						result = true;
						break;
					}
				}

				if (!result)
				{
					this->currentBlockDocComment = line.substr(index + 3);
					this->isInBlockDocComment = true;
					break;
				}
				else
				{
					std::string identifier = line.substr(index + 3, i - index - 3);
					
					if (!identifier.empty())
					{
						if (this->currentDocComment == 0)
						{
							this->gotDocComment = true;
							this->currentDocComment = new class DocComment(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine);
							Vjassdoc::parser()->add(this->currentDocComment);
						}
						else
							this->currentDocComment->addIdentifier(identifier);
					}
					
					line.erase(index, i - index + 3);
					index = i - identifier.length() + 3; //-2
				}
			}
			else if (index < line.length() - 2 && line.substr(index, 2) == expressionText[BlockCommentExpression])
			{
				bool result = false;
				std::size_t i;

				for (i = index + 2; i < line.length() - 1; ++i)
				{
					if (line.substr(i, 2) == expressionText[EndBlockCommentExpression])
					{
						result = true;
						break;
					}
				}

				if (!result)
				{
					this->currentBlockComment = line.substr(index + 2);
					this->isInBlockComment = true;
					break;
				}
				else
				{
					std::string identifier = line.substr(index + 2, i - index - 2);
					
					if (!identifier.empty())
					{
						Vjassdoc::parser()->add(new class Comment(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment));
						this->clearDocComment();
					}
					
					line.erase(index, i - index + 2);
					index = i - identifier.length() + 2; //-2
				}
			}
		}
		
		++index;
	}
}

void File::getDocComment(const std::string &line, unsigned int index)
{
	std::string docCommentItem = getToken(line, index, true);

	if (!docCommentItem.empty())
	{
		if (this->currentDocComment == 0)
		{
			this->currentDocComment = new DocComment(docCommentItem, Vjassdoc::parser()->currentSourceFile(), this->currentLine);
			Vjassdoc::parser()->add(this->currentDocComment);
			this->gotDocComment = true;
		}
		else
			this->currentDocComment->addIdentifier('\n' + docCommentItem); //parse before and modify, space characters are important!
	}
	else if (Vjassdoc::optionVerbose())
		std::cout << _("Detected empty documentation comment.") << std::endl;
}

void File::clearDocComment()
{
	if (this->gotDocComment)
	{
		this->currentDocComment = 0; //reset
		this->gotDocComment = false;
	}
}

inline void File::getKeyword(const std::string &line, unsigned int &index, bool isPrivate)
{
	Vjassdoc::parser()->add(new Keyword(getToken(line, index), Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate));
}

inline void File::getKey(const std::string &line, unsigned int &index, bool isPrivate)
{
	Vjassdoc::parser()->add(new Key(getToken(line, index), Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate));
}

bool File::getGlobal(const std::string &line, unsigned int &index, bool isPrivate, bool isPublic, bool isConstant, bool isStatic, bool isDelegate)
{
	if (!this->isInGlobals && (Vjassdoc::optionJass() || (!Vjassdoc::optionJass() && !this->isInGlobals && !this->isInInterface && !this->isInStruct)))
			return false;

	std::string identifier = getToken(line, index);
	
	if (!Vjassdoc::optionJass() && this->isInVjassBlock())
	{
		/*
		If the member is public it HAS to start with the public keyword
		//Do not list private members
		if (identifier == expressionText[PrivateExpression])
			return false;
	
		//If there is not postfix the member is public
		if (identifier == expressionText[PublicExpression])
		{
			identifier = getToken(line, index);
			newType = true;
		}
		*/
	
		if ((this->isInInterface || this->isInStruct) && identifier == expressionText[StaticExpression])
		{
			//NOTE Syntax checker
			/*
			if (isStatic)
			{
				if (Vjassdoc::showVerbose())
					std::cerr << _("Member was declared as static at twice.") << std::endl;
				
				return false;
			}
			*/
	
			identifier = getToken(line, index);
			isStatic = true;
		}
	
		if (identifier == expressionText[ConstantExpression])
		{
			//NOTE Syntax checker
			/*
			if ((this->isInInterface || this->isInStruct) && !isStatic)
			{
				if (Vjassdoc::showVerbose())
					std::cerr << _("Constant members must be static.") << std::endl;
				
				return false;
			}
			*/
			
			//NOTE Syntax checker
			/*
			else if (isDelegate)
			{
				if (Vjassdoc::showVerbose())
					std::cerr << _("Delegates can not be constant.") << std::endl;
					
				return false;
			}
			*/
	
			isConstant = true;
			identifier = getToken(line, index);
		}
	}

	std::string type = identifier;
	identifier = getToken(line, index);
	bool isArray = false;
	
	//Jass does not support constant arrays
	if (!isConstant && identifier == expressionText[ArrayExpression])
	{
		isArray = true;
		identifier = getToken(line, index);
	}

	std::string size;
	
	if (isArray)
	{
		std::size_t position = identifier.find('[', 0);

		if (position != std::string::npos)
		{
			std::size_t length = identifier.find(']', position);

			if (length != std::string::npos)
			{
				length = (length - position) + 1;
				size = identifier.substr(position, length);
				identifier = identifier.substr(0, position); //cut the [size] part of the identifier
			}
		}
	}

	std::string value;
	
	//Jass does not support array initialization
	if (!isArray && getToken(line, index) == "=")
		value = getToken(line, index, true); //End of line for function calls

	if (Vjassdoc::optionJass() || (!this->isInStruct && !this->isInInterface))
		Vjassdoc::parser()->add(new Global(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate, isPublic, isConstant, type, value, size));
	else
	{
		Vjassdoc::parser()->add(new Member(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate, isPublic, isConstant, type, value, size, this->getCurrentContainer(), isStatic, isDelegate));
	}
	
	this->clearDocComment();
	return true;
}

bool File::getFunction(const std::string &line, unsigned int &index, bool isPrivate, bool isPublic, bool isConstant, bool isNative, bool isStatic, bool isStub)
{
	std::string identifier = getToken(line, index);
	bool isOperator = false;

	if (!Vjassdoc::optionJass() && this->isInVjassBlock())
	{
		/*
		If the method is public it HAS to start with the public keyword, so the parameter will be set.
		if (identifier == expressionText[PrivateExpression])
		{
			if (Vjassdoc::showVerbose())
				std::cout << "Ignoring private method." << std::endl;
			
			return false;
		}
		
		if (identifier == expressionText[PublicExpression])
		{
			hasPrefix = true;
			identifier = getToken(line, index);
			isPublic = true;
		}
		*/
		
		bool hasPrefix = false;
	
		if (!isStatic && (this->isInStruct || this->isInInterface || this->isInModule) && identifier == expressionText[StaticExpression])
		{
			hasPrefix = true;
			isStatic = true;
			identifier = getToken(line, index);
		}

		if (!isStub && (this->isInStruct || this->isInInterface || this->isInModule) && identifier == expressionText[StubExpression]) //no statics and before constant?
		{
			hasPrefix = true;
			isStub = true;
			identifier = getToken(line, index);
		}

		if (!isConstant && identifier == expressionText[ConstantExpression])
		{
			hasPrefix = true;
			isConstant = true;
			identifier = getToken(line, index);
		}
	
		if ((this->isInStruct || this->isInInterface || this->isInModule) && identifier == expressionText[MethodExpression])
			hasPrefix = true;
	
		if (hasPrefix)
		{
			//no syntax checker, required for global identification
			if ((this->isInStruct || this->isInInterface || this->isInModule) && identifier != expressionText[MethodExpression])
				return false;
			else if (!this->isInStruct && !this->isInInterface && !this->isInModule && identifier != expressionText[FunctionExpression])
				return false;
	
			/*
			If the method is private it has to start with private.
			if (isPrivate)
			{
				this->notRequiredSpace = Function;
				std::cout << "Not required space starts with token " << identifier << '.' << std::endl;
				return true;
			}
			*/

			identifier = getToken(line, index);
		}

		if (identifier == File::expressionText[File::OperatorExpression])
		{
			isOperator = true;
			identifier = getToken(line, index);
		}
	}

	bool isFunctionInterface = false;
	
	if (!Vjassdoc::optionJass() && identifier == expressionText[InterfaceExpression])
	{
		isFunctionInterface = true;
		identifier = getToken(line, index);
	}
	//constant native
	else if (isConstant && !isNative && identifier == expressionText[NativeExpression])
	{
		isNative = true;
		identifier = getToken(line, index);
	}
	else if ((isConstant || isPrivate || isPublic) && identifier == expressionText[FunctionExpression])
		identifier = getToken(line, index); //get the name

	//no syntax checker, required for global identification
	if (getToken(line, index) != expressionText[TakesExpression])
		return false;

	std::list<class Parameter*> parameters;
	std::string typeExpression = getToken(line, index);

	if (typeExpression != expressionText[NothingExpression])
	{
		while (typeExpression != expressionText[ReturnsExpression])
		{
			std::string identifier = getToken(line, index);

			/// @todo Text macros are uncheckable :-/
			if (typeExpression[0] == '$' && identifier == expressionText[ReturnsExpression]) //takes $BLA$ returns integer
				break;

			std::string oldIdentifier = identifier;
			std::string::size_type position = identifier.find(',');
			
			if (position != std::string::npos) //takes integer x,integer ... or takes integer x, integer ...
				identifier.erase(position);

			class Parameter *parameter = new class Parameter(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, 0, typeExpression);
			parameters.push_back(parameter);
			Vjassdoc::parser()->add(parameter);

			if (position == std::string::npos || position == oldIdentifier.length() - 1)
			{
				typeExpression = getToken(line, index);

				if (typeExpression == ",") //takes integer x , real y ...
					typeExpression = getToken(line, index);
			}
			else
				typeExpression = oldIdentifier.substr(position + 1);
		}
	}

	std::string type = getToken(line, index);
	
	if (type == expressionText[NothingExpression])
		type.clear();

	if (!isFunctionInterface)
	{
		if (Vjassdoc::optionJass() || (!this->isInStruct && !this->isInInterface && !this->isInModule))
		{
			if (!Vjassdoc::optionFunctions() && !isNative)
			{
				if (Vjassdoc::optionVerbose())
					std::cout << _("Ignoring function space.") << std::endl;

				this->notRequiredSpace = FunctionExpression;
			}

			class Function *function = new Function(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate, parameters, type, isPublic, isConstant, isNative);
			Vjassdoc::parser()->add(function);
			this->currentFunction = function;
		}
		else
		{
			std::string defaultReturnValueExpression;

			//Do not expect endmethod in interfaces!
			if (!Vjassdoc::optionFunctions() && !this->isInInterface)
			{
				if (Vjassdoc::optionVerbose())
					std::cout << _("Ignoring method space.") << std::endl;

				this->notRequiredSpace = MethodExpression;
			}
			else
			{
				defaultReturnValueExpression = getToken(line, index);
				
				if (defaultReturnValueExpression == File::expressionText[File::DefaultsExpression])
					defaultReturnValueExpression = getToken(line, index);
				
				if (Vjassdoc::optionVerbose() && defaultReturnValueExpression.empty())
					std::cerr << _("Missing default return value expression.") << std::endl;
			}

			class Method *method = new Method(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate, parameters, type, isPublic, isConstant, this->getCurrentContainer(), isStatic, isStub, isOperator, defaultReturnValueExpression);
			Vjassdoc::parser()->add(method);
			
			if (!this->isInInterface) //interfaces have single line methods without endmethod
				this->currentFunction = method;
		}
	}
	else
		Vjassdoc::parser()->add(new FunctionInterface(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate, parameters, type));

	this->clearDocComment();
	
	return true;
}


void File::getImplementation(const std::string &line, unsigned int &index)
{
	bool isOptional = false;
	std::string identifier = getToken(line, index);

	if (identifier == File::expressionText[File::OptionalExpression])
	{
		isOptional = true;
		identifier = getToken(line, index);
	}

	Vjassdoc::parser()->add(new Implementation(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->getCurrentContainer(), identifier, isOptional));
}

void File::getHook(const std::string &line, unsigned int &index)
{
	std::string functionExpression = getToken(line, index);
	std::string hookFunctionExpression = getToken(line, index);

	Vjassdoc::parser()->add(new Hook(functionExpression, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, functionExpression, hookFunctionExpression));
}

void File::getLibrary(const std::string &line, unsigned int &index, bool isOnce)
{
	this->isInLibrary = true;
	std::string identifier = getToken(line, index); //name
	std::string initializer;
	std::list<std::string> *requirement = 0;
	std::string token = getToken(line, index);
	
	if (token == expressionText[InitializerExpression])
	{
		initializer = getToken(line, index);
		token = getToken(line, index);
	}
	
	std::list<bool> *optionalRequirement = new std::list<bool>();
	
	if (token == expressionText[RequiresExpression] || token == expressionText[NeedsExpression] || token == expressionText[UsesExpression])
		requirement = this->getLibraryRequirement(line, index, optionalRequirement);

	class Library *library = new Library(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, isOnce, initializer, requirement, optionalRequirement);
	this->currentLibrary = library;
	Vjassdoc::parser()->add(library);
	this->clearDocComment();
}

void File::getScope(const std::string &line, unsigned int &index, bool isPrivate)
{
	std::string identifier = getToken(line, index);
	std::string initializerExpression;
	
	if (getToken(line, index) == expressionText[InitializerExpression])
		initializerExpression = getToken(line, index);

	class Scope *scope = new Scope(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, isPrivate, initializerExpression);
	this->isInScope = true;
	this->currentScope = scope;
	Vjassdoc::parser()->add(scope);
}

void File::getInterface(const std::string &line, unsigned int &index, bool isPrivate)
{
	std::string identifier = getToken(line, index);
	class Interface *interface = new Interface(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate);
	this->isInInterface = true;
	this->currentInterface = interface;
	Vjassdoc::parser()->add(interface);
}

void File::getStruct(const std::string &line, unsigned int &index, bool isPrivate)
{
	std::string identifier = getToken(line, index);
	std::string sizeExpression, extensionExpression;
	
	std::string::size_type position = identifier.find('[');

	if (position != std::string::npos)
	{
		sizeExpression = identifier.substr(position + 1);
		sizeExpression.erase(sizeExpression.end() - 1);
		identifier.erase(position);
	}

	if (getToken(line, index) == expressionText[ExtendsExpression])
		extensionExpression = getToken(line, index);

	class Struct *usedStruct = new Struct(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate, sizeExpression, extensionExpression);
	this->isInStruct = true;
	this->currentStruct = usedStruct;
	Vjassdoc::parser()->add(usedStruct);
}

void File::getModule(const std::string &line, unsigned int &index, bool isPrivate)
{
	std::string identifier = getToken(line, index);
	class Module *module = new Module(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate);
	this->isInModule = true;
	this->currentModule = module;
	Vjassdoc::parser()->add(module);
}

void File::getLocal(const std::string &line, unsigned int &index)
{
	std::string typeExpression = getToken(line, index);
	std::string identifier = getToken(line, index);
	std::string valueExpression = getToken(line, index);

	if (!valueExpression.empty())
	{
		std::cout << "=-Operator: " << valueExpression << std::endl;
		valueExpression = getToken(line, index);
	}
	else
		valueExpression.clear();

	Vjassdoc::parser()->add(new Local(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentFunction, typeExpression, valueExpression));
}

std::string File::removeFirstSpace(const std::string &line, unsigned int index) const
{
	while (index < line.length() && (line[index] == ' ' || line[index] == '\t'))
		++index;

	return line.substr(index, line.length());
}


static std::string getTextMacroArguments(const std::string &line, unsigned int &index)
{
	//get token takes
	if (getToken(line, index) != File::expressionText[File::TakesExpression])
		return std::string();

	std::string argList = " ";
	std::string arg;
	
	while (true)
	{
		arg = getToken(line, index);

		if (arg.empty())
			return argList;

		argList += arg + " ";
	}

	return std::string();
}

void File::getTextMacro(const std::string &line, unsigned int &index, bool isOnce)
{
	std::string identifier = getToken(line, index);
	//std::cout << "Start text macro " << identifier << std::endl;
	std::string args = getTextMacroArguments(line, index); //Nicht direkt übergeben, dann stimmt index nicht
	//std::cout << "With arguments " << args << std::endl;
	Vjassdoc::parser()->add(new TextMacro(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, isOnce, args));
	
	if (!Vjassdoc::optionTextmacros())
	{
		if (Vjassdoc::optionVerbose())
			std::cout << _("Ignoring text macro space.") << std::endl;

		this->notRequiredSpace = TextmacroExpression;
	}
	
	this->clearDocComment();
}

void File::getTextMacroInstance(const std::string &line, unsigned int &index)
{
	std::string token = getToken(line, index);
	bool isOptional = false;
	
	if (token == expressionText[OptionalExpression])
	{
		isOptional = true;
		token = getToken(line, index, true);
	}
	
	std::string::size_type position = token.find('('); //For the case: //! runtextmacro TextMacroName(...) and not //! runtextmacro TextMacroName (...)

	std::string identifier = token.substr(0, position);
	std::string args = token.substr(position + 1, token.length() - position - 2);

	//parser->addTextMacroInstance(this->docComment, this->getIdentifier(identifier), args);
	Vjassdoc::parser()->add(new TextMacroInstance(identifier, Vjassdoc::parser()->currentSourceFile(), this->currentLine, this->currentDocComment, isOptional, args));
	this->clearDocComment();
}

std::list<std::string>* File::getLibraryRequirement(const std::string &line, unsigned int &index, std::list<bool> *optionalRequirement) const
{
	std::string libraryToken;
	std::list<std::string> *requiredLibraries = new std::list<std::string>();
	
	while (true)
	{
		libraryToken = getToken(line, index);
		
		if (libraryToken.empty())
			break;

		if (libraryToken == ",") //single separator
			continue;

		bool isOptional = false;
		
		if (libraryToken == expressionText[OptionalExpression])
		{
			isOptional = true;
			libraryToken = getToken(line, index);
		}
		
		//cut the separator
		if (libraryToken.substr(libraryToken.length() - 1, libraryToken.length()) == ",")
		{
			libraryToken = libraryToken.substr(0, libraryToken.length() - 1);
		//if there is no separator it must be the last required library
		}
		else
		{
			requiredLibraries->push_back(libraryToken);
			optionalRequirement->push_back(isOptional);
			
			break;
		}
		
		requiredLibraries->push_back(libraryToken);
		optionalRequirement->push_back(isOptional);
	}
	
	return requiredLibraries;
}

inline bool File::isInVjassBlock() const
{
	return (this->isInLibrary || this->isInScope || this->isInInterface || this->isInStruct || this->isInModule);
}

inline class Object* File::getCurrentContainer() const
{
	if (this->currentInterface != 0)
		return this->currentInterface;
	else if (this->currentStruct != 0)
		return this->currentStruct;
	else if (this->currentModule != 0)
		return this->currentModule;

	return 0;
}

}
