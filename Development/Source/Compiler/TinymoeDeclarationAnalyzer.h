#ifndef VCZH_TINYMOEDECLARATIONANALYZER
#define VCZH_TINYMOEDECLARATIONANALYZER

#include "TinymoeLexicalAnalyzer.h"

namespace tinymoe
{
	/*************************************************************
	Common
	*************************************************************/

	class SymbolName : public CodeFragment
	{
	public:
		typedef shared_ptr<SymbolName>				Ptr;
		typedef vector<Ptr>							List;

		vector<CodeToken>					identifiers;

		static bool							ConsumeToken(CodeToken::List::iterator& it, CodeToken::List::iterator end, CodeTokenType tokenType, const string& content, CodeToken ownerToken, CodeError::List& errors);
		static SymbolName::Ptr				ParseToEnd(CodeToken::List::iterator it, CodeToken::List::iterator end, const string& ownerName, CodeToken ownerToken, CodeError::List& errors);
		static SymbolName::Ptr				ParseToFarest(CodeToken::List::iterator& it, CodeToken::List::iterator end, const string& ownerName, CodeToken ownerToken, CodeError::List& errors);
	};

	/*************************************************************
	Function Components
	*************************************************************/

	enum class FunctionDeclarationType
	{
		Phrase,				// expression, cannot be an statement
		Sentence,			// statement
		Block,				// block statement
	};

	enum class FunctionArgumentType
	{
		Normal,				// a normal function argument
		List,				// a tuple marshalled as array
		Argument,			// for block only, represents an argument to the block body
		Expression,			// for sentence and block only, represnets a re-evaluable expression
		Assignable,			// for sentence and block only, represnets a assignable expression
	};

	class FunctionCps : public CodeFragment
	{
	public:
		typedef shared_ptr<FunctionCps>				Ptr;

		SymbolName::Ptr						stateName;			// for accessing the CPS state object
		SymbolName::Ptr						continuationName;	// (optional) for accessing the CPS continuation function, statement only
		
		static FunctionCps::Ptr				Parse(CodeFile::Ptr codeFile, CodeError::List& errors, int& lineIndex);
	};

	class FunctionCategory : public CodeFragment
	{
	public:
		typedef shared_ptr<FunctionCategory>		Ptr;
		
		SymbolName::Ptr						signalName;			// (optional) for non-first block (like else if, catch, etc) to access the result from the previous block
		SymbolName::Ptr						categoryName;		// (optional) category for this block
		SymbolName::List					followCategories;	// (optional) categories to follow
		bool								closable = false;	// (optional) true means this block can be the end of a block series
		
		static FunctionCategory::Ptr		Parse(CodeFile::Ptr codeFile, CodeError::List& errors, int& lineIndex);
	};

	class FunctionFragment abstract : public CodeFragment
	{
	public:
		typedef shared_ptr<FunctionFragment>	Ptr;
		typedef vector<Ptr>						List;
	};

	/*************************************************************
	Declarations
	*************************************************************/

	class Declaration abstract : public CodeFragment
	{
	public:
		typedef shared_ptr<Declaration>				Ptr;
		typedef vector<Ptr>							List;
	};

	class SymbolDeclaration : public Declaration
	{
	public:
		typedef shared_ptr<SymbolDeclaration>		Ptr;

		SymbolName::Ptr						name;

		static SymbolDeclaration::Ptr		Parse(CodeFile::Ptr codeFile, CodeError::List& errors, int& lineIndex);
	};

	class TypeDeclaration : public Declaration
	{
	public:
		typedef shared_ptr<TypeDeclaration>			Ptr;

		SymbolName::Ptr						name;
		SymbolName::Ptr						parent;				// (optional)
		SymbolName::List					fields;
		
		static TypeDeclaration::Ptr			Parse(CodeFile::Ptr codeFile, CodeError::List& errors, int& lineIndex);
	};

	class FunctionDeclaration : public Declaration
	{
	public:
		typedef shared_ptr<FunctionDeclaration>		Ptr;
		
		FunctionCps::Ptr					cps;				// (optional) for statement and block only
		FunctionCategory::Ptr				category;			// (optional) for block only
		FunctionDeclarationType				type = FunctionDeclarationType::Phrase;
		FunctionFragment::Ptr				bodyName;			// (optional) argument for block body, block only
		FunctionFragment::List				name;				// function name and arguments
		SymbolName::Ptr						alias;				// (optional) a name that referencing this function
		int									beginLineIndex = -1;
		int									codeLineIndex = -1;
		int									endLineIndex = -1;
		
		static FunctionDeclaration::Ptr		Parse(CodeToken::List::iterator& it, CodeToken::List::iterator end, FunctionDeclaration::Ptr decl, CodeToken ownerToken, CodeError::List& errors);
		static FunctionDeclaration::Ptr		Parse(CodeFile::Ptr codeFile, CodeError::List& errors, int& lineIndex);
	};

	/*************************************************************
	Function Fragments
	*************************************************************/

	class ArgumentFragment abstract : public FunctionFragment
	{
	public:
		FunctionArgumentType				type = FunctionArgumentType::Normal;

		static ArgumentFragment::Ptr		Parse(CodeToken::List::iterator& it, CodeToken::List::iterator end, CodeToken ownerToken, CodeError::List& errors);
	};

	class NameFragment : public FunctionFragment
	{
	public:
		SymbolName::Ptr						name;				// part of the function name
	};

	class VariableArgumentFragment : public ArgumentFragment
	{
	public:
		FunctionArgumentType				type;				// type of the form
		SymbolName::Ptr						name;				// name of the argument
		SymbolName::Ptr						receivingType;		// (optional) receiving type for Normal argument only to do multiple dispatching
	};

	class FunctionArgumentFragment : public ArgumentFragment
	{
	public:
		FunctionDeclaration::Ptr			declaration;		// declaration for the argument representing a function
	};

	/*************************************************************
	Module
	*************************************************************/

	class Module : public CodeFragment
	{
	public:
		typedef shared_ptr<Module>			Ptr;

		SymbolName::Ptr						name;				// name of the module
		SymbolName::List					usings;				// other modules whose declarations can be referred in this module
		Declaration::List					declarations;		// declarations in this module

		static Module::Ptr					Parse(CodeFile::Ptr codeFile, CodeError::List& errors);
	};
}

#endif