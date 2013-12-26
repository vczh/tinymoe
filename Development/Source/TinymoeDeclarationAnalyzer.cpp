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
				ownerName + " name should be empty.",
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
				ownerName + " name should be empty.",
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
				"CPS definition should start with \"cps\".",
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
				"Category definition should start with \"category\".",
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
				"Symbol definition should start with \"symbol\".",
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
				"Type definition should start with \"type\".",
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
					"Inheriting from a type should begins with a \":\".",
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

	FunctionDeclaration::Ptr FunctionDeclaration::Parse(CodeFile::Ptr codeFile, CodeError::List& errors, int& lineIndex)
	{
		if (0 > (size_t)lineIndex || (size_t)lineIndex >= codeFile->lines.size()) return nullptr;
		auto decl = make_shared<FunctionDeclaration>();
		return decl;
	}

	/*************************************************************
	Module
	*************************************************************/

	Module::Ptr Module::Parse(CodeFile::Ptr codeFile, CodeError::List& errors)
	{
		auto module = make_shared<Module>();
		return module;
	}
}