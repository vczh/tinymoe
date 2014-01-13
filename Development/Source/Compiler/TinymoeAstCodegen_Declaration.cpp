#include "TinymoeAstCodegen.h"

using namespace tinymoe::ast;

namespace tinymoe
{
	namespace compiler
	{

		/*************************************************************
		Declaration::GenerateAst
		*************************************************************/

		shared_ptr<ast::AstDeclaration> SymbolDeclaration::GenerateAst(shared_ptr<SymbolModule> symbolModule)
		{
			auto ast = make_shared<AstSymbolDeclaration>();
			ast->composedName = symbolModule->module->name->GetComposedName() + "::" + name->GetComposedName();
			return ast;
		}

		shared_ptr<ast::AstDeclaration> TypeDeclaration::GenerateAst(shared_ptr<SymbolModule> symbolModule)
		{
			auto ast = make_shared<AstTypeDeclaration>();
			ast->composedName = symbolModule->module->name->GetComposedName() + "::" + name->GetComposedName();
			for (auto field : fields)
			{
				auto astField = make_shared<AstSymbolDeclaration>();
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

		shared_ptr<ast::AstDeclaration> FunctionDeclaration::GenerateAst(shared_ptr<SymbolModule> symbolModule)
		{
			auto ast = make_shared<AstFunctionDeclaration>();
			ast->composedName = symbolModule->module->name->GetComposedName() + "::" + GetComposedName();
			
			{
				auto argument = make_shared<AstSymbolDeclaration>();
				argument->composedName = "$the_result";
				ast->resultVariable = argument;
			}
			{
				auto argument = make_shared<AstSymbolDeclaration>();
				if (cps && cps->stateName)
				{
					argument->composedName = cps->stateName->GetComposedName();
				}
				else
				{
					argument->composedName = "$state";
				}
				ast->arguments.push_back(argument);
				ast->stateArgument = argument;
			}
			if (category && category->signalName)
			{
				auto argument = make_shared<AstSymbolDeclaration>();
				argument->composedName = category->signalName->GetComposedName();
				ast->arguments.push_back(argument);
				ast->signalArgument = argument;
			}
			if (bodyName)
			{
				auto argument = bodyName->CreateAst(ast).first;
				ast->arguments.push_back(argument);
				ast->blockBodyArgument = argument;
			}

			for (auto it = name.begin(); it != name.end(); it++)
			{
				auto pair = (*it)->CreateAst(ast);
				if (pair.first)
				{
					ast->readArgumentAstMap.insert(make_pair(it - name.begin(), ast->arguments.size()));
					ast->arguments.push_back(pair.first);
				}
				if (pair.second)
				{
					ast->writeArgumentAstMap.insert(make_pair(it - name.begin(), ast->arguments.size()));
					ast->arguments.push_back(pair.second);
				}
			}

			{
				auto argument = make_shared<AstSymbolDeclaration>();
				if (cps && cps->continuationName)
				{
					argument->composedName = cps->continuationName->GetComposedName();
				}
				else
				{
					argument->composedName = "$continuation";
				}
				ast->arguments.push_back(argument);
				ast->continuationArgument = argument;
			}
			return ast;
		}
	}
}