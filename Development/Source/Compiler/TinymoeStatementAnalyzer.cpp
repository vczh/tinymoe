#include "TinymoeStatementAnalyzer.h"

namespace tinymoe
{
	namespace compiler
	{
		/*************************************************************
		Declaration::CreateSymbol
		*************************************************************/

		shared_ptr<GrammarSymbol> SymbolDeclaration::CreateSymbol(bool secondary)
		{
			if (secondary) return nullptr;
			auto symbol = make_shared<GrammarSymbol>(GrammarSymbolType::Symbol);
			for (auto token : name->identifiers)
			{
				symbol + token.value;
			}
			return symbol;
		}

		shared_ptr<GrammarSymbol> TypeDeclaration::CreateSymbol(bool secondary)
		{
			if (secondary) return nullptr;
			auto symbol = make_shared<GrammarSymbol>(GrammarSymbolType::Type);
			for (auto token : name->identifiers)
			{
				symbol + token.value;
			}
			return symbol;
		}

		shared_ptr<GrammarSymbol> FunctionDeclaration::CreateSymbol(bool secondary)
		{
			if (secondary)
			{
				if (alias)
				{
					auto symbol = make_shared<GrammarSymbol>(GrammarSymbolType::Symbol);
					for (auto token : alias->identifiers)
					{
						symbol + token.value;
					}
					return symbol;
				}
				else
				{
					return nullptr;
				}
			}
			else
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
					(*i)->AppendFunctionSymbol(symbol, (type == FunctionDeclarationType::Phrase && (i == name.begin() || i + 1 == name.end())));
				}

				auto itname = name.begin();
				auto itfragment = symbol->fragments.begin();
				while (itname != name.end())
				{
					(*itfragment++)->functionFragment = *itname++;
				}
				return symbol;
			}
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
			if (type == FunctionArgumentType::Argument)
			{
				return nullptr;
			}
			auto symbol = make_shared<GrammarSymbol>(GrammarSymbolType::Symbol);
			for (auto token : name->identifiers)
			{
				symbol + token.value;
			}
			return symbol;
		}

		shared_ptr<GrammarSymbol> FunctionArgumentFragment::CreateSymbol()
		{
			return declaration->CreateSymbol(false);
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
						T("Symbol redefinition."),
					};
					errors.push_back(error);
				}
			}
		}

		void SymbolModule::BuildSymbols(CodeError::List& errors)
		{
			for (auto declaration : module->declarations)
			{
				if (auto symbol = declaration->CreateSymbol(false))
				{
					symbolDeclarations.insert(make_pair(symbol, declaration));
				}
				if (auto symbol = declaration->CreateSymbol(true))
				{
					symbolDeclarations.insert(make_pair(symbol, declaration));
				}
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

					if (function->bodyName)
					{
						if (auto symbol = function->bodyName->CreateSymbol())
						{
							symbolFunction->arguments.push_back(symbol);
							symbolFunction->argumentFragments.insert(make_pair(symbol, function->bodyName));
						}
					}

					for (auto fragment : function->name)
					{
						if (auto symbol = fragment->CreateSymbol())
						{
							symbolFunction->arguments.push_back(symbol);
							symbolFunction->argumentFragments.insert(make_pair(symbol, fragment));
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
									CheckOverloading(this, ita->first, ita->second, ref.get(), itb->first, itb->second, true, errors);
								}
							}
						}

						if (func->multipleDispatchingRoot.expired())
						{
							CodeError error =
							{
								decl->keywordToken,
								T("Cannot find the root definition for this multple dispatching child function."),
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

		FunctionDeclaration::Ptr SymbolModule::FindSymbolFunction(GrammarSymbol::Ptr symbol, GrammarSymbolType acceptableType)
		{
			if (symbol && symbol->type == acceptableType)
			{
				if (auto decl = FindDeclaration(symbol))
				{
					if (auto func = dynamic_pointer_cast<FunctionDeclaration>(decl))
					{
						return func;
					}
				}
			}

			return nullptr;
		}

		Statement::Ptr SymbolModule::ParseBlock(CodeFile::Ptr codeFile, GrammarStack::Ptr stack, Statement::Ptr statement, int& lineIndex, int endLineIndex, CodeError::List& errors)
		{
			int pushCount = 0;

	#define PUSH_STACK(ITEM)\
			do\
			{\
				stack->Push(ITEM); \
				pushCount++; \
			}while (0)

	#define POP_STACK\
			do\
			{\
				for (int i = 0; i < pushCount; i++)\
				{\
					stack->Pop(); \
				}\
				pushCount = 0;\
			}while (0)

			try
			{
				while (lineIndex <= endLineIndex)
				{
					auto line = codeFile->lines[lineIndex++];
					GrammarStack::ResultList result;
					auto statementError = stack->ParseStatement(line->tokens.begin(), line->tokens.end(), result);

					CodeError illegalAssignableError =
					{
						line->tokens[0],
						T("Illegal new variable name."),
					};
					multimap<int, Expression::Ptr> statementScores;
					for (auto r : result)
					{
						Expression::List assignables, arguments, modifiedAssignables;
						r.second->CollectNewAssignable(assignables, arguments, modifiedAssignables);

						Expression::Ptr illegalConvertedAssignable;
						int score = stack->CountStatementAssignables(assignables, illegalConvertedAssignable);
						if (score == -1)
						{
							illegalAssignableError.message += T("\r\n\tVariable can be parsed as ¡¾") + illegalConvertedAssignable->ToCode() + T("¡¿ in ¡¾") + r.second->ToCode() + T("¡¿");
						}
						else
						{
							statementScores.insert(make_pair(score, r.second));
						}
					}

					Expression::List legalExpressions;
					if (statementScores.size() > 0)
					{
						auto score = statementScores.begin()->first;
						auto lower = statementScores.lower_bound(score);
						auto upper = statementScores.upper_bound(score);
						for (auto it = lower; it != upper; it++)
						{
							legalExpressions.push_back(it->second);
						}
					}

					if (legalExpressions.size() == 0)
					{
						if (result.size() == 0)
						{
							errors.push_back(statementError);
						}
						else
						{
							errors.push_back(illegalAssignableError);
						}
						if ((size_t)lineIndex < codeFile->lines.size())
						{
							if (line->tokens[0].column < codeFile->lines[lineIndex]->tokens[0].column)
							{
								throw ParsingFailedException();
							}
						}
					}
					else if (legalExpressions.size() > 1)
					{
						CodeError error =
						{
							line->tokens[0],
							T("Ambiguous statement.")
						};
						for (auto r : legalExpressions)
						{
							error.message += T("\r\n\tStatement can be parsed as ¡¾") + r->ToCode() + T("¡¿");
						}
						errors.push_back(error);
						throw ParsingFailedException();
					}
					else
					{
						auto invoke = dynamic_pointer_cast<InvokeExpression>(legalExpressions[0]);
						auto symbol = dynamic_pointer_cast<ReferenceExpression>(invoke->function)->symbol;

						if (auto sentence = FindSymbolFunction(symbol, GrammarSymbolType::Sentence))
						{
							if (sentence->category && sentence->category->insideCategories.size() > 0)
							{
								auto parentStatement = statement;
								while (parentStatement)
								{
									if (auto parent = FindSymbolFunction(parentStatement->statementSymbol, GrammarSymbolType::Block))
									{
										if (parent->category && parent->category->categoryName)
										{
											auto category = parent->category->categoryName->GetName();
											for (auto inside : sentence->category->insideCategories)
											{
												if (category == inside->GetName())
												{
													goto END_OF_INSIDE_CATEGORY_CHECK;
												}
											}
										}
									}
									parentStatement = parentStatement->parentStatement.lock();
								}

							END_OF_INSIDE_CATEGORY_CHECK:
								if (!parentStatement)
								{
									CodeError error =
									{
										line->tokens[0],
										T("This sentence can only appear inside a block of category: "),
									};
									for (auto it = sentence->category->insideCategories.begin(); it != sentence->category->insideCategories.end(); it++)
									{
										error.message += (*it)->GetName();
										if (it + 1 == sentence->category->insideCategories.end())
										{
											error.message += T(".");
										}
										else
										{
											error.message += T(", ");
										}
									}
									errors.push_back(error);
								}
							}
						}

						auto parent = FindSymbolFunction(statement->statementSymbol, GrammarSymbolType::Block);
						auto block = FindSymbolFunction(symbol, GrammarSymbolType::Block);

						switch (symbol->target)
						{
						case GrammarSymbolTarget::End:
							if (statement->statementSymbol)
							{
								if (statement->statementSymbol->target != GrammarSymbolTarget::Select)
								{
									if (!parent || (parent->category && !parent->category->closable))
									{
										CodeError error =
										{
											line->tokens[0],
											T("A non-closable block cannot be closed using \"end\"."),
										};
										errors.push_back(error);
									}
								}
								else
								{
									Statement::Ptr lastCaseStatement;
									for (int i = 0; (size_t)i < statement->statements.size(); i++)
									{
										auto childStat = statement->statements[i];
										switch (childStat->statementSymbol->target)
										{
										case GrammarSymbolTarget::Case:
										case GrammarSymbolTarget::CaseElse:
											lastCaseStatement = childStat;
											break;
										default:
											if (lastCaseStatement)
											{
												statement->statements.erase(statement->statements.begin() + i);
												lastCaseStatement->statements.push_back(childStat);
												i--;
											}
											else
											{
												CodeError error =
												{
													childStat->keywordToken,
													T("Select sentence can only contain case or case else sentence."),
												};
												errors.push_back(error);
											}
										}
									}
								}
							}
							POP_STACK;
							return nullptr;
						case GrammarSymbolTarget::Case:
						case GrammarSymbolTarget::CaseElse:
							if (!statement->statementSymbol || statement->statementSymbol->target != GrammarSymbolTarget::Select)
							{
								CodeError error =
								{
									line->tokens[0],
									T("Case sentence should appear inside select block."),
								};
								errors.push_back(error);
							}
							break;
						case GrammarSymbolTarget::RedirectTo:
							if (auto value = dynamic_pointer_cast<LiteralExpression>(invoke->arguments[0]))
							{
								if (value->token.type != CodeTokenType::String)
								{
									CodeError error =
									{
										line->tokens[0],
										T("A string constant is required after \"redirect to\"."),
									};
									errors.push_back(error);
								}
							}
							else
							{
								CodeError error =
								{
									line->tokens[0],
									T("A string constant is required after \"redirect to\"."),
								};
								errors.push_back(error);
							}
							break;
						}

						auto newStatement = make_shared<Statement>();
						newStatement->keywordToken = line->tokens[0];
						newStatement->statementSymbol = symbol;
						newStatement->statementExpression = invoke;
					
						Expression::List assignables, arguments, modifiedAssignables;
						invoke->CollectNewAssignable(assignables, arguments, modifiedAssignables);

						map<GrammarSymbol::Ptr, CodeToken> symbolTokens;
						for (auto expr : assignables)
						{
							auto argument = dynamic_pointer_cast<ArgumentExpression>(expr);
							GrammarSymbol::Ptr symbol;
							CodeToken token;
							BuildNameSymbol(argument->name->identifiers, symbol, token);
							newStatement->newVariables.insert(make_pair(symbol, expr));
							symbolTokens.insert(make_pair(symbol, token));
						}
						for (auto expr : arguments)
						{
							auto argument = dynamic_pointer_cast<ArgumentExpression>(expr);
							GrammarSymbol::Ptr symbol;
							CodeToken token;
							BuildNameSymbol(argument->name->identifiers, symbol, token);
							newStatement->blockArguments.insert(make_pair(symbol, expr));
							symbolTokens.insert(make_pair(symbol, token));
						}
						for (auto expr : modifiedAssignables)
						{
							auto ref = dynamic_pointer_cast<ReferenceExpression>(expr);
							if ((ref->symbol->target != GrammarSymbolTarget::Custom && ref->symbol->target!=GrammarSymbolTarget::TheResult) || FindDeclaration(ref->symbol))
							{
								CodeError error =
								{
									newStatement->keywordToken,
									T("Symbol \"") + ref->symbol->uniqueId + T("\" cannot be assigned to."),
								};
								errors.push_back(error);
							}
						}

						if (parent && block && parent->category && parent->category->categoryName && block->category)
						{
							auto category = parent->category->categoryName->GetName();
							for (auto blockCategory : block->category->followCategories)
							{
								if (blockCategory->GetName() == category)
								{
									POP_STACK;
									newStatement->connectToPreviousBlock = true;
									return newStatement;
								}
							}
						}

						newStatement->parentStatement = statement;
						statement->statements.push_back(newStatement);

						if (newStatement->newVariables.size() > 0)
						{
							auto item = make_shared<GrammarStackItem>();
							for (auto v : newStatement->newVariables)
							{
								item->symbols.push_back(v.first);
							}
							PUSH_STACK(item);

							GrammarSymbol::List symbols;
							FindOverridedSymbols(stack, item, symbols);
							for (auto symbol : symbols)
							{
								CodeError error =
								{
									symbolTokens.find(symbol)->second,
									T("Symbol \"") + symbol->uniqueId + T("\" overrided other symbols in this scope or parent scopes."),
								};
								errors.push_back(error);
							}
						}

						if (symbol->type == GrammarSymbolType::Block)
						{
							if (symbol->target != GrammarSymbolTarget::Select)
							{
								if (!block || (block->category && (!block->category->categoryName || block->category->followCategories.size() > 0)))
								{
									if (block->category->followCategories.size() > 0)
									{
										CodeError error =
										{
											line->tokens[0],
											T("This is not a initial block. It should follow a block of category: "),
										};
										for (auto it = block->category->followCategories.begin(); it != block->category->followCategories.end(); it++)
										{
											error.message += (*it)->GetName();
											if (it + 1 == block->category->followCategories.end())
											{
												error.message += T(".");
											}
											else
											{
												error.message += T(", ");
											}
										}
										errors.push_back(error);
									}
									else if (!block->category->categoryName)
									{
										CodeError error =
										{
											line->tokens[0],
											T("This is not a initial block."),
										};
										errors.push_back(error);
									}
								}
							}

							while (true)
							{
								{
									auto item = make_shared<GrammarStackItem>();
									for (auto v : newStatement->blockArguments)
									{
										item->symbols.push_back(v.first);
									}
									stack->Push(item);

									GrammarSymbol::List symbols;
									FindOverridedSymbols(stack, item, symbols);
									for (auto symbol : symbols)
									{
										CodeError error =
										{
											symbolTokens.find(symbol)->second,
											T("Symbol \"") + symbol->uniqueId + T("\" overrided other symbols in this scope or parent scopes."),
										};
										errors.push_back(error);
									}
								}
								try
								{
									newStatement = ParseBlock(codeFile, stack, newStatement, lineIndex, endLineIndex, errors);
									stack->Pop();
								}
								catch (const ParsingFailedException&)
								{
									stack->Pop();
								}
								if (!newStatement) break;
								newStatement->parentStatement = statement;
								statement->statements.push_back(newStatement);
							}
						}
					}
				}
				POP_STACK;
			}
			catch (const ParsingFailedException&)
			{
				POP_STACK;
				throw;
			}
			if (statement->statements.size()>0)
			{
				auto last = statement->statements.back();
				if (last->statementSymbol && last->statementSymbol->target == GrammarSymbolTarget::End)
				{
					return nullptr;
				}
			}

			CodeError error =
			{
				codeFile->lines[lineIndex - 1]->tokens[0],
				T("Block should be closed using \"end\"."),
			};
			errors.push_back(error);
			throw ParsingFailedException();
		}

		GrammarSymbol::Ptr SymbolModule::FindType(SymbolName::Ptr name, GrammarStack::Ptr stack, CodeError::List& errors)
		{
			GrammarStack::ResultList result;
			auto error = stack->ParseType(name->identifiers.begin(), name->identifiers.end(), result);
			auto first = find_if(result.begin(), result.end(), [name](GrammarStack::ResultItem pair)
			{
				return pair.first == name->identifiers.end();
			});

			if (first == result.end())
			{
				CodeError error =
				{
					name->identifiers[0],
					T("Type \"") + name->GetName() + T("\" does not exist."),
				};
				errors.push_back(error);
			}
			else if (auto ref = dynamic_pointer_cast<ReferenceExpression>(result[0].second))
			{
				return ref->symbol;
			}
			return nullptr;
		}

		Declaration::Ptr SymbolModule::FindDeclaration(GrammarSymbol::Ptr symbol)
		{
			auto it = symbolDeclarations.find(symbol);
			if (it != symbolDeclarations.end())
			{
				return it->second;
			}
			else
			{
				for (auto weakModule : usingSymbolModules)
				{
					auto module = weakModule.lock();
					it = module->symbolDeclarations.find(symbol);
					if (it != module->symbolDeclarations.end())
					{
						return it->second;
					}
				}
			}
			return nullptr;
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
						auto itdfr = ref->declarationFunctions.find(sdr.second);
						if (itdfr == ref->declarationFunctions.end() || itdfr->second->multipleDispatchingRoot.expired())
						{
							item->symbols.push_back(sdr.first);
						}
					}
				}
				stack->Push(item);
			}
			{
				auto item = make_shared<GrammarStackItem>();
				for (auto sdr : symbolDeclarations)
				{
					auto itdfr = declarationFunctions.find(sdr.second);
					if (itdfr == declarationFunctions.end() || itdfr->second->multipleDispatchingRoot.expired())
					{
						item->symbols.push_back(sdr.first);
					}
				}
				stack->Push(item);
			}

			for (auto sdp : symbolDeclarations)
			{
				if (auto type = dynamic_pointer_cast<TypeDeclaration>(sdp.second))
				{
					if (type->parent)
					{
						if (auto baseType = FindType(type->parent, stack, errors))
						{
							baseTypes.insert(make_pair(sdp.second, baseType));
						}
					}
				}
			}

			for (auto dfp : declarationFunctions)
			{
				auto funcdecl = dynamic_pointer_cast<FunctionDeclaration>(dfp.first);
				auto func = dfp.second;
				func->resultVariable = stack->resultSymbol;
				for (auto sfp : func->argumentFragments)
				{
					if (auto var = dynamic_pointer_cast<VariableArgumentFragment>(sfp.second))
					{
						if (var->receivingType)
						{
							if (auto receivingType = FindType(var->receivingType, stack, errors))
							{
								func->argumentTypes.insert(make_pair(sfp.second, receivingType));
							}
						}
					}
				}
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
							func->cpsStateVariable = symbol;
							item->symbols.push_back(symbol);
							symbolTokens.insert(make_pair(symbol, token));
						}
						if (funcdecl->cps->continuationName)
						{
							GrammarSymbol::Ptr symbol;
							CodeToken token;
							BuildNameSymbol(funcdecl->cps->continuationName->identifiers, symbol, token);
							func->cpsContinuationVariable = symbol;
							item->symbols.push_back(symbol);
							symbolTokens.insert(make_pair(symbol, token));
						}
					}
					if (funcdecl->category)
					{
						if (funcdecl->category->signalName)
						{
							GrammarSymbol::Ptr symbol;
							CodeToken token;
							BuildNameSymbol(funcdecl->category->signalName->identifiers, symbol, token);
							func->categorySignalVariable = symbol;
							item->symbols.push_back(symbol);
							symbolTokens.insert(make_pair(symbol, token));
						}
					}

					for (auto argument : func->argumentFragments)
					{
						item->symbols.push_back(argument.first);
						symbolTokens.insert(make_pair(argument.first, func->argumentFragments.find(argument.first)->second->keywordToken));
					}
					stack->Push(item);

					GrammarSymbol::List symbols;
					FindOverridedSymbols(stack, item, symbols);
					for (auto symbol : symbols)
					{
						CodeError error =
						{
							symbolTokens.find(symbol)->second,
							T("Symbol \"") + symbol->uniqueId + T("\" overrided other symbols in this scope or parent scopes."),
						};
						errors.push_back(error);
					}
				
					if (funcdecl->codeLineIndex == -1)
					{
						CodeError error =
						{
							funcdecl->keywordToken,
							T("Block should be closed using \"end\"."),
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
							if (lineIndex <= endLineIndex)
							{
								CodeError error =
								{
									codeFile->lines[lineIndex]->tokens[0],
									T("Too many code.")
								};
								errors.push_back(error);
							}
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

		SymbolAssembly::Ptr SymbolAssembly::Parse(vector<string_t>& codes, CodeError::List& errors)
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
				multimap<string_t, SymbolModule::Ptr> moduleMap;

				for (auto module : assembly->symbolModules)
				{
					moduleMap.insert(make_pair(module->module->name->GetName(), module));
				}

				for (auto module : assembly->symbolModules)
				{
					string_t moduleName = module->module->name->GetName();
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
						string_t refName = ref->GetName();
						auto lower = moduleMap.lower_bound(refName);
						auto upper = moduleMap.upper_bound(refName);
						if (lower == moduleMap.end())
						{
							CodeError error =
							{
								ref->identifiers[0],
								T("Cannot find the referencing module \"") + refName + T("\"."),
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
}