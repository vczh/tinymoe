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
	Module
	*************************************************************/

	Module::Ptr Module::Parse(CodeFile::Ptr codeFile, CodeError::List& errors)
	{
		auto module = make_shared<Module>();
		return module;
	}
}