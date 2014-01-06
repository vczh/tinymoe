#include "TinymoeAstCodegen.h"

using namespace tinymoe::ast;

namespace tinymoe
{
	namespace compiler
	{
		/*************************************************************
		FunctionFragment::GetComposedName
		*************************************************************/

		string NameFragment::GetComposedName(bool primitive)
		{
			return name->GetComposedName();
		}

		string VariableArgumentFragment::GetComposedName(bool primitive)
		{
			switch (type)
			{
			case FunctionArgumentType::Argument:
				return "$argument";
			case FunctionArgumentType::Assignable:
				return "assignable";
			case FunctionArgumentType::Expression:
				return (primitive ? "$primitive" : "$expression");
			case FunctionArgumentType::List:
				return "list";
			case FunctionArgumentType::Normal:
				return (primitive ? "$primitive" : "$expression");
			}
			return "";
		}

		string FunctionArgumentFragment::GetComposedName(bool primitive)
		{
			return (primitive ? "$primitive" : "$expression");
		}

		/*************************************************************
		FunctionFragment::GenerateAst
		*************************************************************/

		shared_ptr<ast::AstSymbolDeclaration> NameFragment::CreateAst(weak_ptr<ast::AstNode> parent)
		{
			return nullptr;
		}

		shared_ptr<ast::AstSymbolDeclaration> VariableArgumentFragment::CreateAst(weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstSymbolDeclaration>();
			ast->parent = parent;
			ast->composedName = name->GetComposedName();
			return ast;
		}

		shared_ptr<ast::AstSymbolDeclaration> FunctionArgumentFragment::CreateAst(weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstSymbolDeclaration>();
			ast->parent = parent;
			ast->composedName = declaration->GetComposedName();
			return ast;
		}

		/*************************************************************
		Declaration::GenerateAst
		*************************************************************/

		shared_ptr<ast::AstDeclaration> SymbolDeclaration::GenerateAst(shared_ptr<SymbolAssembly> symbolAssembly, shared_ptr<SymbolModule> symbolModule, weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstSymbolDeclaration>();
			ast->parent = parent;
			ast->composedName = symbolModule->module->name->GetComposedName() + ":" + name->GetComposedName();
			return ast;
		}

		shared_ptr<ast::AstDeclaration> TypeDeclaration::GenerateAst(shared_ptr<SymbolAssembly> symbolAssembly, shared_ptr<SymbolModule> symbolModule, weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstTypeDeclaration>();
			ast->parent = parent;
			ast->composedName = symbolModule->module->name->GetComposedName() + ":" + name->GetComposedName();
			for (auto field : fields)
			{
				auto astField = make_shared<AstSymbolDeclaration>();
				astField->parent = ast;
				astField->composedName = field->GetComposedName();
				ast->fields.push_back(astField);
			}
			return ast;
		}

		string FunctionDeclaration::GetComposedName()
		{
			string result;
			for (auto it = name.begin(); it != name.end(); it++)
			{
				result += (*it)->GetComposedName(type == FunctionDeclarationType::Phrase && (it == name.begin() || it + 1 == name.end()));
				if (it + 1 != name.end())
				{
					result += "_";
				}
			}
			return result;
		}

		shared_ptr<ast::AstDeclaration> FunctionDeclaration::GenerateAst(shared_ptr<SymbolAssembly> symbolAssembly, shared_ptr<SymbolModule> symbolModule, weak_ptr<ast::AstNode> parent)
		{
			auto symbolFunction = symbolModule->declarationFunctions.find(shared_from_this())->second;
			if (symbolFunction->multipleDispatchingRoot.expired())
			{
				auto ast = make_shared<AstFunctionDeclaration>();
				ast->parent = parent;

				ast->resultVariable = make_shared <AstSymbolDeclaration>();
				ast->resultVariable->parent = ast;
				ast->resultVariable->composedName = "the_result";

				ast->composedName = symbolModule->module->name->GetComposedName() + ":" + GetComposedName();
				for (auto it = name.begin(); it != name.end(); it++)
				{
					if (auto argument = (*it)->CreateAst(ast))
					{
						ast->arguments.push_back(argument);
					}
				}

				ast->statement = make_shared<AstBlockStatement>();
				ast->statement->parent = ast;
				return ast;
			}
			else
			{
				return nullptr;
			}
		}

		/*************************************************************
		GenerateAst
		*************************************************************/

		ast::AstAssembly::Ptr GenerateAst(SymbolAssembly::Ptr symbolAssembly)
		{
			auto assembly = make_shared<AstAssembly>();
			for (auto module : symbolAssembly->symbolModules)
			{
				for (auto decl : module->module->declarations)
				{
					assembly->declarations.push_back(decl->GenerateAst(symbolAssembly, module, AstNode::WeakPtr(assembly)));
				}
			}
			return assembly;
		}
	}
}