#include "TinymoeDeclarationAnalyzer.h"

namespace tinymoe
{

	/*************************************************************
	CodeFragment
	*************************************************************/

	CodeFragment::CodeFragment()
	{
	}

	CodeFragment::~CodeFragment()
	{
	}

	/*************************************************************
	SymbolName
	*************************************************************/

	bool SymbolName::ConsumeToken(CodeToken::List::iterator& it, CodeToken::List::iterator end, CodeTokenType tokenType, const string& content, CodeToken ownerToken, CodeError::List& errors)
	{
		if (it == end)
		{
			CodeError error = {
				ownerToken,
				ownerToken,
				"Incomplete code, \"" + content + "\" expected.",
			};
			errors.push_back(error);
			return false;
		}

		if (it->type != tokenType)
		{
			CodeError error = {
				*it,
				*it,
				"\"" + content + "\" expected but \"" + it->value + "\" found.",
			};
			errors.push_back(error);
			return false;
		}

		it++;
		return true;
	}

	SymbolName::Ptr SymbolName::ParseToEnd(CodeToken::List::iterator it, CodeToken::List::iterator end, const string& ownerName, CodeToken ownerToken, CodeError::List& errors)
	{
		auto symbolName = make_shared<SymbolName>();
		while (it != end)
		{
			if (it->IsNameFragmentToken())
			{
				symbolName->identifiers.push_back(*it);
			}
			else
			{
				CodeError error = {
					*it,
					*it,
					"Token is not a legal name: \"" + it->value + "\".",
				};
				errors.push_back(error);
			}
			it++;
		}

		if (symbolName->identifiers.size() == 0)
		{
			CodeError error = {
				ownerToken,
				ownerToken,
				ownerName + " name should not be empty.",
			};
			errors.push_back(error);
		}
		return symbolName;
	}

	SymbolName::Ptr SymbolName::ParseToFarest(CodeToken::List::iterator& it, CodeToken::List::iterator end, const string& ownerName, CodeToken ownerToken, CodeError::List& errors)
	{
		auto symbolName = make_shared<SymbolName>();
		while (it != end)
		{
			if (it->IsNameFragmentToken())
			{
				symbolName->identifiers.push_back(*it++);
			}
			else
			{
				break;
			}
		}

		if (symbolName->identifiers.size() == 0)
		{
			CodeError error = {
				ownerToken,
				ownerToken,
				ownerName + " name should not be empty.",
			};
			errors.push_back(error);
		}
		return symbolName;
	}

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
			CodeError error = {
				*it,
				*it,
				"CPS definition should begin with \"cps\".",
			};
			errors.push_back(error);
			return nullptr;
		}
		it++;

		auto decl = make_shared<FunctionCps>();
		if (!SymbolName::ConsumeToken(it, line->tokens.end(), CodeTokenType::OpenBracket, "(", cpsToken, errors)) goto END_OF_PARSING;
		decl->stateName = SymbolName::ParseToFarest(it, line->tokens.end(), "CPS state", cpsToken, errors);
		if (!SymbolName::ConsumeToken(it, line->tokens.end(), CodeTokenType::CloseBracket, ")", cpsToken, errors)) goto END_OF_PARSING;
		if (it == line->tokens.end()) goto END_OF_PARSING;
		
		if (!SymbolName::ConsumeToken(it, line->tokens.end(), CodeTokenType::OpenBracket, "(", cpsToken, errors)) goto END_OF_PARSING;
		decl->continuationName = SymbolName::ParseToFarest(it, line->tokens.end(), "CPS continuation", cpsToken, errors);
		if (!SymbolName::ConsumeToken(it, line->tokens.end(), CodeTokenType::CloseBracket, ")", cpsToken, errors)) goto END_OF_PARSING;
		if (it != line->tokens.end())
		{
			CodeError error = {
				*it,
				*it,
				"Too many tokens.",
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
			CodeError error = {
				*it,
				*it,
				"Category definition should begin with \"category\".",
			};
			errors.push_back(error);
			return nullptr;
		}
		it++;

		auto decl = make_shared<FunctionCategory>();
		if (it != line->tokens.end())
		{
			if (!SymbolName::ConsumeToken(it, line->tokens.end(), CodeTokenType::OpenBracket, "(", categoryToken, errors)) goto END_OF_SIGNAL_PARSING;
			decl->signalName = SymbolName::ParseToFarest(it, line->tokens.end(), "Category signal", categoryToken, errors);
			if (!SymbolName::ConsumeToken(it, line->tokens.end(), CodeTokenType::CloseBracket, ")", categoryToken, errors)) goto END_OF_SIGNAL_PARSING;

			if (it != line->tokens.end())
			{
				CodeError error = {
					*it,
					*it,
					"Too many tokens.",
				};
				errors.push_back(error);
			}
		}
	END_OF_SIGNAL_PARSING:

		while ((size_t)lineIndex < codeFile->lines.size())
		{
			line = codeFile->lines[lineIndex++];
			it = line->tokens.begin();

			if (it->value == "start")
			{
				if (decl->categoryName)
				{
					CodeError error = {
						*it,
						*it,
						"Too many start category name.",
					};
					errors.push_back(error);
				}
				else
				{
					auto startToken = *it;
					decl->categoryName = SymbolName::ParseToEnd(++it, line->tokens.end(), "Start category", startToken, errors);
				}
			}
			else if (it->value == "follow")
			{
				auto followToken = *it;
				decl->followCategories.push_back(SymbolName::ParseToEnd(++it, line->tokens.end(), "Follow category", followToken, errors));
			}
			else if (it->value == "closable")
			{
				decl->closable = true;
				if (++it != line->tokens.end())
				{
					CodeError error = {
						*it,
						*it,
						"Too many tokens.",
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
				CodeError error = {
					categoryToken,
					categoryToken,
					"A category with signal parameter should have follow categories.",
				};
				errors.push_back(error);
			}
		}

		if (!decl->categoryName)
		{
			if (!decl->closable)
			{
				CodeError error = {
					categoryToken,
					categoryToken,
					"A category without start category name should be closable.",
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
			CodeError error = {
				*it,
				*it,
				"Symbol definition should begin with \"symbol\".",
			};
			errors.push_back(error);
			return nullptr;
		}

		auto decl = make_shared<SymbolDeclaration>();
		decl->name = SymbolName::ParseToEnd(++it, line->tokens.end(), "Symbol", line->tokens[0], errors);
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
			CodeError error = {
				*it,
				*it,
				"Type definition should begin with \"type\".",
			};
			errors.push_back(error);
			return nullptr;
		}

		auto decl = make_shared<TypeDeclaration>();
		decl->name = SymbolName::ParseToFarest(++it, line->tokens.end(), "Type", line->tokens[0], errors);
		if (it != line->tokens.end())
		{
			if (it->type == CodeTokenType::Colon)
			{
				auto parentTypeToken = *it;
				decl->parent = SymbolName::ParseToEnd(++it, line->tokens.end(), "Parent type", parentTypeToken, errors);
			}
			else
			{
				CodeError error = {
					*it,
					*it,
					"Inheriting from a type should begin with a \":\".",
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
				decl->fields.push_back(SymbolName::ParseToEnd(line->tokens.begin(), line->tokens.end(), "Field", line->tokens[0], errors));
			}
		}

		if (!reachTheEnd)
		{
			CodeError error = {
				typeToken,
				typeToken,
				"Ending of the type is not found.",
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
			CodeError error = {
				ownerToken,
				ownerToken,
				"Function declaration should begin with \"phrase\", \"sentence\" or \"block\".",
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
				CodeError error = {
					ownerToken,
					ownerToken,
					"Function declaration should begin with \"phrase\", \"sentence\" or \"block\".",
				};
				errors.push_back(error);
				goto END_OF_PARSING;
			}
		}
		it++;

		while (it != end)
		{
			if (it->type == CodeTokenType::Colon)
			{
				decl->alias = SymbolName::ParseToFarest(++it, end, "Function alias", ownerToken, errors);
				break;
			}
			else if (it->type == CodeTokenType::OpenBracket)
			{
				if (auto argument = ArgumentFragment::Parse(++it, end, ownerToken, errors))
				{
					decl->name.push_back(argument);
					if (!SymbolName::ConsumeToken(it, end, CodeTokenType::CloseBracket, ")", ownerToken, errors)) goto END_OF_PARSING;
				}
				else
				{
					goto END_OF_PARSING;
				}
			}
			else if (it->IsNameFragmentToken())
			{
				auto nameFragment = make_shared<NameFragment>();
				nameFragment->name = SymbolName::ParseToFarest(it, end, "Function", ownerToken, errors);
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
			CodeError error = {
				functionToken,
				functionToken,
				"Function declaration should begin with \"phrase\", \"sentence\" or \"block\".",
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
			CodeError error = {
				functionToken,
				functionToken,
				"Function declaration should begin with \"phrase\", \"sentence\" or \"block\".",
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
					CodeError error = {
						*it,
						*it,
						"Too many tokens.",
					};
					errors.push_back(error);
				}
				break;
			default:
				{
					CodeError error = {
						functionToken,
						functionToken,
						"Function declaration should begin with \"phrase\", \"sentence\" or \"block\".",
					};
					errors.push_back(error);
					lineIndex--;
					goto END_OF_PARSING;
				}
			}

			if (decl->cps)
			{
				if (decl->type == FunctionDeclarationType::Phrase)
				{
					CodeError error = {
						functionToken,
						functionToken,
						"Phrase should not have a continuation definition.",
					};
					errors.push_back(error);
				}
			}
			if (decl->category)
			{
				if (decl->type != FunctionDeclarationType::Block)
				{
					CodeError error = {
						functionToken,
						functionToken,
						"Phrase and sentence should not have a category definition.",
					};
					errors.push_back(error);
				}
			}
			if (decl->type == FunctionDeclarationType::Block)
			{
				if (!decl->bodyName)
				{
					CodeError error = {
						functionToken,
						functionToken,
						"Block name should start with an argument for the block body.",
					};
					errors.push_back(error);
				}
			}

			if (decl->name.size() == 0)
			{
				CodeError error = {
					functionToken,
					functionToken,
					"Function name should not be empty.",
				};
				errors.push_back(error);
			}
			else 
			{
				if (decl->type != FunctionDeclarationType::Phrase)
				{
					if (!dynamic_pointer_cast<NameFragment>(decl->name[0]))
					{
						CodeError error = {
							functionToken,
							functionToken,
							"Sentence and block's name should not begin with an argument.",
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
							CodeError error = {
								functionToken,
								functionToken,
								"Function argument cannot appear just after another function argument.",
							};
							errors.push_back(error);
						}
						lastNameIsArgument = true;
					}
				}

				if (nameCount == 0)
				{
					CodeError error = {
						functionToken,
						functionToken,
						"Function name should not be form just by function arguments.",
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
			CodeError error = {
				ownerToken,
				ownerToken,
				"Function argument should begin with \"expression\", \"argument\", \"phrase\", \"sentence\" or a name.",
			};
			errors.push_back(error);
			return nullptr;
		}

		if (it->type == CodeTokenType::Phrase || it->type == CodeTokenType::Sentence)
		{
			auto decl = make_shared<FunctionArgumentFragment>();
			decl->declaration = FunctionDeclaration::Parse(it, end, nullptr, ownerToken, errors);
			return decl;
		}
		if (it->IsNameFragmentToken() && it->type != CodeTokenType::Block)
		{
			auto decl = make_shared<VariableArgumentFragment>();
			if (it->type == CodeTokenType::Expression)
			{
				decl->type = FunctionArgumentType::Expression;
				it++;
			}
			else if (it->type == CodeTokenType::Argument)
			{
				decl->type = FunctionArgumentType::Argument;
				it++;
			}
			else
			{
				decl->type = FunctionArgumentType::Normal;
			}
			decl->name = SymbolName::ParseToFarest(it, end, "Function argument", ownerToken, errors);

			if (it->type == CodeTokenType::Colon)
			{
				decl->receivingType = SymbolName::ParseToFarest(++it, end, "Function argument type", ownerToken, errors);
			}
			return decl;
		}
		else
		{
			CodeError error = {
				*it,
				*it,
				"Function argument should begin with \"expression\", \"argument\", \"phrase\", \"sentence\" or a name.",
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
					CodeError error = {
						*it,
						*it,
						"A module can only have one name.",
					};
					errors.push_back(error);
				}
				else
				{
					auto token = *it;
					module->name = SymbolName::ParseToEnd(++it, line->tokens.end(), "Module", token, errors);
				}
				lineIndex++;
				break;
			case CodeTokenType::Using:
				{
					auto token = *it;
					module->usings.push_back(SymbolName::ParseToEnd(++it, line->tokens.end(), "Module using", token, errors));
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
					CodeError error = {
						*it,
						*it,
						"Cannot process a declaration that begins with \"" + it->value + "\".",
					};
					errors.push_back(error);
					lineIndex++;
				}
			}
		}

		return module;
	}
}