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
		Phrase,
		Sentence,
		Block,
	};

	enum class FunctionArgumentType
	{
		Normal,
		Argument,
		Expression,
	};

	class FunctionCategory : public CodeFragment
	{
	public:
		typedef shared_ptr<FunctionCategory>		Ptr;
		
		SymbolName::Ptr						signalName;
		SymbolName::Ptr						categoryName;
		SymbolName::List					followCategories;
		bool								closable = false;
	};

	class FunctionCps : public CodeFragment
	{
	public:
		typedef shared_ptr<FunctionCps>				Ptr;

		SymbolName::Ptr						stateName;
		SymbolName::Ptr						continuationName;
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
		SymbolName::List					fields;
	};

	class FunctionDeclaration : public Declaration
	{
	public:
		typedef shared_ptr<FunctionDeclaration>		Ptr;
		
		FunctionCategory::Ptr				category;
		FunctionCps::Ptr					cps;
		FunctionDeclarationType				type = FunctionDeclarationType::Phrase;
		FunctionFragment::List				name;
		SymbolName::Ptr						alias;
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
		FunctionArgumentType			type = FunctionArgumentType::Normal;
	};

	class NameFragment : public FunctionFragment
	{
	public:
		SymbolName::Ptr					name;
	};

	class VariableArgumentFragment : public ArgumentFragment
	{
	public:
		SymbolName::Ptr					name;
	};

	class FunctionArgumentFragment : public ArgumentFragment
	{
	public:
		FunctionDeclaration::Ptr		declaration;
	};

	/*************************************************************
	Module
	*************************************************************/

	class Module : public CodeFragment
	{
	public:
		typedef shared_ptr<Module>			Ptr;

		SymbolName::Ptr						name;
		SymbolName::List					usings;
		Declaration::List					declarations;

		static Module::Ptr					Parse(CodeFile::Ptr codeFile, CodeError::List& errors);
	};
}

#endif