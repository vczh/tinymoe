#include "TinymoeStatementAnalyzer.h"

namespace tinymoe
{
	/*************************************************************
	SymbolModule
	*************************************************************/

	void SymbolModule::Build(GrammarStack::Ptr stack, CodeError::List& errors)
	{
	}

	/*************************************************************
	SymbolAssembly
	*************************************************************/

	void SymbolAssembly::InstallModules(Module::List& modules, CodeFile::List& codeFiles, CodeError::List& errors)
	{
		for (int i = 0; (size_t)i < modules.size(); i++)
		{
			auto symbolModule = make_shared<SymbolModule>();
			symbolModule->codeFile = codeFiles[i];
			symbolModule->module = modules[i];
			symbolModules.push_back(symbolModule);
		}
	}
}