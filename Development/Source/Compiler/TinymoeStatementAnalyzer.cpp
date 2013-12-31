#include "TinymoeStatementAnalyzer.h"

namespace tinymoe
{
	/*************************************************************
	Declaration::CreateSymbol
	*************************************************************/

	shared_ptr<GrammarSymbol> SymbolDeclaration::CreateSymbol()
	{
		auto symbol = make_shared<GrammarSymbol>(GrammarSymbolType::Symbol);
		for (auto token : name->identifiers)
		{
			symbol + token.value;
		}
		return symbol;
	}

	shared_ptr<GrammarSymbol> TypeDeclaration::CreateSymbol()
	{
		auto symbol = make_shared<GrammarSymbol>(GrammarSymbolType::Type);
		for (auto token : name->identifiers)
		{
			symbol + token.value;
		}
		return symbol;
	}

	shared_ptr<GrammarSymbol> FunctionDeclaration::CreateSymbol()
	{
		auto symbolType = GrammarSymbolType::Phrase;
		switch (type)
		{
		case FunctionDeclarationType::Phrase:
			symbolType = GrammarSymbolType::Phrase;
			break;
		case FunctionDeclarationType::Sentence:
			symbolType = GrammarSymbolType::Sentence;
			break;
		case FunctionDeclarationType::Block:
			symbolType = GrammarSymbolType::Block;
			break;
		}

		auto symbol = make_shared<GrammarSymbol>(symbolType);
		for (auto i = name.begin(); i != name.end(); i++)
		{
			(*i)->AppendFunctionSymbol(symbol, (type == FunctionDeclarationType::Phrase && (i == name.begin() || i == name.end() - 1)));
		}
		return symbol;
	}

	/*************************************************************
	FunctionArgument::AppendFunctionSymbol
	*************************************************************/

	void NameFragment::AppendFunctionSymbol(shared_ptr<GrammarSymbol> symbol, bool primitive)
	{
		for (auto token : name->identifiers)
		{
			symbol + token.value;
		}
	}

	void VariableArgumentFragment::AppendFunctionSymbol(shared_ptr<GrammarSymbol> symbol, bool primitive)
	{
		switch (type)
		{
		case FunctionArgumentType::Argument:
			symbol + GrammarFragmentType::Argument;
			break;
		case FunctionArgumentType::Assignable:
			symbol + GrammarFragmentType::Assignable;
			break;
		case FunctionArgumentType::Expression:
			symbol + (primitive ? GrammarFragmentType::Primitive : GrammarFragmentType::Expression);
			break;
		case FunctionArgumentType::List:
			symbol + GrammarFragmentType::List;
			break;
		case FunctionArgumentType::Normal:
			symbol + (primitive ? GrammarFragmentType::Primitive : GrammarFragmentType::Expression);
			break;
		}
	}

	void FunctionArgumentFragment::AppendFunctionSymbol(shared_ptr<GrammarSymbol> symbol, bool primitive)
	{
		symbol + (primitive ? GrammarFragmentType::Primitive : GrammarFragmentType::Expression);
	}

	/*************************************************************
	FunctionArgument::CreateSymbol
	*************************************************************/

	shared_ptr<GrammarSymbol> NameFragment::CreateSymbol()
	{
		return nullptr;
	}

	shared_ptr<GrammarSymbol> VariableArgumentFragment::CreateSymbol()
	{
		auto symbol = make_shared<GrammarSymbol>(GrammarSymbolType::Symbol);
		for (auto token : name->identifiers)
		{
			symbol + token.value;
		}
		return symbol;
	}

	shared_ptr<GrammarSymbol> FunctionArgumentFragment::CreateSymbol()
	{
		return declaration->CreateSymbol();
	}

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
			function<bool(SymbolName::Ptr, SymbolName::Ptr)> comp = [](SymbolName::Ptr a, SymbolName::Ptr b)
			{
				auto ita = a->identifiers.begin();
				auto itb = b->identifiers.begin();
				auto ea = a->identifiers.end();
				auto eb = b->identifiers.end();

				while (ita != ea&&itb != eb)
				{
					auto compare = ita->value.compare(itb->value);
					if (compare < 0)return true;
					if (compare>0)return false;
					ita++;
					itb++;
				}
				return ita == ea;
			};
			multimap<SymbolName::Ptr, SymbolModule::Ptr, decltype(comp)> moduleMap(comp);

			for (auto module : assembly->symbolModules)
			{
				moduleMap.insert(make_pair(module->module->name, module));
			}

			for (auto module : assembly->symbolModules)
			{
				auto lower = moduleMap.lower_bound(module->module->name);
				auto upper = moduleMap.upper_bound(module->module->name);
				for (auto it = lower; it != upper; it++)
				{
					if (it->second != module)
					{
						module->usingSymbolModules.push_back(it->second);
					}
				}

				for (auto ref : module->module->usings)
				{
					auto lower = moduleMap.lower_bound(ref);
					auto upper = moduleMap.upper_bound(ref);
					if (lower == moduleMap.end())
					{
						CodeError error =
						{
							ref->identifiers[0],
							ref->identifiers[0],
							"Cannot find the referencing module.",
						};
					}
					else
					{
						for (auto it = lower; it != upper; it++)
						{
							module->usingSymbolModules.push_back(it->second);
						}
					}
				}
			}
		}
		return assembly;
	}
}