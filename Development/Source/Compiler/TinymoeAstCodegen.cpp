#include "TinymoeAstCodegen.h"

using namespace tinymoe::ast;

namespace tinymoe
{
	namespace compiler
	{
		class SymbolAstScope
		{
		public:
			typedef map<GrammarSymbol::Ptr, AstDeclaration::Ptr>		SymbolAstDeclarationMap;

			SymbolAstDeclarationMap			readAsts;
			SymbolAstDeclarationMap			writeAsts;
		};

		/*************************************************************
		FunctionFragment::GetComposedName
		*************************************************************/

		string NameFragment::GetComposedName(bool primitive)
		{
			return name->GetComposedName();
		}

		string VariableArgumentFragment::GetComposedName(bool primitive)
		{
			string result;
			switch (type)
			{
			case FunctionArgumentType::Argument:
				result = "$argument";
				break;
			case FunctionArgumentType::Assignable:
				result = "assignable";
				break;
			case FunctionArgumentType::Expression:
				result = (primitive ? "$primitive" : "$expression");
				break;
			case FunctionArgumentType::List:
				result = "list";
				break;
			case FunctionArgumentType::Normal:
				result = (primitive ? "$primitive" : "$expression");
				break;
			}
			
			if (receivingType)
			{
				result += "<" + receivingType->GetComposedName() + ">";
			}
			return result;
		}

		string FunctionArgumentFragment::GetComposedName(bool primitive)
		{
			return (primitive ? "$primitive" : "$expression");
		}

		/*************************************************************
		FunctionFragment::GenerateAst
		*************************************************************/

		FunctionFragment::AstPair NameFragment::CreateAst(weak_ptr<ast::AstNode> parent)
		{
			return AstPair(nullptr, nullptr);
		}

		FunctionFragment::AstPair VariableArgumentFragment::CreateAst(weak_ptr<ast::AstNode> parent)
		{
			if (type == FunctionArgumentType::Assignable)
			{
				auto read = make_shared<AstSymbolDeclaration>();
				read->parent = parent;
				read->composedName = "$read_" + name->GetComposedName();

				auto write = make_shared<AstSymbolDeclaration>();
				write->parent = parent;
				write->composedName = "$write_" + name->GetComposedName();
				return AstPair(read, write);
			}
			else
			{
				auto ast = make_shared<AstSymbolDeclaration>();
				ast->parent = parent;
				ast->composedName = name->GetComposedName();
				return AstPair(ast, nullptr);
			}
		}

		FunctionFragment::AstPair FunctionArgumentFragment::CreateAst(weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstSymbolDeclaration>();
			ast->parent = parent;
			ast->composedName = declaration->GetComposedName();
			return AstPair(ast, nullptr);
		}

		/*************************************************************
		Declaration::GenerateAst
		*************************************************************/

		shared_ptr<ast::AstDeclaration> SymbolDeclaration::GenerateAst(shared_ptr<SymbolAstScope> scope, shared_ptr<SymbolModule> symbolModule, weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstSymbolDeclaration>();
			ast->parent = parent;
			ast->composedName = symbolModule->module->name->GetComposedName() + "::" + name->GetComposedName();
			return ast;
		}

		shared_ptr<ast::AstDeclaration> TypeDeclaration::GenerateAst(shared_ptr<SymbolAstScope> scope, shared_ptr<SymbolModule> symbolModule, weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstTypeDeclaration>();
			ast->parent = parent;
			ast->composedName = symbolModule->module->name->GetComposedName() + "::" + name->GetComposedName();
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

		shared_ptr<ast::AstDeclaration> FunctionDeclaration::GenerateAst(shared_ptr<SymbolAstScope> scope, shared_ptr<SymbolModule> symbolModule, weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstFunctionDeclaration>();
			ast->parent = parent;
			ast->composedName = symbolModule->module->name->GetComposedName() + "::" + GetComposedName();
			
			{
				auto argument = make_shared<AstSymbolDeclaration>();
				argument->parent = ast;
				argument->composedName = "$the_result";
				ast->resultVariable = argument;
			}
			{
				auto argument = make_shared<AstSymbolDeclaration>();
				argument->parent = ast;
				if (cps && cps->stateName)
				{
					argument->composedName = cps->stateName->GetComposedName();
				}
				else
				{
					argument->composedName = "$state";
				}
				ast->arguments.push_back(argument);
				ast->cpsStateArgument = argument;
			}
			if (cps && cps->continuationName)
			{
				auto argument = make_shared<AstSymbolDeclaration>();
				argument->parent = ast;
				argument->composedName = cps->continuationName->GetComposedName();
				ast->arguments.push_back(argument);
				ast->cpsContinuationArgument = argument;
			}

			if (category && category->signalName)
			{
				auto argument = make_shared<AstSymbolDeclaration>();
				argument->parent = ast;
				argument->composedName = category->signalName->GetComposedName();
				ast->arguments.push_back(argument);
				ast->categorySignalArgument = argument;
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
					ast->arguments.push_back(pair.first);
				}
				if (pair.second)
				{
					ast->arguments.push_back(pair.second);
				}
			}
			{
				auto argument = make_shared<AstSymbolDeclaration>();
				argument->parent = ast;
				argument->composedName = "$continuation";
				ast->arguments.push_back(argument);
			}

			ast->statement = make_shared<AstBlockStatement>();
			ast->statement->parent = ast;
			return ast;
		}

		/*************************************************************
		GenerateAst
		*************************************************************/

		ast::AstAssembly::Ptr GenerateAst(SymbolAssembly::Ptr symbolAssembly)
		{
			auto assembly = make_shared<AstAssembly>();
			auto scope = make_shared<SymbolAstScope>();
			for (auto module : symbolAssembly->symbolModules)
			{
				map<Declaration::Ptr, AstDeclaration::Ptr> decls;
				for (auto sdp : module->symbolDeclarations)
				{
					auto it = decls.find(sdp.second);
					if (it == decls.end())
					{
						auto ast = sdp.second->GenerateAst(scope, module, AstNode::WeakPtr(assembly));
						assembly->declarations.push_back(ast);
						decls.insert(make_pair(sdp.second, ast));
						scope->readAsts.insert(make_pair(sdp.first, ast));
					}
					else
					{
						scope->readAsts.insert(make_pair(sdp.first, it->second));
					}
				}
			}
			return assembly;
		}
	}
}