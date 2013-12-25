#ifndef VCZH_TINYMOEDECLARATIONANALYZER
#define VCZH_TINYMOEDECLARATIONANALYZER

#include "TinymoeLexicalAnalyzer.h"

namespace tinymoe
{
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

		SymbolName();
	};

	class Declaration abstract : public CodeFragment
	{
	public:
		typedef shared_ptr<Declaration>				Ptr;
		typedef vector<Ptr>							List;

		Declaration();
	};

	/*************************************************************
	Declarations
	*************************************************************/

	class SymbolDeclaration : public Declaration
	{
	public:
		SymbolName::Ptr						name;

		SymbolDeclaration();
	};

	class TypeDeclaration : public Declaration
	{
	public:
		SymbolName::Ptr						name;
		SymbolName::List					fields;

		TypeDeclaration();
	};

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

		FunctionCategory();
	};

	class FunctionCps : public CodeFragment
	{
	public:
		typedef shared_ptr<FunctionCps>				Ptr;

		SymbolName::Ptr						stateName;
		SymbolName::Ptr						continuationName;

		FunctionCps();
	};

	class FunctionDeclaration : public Declaration
	{
	public:
		typedef shared_ptr<FunctionDeclaration>		Ptr;

		//-----------------------------------------------------

		class Fragment abstract : public CodeFragment
		{
		public:
			typedef shared_ptr<Fragment>			Ptr;
			typedef vector<Ptr>						List;

			Fragment();
		};

		class ArgumentFragment abstract : public Fragment
		{
		public:
			FunctionArgumentType			type = FunctionArgumentType::Normal;

			ArgumentFragment();
		};

		//-----------------------------------------------------

		class NameFragment : public Fragment
		{
		public:
			SymbolName::Ptr					name;

			NameFragment();
		};

		class VariableArgumentFragment : public ArgumentFragment
		{
		public:
			SymbolName::Ptr					name;

			VariableArgumentFragment();
		};

		class FunctionArgumentFragment : public ArgumentFragment
		{
		public:
			FunctionDeclaration::Ptr		declaration;

			FunctionArgumentFragment();
		};
	public:
		FunctionDeclarationType				type = FunctionDeclarationType::Phrase;
		Fragment::List						name;
		FunctionCategory::Ptr				category;
		FunctionCps::Ptr					cps;
		int									beginLineIndex = -1;
		int									codeLineIndex = -1;
		int									endLineIndex = -1;

		FunctionDeclaration();
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

		Module();

		static Module::Ptr					Parse(CodeFile::Ptr codeFile, CodeError::List& errors);
	};
}

#endif