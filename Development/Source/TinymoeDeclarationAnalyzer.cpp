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
		decl->name = make_shared<SymbolName>();

		while (++it != line->tokens.end())
		{
			if (it->IsNameFragmentToken())
			{
				decl->name->identifiers.push_back(*it);
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
		}

		if (decl->name->identifiers.size() == 0)
		{
			CodeError error = {
				line->tokens[0],
				line->tokens[0],
				"Symbol name should be empty.",
			};
			errors.push_back(error);
		}
		else
		{
		}
		return decl;
	}

	/*************************************************************
	TypeDeclaration
	*************************************************************/

	TypeDeclaration::Ptr TypeDeclaration::Parse(CodeFile::Ptr codeFile, CodeError::List& errors, int& lineIndex)
	{
		if (0 > (size_t)lineIndex || (size_t)lineIndex >= codeFile->lines.size()) return nullptr;
		auto decl = make_shared<TypeDeclaration>();
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