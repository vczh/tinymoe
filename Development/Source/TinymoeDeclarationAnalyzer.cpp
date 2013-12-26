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