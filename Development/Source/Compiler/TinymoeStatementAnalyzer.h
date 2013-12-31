#ifndef VCZH_TINYMOESTATEMENTANALYZER
#define VCZH_TINYMOESTATEMENTANALYZER

#include "TinymoeDeclarationAnalyzer.h"
#include "TinymoeExpressionAnalyzer.h"

namespace tinymoe
{
	/*************************************************************
	Statement
	*************************************************************/

	class Statement : public CodeFragment
	{
	public:
		typedef shared_ptr<Statement>								Ptr;
		typedef vector<Ptr>											List;
		typedef map<GrammarSymbol::Ptr, Expression::Ptr>			GrammarExpressionMap;

		GrammarSymbol::Ptr				statementSymbol;
		InvokeExpression::Ptr			statementExpression;
		Statement::List					statements;
		GrammarExpressionMap			newVariables;
		GrammarExpressionMap			blockArguments;
	};

	/*************************************************************
	Symbol
	*************************************************************/

	class SymbolFunction
	{
	public:
		typedef shared_ptr<SymbolFunction>							Ptr;
		typedef weak_ptr<SymbolFunction>							WeakPtr;
		typedef vector<Ptr>											List;
		typedef map<GrammarSymbol::Ptr, FunctionFragment::Ptr>		SymbolFragmentMap;

		FunctionDeclaration::Ptr		function;					// the original function
		SymbolFunction::WeakPtr			multipleDispatchingRoot;	// the multiple-dispatching root declaration
		SymbolFragmentMap				arguments;					// all arguments
		Statement::Ptr					statement;					// the function body
	};

	class SymbolModule
	{
	public:
		typedef shared_ptr<SymbolModule>							Ptr;
		typedef vector<Ptr>											List;
		typedef weak_ptr<SymbolModule>								WeakPtr;
		typedef vector<WeakPtr>										WeakList;
		typedef map<GrammarSymbol::Ptr, Declaration::Ptr>			SymbolDeclarationMap;
		typedef map<Declaration::Ptr, SymbolFunction::Ptr>			DeclarationFunctionMap;

		struct ParsingFailedException{};

		CodeFile::Ptr					codeFile;					// the original code file
		Module::Ptr						module;						// the original module
		WeakList						usingSymbolModules;			// all referenced modules
		SymbolDeclarationMap			symbolDeclarations;			// map a grammar symbol to the creator declaration
		DeclarationFunctionMap			declarationFunctions;		// map a declaration to the symbol function

		bool							IsOverloading(GrammarSymbol::Ptr a, GrammarSymbol::Ptr b);
		bool							IsMultipleDispatchingChild(FunctionDeclaration::Ptr func);
		void							CheckOverloading(SymbolModule* modulea, GrammarSymbol::Ptr symbola, Declaration::Ptr decla, SymbolModule* moduleb, GrammarSymbol::Ptr symbolb, Declaration::Ptr declb, bool foreignCheck, CodeError::List& errors);
		void							BuildSymbols(CodeError::List& errors);									// sync step: build all declaration symbols
		void							BuildFunctions(CodeError::List& errors);								// sync step: build all symbol functions
		void							BuildFunctionLinkings(CodeError::List& errors);							// sync step: link all multiple-dispatching functions to it's parent

		void							FindOverridedSymbols(GrammarStack::Ptr stack, GrammarStackItem::Ptr item, GrammarSymbol::List& symbols);
		void							ParseBlock(CodeFile::Ptr codeFile, GrammarStack::Ptr stack, Statement::Ptr statement, int& lineIndex, int endLineIndex, CodeError::List& errors);
		void							BuildStatements(GrammarStack::Ptr stack, CodeError::List& errors);		// sync step: parse all statements
	};

	class SymbolAssembly
	{
	public:
		typedef shared_ptr<SymbolAssembly>		Ptr;

		SymbolModule::List				symbolModules;

		static SymbolAssembly::Ptr		Parse(vector<string>& codes, CodeError::List& errors);
	};
}

#endif