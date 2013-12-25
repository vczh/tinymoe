#ifndef VCZH_TINYMOEDECLARATIONANALYZER
#define VCZH_TINYMOEDECLARATIONANALYZER

#include "TinymoeLexicalAnalyzer.h"

namespace tinymoe
{
	/*************************************************************
	Common
	*************************************************************/

	class CodeFragment
	{
	public:
		CodeToken							begin;
		CodeToken							end;

		CodeFragment();
		virtual ~CodeFragment();
	};

	class SymbolName : public CodeFragment
	{
	public:
		typedef shared_ptr<SymbolName>				Ptr;
		typedef vector<Ptr>							List;

		vector<CodeToken>					identifiers;
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
		Argument,			// for block only, represents an argument to the block body
		Expression,			// for block only, represnets a re-evaluable expression
	};

	class FunctionCategory : public CodeFragment
	{
	public:
		typedef shared_ptr<FunctionCategory>		Ptr;
		
		SymbolName::Ptr						signalName;			// for non-first block (like else if, catch, etc) to access the result from the previous block
		SymbolName::Ptr						categoryName;		// category for this block
		SymbolName::List					followCategories;	// categories to follow
		bool								closable = false;	// true means this block can be the end of a block series
	};

	class FunctionCps : public CodeFragment
	{
	public:
		typedef shared_ptr<FunctionCps>				Ptr;

		SymbolName::Ptr						stateName;			// for accessing the CPS state object
		SymbolName::Ptr						continuationName;	// for accessing the CPS continuation function, statement only
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
		SymbolName::Ptr						name;
	};

	class TypeDeclaration : public Declaration
	{
	public:
		SymbolName::Ptr						name;
		SymbolName::Ptr						parent;				// optional
		SymbolName::List					fields;
	};

	class FunctionDeclaration : public Declaration
	{
	public:
		typedef shared_ptr<FunctionDeclaration>		Ptr;
		
		FunctionCategory::Ptr				category;			// for block only
		FunctionCps::Ptr					cps;				// for statement and block only
		FunctionDeclarationType				type = FunctionDeclarationType::Phrase;
		FunctionFragment::List				name;
		SymbolName::Ptr						alias;				// a name that referencing this function
		int									beginLineIndex = -1;
		int									codeLineIndex = -1;
		int									endLineIndex = -1;
	};

	/*************************************************************
	Function Fragments
	*************************************************************/

	class ArgumentFragment abstract : public FunctionFragment
	{
	public:
		FunctionArgumentType				type = FunctionArgumentType::Normal;
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
		SymbolName::Ptr						receivingType;		// receiving type for Normal argument only to do multiple dispatching
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