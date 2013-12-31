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

	bool SymbolModule::IsOverloading(GrammarSymbol::Ptr a, GrammarSymbol::Ptr b)
	{
		if (a->fragments.size() != b->fragments.size())
		{
			return false;
		}

		auto ita = a->fragments.begin();
		auto itb = b->fragments.begin();
		while (ita != a->fragments.end() && itb != b->fragments.end())
		{
			auto fa = *ita;
			auto fb = *itb;

			if (fa->type != fb->type)
			{
				return false;
			}

			if (fa->type == GrammarFragmentType::Name)
			{
				if (fa->identifiers.size() != fb->identifiers.size())
				{
					return false;
				}

				auto itfa = fa->identifiers.begin();
				auto itfb = fb->identifiers.begin();

				while (itfa != fa->identifiers.end() && itfb != fb->identifiers.end())
				{
					if (*itfa != *itfb)
					{
						return false;
					}
					itfa++;
					itfb++;
				}
			}

			ita++;
			itb++;
		}

		return true;
	}

	bool SymbolModule::IsMultipleDispatchingChild(FunctionDeclaration::Ptr func)
	{
		for (auto fragment : func->name)
		{
			if (auto var = dynamic_pointer_cast<VariableArgumentFragment>(fragment))
			{
				if (var->receivingType)
				{
					return true;
				}
			}
		}
		return false;
	}

	void SymbolModule::CheckOverloading(SymbolModule* modulea, GrammarSymbol::Ptr symbola, Declaration::Ptr decla, SymbolModule* moduleb, GrammarSymbol::Ptr symbolb, Declaration::Ptr declb, bool foreignCheck, CodeError::List& errors)
	{
		if (IsOverloading(symbola, symbolb))
		{
			auto funca = dynamic_pointer_cast<FunctionDeclaration>(decla);
			auto funcb = dynamic_pointer_cast<FunctionDeclaration>(declb);

			if (funca && funcb)
			{
				bool childa = IsMultipleDispatchingChild(funca);
				bool childb = IsMultipleDispatchingChild(funcb);
				if (childa)
				{
					if (childb)
					{
						return;
					}
					else
					{
						modulea->declarationFunctions.find(decla)->second->multipleDispatchingRoot = moduleb->declarationFunctions.find(declb)->second;
						return;
					}
				}
				else if (childb)
				{
					if (!foreignCheck)
					{
						moduleb->declarationFunctions.find(declb)->second->multipleDispatchingRoot = modulea->declarationFunctions.find(decla)->second;
						return;
					}
				}
			}

			if (!foreignCheck)
			{
				CodeError error =
				{
					declb->keywordToken,
					"Symbol redefinition.",
				};
				errors.push_back(error);
			}
		}
	}

	void SymbolModule::BuildSymbols(CodeError::List& errors)
	{
		for (auto declaration : module->declarations)
		{
			auto symbol = declaration->CreateSymbol();
			symbolDeclarations.insert(make_pair(symbol, declaration));
		}
	}

	void SymbolModule::BuildFunctions(CodeError::List& errors)
	{
		for (auto declaration : module->declarations)
		{
			if (auto function = dynamic_pointer_cast<FunctionDeclaration>(declaration))
			{
				auto symbolFunction = make_shared<SymbolFunction>();
				symbolFunction->function = function;
				declarationFunctions.insert(make_pair(declaration, symbolFunction));

				for (auto fragment : function->name)
				{
					if (auto symbol = fragment->CreateSymbol())
					{
						symbolFunction->arguments.insert(make_pair(symbol, fragment));
					}
				}
			}
		}
	}

	void SymbolModule::BuildFunctionLinkings(CodeError::List& errors)
	{
		for (auto ita = symbolDeclarations.begin(); ita != symbolDeclarations.end(); ita++)
		{
			for (auto itb = ita; ++itb != symbolDeclarations.end();)
			{
				CheckOverloading(this, ita->first, ita->second, this, itb->first, itb->second, false, errors);
			}
		}

		for (auto ita = symbolDeclarations.begin(); ita != symbolDeclarations.end(); ita++)
		{
			auto decl = ita->second;
			if (auto funcdecl = dynamic_pointer_cast<FunctionDeclaration>(decl))
			{
				if (IsMultipleDispatchingChild(funcdecl))
				{
					auto func = declarationFunctions.find(decl)->second;
					if (func->multipleDispatchingRoot.expired())
					{
						for (auto weakRef : usingSymbolModules)
						{
							auto ref = weakRef.lock();
							for (auto itb = ref->symbolDeclarations.begin(); itb != ref->symbolDeclarations.end(); itb++)
							{
								CheckOverloading(this, ita->first, ita->second, this, itb->first, itb->second, true, errors);
							}
						}
					}

					if (func->multipleDispatchingRoot.expired())
					{
						CodeError error =
						{
							decl->keywordToken,
							"Cannot find the root definition for this multple dispatching child function.",
						};
						errors.push_back(error);
					}
				}
			}
		}
	}

	void SymbolModule::FindOverridedSymbols(GrammarStack::Ptr stack, GrammarStackItem::Ptr item, GrammarSymbol::List& symbols)
	{
		for (auto symbol : item->symbols)
		{
			auto lower = stack->availableSymbols.lower_bound(symbol->uniqueId);
			auto upper = stack->availableSymbols.upper_bound(symbol->uniqueId);

			bool found = false;
			for (auto it = lower; it != upper; it++)
			{
				if (!found)
				{
					found = true;
				}
				else
				{
					symbols.push_back(symbol);
					break;
				}
			}
		}
	}

	void SymbolModule::BuildNameSymbol(vector<CodeToken>& tokens, GrammarSymbol::Ptr& symbol, CodeToken& token)
	{
		symbol = make_shared<GrammarSymbol>(GrammarSymbolType::Symbol);
		for (auto token : tokens)
		{
			symbol + token.value;
		}
		token = tokens[0];
	}

	void SymbolModule::ParseBlock(CodeFile::Ptr codeFile, GrammarStack::Ptr stack, Statement::Ptr statement, int& lineIndex, int endLineIndex, CodeError::List& errors)
	{
		while (lineIndex <= endLineIndex)
		{
			auto line = codeFile->lines[lineIndex++];
			GrammarStack::ResultList result;
			auto error = stack->ParseStatement(line->tokens.begin(), line->tokens.end(), result);
			if (result.size() == 0)
			{
				errors.push_back(error);
				if ((size_t)lineIndex < codeFile->lines.size())
				{
					if (line->tokens[0].column < codeFile->lines[lineIndex]->tokens[0].column)
					{
						throw ParsingFailedException();
					}
				}
			}
			else
			{
			}
		}

		if (statement->statements.size()>0)
		{
			auto last = statement->statements.back();
			if (last->statementSymbol && last->statementSymbol->target == GrammarSymbolTarget::End)
			{
				return;
			}
		}

		CodeError error =
		{
			codeFile->lines[lineIndex - 1]->tokens[0],
			"Block should be closed using \"end\".",
		};
	}

	void SymbolModule::BuildStatements(GrammarStack::Ptr stack, CodeError::List& errors)
	{
		{
			auto item = make_shared<GrammarStackItem>();
			for (auto weakRef : usingSymbolModules)
			{
				auto ref = weakRef.lock();
				for (auto sdr : ref->symbolDeclarations)
				{
					item->symbols.push_back(sdr.first);
				}
			}
			stack->Push(item);
		}
		{
			auto item = make_shared<GrammarStackItem>();
			for (auto sdp : symbolDeclarations)
			{
				item->symbols.push_back(sdp.first);
			}
			stack->Push(item);
		}

		for (auto dfp : declarationFunctions)
		{
			auto funcdecl = dynamic_pointer_cast<FunctionDeclaration>(dfp.first);
			auto func = dfp.second;
			{
				map<GrammarSymbol::Ptr, CodeToken> symbolTokens;
				auto item = make_shared<GrammarStackItem>();

				if (funcdecl->cps)
				{
					if (funcdecl->cps->stateName)
					{
						GrammarSymbol::Ptr symbol;
						CodeToken token;
						BuildNameSymbol(funcdecl->cps->stateName->identifiers, symbol, token);
						item->symbols.push_back(symbol);
						symbolTokens.insert(make_pair(symbol, token));
					}
					if (funcdecl->cps->continuationName)
					{
						GrammarSymbol::Ptr symbol;
						CodeToken token;
						BuildNameSymbol(funcdecl->cps->continuationName->identifiers, symbol, token);
						item->symbols.push_back(symbol);
						symbolTokens.insert(make_pair(symbol, token));
					}
				}

				for (auto argument : func->arguments)
				{
					item->symbols.push_back(argument.first);
					symbolTokens.insert(make_pair(argument.first, func->arguments.find(argument.first)->second->keywordToken));
				}
				stack->Push(item);

				GrammarSymbol::List symbols;
				FindOverridedSymbols(stack, item, symbols);
				for (auto symbol : symbols)
				{
					CodeError error =
					{
						symbolTokens.find(symbol)->second,
						"Symbol \"" + symbol->uniqueId + "\" overrided other symbols in this scope or parent scopes.",
					};
					errors.push_back(error);
				}
				
				if (funcdecl->codeLineIndex == -1)
				{
					CodeError error =
					{
						funcdecl->keywordToken,
						"Block should be closed using \"end\".",
					};
					errors.push_back(error);
				}
				else
				{
					int lineIndex = funcdecl->codeLineIndex;
					int endLineIndex = funcdecl->endLineIndex;
					auto statement = make_shared<Statement>();
					func->statement = statement;
					
					try
					{
						ParseBlock(codeFile, stack, statement, lineIndex, endLineIndex, errors);
					}
					catch (const ParsingFailedException&)
					{
					}
				}

				stack->Pop();
			}
		}

		stack->Pop();
		stack->Pop();
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
			multimap<string, SymbolModule::Ptr> moduleMap;

			for (auto module : assembly->symbolModules)
			{
				moduleMap.insert(make_pair(module->module->name->GetName(), module));
			}

			for (auto module : assembly->symbolModules)
			{
				string moduleName = module->module->name->GetName();
				auto lower = moduleMap.lower_bound(moduleName);
				auto upper = moduleMap.upper_bound(moduleName);
				for (auto it = lower; it != upper; it++)
				{
					if (it->second != module)
					{
						module->usingSymbolModules.push_back(it->second);
					}
				}

				for (auto ref : module->module->usings)
				{
					string refName = ref->GetName();
					auto lower = moduleMap.lower_bound(refName);
					auto upper = moduleMap.upper_bound(refName);
					if (lower == moduleMap.end())
					{
						CodeError error =
						{
							ref->identifiers[0],
							"Cannot find the referencing module \"" + refName + "\".",
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

		if (errors.size() == 0)
		{
			for (auto module : assembly->symbolModules)
			{
				module->BuildSymbols(errors);
			}
		}

		if (errors.size() == 0)
		{
			for (auto module : assembly->symbolModules)
			{
				module->BuildFunctions(errors);
			}
		}

		if (errors.size() == 0)
		{
			for (auto module : assembly->symbolModules)
			{
				module->BuildFunctionLinkings(errors);
			}
		}

		if (errors.size() == 0)
		{
			auto item = make_shared<GrammarStackItem>();
			item->FillPredefinedSymbols();
			auto stack = make_shared<GrammarStack>();
			stack->Push(item);

			for (auto module : assembly->symbolModules)
			{
				module->BuildStatements(stack, errors);
			}
		}

		return assembly;
	}
}