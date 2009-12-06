%{

#include "vjassdoc.hpp"
 
void yyerror(const char *str)
{
	Vjassdoc::parser->()->add(new SyntaxError(Vjassdoc::parser()->currentFile()->sourceFile(), Vjassdoc::parser()->currentFile()->lines(), str));
}
 
int yywrap()
{
	// open other files, this is called when file ends, return 1 if it's the end
        return 1;
} 

/*  
main()
{
        yyparse();
}
*/

%}

%token LDC
%token PP
%token LC
%token NEWLINE
%token DIGIT
%token HEXDIGIT
%token ID
%token SPACE
%token DCSTART
%token CSTART
%token CEND
%token STRINGSTART
%token STILLSTRINGA
%token STILLSTRINGB
%token STRINGDONE
%token FSTRINGSTART
%token FSTILLSTRINGA
%token FSTILLSTRINGB
%token FSTRINGDONE
%token UNITTYPEINT

%token DocComment
%token Comment
%token BlockComment
%token BlockDocComment
%token NewLine
%token Identifier
%token UnitTypeIntegerLiteral
%token IntegerLiteral
%token BooleanLiteral
%token RealLiteral
%token StringLiteral
%token NullKeyword
%token TypeKeyword
%token ConstantKeyword
%token NativeKeyword
%token FunctionKeyword
%token EndfunctionKeyword
%token GlobalsKeyword
%token EndglobalsKeyword
%token LocalKeyword
%token SetKeyword
%token CallKeyword
%token IfKeyword
%token ThenKeyword
%token ElseKeyword
%token ElseifKeyword
%token EndifKeyword
%token ReturnKeyword
%token LoopKeyword;
%token EndloopKeyword
%token ExitwhenKeyword
%token NothingKeyword
%token TakesKeyword
%token ReturnsKeyword
%token ExtendsKeyword
%token ArrayKeyword
%token AndKeyword
%token OrKeyword
%token Dot
%token Comma
%token LeftParenthesis
%token RightParenthesis
%token LeftSquaredBracked
%token RightSquaredBracked
%token SetOperator
%token MultiplicationOperator
%token DivisionOperator
%token PlusOperator
%token MinusOperator
%token LessOperator
%token GreaterOperator
%token EqualOperator
%token LessEqualOperator
%token GreaterEqualOperator
%token NotEqualOperator
%token VjassPreprocessor
%token JassppPreprocessor
%token MethodKeyword
%token EndmethodKeyword
%token PrivateKeyword
%token PublicKeyword
%token StaticKeyword
%token DebugKeyword
%token KeywordKeyword
%token LibraryKeyword
%token Library_onceKeyword
%token EndlibraryKeyword
%token ScopeKeyword
%token EndscopeKeyword
%token StructKeyword
%token EndstructKeyword
%token InterfaceKeyword
%token EndinterfaceKeyword
%token DelegateKeyword
%token StubKeyword
%token ModuleKeyword
%token EndmoduleKeyword
%token ImplementKeyword
%token HookKeyword
%token KeyKeyword
%token ImportKeyword
%token DovjassinitKeyword
%token InjectKeyword
%token EndinjectKeyword
%token NovjassKeyword
%token EndnovjassKeyword
%token VjassKeyword
%token EndvjassKeyword
%token ZincKeyword
%token EndzincKeyword
%token ExternalblockKeyword
%token EndexternalblockKeyword
%token LoaddataKeyword
%token ExternalKeyword
%token TextmacroKeyword
%token TextmacroonceKeyword
%token EndtextmacroKeyword
%token RuntextmacroKeyword
%token RequiresKeyword
%token NeedsKeyword
%token UsesKeyword
%token InitializerKeyword
%token DefaultsKeyword
%token OptionalKeyword
%token SuperKeyword
%token ThistypeKeyword
%token OperatorKeyword
%token ExecuteKeyword
%token EvaluateKeyword
%token GlobalKeyword

%token JassppKeyword;
%token EndjassppKeyword;
%token IncludeKeyword;

%%

IncreaseLine				: NewLine
					{
						Vjassdoc::parser()->currentFile()->addLines(1);
					}
					;

Preprocessor				: VjassPreprocessor
					| JassppPreprocessor
					;

FileImportLanguage			: ZincKeyword
					| VjassKeyword
					| JassppKeyword
					|
					{
					}
					;

FileImport				: Preprocessor ImportKeyword FileImportLanguage StringLiteral
					| Preprocessor IncludeKeyword FileImportLanguage StringLiteral
					;

VjassScopePrefix			: PublicKeyword
					| PrivateKeyword
					|
					;

VjassScope				: VjassScopePrefix ScopeKeyword Identifier
					| VjassScopePrefix ScopeKeyword Identifier InitializerKeyword Identifier
					;

LibraryPrefix				: LibraryKeyword
					| Library_onceKeyword
					;

LibraryRequirementPrefix		: RequiresKeyword
					| NeedsKeyword
					| UsesKeyword
					;

LibraryRequirement			: Identifier
					| Identifier Comma LibraryRequirement
					| OptionalKeyword Comma LibraryRequirement
					;

Library					: LibraryPrefix Identifier
					| LibraryPrefix Identifier InitializerKeyword Identifier
					| LibraryPrefix Identifier LibraryRequirementPrefix LibraryRequirement
					| LibraryPrefix Identifier InitializerKeyword Identifier LibraryRequirementPrefix LibraryRequirement
					;

EndLibrary				: EndlibraryKeyword
					;

Global					: VjassScopePrefix GlobalKeyword Identifier ArrayKeyword Identifier
					| VjassScopePrefix GlobalKeyword Identifier Identifier
					;

MethodPrefix				: StaticKeyword
					|
					;


Parameter				: Identifier Identifier
					;

Parameters				: Parameter
					| Parameter Comma Parameter
					| NothingKeyword
					;

ReturnValue				: Identifier
					| NothingKeyword
					;


FunctionInterfaceDeclaration		: VjassScopePrefix FunctionKeyword InterfaceKeyword Identifier TakesKeyword Parameters ReturnsKeyword ReturnValue
					;

VariableIdentifier			: Identifier
					| Identifier Dot VariableIdentifier
					;

FunctionIdentifier			: Identifier
					| Identifier Dot FunctionIdentifier
					;

FunctionCallValues			: Value
					| Value Comma FunctionCallValues
					;


FunctionCall				: FunctionIdentifier LeftParenthesis FunctionCallValues RightParenthesis
					;

IntegerOperator				: MultiplicationOperator
					| DivisionOperator
					| PlusOperator
					| MinusOperator
					;

IntegerValue				: IntegerLiteral
					| UnitTypeIntegerLiteral
					| IntegerLiteral IntegerOperator IntegerValue
					| VariableIdentifier IntegerOperator IntegerValue
					| FunctionCall IntegerOperator IntegerValue
					| LeftParenthesis IntegerValue RightParenthesis
					;

RealValue				: RealLiteral
					| IntegerLiteral
					| RealLiteral IntegerOperator RealValue
					| VariableIdentifier IntegerOperator RealValue
					| FunctionCall IntegerOperator RealValue
					| LeftParenthesis RealValue RightParenthesis
					;

StringOperator				: PlusOperator
					;

StringValue				: StringLiteral
					| NullKeyword
					| StringLiteral StringOperator StringValue
					| NullKeyword StringOperator StringValue
					| LeftParenthesis StringValue RightParenthesis
					;

EqualityComparisonOperator		: EqualOperator
					| NotEqualOperator
					;

ComparisonOperator			: EqualityComparisonOperator
					| LessOperator
					| GreaterOperator
					| LessEqualOperator
					| GreaterEqualOperator
					;

BooleanValue				: BooleanLiteral
					| VariableIdentifier
					| FunctionCall
					| IntegerValue ComparisonOperator IntegerValue
					| RealValue ComparisonOperator RealValue
					| StringValue EqualityComparisonOperator StringValue
					| BooleanValue EqualityComparisonOperator BooleanValue
					| LeftParenthesis BooleanValue RightParenthesis
					;

CodeValue				: NullKeyword
					| VariableIdentifier
					| FunctionCall
					| FunctionIdentifier
					| LeftParenthesis CodeValue RightParenthesis
					;

HandleValue				: NullKeyword
					| VariableIdentifier
					| FunctionCall
					| LeftParenthesis HandleValue RightParenthesis
					;

Value					: IntegerValue
					| RealValue
					| StringValue
					| BooleanValue
					| CodeValue
					| HandleValue
					;

SetStatement				: SetKeyword VariableIdentifier SetOperator Value
					;

CallStatement				: CallKeyword FunctionCall
					;

IfStatement				: IfKeyword BooleanValue ThenKeyword
					;

ElseifStatement				: ElseifKeyword BooleanValue ThenKeyword
					;

ExitwhenStatement			: ExitwhenKeyword BooleanValue
					;

ReturnStatement				: ReturnKeyword Value
					;

Statement				: SetStatement
					| CallStatement
					| IfStatement
					| ElseifStatement
					| EndifKeyword
					| LoopKeyword
					| ExitwhenStatement
					| EndloopKeyword
					| ReturnStatement
					| DebugKeyword Statement
					;
					

FunctionBody				: Statement IncreaseLine FunctionBody
					| Statement IncreaseLine
					| IncreaseLine
					;

FunctionDeclaration			: VjassScopePrefix FunctionKeyword Identifier TakesKeyword Parameters ReturnsKeyword ReturnValue IncreaseLine FunctionBody EndfunctionKeyword
					;

MethodDeclaration			: VjassScopePrefix MethodPrefix MethodKeyword Identifier TakesKeyword Parameters ReturnsKeyword ReturnValue IncreaseLine FunctionBody EndmethodKeyword ;

InterfaceMethodReturnValue		: ReturnValue
					| ReturnValue SetOperator Identifier
					;

InterfaceMethodDeclaration		: VjassScopePrefix MethodPrefix MethodKeyword Identifier TakesKeyword Parameters ReturnsKeyword InterfaceMethodReturnValue
					;

%%
