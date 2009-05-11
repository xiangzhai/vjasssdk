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
#include <iostream>
#include <sstream>
#include <list>

#include "file.h"
#include "vjassdoc.h"
#include "objects.h"
#include "parser.h"
#include "internationalisation.h"

namespace vjassdoc
{

const char *File::expressionText[] =
{
	//Jass stand-alone expressions
	"///", //This tool
	"//",
	"type",
	"constant",
	"native",
	"function",
	"endfunction",
	"globals",
	"endglobals",
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

const char *File::docExpressionText[] =
{
	"author",
	"todo",
	"param",
	"return",
	"source",
	"state"
};

std::string File::getToken(const std::string &line, unsigned int &index, bool endOfLine)
{
	if (index >= line.length() || line.empty()) //important
		return std::string();

	while (index < line.length() && (line[index] == ' ' || line[index] == '\t'))
		++index;

	int position = index;
	int length = 1;

	if (!endOfLine)
	{
		do
		{
			++index;
		}
		while (index <= line.length() && line[index] != ' ' && line[index] != '\t');

		length = index - position;
	}
	else
	{
		index = line.length();
		length = index;
	}

	return line.substr(position, length);
}

File::File(const std::string &filePath) : filePath(filePath), notRequiredSpace(File::InvalidExpression), isInGlobals(false), isInLibrary(false), isInScope(false), isInInterface(false), isInStruct(false), isInModule(false), currentLine(0), currentDocComment(0), currentLibrary(0), currentScope(0), currentInterface(0), currentStruct(0), currentModule(0), gotDocComment(false)
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
		
		if (Vjassdoc::showVerbose())
			printf(_("Reading line %d of file %s.\n"), this->currentLine, filePath.c_str());

		index = 0; //reset index!
		expression = this->getFirstLineExpression(line, index);

		if (expression == File::NoExpression || expression == File::InvalidExpression) /// @todo If it's a debug expression, line should be skipped. Therefore you could add File::DebugExpression.
			continue;
	
		switch (expression)
		{
			//optional
			case DocCommentExpression:
			{
				this->getDocComment(line, index);
				break;
			}

			case CommentExpression:
			{
				this->truncateComments(line, index);
				std::string identifier = File::getToken(line, index, true);
			
				if (!identifier.empty())
					Vjassdoc::getParser()->add(new Comment(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment));
				
				break;
			}

			case KeywordExpression:
				this->truncateComments(line, index);
				this->getKeyword(line, index, false);
				break;

			case PreprocessorExpression:
			{
				this->truncateComments(line, index);
				std::string preprocessor = File::getToken(line, index);
				
				if (preprocessor == expressionText[ImportExpression])
				{
					std::string filePath = File::getToken(line, index, true);
					filePath = filePath.substr(1, filePath.length() - 2);
					std::string openPath;
					std::ifstream fin;
					bool result = false;
					std::list<std::string> list = Vjassdoc::getImportDirs();
					std::list<std::string>::const_iterator iterator = list.begin();
					
					while (iterator != list.end())
					{
						openPath = *iterator;
						std::size_t length = strlen(Vjassdoc::dirSeparator);
						
						if (strcmp(openPath.substr(openPath.length() - length, length).c_str(), Vjassdoc::dirSeparator) != 0)
							openPath += Vjassdoc::dirSeparator;
						
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
						if (Vjassdoc::showVerbose())
							printf(_("Importing file %s.\n"), openPath.c_str());
						
						Vjassdoc::getParser()->add(new SourceFile(filePath, openPath));
						this->clearDocComment();
					}
					else
					{
						if (Vjassdoc::showVerbose())
							printf(_("Importing file %s.\n"), filePath.c_str());
						
						Vjassdoc::getParser()->add(new SourceFile(filePath, filePath));
						this->clearDocComment();
					}
				}
				else if (preprocessor == expressionText[TextmacroExpression])
				{
					std::string identifier = File::getToken(line, index);
					std::string args = this->getTextMacroArguments(line, index); //Nicht direkt übergeben, dann stimmt index nicht
					Vjassdoc::getParser()->add(new TextMacro(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, args));
					
					if (!Vjassdoc::parseTextMacroSpace())
						this->notRequiredSpace = TextmacroExpression;
					
					this->clearDocComment();
				}
				else if (preprocessor == expressionText[RuntextmacroExpression])
				{
					std::string token = File::getToken(line, index, true);
					unsigned int argsPosition = token.find('('); //For the case: //! runtextmacro TextMacroName(...) and not //! runtextmacro TextMacroName (...)

					std::string identifier = token.substr(0, argsPosition);
					std::string args = token.substr(argsPosition + 1, token.length() - argsPosition - 2);

					//parser->addTextMacroInstance(this->docComment, this->getIdentifier(identifier), args);
					Vjassdoc::getParser()->add(new TextMacroInstance(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, args));
					this->clearDocComment();
				}
				//NOTE Syntax checker!
				//else if (Vjassdoc::showVerbose() && preprocessor != expressionText[DovjassinitExpression] && preprocessor != expressionText[InjectExpression] && preprocessor != expressionText[EndinjectExpression] && preprocessor != expressionText[NovjassExpression] && preprocessor != expressionText[EndnovjassExpression] && preprocessor != expressionText[LoaddataExpression] && preprocessor != expressionText[ExternalExpression])
						//std::cerr << _("Unknown preprocessor.") << std::endl;
				
				break;
			}

			//Example: type MyType extends integer array[10]
			//Output: Typ: MyType - Enthaltener Typ: integer - Gr��e: array[10]
			case TypeExpression:
			{
				this->truncateComments(line, index);
				std::string identifier = File::getToken(line, index);
				std::string parentTypeIdentifier;
				
				if (File::getToken(line, index) == expressionText[ExtendsExpression])
					parentTypeIdentifier = File::getToken(line, index);
				
				std::string size = File::getToken(line, index);
				
				if (!size.empty())
				{
					if (size == File::expressionText[File::ArrayExpression]) //expression: type TypeName extends array [10]
						size = File::getToken(line, index);
	
					int start = size.find('[') + 1;
					int length = size.find(']') - start;
					size = size.substr(start, length);
				}

				Vjassdoc::getParser()->add(new Type(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, parentTypeIdentifier, size));
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

			case MethodExpression:
				this->truncateComments(line, index);
				this->getFunction(line, index, false, false, false, false, false, false);
				break;

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
				
				if (!this->getFunction(line, index, false, true, false, false, false, false))
					this->getGlobal(line, lastIndex, false, true, false, false, false);

				break;
			}

			//Private objects won't be parsed
			case PrivateExpression:
			{
				unsigned int lastIndex = index;
				token = File::getToken(line, index);

				if (!Vjassdoc::parsePrivateSpace())
				{
					//just possible if it is a method
					if (this->isInStruct && token == expressionText[StaticExpression])
						token = File::getToken(line, index);
				
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
					
					if (Vjassdoc::showVerbose())
						printf(_("Starting with not required space. Reason: %d.\n"), this->notRequiredSpace);
					
					//as there will be an object let the program clear the current doc comment
					this->clearDocComment();
				}
				else
				{
					this->truncateComments(line, index);
					
					if (token == expressionText[KeywordExpression])
						this->getKeyword(line, index, true);
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

			//new
			case ImplementExpression:
				this->truncateComments(line, index);
				this->getImplementation(line, index);
				break;
		}
	}

	Vjassdoc::addLines(this->currentLine);
	Vjassdoc::addFile();
	printf(_("Parsed file %s successfully (number %d, %d lines).\n"), filePath.c_str(), Vjassdoc::getFiles(), this->currentLine);
	fin.close();
}

File::Expression File::getFirstLineExpression(std::string &line, unsigned int &index)
{
	std::string token = File::getToken(line, index);

	if (token.empty())
	{
		if (Vjassdoc::showVerbose())
			std::cout << _("Empty line, ignoring line.") << std::endl;
			
		return File::NoExpression;
	}

	if (!Vjassdoc::jassOnly() && token == expressionText[File::DebugExpression])
	{
		if (Vjassdoc::useDebugMode())
			File::getToken(line, index); //cut the keyword
		else
		{
			if (Vjassdoc::showVerbose())
				std::cout << _("Debug statement, ignoring line.") << std::endl;
			
			return File::DebugExpression;
		}
	}

	//Function and text macro blocks won't be parsed
	if (this->notRequiredSpace != File::InvalidExpression)
	{
		if ((this->notRequiredSpace == File::FunctionExpression && token == expressionText[File::EndfunctionExpression]) ||
			(this->notRequiredSpace == File::MethodExpression && token == expressionText[File::EndmethodExpression]) ||
			(this->notRequiredSpace == File::TextmacroExpression && token == expressionText[File::PreprocessorExpression] && File::getToken(line, index) == expressionText[File::EndtextmacroExpression]) || /// @todo Truncate the //! prefix
			(this->notRequiredSpace == File::ScopeExpression && token == expressionText[File::EndscopeExpression]) ||
			(this->notRequiredSpace == File::InterfaceExpression && token == expressionText[File::EndinterfaceExpression]) ||
			(this->notRequiredSpace == File::StructExpression && token == expressionText[File::EndstructExpression]) || (this->notRequiredSpace == File::ModuleExpression && token == File::expressionText[File::EndmoduleExpression]))
		{
			if (Vjassdoc::showVerbose())
				printf(_("Not required space. Reason: %d.\n"), this->notRequiredSpace);

			this->notRequiredSpace = File::InvalidExpression;
		}
		
		return File::NoExpression; //stop parsing, ignore comments and documentation comments because it is not required space
	}

	unsigned int maxExpressions = NothingExpression; //maximum stand-alone vJass expression

	if (Vjassdoc::jassOnly())
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
		//token = this->getIdentifier(File::getToken(line, index, true).substr(2, line.length()));
		token = File::getToken(line, index, true).substr(2, line.length());
		Vjassdoc::getParser()->add(new class Comment(token, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment));
		this->clearDocComment();
		return File::CommentExpression;
	}
	
	this->truncateComments(line, index);
	index = 0; //Restart, the getGlobal method searches for the type

	if (!this->getGlobal(line, index, false, false, false, false, false) && Vjassdoc::showVerbose())
	{
		std::cerr << _("Unknown expression.") << std::endl;
		return File::NoExpression; //stop parsing
	}
	else
		this->clearDocComment();


	if (Vjassdoc::showVerbose())
		std::cout << _("No valid expression has been found.") << std::endl;

	return File::InvalidExpression;
}

void File::truncateComments(std::string &line, unsigned int index)
{
	bool isInString = false;
	int length = line.length() - 3; //any documentation comment has to start with /// (3 characters)
	
	while (index < line.length())
	{
		if (line[index] == '\"')
		{
			isInString = !isInString;
			
			if (Vjassdoc::showVerbose())
			{
				if (isInString)
					std::cout << _("Detected \" character. Ignoring // and /// expressions.") << std::endl;
				else
					std::cout << _("Detected \" character. Do not ignore // and /// expressions anymore.") << std::endl;
			}
		}
		else if (!isInString)
		{
			if (line.substr(index, 3) == expressionText[DocCommentExpression])
			{
				index += 3;
				this->getDocComment(line, index);
				
				break;
				/*
				if (this->currentDocComment == 0)
				{
					std::cout << "Before allocating a new documentation comment." << std::endl;
					this->currentDocComment = new DocComment(0, Vjassdoc::getParser()->currentSourceFile(), index);
					Vjassdoc::getParser()->add(this->currentDocComment);
					std::cout << "After allocating a new documentation comment." << std::endl;
				}
	
				std::string token = line.substr(index + 3, line.length() - (index + 3));
				this->currentDocComment->addIdentifier(token);
				line.erase(index - 1);
	
				return true;
				*/
			}
			else if (line.substr(index, 2) == expressionText[CommentExpression])
			{
				std::string token = line.substr(index + 2, line.length() - (index + 2));
			
				if (!token.empty())
				{
					Vjassdoc::getParser()->add(new Comment(token, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment));
					line.erase(index - 1);
				}
				else if (Vjassdoc::showVerbose())
					std::cout << _("Detected empty comment.") << std::endl;
				
				break;
			}
		}
		
		++index;
	}
}

void File::getDocComment(const std::string &line, unsigned int index)
{
	std::string docCommentItem = File::getToken(line, index, true);
	std::string result;
	unsigned int oldIndex = 0;
	unsigned int newIndex = docCommentItem.find('@');

	while (newIndex != std::string::npos)
	{
		result += docCommentItem.substr(oldIndex, newIndex - oldIndex);
		++newIndex;
		
		if (newIndex == docCommentItem.length())
			break;
		
		std::string token = File::getToken(docCommentItem, newIndex);
		//std::cout << "Token: " << token << " and index " << newIndex << std::endl;
		bool found = false;
		
		for (int i = 0; i < MaxDocExpressions; ++i)
		{
			if (docExpressionText[i] == token)
			{
				//std::cout << "Found keyword: " << DocComment::keyword[i] << " and has index " << index << std::endl;
				found = true;
				
				switch (i)
				{
					case Author:
						result += _("<strong>Author:</strong>");
						break;
				
					case ToDo:
						result += _("<strong>Todo:</strong>");
						break;
				
					case Parameter:
						result += _("<strong>Parameter:</strong>");
						break;
				
					case Return:
						result += _("<strong>Return:</strong>");
						break;
					
					case State:
						result += _("<strong>State:</strong>");
						break;
					
					case Source:
						result += _("<strong>Source:</strong>"); // TODO Link to website
						break;
				}
				
				break;
			}
		}
		
		if (!found)
			result += token;
		
		if (newIndex >= docCommentItem.length() - 1)
			break;
		
		oldIndex = newIndex;
		newIndex = docCommentItem.find('@', newIndex);
	}
	
	if (oldIndex <  docCommentItem.length() - 1)
		result += docCommentItem.substr(oldIndex);
	
	if (!result.empty())
	{
		if (this->currentDocComment == 0)
		{
			this->currentDocComment = new DocComment(result, Vjassdoc::getParser()->currentSourceFile(), this->currentLine);
			Vjassdoc::getParser()->add(this->currentDocComment);
			this->gotDocComment = true;
		}
		else
			this->currentDocComment->addIdentifier(" <br> " + result); //parse before and modify, space characters are important!
	}
	else if (Vjassdoc::showVerbose())
		std::cout << _("Detected empty documentation comment.") << std::endl;
/*












	std::string docCommentItem = File::getToken(line, index, true);
	std::string result;
	//parse the item
	unsigned int oldIndex = 0;
	unsigned int newIndex = 0;
	
	do
	{
		oldIndex = newIndex;
		std::string token = File::getToken(docCommentItem, newIndex);
		std::cout << "Token " << token << " with index " << newIndex << std::endl;
		
		if (token.empty())
			break;
		
		if (token[0] != '@')
		{
			result += docCommentItem.substr(oldIndex, newIndex);
			continue;
		}
		
		token = token.substr(1);
		
		std::cout << "Substring " << token << std::endl;
		
		int expression;
		
		for (expression = 0; expression < MaxDocExpressions; ++expression)
		{
			if (docExpressionText[expression] == token)
			{
				std::cout << "Found doc comment expression " << docExpressionText[expression] << " with index " << expression << std::endl;
				break;
			}
		}
		
		if (expression == MaxDocExpressions) //no error message! Can be @struct, @comment etc.
			continue;

		switch (expression)
		{
			case Author:
				result += _("<strong>Author:</strong>");
				break;
		
			case ToDo:
				result += _("<strong>Todo:</strong>");
				break;
		
			case Parameter:
				result += _("<strong>Parameter:</strong>");
				break;
		
			case Return:
				result += _("<strong>Return:</strong>");
				break;
			
			case State:
				result += _("<strong>State:</strong>");
				break;
			
			case Source:
				result += _("<strong>Source:</strong>"); // TODO Link to website
				break;
		}
	}
	while(true);
	
	if (!result.empty())
	{
		if (this->currentDocComment == 0)
		{
			this->currentDocComment = new DocComment(result, Vjassdoc::getParser()->currentSourceFile(), this->currentLine);
			Vjassdoc::getParser()->add(this->currentDocComment);
			this->gotDocComment = true;
		}
		else
			this->currentDocComment->addIdentifier("<br>" + result); //parse before and modify
	}
	else if (Vjassdoc::showVerbose())
		std::cout << _("Detected empty documentation comment.") << std::endl;
		*/
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
	Vjassdoc::getParser()->add(new Keyword(File::getToken(line, index), Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate));
}

bool File::getGlobal(const std::string &line, unsigned int &index, bool isPrivate, bool isPublic, bool isConstant, bool isStatic, bool isDelegate)
{
	if (!this->isInGlobals && Vjassdoc::jassOnly() || (!Vjassdoc::jassOnly() && !this->isInGlobals && !this->isInInterface && !this->isInStruct))
			return false;

	std::string identifier = File::getToken(line, index);
	
	if (!Vjassdoc::jassOnly() && this->isInVjassBlock())
	{
		/*
		If the member is public it HAS to start with the public keyword
		//Do not list private members
		if (identifier == expressionText[PrivateExpression])
			return false;
	
		//If there is not postfix the member is public
		if (identifier == expressionText[PublicExpression])
		{
			identifier = File::getToken(line, index);
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
	
			identifier = File::getToken(line, index);
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
			identifier = File::getToken(line, index);
		}
	}

	std::string type = identifier;
	identifier = File::getToken(line, index);
	bool isArray = false;
	
	//Jass does not support constant arrays
	if (!isConstant && identifier == expressionText[ArrayExpression])
	{
		isArray = true;
		identifier = File::getToken(line, index);
	}

	std::string size;
	
	if (isArray)
	{
		int position = identifier.find('[', 0);
		if (position != std::string::npos)
		{
			int length = identifier.find(']', position);
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
	if (!isArray && File::getToken(line, index) == "=")
		value = File::getToken(line, index, true); //End of line for function calls

	if (Vjassdoc::jassOnly() || !this->isInStruct && !this->isInInterface)
		Vjassdoc::getParser()->add(new Global(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate, isPublic, isConstant, type, value, size));
	else
	{
		Vjassdoc::getParser()->add(new Member(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate, isPublic, isConstant, type, value, size, this->getCurrentContainer(), isStatic, isDelegate));
	}
	
	this->clearDocComment();
	return true;
}

bool File::getFunction(const std::string &line, unsigned int &index, bool isPrivate, bool isPublic, bool isConstant, bool isNative, bool isStatic, bool isStub)
{
	std::string identifier = File::getToken(line, index);
	bool isOperator = false;

	if (!Vjassdoc::jassOnly() && this->isInVjassBlock())
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
			identifier = File::getToken(line, index);
			isPublic = true;
		}
		*/
		
		bool hasPrefix = false;
	
		if (!isStatic && (this->isInStruct || this->isInInterface) && identifier == expressionText[StaticExpression])
		{
			hasPrefix = true;
			isStatic = true;
			identifier = File::getToken(line, index);
		}

		if (!isStub && (this->isInStruct || this->isInInterface) && identifier == expressionText[StubExpression]) //no statics and before constant?
		{
			hasPrefix = true;
			isStub = true;
			identifier = File::getToken(line, index);
		}

		if (!isConstant && identifier == expressionText[ConstantExpression])
		{
			hasPrefix = true;
			isConstant = true;
			identifier = File::getToken(line, index);
		}
	
		if ((this->isInStruct || this->isInInterface) && identifier == expressionText[MethodExpression])
			hasPrefix = true;
	
		if (hasPrefix)
		{
			//no syntax checker, required for global identification
			if ((this->isInStruct || this->isInInterface) && identifier != expressionText[MethodExpression])
				return false;
			else if (!this->isInStruct && !this->isInInterface && identifier != expressionText[FunctionExpression])
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

			identifier = File::getToken(line, index);
		}

		if (identifier == File::expressionText[File::OperatorExpression])
		{
			isOperator = true;
			identifier = File::getToken(line, index);
		}
	}

	bool isFunctionInterface = false;
	
	if (!Vjassdoc::jassOnly() && identifier == expressionText[InterfaceExpression])
	{
		isFunctionInterface = true;
		identifier = File::getToken(line, index);
	}
	//constant native
	else if (isConstant && !isNative && identifier == expressionText[NativeExpression])
	{
		isNative = true;
		identifier = File::getToken(line, index);
	}
	else if ((isConstant || isPrivate || isPublic) && identifier == expressionText[FunctionExpression])
		identifier = File::getToken(line, index); //get the name

	//no syntax checker, required for global identification
	if (File::getToken(line, index) != expressionText[TakesExpression])
		return false;

	std::list<class Parameter*> parameters;
	std::string argToken = File::getToken(line, index);
	
	if (argToken != expressionText[NothingExpression])
	{
		do
		{
			std::string typeExpression = argToken;
			argToken = File::getToken(line, index);
			
			std::string::size_type position = argToken.find(',');
			std::string::size_type lastPosition = argToken.length() - 1;
			std::string oldArgToken;
			
			if (position != std::string::npos) //takes integer x, ...
			{
				oldArgToken = argToken;
				argToken = argToken.substr(0, position);
			}
			
			std::string identifier = argToken;
			class vjassdoc::Parameter *parameter = new class vjassdoc::Parameter(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, 0, typeExpression);
			parameters.push_back(parameter);
			std::cout << "Created parameter with identifier " << identifier << " and type " << typeExpression << std::endl;

			if (position != std::string::npos && position != lastPosition)
				argToken = oldArgToken.substr(position + 1);
			else
			{
				argToken = File::getToken(line, index);
				
				if (argToken == ",") //takes integer x , real y ...
					argToken = File::getToken(line, index);
				else if (argToken[0] == ',') //takes integer x ,real y ...
					argToken = argToken.substr(1);
			}
		}
		while (argToken != expressionText[ReturnsExpression]);
	}
	else
	{
		argToken = File::getToken(line, index);
		
		//NOTE Syntax checker
		//if (argToken != expressionText[ReturnsExpression])
			//return false;
	}

	std::string type = File::getToken(line, index);
	
	if (type == expressionText[NothingExpression])
		type.clear();

	if (!isFunctionInterface)
	{
		if (Vjassdoc::jassOnly() || !this->isInStruct && !this->isInInterface)
		{
			if (!isNative)
				this->notRequiredSpace = FunctionExpression;

			Vjassdoc::getParser()->add(new Function(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate, parameters, type, isPublic, isConstant, isNative));
		}
		else
		{
			std::string defaultReturnValueExpression;

			//Do not expect endmethod in interfaces!
			if (!this->isInInterface)
				this->notRequiredSpace = MethodExpression;
			else
			{
				defaultReturnValueExpression = File::getToken(line, index);
				
				if (defaultReturnValueExpression == File::expressionText[File::DefaultsExpression])
					defaultReturnValueExpression = File::getToken(line, index);
				
				if (Vjassdoc::showVerbose() && defaultReturnValueExpression.empty())
					std::cerr << _("Missing default return value expression.") << std::endl;
			}

			Vjassdoc::getParser()->add(new Method(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate, parameters, type, isPublic, isConstant, this->getCurrentContainer(), isStatic, isStub, isOperator, defaultReturnValueExpression));
	
		}
	}
	else
		Vjassdoc::getParser()->add(new FunctionInterface(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate, parameters, type));

	this->clearDocComment();
	
	return true;
}


void File::getImplementation(const std::string &line, unsigned int &index)
{
	bool isOptional = false;
	std::string identifier = File::getToken(line, index);

	if (identifier == File::expressionText[File::OptionalExpression])
	{
		isOptional = true;
		identifier = File::getToken(line, index);
	}

	Vjassdoc::getParser()->add(new Implementation(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->getCurrentContainer(), identifier, isOptional));
}

void File::getLibrary(const std::string &line, unsigned int &index, bool isOnce)
{
	this->isInLibrary = true;
	std::string identifier = File::getToken(line, index); //name
	std::string initializer;
	std::list<std::string> *requirement = 0;
	std::string token = File::getToken(line, index);
	
	if (token == expressionText[InitializerExpression])
	{
		initializer = File::getToken(line, index);
		token = File::getToken(line, index);
	}
	
	if (token == expressionText[RequiresExpression] || token == expressionText[NeedsExpression] || token == expressionText[UsesExpression])
		requirement = this->getLibraryRequirement(line, index);

	Library *library = new Library(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, isOnce, initializer, requirement);
	this->currentLibrary = library;
	Vjassdoc::getParser()->add(library);
	this->clearDocComment();
}

void File::getScope(const std::string &line, unsigned int &index, bool isPrivate)
{
	std::string identifier = File::getToken(line, index);
	std::string initializerExpression;
	
	if (File::getToken(line, index) == expressionText[InitializerExpression])
		initializerExpression = File::getToken(line, index);

	class Scope *scope = new Scope(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, isPrivate, initializerExpression);
	this->isInScope = true;
	this->currentScope = scope;
	Vjassdoc::getParser()->add(scope);
}

void File::getInterface(const std::string &line, unsigned int &index, bool isPrivate)
{
	std::string identifier = File::getToken(line, index);
	class Interface *interface = new Interface(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate);
	this->isInInterface = true;
	this->currentInterface = interface;
	Vjassdoc::getParser()->add(interface);
}

void File::getStruct(const std::string &line, unsigned int &index, bool isPrivate)
{
	std::string identifier = File::getToken(line, index);
	std::string extensionExpression;
	
	if (File::getToken(line, index) == expressionText[ExtendsExpression])
		extensionExpression = File::getToken(line, index);

	class Struct *usedStruct = new Struct(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate, extensionExpression);
	this->isInStruct = true;
	this->currentStruct = usedStruct;
	Vjassdoc::getParser()->add(usedStruct);
}

void File::getModule(const std::string &line, unsigned int &index, bool isPrivate)
{
	std::string identifier = File::getToken(line, index);
	class Module *module = new Module(identifier, Vjassdoc::getParser()->currentSourceFile(), this->currentLine, this->currentDocComment, this->currentLibrary, this->currentScope, isPrivate);
	this->isInModule = true;
	this->currentModule = module;
	Vjassdoc::getParser()->add(module);
}

std::string File::removeFirstSpace(const std::string &line, unsigned int index) const
{
	while (index < line.length() && (line[index] == ' ' || line[index] == '\t'))
		++index;

	return line.substr(index, line.length());
}

std::string File::getTextMacroArguments(const std::string &line, unsigned int &index) const
{
	//get token takes
	if (getToken(line, index) != expressionText[TakesExpression])
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

std::list<std::string>* File::getLibraryRequirement(const std::string &line, unsigned int &index) const
{
	std::string libraryToken;
	std::list<std::string> *requiredLibraries = new std::list<std::string>();
	
	while (true)
	{
		libraryToken = File::getToken(line, index);
		
		if (libraryToken.empty())
			break;

		if (libraryToken == ",") //single separator
			continue;

		//cut the separator
		if (libraryToken.substr(libraryToken.length() - 1, libraryToken.length()) == ",")
		{
			libraryToken = libraryToken.substr(0, libraryToken.length() - 1);
		//if there is no separator it must be the last required library
		}
		else
		{
			requiredLibraries->push_back(libraryToken);
			break;
		}
		requiredLibraries->push_back(libraryToken);
	}
	return requiredLibraries;
}

inline bool File::isInVjassBlock() const
{
	return (this->isInLibrary || this->isInScope || this->isInInterface || this->isInStruct);
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
