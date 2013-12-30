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
		typedef shared_ptr<Statement>			Ptr;
		typedef vector<Ptr>						List;

		GrammarSymbol::Ptr				statementSymbol;
		InvokeExpression::Ptr			statementExpression;
		Statement::List					statements;
		GrammarSymbol::List				newVariables;
		GrammarSymbol::List				blockArguments;
	};

	/*************************************************************
	Symbol
	*************************************************************/

	class SymbolModule
	{
	public:
		typedef shared_ptr<SymbolModule>					Ptr;
		typedef vector<Ptr>									List;
		typedef weak_ptr<SymbolModule>						WeakPtr;
		typedef vector<WeakPtr>								WeakList;
		typedef map<Declaration::Ptr, Statement::Ptr>		DeclarationStatementMap;
		typedef map < Declaration::Ptr, Declaration::Ptr>	DeclarationDeclarationMap;
		typedef map<GrammarSymbol::Ptr, Declaration::Ptr>	SymbolDeclarationMap;
		
		CodeFile::Ptr					codeFile;					// the original code file
		Module::Ptr						module;						// the original module
		WeakList						usingSymbolModules;			// all referenced modules
		DeclarationStatementMap			declarationStatements;		// map a declaration to function body
		DeclarationDeclarationMap		declarationParents;			// map a multiple-dispatching declaration to it's parent
		SymbolDeclarationMap			symbolDeclarations;			// map a grammar symbol to the creator declaration

		void							Build(GrammarStack::Ptr stack, CodeError::List& errors);
	};

	class SymbolAssembly
	{
	public:
		typedef shared_ptr<SymbolAssembly>		Ptr;

		SymbolModule::List				symbolModules;

		static string					GetStandardLibraryCode();

		void							InstallModules(Module::List& modules, CodeFile::List& codeFiles, CodeError::List& errors);
	};
}

#endif