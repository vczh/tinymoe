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

	SymbolAssembly::Ptr SymbolAssembly::Parse(vector<string>& codes, CodeError::List& errors)
	{
		Module::List modules;
		CodeFile::List codeFiles;

		int codeIndex = 0;
		for (auto code : codes)
		{
			auto codeFile = CodeFile::Parse(code, codeIndex++, errors);
			auto module = Module::Parse(codeFile, errors);

			codeFiles.push_back(codeFile);
			modules.push_back(module);
		}

		auto assembly = make_shared<SymbolAssembly>();
		for (int i = 0; (size_t)i < modules.size(); i++)
		{
			auto symbolModule = make_shared<SymbolModule>();
			symbolModule->codeFile = codeFiles[i];
			symbolModule->module = modules[i];
			assembly->symbolModules.push_back(symbolModule);
		}

		if (errors.size() == 0)
		{
		}
		return assembly;
	}
}