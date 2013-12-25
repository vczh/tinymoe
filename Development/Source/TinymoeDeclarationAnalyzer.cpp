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

	SymbolName::SymbolName()
	{
	}

	/*************************************************************
	Declaration
	*************************************************************/

	Declaration::Declaration()
	{
	}

	/*************************************************************
	SymbolDeclaration
	*************************************************************/

	SymbolDeclaration::SymbolDeclaration()
	{
	}

	/*************************************************************
	TypeDeclaration
	*************************************************************/

	TypeDeclaration::TypeDeclaration()
	{
	}

	/*************************************************************
	FunctionCategory
	*************************************************************/

	FunctionCategory::FunctionCategory()
	{
	}

	/*************************************************************
	FunctionDeclaration::Fragment
	*************************************************************/

	FunctionDeclaration::Fragment::Fragment()
	{
	}

	/*************************************************************
	FunctionDeclaration::ArgumentFragment
	*************************************************************/

	FunctionDeclaration::ArgumentFragment::ArgumentFragment()
	{
	}

	/*************************************************************
	FunctionDeclaration::NameFragment
	*************************************************************/

	FunctionDeclaration::NameFragment::NameFragment()
	{
	}

	/*************************************************************
	FunctionDeclaration::VariableArgumentFragment
	*************************************************************/

	FunctionDeclaration::VariableArgumentFragment::VariableArgumentFragment()
	{
	}

	/*************************************************************
	FunctionDeclaration::FunctionArgumentFragment
	*************************************************************/

	FunctionDeclaration::FunctionArgumentFragment::FunctionArgumentFragment()
	{
	}

	/*************************************************************
	FunctionDeclaration
	*************************************************************/

	FunctionDeclaration::FunctionDeclaration()
	{
	}

	/*************************************************************
	Module
	*************************************************************/

	Module::Module()
	{
	}

	Module::Ptr Module::Parse(CodeFile::Ptr codeFile, CodeError::List& errors)
	{
		auto module = make_shared<Module>();
		return module;
	}
}