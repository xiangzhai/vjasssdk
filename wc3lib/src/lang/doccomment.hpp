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

#ifndef WC3LIB_LANG_DOCCOMMENT_HPP
#define WC3LIB_LANG_DOCCOMMENT_HPP

#include <list>

#include "object.hpp"

namespace wc3lib
{
	
namespace lang
{

class DocComment : public Object
{
	public:
		class List : public Object::List
		{
			public:
				List();
				virtual ~List();			
#ifdef HTML
				virtual const std::string& htmlCategoryName() const = 0;
				virtual const std::string& htmlFolderName() const = 0;
#endif

				
			protected:
#ifdef SQLITE
				virtual std::string sqlTableName() const;
				virtual std::size_t sqlColumns() const;
				virtual const std::string& sqlColumnDataType(std::size_t column) const throw (class Exception);
				virtual const std::string& sqlColumnName(std::size_t column) const throw (class Exception);
#endif
		};

		DocComment(const std::string &identifier, class SourceFile *sourceFile, unsigned int line);
#ifdef SQLITE		
		DocComment(std::vector<Object::SqlValueDataType> &columnVector);
#endif
		virtual ~DocComment();

		virtual void init();
#ifdef SQLITE
		virtual const std::string& sqlValue(std::size_t column) const;
#endif
#ifdef HTML
		virtual void writeHtmlPageNavigation(std::ostream &ostream) const;
		virtual void writeHtmlPageContent(std::ostream &ostream) const;
#endif

#ifdef SQLITE
		virtual const std::string& sqlStatement() const;
#endif
		void setObject(class Object *object); //Just used by the Object class.
		const std::string& briefDescription() const;
		const class Object* object() const;
		const std::list<std::string>& authors() const;
		const std::list<class Object*>& seeObjects() const;
		const std::list<std::string>& todos() const;

	protected:
		enum Keyword
		{
			CommentKeyword,
			KeywordKeyword,
			TextMacroKeyword,
			TextMacroInstanceKeyword,
			TypeKeyword,
			LocalKeyword,
			GlobalKeyword,
			MemberKeyword,
			ParameterKeyword,
			FunctionInterfaceKeyword,
			FunctionKeyword,
			MethodKeyword,
			ImplementationKeyword,
			InterfaceKeyword,
			StructKeyword,
			ModuleKeyword,
			ScopeKeyword,
			LibraryKeyword,
			SourceFileKeyword,
			DocCommentKeyword,
			ArgKeyword,
			AttentionKeyword,
			AuthorKeyword,
			CallgraphKeyword,
			CodeKeyword,
			DotKeyword,
			ElseKeyword,
			EndcodeKeyword,
			EndcondKeyword,
			EnddotKeyword,
			EndhtmlonlyKeyword,
			EndifKeyword,
			EndlatexonlyKeyword,
			EndlinkKeyword,
			EndmanonlyKeyword,
			EndverbatimKeyword,
			EndxmlonlyKeyword,
			FBracket0Keyword, //[
			FBracket1Keyword, //]
			F$Keyword,
			HideinitializerKeyword,
			HtmlonlyKeyword,
			InternalKeyword,
			InvariantKeyword,
			TildKeyword, //~
			AtKeyword,
			$Keyword,
			BackslashKeyword,
			SharpKeyword,
			LatexonlyKeyword,
			LiKeyword,
			ManonlyKeyword,
			NKeyword,
			NosubgroupingKeyword,
			NoteKeyword,
			OnlyKeyword,
			PostKeyword,
			PreKeyword,
			RemarksKeyword,
			ReturnKeyword,
			ReturnsKeyword,
			SaKeyword,
			SeeKeyword,
			ShowinitializerKeyword,
			SinceKeyword,
			TestKeyword,
			TodoKeyword,
			VerbatimKeyword,
			WarningKeyword,
			XmlonlyKeyword,
			AddtogroupKeyword,
			AKeyword,
			AnchorKeyword,
			BKeyword,
			CKeyword,
			CondKeyword,
			CopydocKeyword,
			DefKeyword,
			DontincludeKeyword,
			DotfileKeyword,
			EKeyword,
			ElseifKeyword,
			EmKeyword,
			ExampleKeyword,
			FileKeyword,
			HtmlincludeKeyword,
			IfKeyword,
			IfnotKeyword,
			IncludeKeyword,
			LinkKeyword,
			PKeyword,
			PackageKeyword,
			RefKeyword,
			RelatesalsoKeyword,
			RelatesKeyword,
			RetvalKeyword,
			VerbincludeKeyword,
			VersionKeyword,
			XrefitemKeyword,
			ParamKeyword,
			ImageKeyword,
			DefgroupKeyword,
			PageKeyword,
			ParagraphKeyword,
			SectionKeyword,
			SubsectionKeyword,
			SubsubsectionKeyword,
			WeakgroupKeyword,
			AddindexKeyword,
			BriefKeyword,
			BugKeyword,
			DateKeyword,
			DeprecatedKeyword,
			FnKeyword,
			IngroupKeyword,
			LineKeyword,
			MainpageKeyword,
			NameKeyword,
			OverloadKeyword,
			ParKeyword,
			ShortKeyword,
			SkipKeyword,
			SkiplineKeyword,
			UntilKeyword,
			VarKeyword,
			MaxKeywords
		};
		
#ifdef HTML
		virtual const std::string& htmlPageName() const;
#endif
		
#ifdef SQLITE
		static const std::size_t sqlMaxAuthors;
		static const std::size_t sqlMaxSeeObjects;
#endif
		static const char *keyword[DocComment::MaxKeywords];
	
		std::string m_briefDescription;
		class Object *m_object;
		std::list<std::string> m_authors;
		std::list<class Object*> m_seeObjects;
		std::list<std::string> m_todos;
	
		class DocComment* docComment() const; //Do not use
};

inline void DocComment::setObject(class Object *object)
{
	this->m_object = object;
}

inline const std::string& DocComment::briefDescription() const
{
	return this->m_briefDescription;
}

inline const class Object* DocComment::object() const
{
	return this->m_object;
}

inline const std::list<std::string>& DocComment::authors() const
{
	return this->m_authors;
}

inline const std::list<class Object*>& DocComment::seeObjects() const
{
	return this->m_seeObjects;
}

inline const std::list<std::string>& DocComment::todos() const
{
	return this->m_todos;
}

}

}

#endif
