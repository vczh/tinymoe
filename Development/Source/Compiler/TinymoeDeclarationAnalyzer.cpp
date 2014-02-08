#include "TinymoeDeclarationAnalyzer.h"

namespace tinymoe
{
	namespace compiler
	{

		/*************************************************************
		FunctionCps
		*************************************************************/

		FunctionCps::Ptr FunctionCps::Parse(CodeFile::Ptr codeFile, CodeError::List& errors, int& lineIndex)
		{
			if (0 > (size_t)lineIndex || (size_t)lineIndex >= codeFile->lines.size()) return nullptr;
			auto line = codeFile->lines[lineIndex++];
			auto it = line->tokens.begin();
			auto cpsToken = *it;
			if (it->type != CodeTokenType::CPS)
			{
				CodeError error =
				{
					*it,
					T("CPS definition should begin with \"cps\"."),
				};
				errors.push_back(error);
				return nullptr;
			}
			it++;

			auto decl = make_shared<FunctionCps>();
			if (!SymbolName::ConsumeToken(it, line->tokens.end(), CodeTokenType::OpenBracket, T("("), cpsToken, errors)) goto END_OF_PARSING;
			decl->stateName = SymbolName::ParseToFarest(it, line->tokens.end(), T("CPS state"), cpsToken, errors);
			if (!SymbolName::ConsumeToken(it, line->tokens.end(), CodeTokenType::CloseBracket, T(")"), cpsToken, errors)) goto END_OF_PARSING;
			if (it == line->tokens.end()) goto END_OF_PARSING;
		
			if (!SymbolName::ConsumeToken(it, line->tokens.end(), CodeTokenType::OpenBracket, T("("), cpsToken, errors)) goto END_OF_PARSING;
			decl->continuationName = SymbolName::ParseToFarest(it, line->tokens.end(), T("CPS continuation"), cpsToken, errors);
			if (!SymbolName::ConsumeToken(it, line->tokens.end(), CodeTokenType::CloseBracket, T(")"), cpsToken, errors)) goto END_OF_PARSING;
			if (it != line->tokens.end())
			{
				CodeError error =
				{
					*it,
					T("Too many tokens."),
				};
				errors.push_back(error);
			}
		END_OF_PARSING:
			return decl;
		}

		/*************************************************************
		FunctionCategory
		*************************************************************/

		FunctionCategory::Ptr FunctionCategory::Parse(CodeFile::Ptr codeFile, CodeError::List& errors, int& lineIndex)
		{
			if (0 > (size_t)lineIndex || (size_t)lineIndex >= codeFile->lines.size()) return nullptr;
			auto line = codeFile->lines[lineIndex++];
			auto it = line->tokens.begin();
			auto categoryToken = *it;
			if (it->type != CodeTokenType::Category)
			{
				CodeError error =
				{
					*it,
					T("Category definition should begin with \"category\"."),
				};
				errors.push_back(error);
				return nullptr;
			}
			it++;

			auto decl = make_shared<FunctionCategory>();
			if (it != line->tokens.end())
			{
				if (!SymbolName::ConsumeToken(it, line->tokens.end(), CodeTokenType::OpenBracket, T("("), categoryToken, errors)) goto END_OF_SIGNAL_PARSING;
				decl->signalName = SymbolName::ParseToFarest(it, line->tokens.end(), T("Category signal"), categoryToken, errors);
				if (!SymbolName::ConsumeToken(it, line->tokens.end(), CodeTokenType::CloseBracket, T(")"), categoryToken, errors)) goto END_OF_SIGNAL_PARSING;

				if (it != line->tokens.end())
				{
					CodeError error =
					{
						*it,
						T("Too many tokens."),
					};
					errors.push_back(error);
				}
			}
		END_OF_SIGNAL_PARSING:

			while ((size_t)lineIndex < codeFile->lines.size())
			{
				line = codeFile->lines[lineIndex++];
				it = line->tokens.begin();

				if (it->value == T("start"))
				{
					if (decl->categoryName)
					{
						CodeError error =
						{
							*it,
							T("Too many start category name."),
						};
						errors.push_back(error);
					}
					else
					{
						auto startToken = *it;
						decl->categoryName = SymbolName::ParseToEnd(++it, line->tokens.end(), T("Start category"), startToken, errors);
					}
				}
				else if (it->value == T("follow"))
				{
					auto followToken = *it;
					decl->followCategories.push_back(SymbolName::ParseToEnd(++it, line->tokens.end(), T("Follow category"), followToken, errors));
				}
				else if (it->value == T("inside"))
				{
					auto insideToken = *it;
					decl->insideCategories.push_back(SymbolName::ParseToEnd(++it, line->tokens.end(), T("Inside category"), insideToken, errors));
				}
				else if (it->value == T("closable"))
				{
					decl->closable = true;
					if (++it != line->tokens.end())
					{
						CodeError error =
						{
							*it,
							T("Too many tokens."),
						};
						errors.push_back(error);
					}
				}
				else
				{
					lineIndex--;
					break;
				}
			}

			if (decl->signalName)
			{
				if (decl->followCategories.size() == 0)
				{
					CodeError error =
					{
						categoryToken,
						T("A category with signal parameter should have follow categories."),
					};
					errors.push_back(error);
				}
			}

			if (!decl->categoryName && decl->insideCategories.size() == 0)
			{
				if (!decl->closable)
				{
					CodeError error =
					{
						categoryToken,
						T("A category without start category name and inside categories should be closable."),
					};
					errors.push_back(error);
				}
			}
			return decl;
		}

		/*************************************************************
		SymbolDeclaration
		*************************************************************/

		SymbolDeclaration::Ptr SymbolDeclaration::Parse(CodeFile::Ptr codeFile, CodeError::List& errors, int& lineIndex)
		{
			if (0 > (size_t)lineIndex || (size_t)lineIndex >= codeFile->lines.size()) return nullptr;
			auto line = codeFile->lines[lineIndex++];
			auto it = line->tokens.begin();
			if (it->type != CodeTokenType::Symbol)
			{
				CodeError error =
				{
					*it,
					T("Symbol definition should begin with \"symbol\"."),
				};
				errors.push_back(error);
				return nullptr;
			}

			auto decl = make_shared<SymbolDeclaration>();
			decl->keywordToken = *it;
			decl->name = SymbolName::ParseToEnd(++it, line->tokens.end(), T("Symbol"), line->tokens[0], errors);
			return decl;
		}

		/*************************************************************
		TypeDeclaration
		*************************************************************/

		TypeDeclaration::Ptr TypeDeclaration::Parse(CodeFile::Ptr codeFile, CodeError::List& errors, int& lineIndex)
		{
			if (0 > (size_t)lineIndex || (size_t)lineIndex >= codeFile->lines.size()) return nullptr;
			auto line = codeFile->lines[lineIndex++];
			auto it = line->tokens.begin();
			auto typeToken = *it;
			if (it->type != CodeTokenType::Type)
			{
				CodeError error =
				{
					*it,
					T("Type definition should begin with \"type\"."),
				};
				errors.push_back(error);
				return nullptr;
			}

			auto decl = make_shared<TypeDeclaration>();
			decl->keywordToken = *it;
			decl->name = SymbolName::ParseToFarest(++it, line->tokens.end(), T("Type"), line->tokens[0], errors);
			if (it != line->tokens.end())
			{
				if (it->type == CodeTokenType::Colon)
				{
					auto parentTypeToken = *it;
					decl->parent = SymbolName::ParseToEnd(++it, line->tokens.end(), T("Parent type"), parentTypeToken, errors);
				}
				else
				{
					CodeError error =
					{
						*it,
						T("Inheriting from a type should begin with a \":\"."),
					};
					errors.push_back(error);
				}
			}

			bool reachTheEnd = false;
			while ((size_t)lineIndex < codeFile->lines.size())
			{
				line = codeFile->lines[lineIndex++];
				if (line->tokens.size() == 1 && line->tokens[0].type == CodeTokenType::End)
				{
					reachTheEnd = true;
					break;
				}
				else
				{
					decl->fields.push_back(SymbolName::ParseToEnd(line->tokens.begin(), line->tokens.end(), T("Field"), line->tokens[0], errors));
				}
			}

			if (!reachTheEnd)
			{
				CodeError error =
				{
					typeToken,
					T("Ending of the type is not found."),
				};
				errors.push_back(error);
			}
			return decl;
		}

		/*************************************************************
		FunctionDeclaration
		*************************************************************/

		FunctionDeclaration::Ptr FunctionDeclaration::Parse(CodeToken::List::iterator& it, CodeToken::List::iterator end, FunctionDeclaration::Ptr decl, CodeToken ownerToken, CodeError::List& errors)
		{
			if (!decl)
			{
				decl = make_shared<FunctionDeclaration>();
			}
			if (it == end)
			{
				CodeError error =
				{
					ownerToken,
					T("Function declaration should begin with \"phrase\", \"sentence\" or \"block\"."),
				};
				errors.push_back(error);
				goto END_OF_PARSING;
			}
			ownerToken = *it;

			switch (it->type)
			{
			case CodeTokenType::Phrase:
				decl->type = FunctionDeclarationType::Phrase;
				break;
			case CodeTokenType::Sentence:
				decl->type = FunctionDeclarationType::Sentence;
				break;
			case CodeTokenType::Block:
				decl->type = FunctionDeclarationType::Block;
				break;
			default:
				{
					CodeError error =
					{
						ownerToken,
						T("Function declaration should begin with \"phrase\", \"sentence\" or \"block\"."),
					};
					errors.push_back(error);
					goto END_OF_PARSING;
				}
			}
			decl->keywordToken = *it;
			it++;

			while (it != end)
			{
				if (it->type == CodeTokenType::Colon)
				{
					decl->alias = SymbolName::ParseToFarest(++it, end, T("Function alias"), ownerToken, errors);
					break;
				}
				else if (it->type == CodeTokenType::OpenBracket)
				{
					if (auto argument = ArgumentFragment::Parse(++it, end, ownerToken, errors))
					{
						decl->name.push_back(argument);
						if (!SymbolName::ConsumeToken(it, end, CodeTokenType::CloseBracket, T(")"), ownerToken, errors)) goto END_OF_PARSING;
					}
					else
					{
						goto END_OF_PARSING;
					}
				}
				else if (it->IsNameFragmentToken())
				{
					auto nameFragment = make_shared<NameFragment>();
					nameFragment->name = SymbolName::ParseToFarest(it, end, T("Function"), ownerToken, errors);
					if (nameFragment->name->identifiers.size() > 0)
					{
						nameFragment->keywordToken = nameFragment->name->identifiers[0];
					}
					decl->name.push_back(nameFragment);
				}
				else
				{
					break;
				}
			}

			if (decl->type == FunctionDeclarationType::Block && decl->name.size() > 0)
			{
				if (auto argument = dynamic_pointer_cast<ArgumentFragment>(decl->name[0]))
				{
					decl->name.erase(decl->name.begin());
					decl->bodyName = argument;
				}
			}

			if (decl->name.size() == 0)
			{
				CodeError error =
				{
					ownerToken,
					T("Function name should not be empty."),
				};
				errors.push_back(error);
			}
			else 
			{
				if (decl->type != FunctionDeclarationType::Phrase)
				{
					if (!dynamic_pointer_cast<NameFragment>(decl->name[0]))
					{
						CodeError error =
						{
							ownerToken,
							T("Sentence and block's name should not begin with an argument."),
						};
						errors.push_back(error);
					}
				}

				int nameCount = 0;
				bool lastNameIsArgument = false;
				for (auto name : decl->name)
				{
					if (dynamic_pointer_cast<NameFragment>(name))
					{
						nameCount++;
						lastNameIsArgument = false;
					}
					else
					{
						if (lastNameIsArgument)
						{
							CodeError error =
							{
								ownerToken,
								T("Function argument cannot appear just after another function argument."),
							};
							errors.push_back(error);
						}
						lastNameIsArgument = true;

						if (auto argument = dynamic_pointer_cast<VariableArgumentFragment>(name))
						{
							switch (argument->type)
							{
							case FunctionArgumentType::Argument:
								if (decl->type != FunctionDeclarationType::Block)
								{
									CodeError error =
									{
										ownerToken,
										T("Argument of type \"argument\" is only allowed in block declaration."),
									};
									errors.push_back(error);
								}
								break;
							case FunctionArgumentType::Expression:
							case FunctionArgumentType::Assignable:
								if (decl->type == FunctionDeclarationType::Phrase)
								{
									CodeError error =
									{
										ownerToken,
										T("Argument of type \"assignable\" or \"expression\" is only allowed in sentence or block declaration."),
									};
									errors.push_back(error);
								}
								break;
							}
						}
					}
				}

				if (nameCount == 0)
				{
					CodeError error =
					{
						ownerToken,
						T("Function name should not be form just by function arguments."),
					};
					errors.push_back(error);
				}
			}
		END_OF_PARSING:
			return decl;
		}

		FunctionDeclaration::Ptr FunctionDeclaration::Parse(CodeFile::Ptr codeFile, CodeError::List& errors, int& lineIndex)
		{
			if (0 > (size_t)lineIndex || (size_t)lineIndex >= codeFile->lines.size()) return nullptr;
			auto functionToken = codeFile->lines[lineIndex]->tokens[0];
			auto decl = make_shared<FunctionDeclaration>();
			decl->beginLineIndex = lineIndex;
		
			if (codeFile->lines[lineIndex]->tokens[0].type == CodeTokenType::CPS)
			{
				decl->cps = FunctionCps::Parse(codeFile, errors, lineIndex);
			}
			if (0 > (size_t)lineIndex || (size_t)lineIndex >= codeFile->lines.size())
			{
				CodeError error =
				{
					functionToken,
					T("Function declaration should begin with \"phrase\", \"sentence\" or \"block\"."),
				};
				errors.push_back(error);
				goto END_OF_PARSING;
			}
			if (codeFile->lines[lineIndex]->tokens[0].type == CodeTokenType::Category)
			{
				decl->category = FunctionCategory::Parse(codeFile, errors, lineIndex);
			}
			if (0 > (size_t)lineIndex || (size_t)lineIndex >= codeFile->lines.size())
			{
				CodeError error =
				{
					functionToken,
					T("Function declaration should begin with \"phrase\", \"sentence\" or \"block\"."),
				};
				errors.push_back(error);
				goto END_OF_PARSING;
			}
		
			{
				auto line = codeFile->lines[lineIndex++];
				auto it = line->tokens.begin();
				functionToken = *it;
			
				switch (it->type)
				{
				case CodeTokenType::Phrase:
				case CodeTokenType::Sentence:
				case CodeTokenType::Block:
					Parse(it, line->tokens.end(), decl, functionToken, errors);
					if (it != line->tokens.end())
					{
						CodeError error =
						{
							*it,
							T("Too many tokens."),
						};
						errors.push_back(error);
					}
					break;
				default:
					{
						CodeError error =
						{
							functionToken,
							T("Function declaration should begin with \"phrase\", \"sentence\" or \"block\"."),
						};
						errors.push_back(error);
						lineIndex--;
						goto END_OF_PARSING;
					}
				}

				if (decl->cps)
				{
					switch (decl->type)
					{
					case FunctionDeclarationType::Phrase:
						{
							CodeError error =
							{
								functionToken,
								T("Phrase should not have a continuation definition."),
							};
							errors.push_back(error);
						}
						break;
					case FunctionDeclarationType::Sentence:
						if (!decl->cps->continuationName)
						{
							CodeError error =
							{
								functionToken,
								T("CPS sentence should have a CPS continuation argument."),
							};
							errors.push_back(error);
						}
						break;
					case FunctionDeclarationType::Block:
						if (decl->cps->continuationName)
						{
							CodeError error =
							{
								functionToken,
								T("CPS block should not have a CPS continuation argument."),
							};
							errors.push_back(error);
						}
						break;
					}
				}
				if (decl->category)
				{
					switch (decl->type)
					{
					case FunctionDeclarationType::Phrase:
						{
							CodeError error =
							{
								functionToken,
								T("Phrase should not have a category definition."),
							};
							errors.push_back(error);
						}
						break;
					case FunctionDeclarationType::Sentence:
						if (decl->category->insideCategories.size() == 0)
						{
							CodeError error =
							{
								functionToken,
								T("A category of a sentence should have inside categories."),
							};
							errors.push_back(error);
						}
						if (decl->category->categoryName || decl->category->signalName || decl->category->followCategories.size() != 0)
						{
							CodeError error =
							{
								functionToken,
								T("A category of a sentence should not have start category name, signal name or follow categories."),
							};
							errors.push_back(error);
						}
						if (decl->category->closable)
						{
							CodeError error =
							{
								functionToken,
								T("A category of a sentence cannot be closable."),
							};
							errors.push_back(error);
						}
						break;
					case FunctionDeclarationType::Block:
						if (decl->category->insideCategories.size() != 0)
						{
							CodeError error =
							{
								functionToken,
								T("A category of a block cannot have inside categories."),
							};
							errors.push_back(error);
						}
						break;
					}
				}
				if (decl->type == FunctionDeclarationType::Block)
				{
					if (!decl->bodyName)
					{
						CodeError error =
						{
							functionToken,
							T("Block name should start with an argument for the block body."),
						};
						errors.push_back(error);
					}
				}
			}

		END_OF_PARSING:
			decl->endLineIndex = lineIndex - 1;
			while ((size_t)lineIndex < codeFile->lines.size())
			{
				auto token = codeFile->lines[lineIndex]->tokens[0];
				switch (token.type)
				{
				case CodeTokenType::Symbol:
				case CodeTokenType::Type:
				case CodeTokenType::CPS:
				case CodeTokenType::Category:
				case CodeTokenType::Phrase:
				case CodeTokenType::Sentence:
				case CodeTokenType::Block:
					goto END_OF_FUNCTION_BODY_SEARCHING;
				default:
					lineIndex++;
				}
			}

		END_OF_FUNCTION_BODY_SEARCHING:
			if (lineIndex > decl->endLineIndex + 1)
			{
				decl->codeLineIndex = decl->endLineIndex + 1;
				decl->endLineIndex = lineIndex - 1;
			}
			return decl;
		}

		/*************************************************************
		ArgumentFragment
		*************************************************************/

		ArgumentFragment::Ptr ArgumentFragment::Parse(CodeToken::List::iterator& it, CodeToken::List::iterator end, CodeToken ownerToken, CodeError::List& errors)
		{
			if (it == end)
			{
				CodeError error =
				{
					ownerToken,
					T("Function argument needed."),
				};
				errors.push_back(error);
				return nullptr;
			}

			if (it->type == CodeTokenType::Phrase || it->type == CodeTokenType::Sentence)
			{
				auto decl = make_shared<FunctionArgumentFragment>();
				decl->keywordToken = *it;
				decl->declaration = FunctionDeclaration::Parse(it, end, nullptr, ownerToken, errors);
				return decl;
			}
			if (it->IsNameFragmentToken() && it->type != CodeTokenType::Block)
			{
				auto decl = make_shared<VariableArgumentFragment>();
				decl->keywordToken = *it;
				if (it->type == CodeTokenType::Expression)
				{
					decl->type = FunctionArgumentType::Expression;
					it++;
				}
				else if (it->type == CodeTokenType::Assignable)
				{
					decl->type = FunctionArgumentType::Assignable;
					it++;
				}
				else if (it->type == CodeTokenType::Argument)
				{
					decl->type = FunctionArgumentType::Argument;
					it++;
				}
				else if (it->type == CodeTokenType::List)
				{
					decl->type = FunctionArgumentType::List;
					it++;
				}
				else
				{
					decl->type = FunctionArgumentType::Normal;
				}
				decl->name = SymbolName::ParseToFarest(it, end, T("Function argument"), ownerToken, errors);

				if (it->type == CodeTokenType::Colon)
				{
					if (decl->type != FunctionArgumentType::Normal)
					{
						CodeError error =
						{
							*it,
							T("Argument type for multiple dispatch is only allowed for value argument (that is, not \"list\", \"expression\", \"assignable\" and \"argument\")."),
						};
					}
					decl->receivingType = SymbolName::ParseToFarest(++it, end, T("Function argument type"), ownerToken, errors);
				}
				return decl;
			}
			else
			{
				CodeError error =
				{
					*it,
					T("Function argument should begin with \"expression\", \"assignable\", \"argument\", \"phrase\", \"sentence\" or a name."),
				};
				errors.push_back(error);
				return nullptr;
			}
		}

		/*************************************************************
		Module
		*************************************************************/

		Module::Ptr Module::Parse(CodeFile::Ptr codeFile, CodeError::List& errors)
		{
			auto module = make_shared<Module>();

			int lineIndex = 0;
			while ((size_t)lineIndex < codeFile->lines.size())
			{
				auto line = codeFile->lines[lineIndex];
				auto it = line->tokens.begin();
				switch (it->type)
				{
				case CodeTokenType::Module:
					if (module->name)
					{
						CodeError error =
						{
							*it,
							T("A module can only have one name."),
						};
						errors.push_back(error);
					}
					else
					{
						auto token = *it;
						module->name = SymbolName::ParseToEnd(++it, line->tokens.end(), T("Module"), token, errors);
					}
					lineIndex++;
					break;
				case CodeTokenType::Using:
					{
						auto token = *it;
						module->usings.push_back(SymbolName::ParseToEnd(++it, line->tokens.end(), T("Module using"), token, errors));
						lineIndex++;
					}
					break;
				case CodeTokenType::Symbol:
					module->declarations.push_back(SymbolDeclaration::Parse(codeFile, errors, lineIndex));
					break;
				case CodeTokenType::Type:
					module->declarations.push_back(TypeDeclaration::Parse(codeFile, errors, lineIndex));
					break;
				case CodeTokenType::CPS:
				case CodeTokenType::Category:
				case CodeTokenType::Phrase:
				case CodeTokenType::Sentence:
				case CodeTokenType::Block:
					module->declarations.push_back(FunctionDeclaration::Parse(codeFile, errors, lineIndex));
					break;
				default:
					{
						CodeError error =
						{
							*it,
							T("Cannot process a declaration that begins with \"") + it->value + T("\"."),
						};
						errors.push_back(error);
						lineIndex++;
					}
				}
			}

			return module;
		}
	}
}