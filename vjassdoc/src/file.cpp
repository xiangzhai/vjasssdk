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
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <list>

#include <boost/format.hpp>
#include <boost/foreach.hpp>

#include "file.hpp"
#include "objects.hpp"
#include "parser.hpp"
#include "internationalisation.hpp"
#include "syntaxerror.hpp"
#include "utilities.hpp"

namespace vjassdoc
{

File::File() : m_parser(0), m_parser(0), m_language(Vjassdoc::Jass), m_lines(0), m_docComment(0)
{
}

std::size_t File::parse(class Parser *parser, std::istream &istream)
{
	this->m_parser = parser;
	
	istream.seekg(0, std::ios_base::end);
	std::streampos position = ifstream.tellg();
	istream.seekg(0);
	char *bytes = new char[position + 3];
	istream.read(bytes, position + 1);
	bytes[position + 1] = YY_END_OF_BUFFER_CHAR;
	bytes[position + 2] = YY_END_OF_BUFFER_CHAR;
	YY_BUFFER_STATE buffer = yy_scan_bytes(bytes, position + 3);
	
	if (stateHandle == 0)
		return 0;
	
	yy_switch_to_buffer(buffer);
	//yy_delete_buffer(buffer);
	delete[] bytes;
	bytes = 0;
	/*
	std::string line, token, output;
	std::string::size_type index;
	File::Expression expression;
	
	//ignore spaces and tabs before expressions!
	while (std::getline(ifstream, line))
	{
		++this->m_currentLine;
		
		if (Vjassdoc::optionVerbose())
			std::cout << boost::format(_("Reading line %1%.\n")) % this->m_currentLine << std::endl;;

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
					this->m_parser->add(new Comment(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment));
				
				break;
			}

			case BlockCommentExpression:
			{
				std::string::size_type position = std::string::npos; 

				if (index < line.length())
					position = line.find(expressionText[EndBlockCommentExpression], index);

				if (position == std::string::npos)
				{
					this->m_isInBlockComment = true;
					
					if (index < line.length())
						this->m_currentBlockComment = line.substr(index);
				}
				else
				{
					this->m_parser->add(new class Comment(line.substr(index), this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment));
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
					this->m_isInBlockDocComment = true;
					
					if (index < line.length())
						this->m_currentBlockDocComment = line.substr(index);
				}
				else
				{
					if (this->m_currentDocComment == 0)
					{
						this->m_gotDocComment = true;
						this->m_currentDocComment = new class DocComment(line.substr(index), this->m_parser->currentSourceFile(), this->m_currentLine);
						this->m_parser->add(this->m_currentDocComment);
					}
					else
						this->m_currentDocComment->addIdentifier(line.substr(index));
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
					std::string filePath = getToken(line, index);
					
					if (filePath == expressionText[File::OptionVjassExpression] || filePath == expressionText[File::OptionZincExpression])
						filePath = getToken(line, index, true);
					else if (index < line.size())
						filePath += line.substr(index);
					
					filePath = filePath.substr(1, filePath.length() - 2);
					std::string openPath;
					std::ifstream fin;
					bool result = false;
					
					BOOST_FOREACH(boost::filesystem::path path, Vjassdoc::optionImport())
					{
						fin.open((path / filePath).string().c_str(), std::ios_base::ate);

						if (fin.good())
						{
							result = true;
							fin.close(); //TODO Just close if good?
							
							break;
						}
					}

					if (!result)
					{
						fin.open(filePath.c_str(), std::ios_base::ate);

						if (fin.good())
							fin.close(); /// @todo Just close if good?
						else
						{
							std::cerr << boost::format(_("Was unable to import file %1%.\n")) % filePath << std::endl;;
							
							continue;
						}
					}

					if (result)
					{
						if (Vjassdoc::optionVerbose())
							printf(_("Importing file %s.\n"), openPath.c_str());
						
						this->m_parser->add(new SourceFile(filePath, openPath));
						this->clearDocComment();
					}
					else
					{
						if (Vjassdoc::optionVerbose())
							printf(_("Importing file %s.\n"), filePath.c_str());
						
						this->m_parser->add(new SourceFile(filePath, filePath));
						this->clearDocComment();
					}
				}
				else if (preprocessor == expressionText[TextmacroExpression])
					this->getTextMacro(line, index, false);
				else if (preprocessor == expressionText[TextmacroonceExpression])
					this->getTextMacro(line, index, true);
				else if (preprocessor == expressionText[RuntextmacroExpression])
					this->getTextMacroInstance(line, index);
				else if (preprocessor == expressionText[File::ExternalblockExpression])
				{
					/// @todo Add classes ExternalBlock and External?
					this->clearDocComment();
					this->m_notRequiredSpace = File::ExternalblockExpression;
				}
				//NOTE Syntax checker!
				else if (Vjassdoc::optionSyntax() && preprocessor != expressionText[DovjassinitExpression] && preprocessor != expressionText[InjectExpression] && preprocessor != expressionText[EndinjectExpression] && preprocessor != expressionText[NovjassExpression] && preprocessor != expressionText[EndnovjassExpression] && preprocessor != expressionText[ZincExpression] && preprocessor != expressionText[EndzincExpression] && preprocessor != expressionText[LoaddataExpression] && preprocessor != expressionText[ExternalExpression])
				{
					std::ostringstream osstream;

					if (!preprocessor.empty())
						osstream << boost::str(boost::format(_("Unknown preprocessor statement: \"%1%\".")) % preprocessor);
					else
						osstream << _("Missing preprocessor statement.");

					this->m_parser->add(new SyntaxError(this->m_parser->currentSourceFile(), this->m_currentLine, osstream.str()));
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
					this->m_parser->add(new SyntaxError(this->m_parser->currentSourceFile(), this->m_currentLine, _("Missing type identifier.")));

				std::string parentTypeIdentifier;
				parentTypeIdentifier = getToken(line, index);
				
				if (parentTypeIdentifier == expressionText[ExtendsExpression])
					parentTypeIdentifier = getToken(line, index);
				else if (Vjassdoc::optionSyntax() && !parentTypeIdentifier.empty())
					this->m_parser->add(new SyntaxError(this->m_parser->currentSourceFile(), this->m_currentLine, boost::str(boost::format(_("Unknown expression: \"%1%\". Expected \"extends\".")) % parentTypeIdentifier)));
				
				std::string size = getToken(line, index);
				
				if (!size.empty())
				{
					bool getSize = true;

					if (size == File::expressionText[File::ArrayExpression]) //expression: type TypeName extends array [10]
						size = getToken(line, index);
	
					std::string::size_type start = size.find('[') + 1;

					if (Vjassdoc::optionSyntax() && start == std::string::npos)
					{
						this->m_parser->add(new SyntaxError(this->m_parser->currentSourceFile(), this->m_currentLine, _("Missing \"[\" expression.")));
						getSize = false;
					}

					std::string::size_type length = size.find(']') - start;

					if (Vjassdoc::optionSyntax() && length == std::string::npos)
					{
						this->m_parser->add(new SyntaxError(this->m_parser->currentSourceFile(), this->m_currentLine, _("Missing \"]\" expression.")));
						getSize = false;
					}

					if (getSize) //can only be false when syntax checker is enabled
						size = size.substr(start, length);
				}

				this->m_parser->add(new Type(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, parentTypeIdentifier, size));
				this->clearDocComment();
				
				break;
			}

			case ConstantExpression:
			{
				this->truncateComments(line, index);
				std::string::size_type lastIndex = index;
				
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
				this->m_currentFunction = 0;
				
				break;

			case MethodExpression:
				this->truncateComments(line, index);
				this->getFunction(line, index, false, false, false, false, false, false);
				
				break;

			case EndmethodExpression:
				this->m_currentFunction = 0;
				
				break;

			case StaticExpression:
			{
				this->truncateComments(line, index);
				std::string::size_type lastIndex = index;
				
				std::string token = getToken(line, index);
				
				if (token == File::expressionText[IfExpression])
				{
					/// @todo Add static if support, should be evaluated during parse process (ignore code between static ifs, if variable is false).
				}
				else
				{
					index = lastIndex;
				
					if (!this->getFunction(line, index, false, false, false, false, true, false))
						this->getGlobal(line, lastIndex, false, false, false, true, false);
				}

				break;
			}

			case PublicExpression:
			{
				this->truncateComments(line, index);
				std::string::size_type lastIndex = index;
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
				std::string::size_type lastIndex = index;
				token = getToken(line, index);

				if (Vjassdoc::optionSyntax() && token.empty())
					this->m_parser->add(new SyntaxError(this->m_parser->currentSourceFile(), this->m_currentLine, _("Missing expression after \"private\" keyword.")));

				if (!Vjassdoc::optionPrivate())
				{
					//just possible if it is a method
					if (this->m_isInStruct && token == expressionText[StaticExpression])
						token = getToken(line, index);
				
					if (token == expressionText[FunctionExpression])
						this->m_notRequiredSpace = FunctionExpression;
					else if (token == expressionText[MethodExpression])
						this->m_notRequiredSpace = MethodExpression;
					else if (token == expressionText[ScopeExpression])
						this->m_notRequiredSpace = ScopeExpression;
					else if (token == expressionText[InterfaceExpression])
						this->m_notRequiredSpace = InterfaceExpression;
					else if (token == expressionText[StructExpression])
						this->m_notRequiredSpace = StructExpression;
					
					if (Vjassdoc::optionVerbose())
						printf(_("Starting with not required space. Reason: %d.\n"), this->m_notRequiredSpace);
					
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
						std::string::size_type secondLastIndex = lastIndex;
				
						if (!this->getFunction(line, lastIndex, true, false, false, false, false, false))
							this->getGlobal(line, secondLastIndex, true, false, false, false, false);
					}
				}
				
				break;
			}

			case GlobalsExpression:
				this->m_isInGlobals = true;
				
				break;
	
			case EndglobalsExpression:
				this->m_isInGlobals = false;
				
				break;

			case LocalExpression:
				this->truncateComments(line, index);
				this->getLocal(line, index);
				
				break;

			case SetExpression:
				std::cout << "Set expression" << std::endl;
				/// @todo get set calls
				
				break;

			case CallExpression:
				std::cout << "call expression" << std::endl;
				this->truncateComments(line, index);
				this->getCall(line, index);
				
				break;

			case IfExpression:
				std::cout << "if expression" << std::endl;
				
				break;

			/// @todo Add static if support.
			case ElseExpression:
				
				break;

			/// @todo Add static if support.
			case ElseifExpression:
				
				break;

			/// @todo Add static if support.
			case EndifExpression:
				
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
				this->m_isInLibrary = false;
				this->m_currentLibrary = 0;
				
				break;
	
			case ScopeExpression:
				this->truncateComments(line, index);
				this->getScope(line, index, false);
				
				break;

			case EndscopeExpression:
				this->m_isInScope = false;
				this->m_currentScope = 0;
				
				break;

			case InterfaceExpression:
				this->truncateComments(line, index);
				this->getInterface(line, index, false);
				
				break;

			case EndinterfaceExpression:
				this->m_isInInterface = false;
				this->m_currentInterface = 0;
				
				break;

			case StructExpression:
				this->truncateComments(line, index);
				this->getStruct(line, index, false);
				
				break;

			case EndstructExpression:
				this->m_isInStruct = false;
				this->m_currentStruct = 0;
				
				break;

			//new
			case ModuleExpression:
				this->truncateComments(line, index);
				this->getModule(line, index, false);
				
				break;

			//new
			case EndmoduleExpression:
				this->m_isInModule = false;
				this->m_currentModule = 0;
				
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
				
				break;
		}
	}

	*/
	std::size_t lines = this->m_currentLine;
	
	// reset members
	this->m_parser = 0;
	this->m_notRequiredSpace = File::InvalidExpression;
	this->m_isInGlobals = false;
	this->m_isInTextMacro = false;
	this->m_isInLibrary = false;
	this->m_isInScope = false;
	this->m_isInInterface = false;
	this->m_isInStruct = false;
	this->m_isInModule = false;
	this->m_isInBlockComment = false;
	this->m_isInBlockDocComment = false;
	this->m_currentLine = 0;
	this->m_currentDocComment = 0;
	this->m_currentTextMacro = 0;
	this->m_currentLibrary = 0;
	this->m_currentScope = 0;
	this->m_currentInterface = 0;
	this->m_currentStruct = 0;
	this->m_currentModule = 0;
	this->m_currentFunction = 0;
	this->m_currentBlockComment.clear();
	this->m_currentBlockDocComment.clear();
	this->m_gotDocComment = false;
	
	return lines;
}

File::Expression File::getFirstLineExpression(std::string &line, std::string::size_type &index)
{
	if (this->m_isInBlockComment)
	{
		std::string::size_type position = line.find(expressionText[EndBlockCommentExpression]);

		if (position == std::string::npos)
		{
			this->m_currentBlockComment += line;
			return File::NoExpression; //stop parsing
		}

		this->m_currentBlockComment += line.substr(0, position);
		this->m_parser->add(new class Comment(this->m_currentBlockComment, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment));
		this->m_currentBlockComment.clear();
		this->m_isInBlockComment = false;
		index = position + 2;
	}
	else if (this->m_isInBlockDocComment)
	{
		std::string::size_type position = line.find(expressionText[EndBlockDocCommentExpression]);
		std::string firstToken = getToken(line, index);

		if (position == std::string::npos)
		{
			if (firstToken == "*") // * character is truncated.
				this->m_currentBlockDocComment += line.erase(0, index);
			else
				this->m_currentBlockDocComment += line;

			return File::NoExpression; //stop parsing
		}

		if (firstToken == "*" && index <= position) // * character is truncated.
			this->m_currentBlockDocComment += line.erase(0, index).substr(0, position);
		else
			this->m_currentBlockDocComment += line.substr(0, position);

		if (this->m_currentDocComment == 0)
		{
			this->m_gotDocComment = true;
			this->m_currentDocComment = new class DocComment(this->m_currentBlockDocComment, this->m_parser->currentSourceFile(), this->m_currentLine);
			this->m_parser->add(this->m_currentDocComment);
		}
		else
			this->m_currentDocComment->addIdentifier(this->m_currentBlockDocComment);

		this->m_currentBlockDocComment.clear();
		this->m_isInBlockDocComment = false;
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
	if (this->m_notRequiredSpace != File::InvalidExpression)
	{
		if (Vjassdoc::optionVerbose())
			printf(_("Not required space. Reason: %d.\n"), this->m_notRequiredSpace);
		
		/// @todo what should we do with tokens 
		if (
			(this->m_notRequiredSpace == File::FunctionExpression && token == expressionText[File::EndfunctionExpression]) ||
			(this->m_notRequiredSpace == File::MethodExpression && token == expressionText[File::EndmethodExpression]) ||
			(this->m_notRequiredSpace == File::ScopeExpression && token == expressionText[File::EndscopeExpression]) ||
			(this->m_notRequiredSpace == File::InterfaceExpression && token == expressionText[File::EndinterfaceExpression]) ||
			(this->m_notRequiredSpace == File::StructExpression && token == expressionText[File::EndstructExpression]) ||
			(this->m_notRequiredSpace == File::ModuleExpression && token == File::expressionText[File::EndmoduleExpression])
			)
		{
			if (Vjassdoc::optionVerbose())
				std::cout << _("Not required space ends.") << std::endl;
			
			this->m_notRequiredSpace = File::InvalidExpression;
		}
		else if (token == expressionText[File::PreprocessorExpression])
		{
			if (this->m_notRequiredSpace == File::TextmacroExpression)
			{
				std::string token = getToken(line, index);
				
				if (token == expressionText[File::EndtextmacroExpression])
				{
					if (Vjassdoc::optionVerbose())
						std::cout << _("Not required space ends.") << std::endl;
					
					this->m_notRequiredSpace = File::InvalidExpression;
					this->m_isInTextMacro = false;
				}
			
			}
			else if (this->m_notRequiredSpace == File::ExternalblockExpression)
			{
				std::string token = getToken(line, index);
				
				if (token == expressionText[File::EndexternalblockExpression])
				{
					if (Vjassdoc::optionVerbose())
						std::cout << _("Not required space ends.") << std::endl;
					
					this->m_notRequiredSpace = File::InvalidExpression;
				}
			}
		}
		
		// stopped not required space
		if (this->m_notRequiredSpace == File::InvalidExpression)
		{
			this->truncateComments(line, index);
		}
		
		return File::NoExpression; // stop parsing, ignore comments and documentation comments because it is not required space
	}

	std::size_t maxExpressions = NothingExpression; //maximum stand-alone vJass expression

	if (Vjassdoc::optionJass())
		maxExpressions = PreprocessorExpression; // Spart Speicher, da die Schleife nur alle Jass Schlüsselwörter durchläuft.
	
	for (std::size_t expression = File::DocCommentExpression; expression < maxExpressions; ++expression)
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
		this->m_parser->add(new class Comment(token, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment));
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

	if (Vjassdoc::optionVerbose())
		std::cerr << boost::format(_("Unknown expression: \"%1%\".")) % line.substr(index) << std::endl;

	if (Vjassdoc::optionSyntax())
		this->m_parser->add(new SyntaxError(this->m_parser->currentSourceFile(), this->m_currentLine, boost::str(boost::format(_("Unknown expression: \"%1%\".")) % line.substr(index))));

	return File::InvalidExpression;
}

void File::truncateComments(std::string &line, std::string::size_type index)
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
					this->m_parser->add(new Comment(token, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment));
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
					this->m_currentBlockDocComment = line.substr(index + 3);
					this->m_isInBlockDocComment = true;
					break;
				}
				else
				{
					std::string identifier = line.substr(index + 3, i - index - 3);
					
					if (!identifier.empty())
					{
						if (this->m_currentDocComment == 0)
						{
							this->m_gotDocComment = true;
							this->m_currentDocComment = new class DocComment(identifier, this->m_parser->currentSourceFile(), this->m_currentLine);
							this->m_parser->add(this->m_currentDocComment);
						}
						else
							this->m_currentDocComment->addIdentifier(identifier);
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
					this->m_currentBlockComment = line.substr(index + 2);
					this->m_isInBlockComment = true;
					
					break;
				}
				else
				{
					std::string identifier = line.substr(index + 2, i - index - 2);
					
					if (!identifier.empty())
					{
						this->m_parser->add(new class Comment(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment));
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

void File::getDocComment(const std::string &line, std::string::size_type index)
{
	std::string docCommentItem = getToken(line, index, true);

	if (!docCommentItem.empty())
	{
		if (this->m_currentDocComment == 0)
		{
			this->m_currentDocComment = new DocComment(docCommentItem, this->m_parser->currentSourceFile(), this->m_currentLine);
			this->m_parser->add(this->m_currentDocComment);
			this->m_gotDocComment = true;
		}
		else
			this->m_currentDocComment->addIdentifier('\n' + docCommentItem); //parse before and modify, space characters are important!
	}
	else if (Vjassdoc::optionVerbose())
		std::cout << _("Detected empty documentation comment.") << std::endl;
}

void File::clearDocComment()
{
	if (this->m_gotDocComment)
	{
		this->m_currentDocComment = 0; //reset
		this->m_gotDocComment = false;
	}
}

inline void File::getKeyword(const std::string &line, std::string::size_type &index, bool isPrivate)
{
	this->m_parser->add(new Keyword(getToken(line, index), this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->m_currentLibrary, this->m_currentScope, isPrivate));
}

inline void File::getKey(const std::string &line, std::string::size_type &index, bool isPrivate)
{
	this->m_parser->add(new Key(getToken(line, index), this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->m_currentLibrary, this->m_currentScope, isPrivate));
}

bool File::getGlobal(const std::string &line, std::string::size_type &index, bool isPrivate, bool isPublic, bool isConstant, bool isStatic, bool isDelegate)
{
	if (!this->m_isInGlobals && (Vjassdoc::optionJass() || (!Vjassdoc::optionJass() && !this->m_isInGlobals && !this->m_isInInterface && !this->m_isInStruct)))
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
	
		if ((this->m_isInInterface || this->m_isInStruct) && identifier == expressionText[StaticExpression])
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

	if (Vjassdoc::optionJass() || (!this->m_isInStruct && !this->m_isInInterface))
		this->m_parser->add(new Global(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->m_currentLibrary, this->m_currentScope, isPrivate, isPublic, isConstant, type, value, size));
	else
	{
		this->m_parser->add(new Member(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->m_currentLibrary, this->m_currentScope, isPrivate, isPublic, isConstant, type, value, size, this->getCurrentContainer(), isStatic, isDelegate));
	}
	
	this->clearDocComment();
	
	return true;
}

bool File::getFunction(const std::string &line, std::string::size_type &index, bool isPrivate, bool isPublic, bool isConstant, bool isNative, bool isStatic, bool isStub)
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
	
		if (!isStatic && (this->m_isInStruct || this->m_isInInterface || this->m_isInModule) && identifier == expressionText[StaticExpression])
		{
			hasPrefix = true;
			isStatic = true;
			identifier = getToken(line, index);
		}

		if (!isStub && (this->m_isInStruct || this->m_isInInterface || this->m_isInModule) && identifier == expressionText[StubExpression]) //no statics and before constant?
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
	
		if ((this->m_isInStruct || this->m_isInInterface || this->m_isInModule) && identifier == expressionText[MethodExpression])
			hasPrefix = true;
	
		if (hasPrefix)
		{
			//no syntax checker, required for global identification
			if ((this->m_isInStruct || this->m_isInInterface || this->m_isInModule) && identifier != expressionText[MethodExpression])
				return false;
			else if (!this->m_isInStruct && !this->m_isInInterface && !this->m_isInModule && identifier != expressionText[FunctionExpression])
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

			class Parameter *parameter = new class Parameter(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, 0, typeExpression);
			parameters.push_back(parameter);
			this->m_parser->add(parameter);

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
		if (Vjassdoc::optionJass() || (!this->m_isInStruct && !this->m_isInInterface && !this->m_isInModule))
		{
			if (!Vjassdoc::optionFunctions() && !isNative)
			{
				if (Vjassdoc::optionVerbose())
					std::cout << _("Ignoring function space.") << std::endl;

				this->m_notRequiredSpace = FunctionExpression;
			}

			class Function *function = new Function(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->m_currentLibrary, this->m_currentScope, isPrivate, parameters, type, isPublic, isConstant, isNative);
			this->m_parser->add(function);
			this->m_currentFunction = function;
		}
		else
		{
			std::string defaultReturnValueExpression;

			//Do not expect endmethod in interfaces!
			if (!Vjassdoc::optionFunctions() && !this->m_isInInterface)
			{
				if (Vjassdoc::optionVerbose())
					std::cout << _("Ignoring method space.") << std::endl;

				this->m_notRequiredSpace = MethodExpression;
			}
			else
			{
				defaultReturnValueExpression = getToken(line, index);
				
				if (defaultReturnValueExpression == File::expressionText[File::DefaultsExpression])
					defaultReturnValueExpression = getToken(line, index);
				
				if (Vjassdoc::optionVerbose() && defaultReturnValueExpression.empty())
					std::cerr << _("Missing default return value expression.") << std::endl;
			}

			class Method *method = new Method(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->m_currentLibrary, this->m_currentScope, isPrivate, parameters, type, isPublic, isConstant, this->getCurrentContainer(), isStatic, isStub, isOperator, defaultReturnValueExpression);
			this->m_parser->add(method);
			
			if (!this->m_isInInterface) //interfaces have single line methods without endmethod
				this->m_currentFunction = method;
		}
	}
	else
		this->m_parser->add(new FunctionInterface(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->m_currentLibrary, this->m_currentScope, isPrivate, parameters, type));

	this->clearDocComment();
	
	return true;
}

void File::getCall(const std::string &line, std::string::size_type &index)
{
	std::string functionIdentifier = getToken(line, index);
	std::string::size_type position = functionIdentifier.find('(');
	
	if (Vjassdoc::optionSyntax() && position == std::string::npos)
	{
		this->m_parser->add(new SyntaxError(this->m_parser->currentSourceFile(), this->m_currentLine, _("Missing \"(\" character.")));
		
		return;
	}
	
	std::string argumentsPart = functionIdentifier.substr(0, position);
	functionIdentifier.erase(position);
	position = functionIdentifier.find_last_of('.');
	bool isExecuted = false;
	bool isEvaluated = false;
	
	if (position != std::string::npos)
	{
		std::string callExtension = functionIdentifier.substr(position);
		callExtension.erase(0, 1);
		
		if (callExtension == File::expressionText[File::ExecuteExpression])
			isExecuted = true;
		else if (callExtension == File::expressionText[File::EvaluateExpression])
			isEvaluated = true;
	}
	
	std::list<std::string> *argumentIdentifiers = new std::list<std::string>();
	
	for (std::string::size_type position = functionIdentifier.find_last_of('('); position != std::string::npos; position = functionIdentifier.find_last_of('('))
	{
		std::string::size_type endPosition = functionIdentifier.find_first_of(')');
		
		if (Vjassdoc::optionSyntax() && endPosition == std::string::npos)
		{
			this->m_parser->add(new SyntaxError(this->m_parser->currentSourceFile(), this->m_currentLine, _("Missing \")\" character.")));
			delete argumentIdentifiers;
			
			return;
		}
		
		std::string::size_type length = endPosition - position;
		std::string token = functionIdentifier.substr(position, length);
		functionIdentifier.erase(position, length);
		/// @todo This token does not have any brackets, parse arguments!
		
	}
	
	this->m_parser->add(new Call("",this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, functionIdentifier, argumentIdentifiers, isExecuted, isEvaluated));

	this->clearDocComment();
}


void File::getImplementation(const std::string &line, std::string::size_type &index)
{
	bool isOptional = false;
	std::string identifier = getToken(line, index);

	if (identifier == File::expressionText[File::OptionalExpression])
	{
		isOptional = true;
		identifier = getToken(line, index);
	}

	this->m_parser->add(new Implementation(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->getCurrentContainer(), identifier, isOptional));
	this->clearDocComment();
}

void File::getHook(const std::string &line, std::string::size_type &index)
{
	std::string functionExpression = getToken(line, index);
	std::string hookFunctionExpression = getToken(line, index);

	this->m_parser->add(new Hook(functionExpression, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, functionExpression, hookFunctionExpression));
	this->clearDocComment();
}

void File::getLibrary(const std::string &line, std::string::size_type &index, bool isOnce)
{
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

	class Library *library = new Library(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, isOnce, initializer, requirement, optionalRequirement);
	this->m_isInLibrary = true;
	this->m_currentLibrary = library;
	this->m_parser->add(library);
	this->clearDocComment();
}

void File::getScope(const std::string &line, std::string::size_type &index, bool isPrivate)
{
	std::string identifier = getToken(line, index);
	std::string initializerExpression;
	
	if (getToken(line, index) == expressionText[InitializerExpression])
		initializerExpression = getToken(line, index);

	class Scope *scope = new Scope(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->m_currentLibrary, isPrivate, initializerExpression);
	this->m_isInScope = true;
	this->m_currentScope = scope;
	this->m_parser->add(scope);
	this->clearDocComment();
}

void File::getInterface(const std::string &line, std::string::size_type &index, bool isPrivate)
{
	std::string identifier = getToken(line, index);
	class Interface *interface = new Interface(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->m_currentLibrary, this->m_currentScope, isPrivate);
	this->m_isInInterface = true;
	this->m_currentInterface = interface;
	this->m_parser->add(interface);
	this->clearDocComment();
}

void File::getStruct(const std::string &line, std::string::size_type &index, bool isPrivate)
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

	class Struct *usedStruct = new Struct(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->m_currentLibrary, this->m_currentScope, isPrivate, sizeExpression, extensionExpression);
	this->m_isInStruct = true;
	this->m_currentStruct = usedStruct;
	this->m_parser->add(usedStruct);
	this->clearDocComment();
}

void File::getModule(const std::string &line, std::string::size_type &index, bool isPrivate)
{
	std::string identifier = getToken(line, index);
	class Module *module = new Module(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->m_currentLibrary, this->m_currentScope, isPrivate);
	this->m_isInModule = true;
	this->m_currentModule = module;
	this->m_parser->add(module);
	this->clearDocComment();
}

void File::getLocal(const std::string &line, std::string::size_type &index)
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

	this->m_parser->add(new Local(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, this->m_currentFunction, typeExpression, valueExpression));
	this->clearDocComment();
}

std::string File::removeFirstSpace(const std::string &line, std::string::size_type index) const
{
	while (index < line.length() && (line[index] == ' ' || line[index] == '\t'))
		++index;

	return line.substr(index, line.length());
}

static std::string getTextMacroArguments(const std::string &line, std::string::size_type &index)
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

void File::getTextMacro(const std::string &line, std::string::size_type &index, bool isOnce)
{
	std::string identifier = getToken(line, index);
	//std::cout << "Start text macro " << identifier << std::endl;
	std::string args = getTextMacroArguments(line, index); //Nicht direkt übergeben, dann stimmt index nicht
	//std::cout << "With arguments " << args << std::endl;
	class TextMacro *textMacro = new TextMacro(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, isOnce, args);
	this->m_parser->add(textMacro);
	
	if (!Vjassdoc::optionTextmacros())
	{
		if (Vjassdoc::optionVerbose())
			std::cout << _("Ignoring text macro space.") << std::endl;

		this->m_notRequiredSpace = TextmacroExpression;
	}
	
	this->clearDocComment();
	this->m_isInTextMacro = true;
	this->m_currentTextMacro = textMacro;
}

void File::getTextMacroInstance(const std::string &line, std::string::size_type &index)
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
	this->m_parser->add(new TextMacroInstance(identifier, this->m_parser->currentSourceFile(), this->m_currentLine, this->m_currentDocComment, isOptional, args));
	this->clearDocComment();
}

std::list<std::string>* File::getLibraryRequirement(const std::string &line, std::string::size_type &index, std::list<bool> *optionalRequirement) const
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
	return (this->m_isInTextMacro || this->m_isInLibrary || this->m_isInScope || this->m_isInInterface || this->m_isInStruct || this->m_isInModule);
}

inline class Object* File::getCurrentContainer() const
{
	if (this->m_currentInterface != 0)
		return this->m_currentInterface;
	else if (this->m_currentStruct != 0)
		return this->m_currentStruct;
	else if (this->m_currentModule != 0)
		return this->m_currentModule;

	return 0;
}

}
