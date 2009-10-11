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

#ifndef VJASSDOC_FILE_HPP
#define VJASSDOC_FILE_HPP

#include <list>
#include <string>
#include <fstream>

namespace vjassdoc
{

class Parser;
class SourceFile;
class DocComment;
class Library;
class Scope;
class Interface;
class Struct;
class Module;
class Function;

/**
* A Jass or vJass code file which is treated by a @class Parser instance.
* This class knows the whole Jass and vJass syntax and is able to parse it.
* Parsing runs automatically after creation of an instance in constructor.
*/
class File
{
	public:
		enum Expression
		{
			DocCommentExpression, //This tool
			CommentExpression,
			BlockDocCommentExpression, //has to be checked before block comment expression!
			EndBlockDocCommentExpression,
			TypeExpression,
			ConstantExpression,
			NativeExpression,
			FunctionExpression,
			EndfunctionExpression,
			GlobalsExpression,
			EndglobalsExpression,
			LocalExpression,
			SetExpression,
			CallExpression,
			IfExpression,
			ElseExpression,
			ElseifExpression,
			EndifExpression,
			ReturnExpression,
			LoopExpression,
			EndloopExpression,
			ExitwhenExpression,
			//vJass stand-alone expressions
			PreprocessorExpression,
			MethodExpression, //Operator is not required because the syntax is "method operator..."
			EndmethodExpression, //Required for function blocks, there can not be custom types
			PrivateExpression,
			PublicExpression,
			StaticExpression,
			DebugExpression,
			KeywordExpression,
			LibraryExpression,
			Library_onceExpression,
			EndlibraryExpression,
			ScopeExpression,
			EndscopeExpression,
			StructExpression,
			EndstructExpression,
			InterfaceExpression,
			EndinterfaceExpression,
			DelegateExpression,
			StubExpression,
			ModuleExpression,
			EndmoduleExpression,
			ImplementExpression,
			HookExpression,
			BlockCommentExpression,
			EndBlockCommentExpression,
			KeyExpression,
			//Jass none-start-line expressions
			NothingExpression,
			TakesExpression,
			ReturnsExpression,
			ExtendsExpression,
			ArrayExpression,
			//vJass none-start-line expressions
			ImportExpression,
			DovjassinitExpression,
			InjectExpression,
			EndinjectExpression,
			NovjassExpression,
			EndnovjassExpression,
			LoaddataExpression,
			ExternalExpression,
			TextmacroExpression,
			TextmacroonceExpression,
			EndtextmacroExpression,
			RuntextmacroExpression, //An instance of the macro
			RequiresExpression,
			NeedsExpression,
			UsesExpression,
			InitializerExpression,
			DefaultsExpression,
			OptionalExpression,
			SuperExpression,
			ThistypeExpression,
			OperatorExpression,
			MaxExpressions,
			NoExpression,
			InvalidExpression,
			CustomExpression
		};
		
		static const char *expressionText[File::MaxExpressions];
		
		File(const std::string &filePath);
		
	private:
		const std::string filePath;

		File::Expression notRequiredSpace;
		bool isInGlobals;
		bool isInLibrary;
		bool isInScope;
		bool isInInterface;
		bool isInStruct;
		bool isInModule;
		bool isInBlockComment;
		bool isInBlockDocComment;

		unsigned int currentLine;
		class DocComment *currentDocComment;
		class Library *currentLibrary;
		class Scope *currentScope;
		//containers
		class Interface *currentInterface;
		class Struct *currentStruct;
		class Module *currentModule;
		class Function *currentFunction;
		std::string currentBlockComment;
		std::string currentBlockDocComment;

		bool gotDocComment;

		std::ifstream fin;

		File::Expression getFirstLineExpression(std::string &line, unsigned int &index);
		void truncateComments(std::string &line, unsigned int index);
		void getDocComment(const std::string &line, unsigned int index);
		void getBlockDocComment(const std::string &line);
		void clearDocComment();
		void getKeyword(const std::string &line, unsigned int &index, bool isPrivate);
		void getKey(const std::string &line, unsigned int &index, bool isPrivate);
		bool getGlobal(const std::string &line, unsigned int &index, bool isPrivate, bool isPublic, bool isConstant, bool isStatic, bool isDelegate);
		bool getFunction(const std::string  &line, unsigned int &index, bool isPrivate, bool isPublic, bool isConstant, bool isNative, bool isStatic, bool isStub);
		void getImplementation(const std::string &line, unsigned int &index);
		void getHook(const std::string &line, unsigned int &index);
		void getLibrary(const std::string &line, unsigned int &index, bool isOnce);
		void getScope(const std::string &line, unsigned int &index, bool isPrivate);
		void getInterface(const std::string &line, unsigned int &index, bool isPrivate);
		void getStruct(const std::string &line, unsigned int &index, bool isPrivate);
		void getModule(const std::string &line, unsigned int &index, bool isPrivate);
		void getLocal(const std::string &line, unsigned int &index);
		std::string removeFirstSpace(const std::string &line, unsigned int index) const;
		void getTextMacro(const std::string &line, unsigned int &index, bool isOnce);
		void getTextMacroInstance(const std::string &line, unsigned int &index);
		std::list<std::string>* getLibraryRequirement(const std::string &line, unsigned int &index, std::list<bool> *optionalRequirement) const;
		bool isInVjassBlock() const;
		class Object* getCurrentContainer() const;
};

}

#endif