#ifndef VCZH_TINYMOEDECLARATIONANALYZER
#define VCZH_TINYMOEDECLARATIONANALYZER

#include "TinymoeLexicalAnalyzer.h"

namespace tinymoe
{
	class SymbolName
	{
	public:
		typedef shared_ptr<SymbolName>				Ptr;
		typedef vector<Ptr>							List;

		vector<CodeToken>					identifiers;

		SymbolName();
		virtual ~SymbolName();
	};

	class Declaration abstract
	{
	public:
		typedef shared_ptr<Declaration>				Ptr;
		typedef vector<Ptr>							List;

		Declaration();
		virtual ~Declaration();
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

	class FunctionCategory
	{
	public:
		typedef shared_ptr<FunctionCategory>		Ptr;
		
		SymbolName::Ptr						signalName;
		SymbolName::Ptr						categoryName;
		SymbolName::List					followCategories;
		bool								closable = false;

		FunctionCategory();
		virtual ~FunctionCategory();
	};

	class FunctionCps
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

		class Fragment abstract
		{
		public:
			typedef shared_ptr<Fragment>			Ptr;
			typedef vector<Ptr>						List;

			Fragment();
			virtual ~Fragment();
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

		class VariableArgumentFragment abstract : public ArgumentFragment
		{
		public:
			SymbolName::Ptr					name;

			VariableArgumentFragment();
		};

		class FunctionArgumentFragment abstract : public ArgumentFragment
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

		FunctionDeclaration();
	};
}

#endif