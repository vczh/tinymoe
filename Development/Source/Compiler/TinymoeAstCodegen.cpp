#include "TinymoeAstCodegen.h"

using namespace tinymoe::ast;

namespace tinymoe
{
	namespace compiler
	{

		/*************************************************************
		SymbolAstScope
		*************************************************************/

		AstType::Ptr SymbolAstScope::GetType(GrammarSymbol::Ptr symbol, AstNode::Ptr parent)
		{
			if (symbol->target == GrammarSymbolTarget::Custom)
			{
				auto type = make_shared<AstReferenceType>();
				type->typeDeclaration = dynamic_pointer_cast<AstTypeDeclaration>(readAsts.find(symbol)->second);
				return type;
			}
			else
			{
				AstPredefinedTypeName typeName = AstPredefinedTypeName::Object;
				switch (symbol->target)
				{
				case GrammarSymbolTarget::Object:
					typeName = AstPredefinedTypeName::Object;
					break;
				case GrammarSymbolTarget::Array:
					typeName = AstPredefinedTypeName::Array;
					break;
				case GrammarSymbolTarget::Symbol:
					typeName = AstPredefinedTypeName::Symbol;
					break;
				case GrammarSymbolTarget::Boolean:
					typeName = AstPredefinedTypeName::Boolean;
					break;
				case GrammarSymbolTarget::Integer:
					typeName = AstPredefinedTypeName::Integer;
					break;
				case GrammarSymbolTarget::Float:
					typeName = AstPredefinedTypeName::Float;
					break;
				case GrammarSymbolTarget::String:
					typeName = AstPredefinedTypeName::String;
					break;
				}

				auto type = make_shared<AstPredefinedType>();
				type->typeName = typeName;
				return type;
			}
		}

		/*************************************************************
		SymbolAstContext
		*************************************************************/

		string SymbolAstContext::GetUniquePostfix()
		{
			stringstream ss;
			ss << "_" << uniqueId++;
			return ss.str();
		}

		/*************************************************************
		SymbolAstResult
		*************************************************************/

		SymbolAstResult::SymbolAstResult()
		{
		}

		SymbolAstResult::SymbolAstResult(shared_ptr<AstExpression> _value)
			:value(_value)
		{
		}

		SymbolAstResult::SymbolAstResult(shared_ptr<AstStatement> _statement)
			:statement(_statement)
		{
		}

		SymbolAstResult::SymbolAstResult(shared_ptr<AstExpression> _value, shared_ptr<AstStatement> _statement, shared_ptr<AstLambdaExpression> _continuation)
			:value(_value)
			, statement(_statement)
			, continuation(_continuation)
		{
		}

		bool SymbolAstResult::RequireCps()const
		{
			return statement && continuation;
		}

		SymbolAstResult SymbolAstResult::ReplaceValue(shared_ptr<AstExpression> _value)
		{
			return SymbolAstResult(_value, statement, continuation);
		}

		SymbolAstResult SymbolAstResult::ReplaceValue(shared_ptr<AstExpression> _value, shared_ptr<AstLambdaExpression> _continuation)
		{
			return SymbolAstResult(_value, statement, _continuation);
		}

		void SymbolAstResult::MergeForExpression(const SymbolAstResult& result, SymbolAstContext& context, vector<AstExpression::Ptr>& exprs, int& exprStart, AstDeclaration::Ptr& state)
		{
			if (result.RequireCps())
			{
				auto block = make_shared<AstBlockStatement>();
				for (int i = exprStart; (size_t)i < exprs.size(); i++)
				{
					auto var = make_shared<AstDeclarationStatement>();
					{
						auto decl = make_shared<AstSymbolDeclaration>();
						decl->composedName = "$var" + context.GetUniquePostfix();
						var->declaration = decl;

						auto declstat = make_shared<AstDeclarationStatement>();
						declstat->declaration = decl;
						block->statements.push_back(declstat);

						auto assign = make_shared<AstAssignmentStatement>();
						block->statements.push_back(assign);

						auto ref = make_shared<AstReferenceExpression>();
						ref->reference = decl;
						assign->target = ref;

						assign->value = exprs[i];
					}
					
					auto ref = make_shared<AstReferenceExpression>();
					ref->reference = var->declaration;
					exprs[i] = ref;
				}

				if (continuation)
				{
					continuation->statement = block;
				}
				else
				{
					statement = block;
				}
				continuation = result.continuation;
				exprStart = exprs.size();
				state = continuation->arguments[0];
			}
			exprs.push_back(result.value);
		}

		void SymbolAstResult::AppendStatement(AstStatement::Ptr& target, AstStatement::Ptr statement)
		{
			if (!target)
			{
				target = statement;
			}
			else if (auto block = dynamic_pointer_cast<AstBlockStatement>(target))
			{
				block->statements.push_back(statement);
			}
			else
			{
				block = make_shared<AstBlockStatement>();
				block->statements.push_back(target);
				block->statements.push_back(statement);
				target = block;
			}
		}

		void SymbolAstResult::AppendStatement(AstStatement::Ptr _statement)
		{
			value = nullptr;
			if (RequireCps())
			{
				if (!continuation->statement)
				{
					continuation->statement = make_shared<AstBlockStatement>();
				}
				AppendStatement(continuation->statement, _statement);
			}
			else
			{
				AppendStatement(statement, _statement);
			}
		}

		void SymbolAstResult::MergeForStatement(const SymbolAstResult& result, AstDeclaration::Ptr& state)
		{
			AppendStatement(result.statement);
			if (result.RequireCps())
			{
				continuation = result.continuation;
			}

			if (continuation)
			{
				state = continuation->arguments[0];
			}
		}

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
			if (type == FunctionArgumentType::Argument)
			{
				return AstPair(nullptr, nullptr);
			}
			else if (type == FunctionArgumentType::Assignable)
			{
				auto read = make_shared<AstSymbolDeclaration>();
				read->composedName = "$read_" + name->GetComposedName();

				auto write = make_shared<AstSymbolDeclaration>();
				write->composedName = "$write_" + name->GetComposedName();
				return AstPair(read, write);
			}
			else
			{
				auto ast = make_shared<AstSymbolDeclaration>();
				ast->composedName = name->GetComposedName();
				return AstPair(ast, nullptr);
			}
		}

		FunctionFragment::AstPair FunctionArgumentFragment::CreateAst(weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstSymbolDeclaration>();
			ast->composedName = declaration->GetComposedName();
			return AstPair(ast, nullptr);
		}

		/*************************************************************
		GenerateAst
		*************************************************************/

		typedef multimap<SymbolFunction::Ptr, SymbolFunction::Ptr>			MultipleDispatchMap;
		typedef map<SymbolFunction::Ptr, SymbolModule::Ptr>					FunctionModuleMap;
		typedef map<SymbolFunction::Ptr, AstFunctionDeclaration::Ptr>		FunctionAstMap;

		void GenerateStaticAst(
			SymbolAssembly::Ptr symbolAssembly,
			AstAssembly::Ptr assembly,
			SymbolAstScope::Ptr scope,
			MultipleDispatchMap& mdc,
			FunctionModuleMap& functionModules,
			FunctionAstMap& functionAsts
			)
		{
			for (auto module : symbolAssembly->symbolModules)
			{
				for (auto dfp : module->declarationFunctions)
				{
					functionModules.insert(make_pair(dfp.second, module));
					if (!dfp.second->multipleDispatchingRoot.expired())
					{
						mdc.insert(make_pair(dfp.second->multipleDispatchingRoot.lock(), dfp.second));
					}
				}
			}

			for (auto module : symbolAssembly->symbolModules)
			{
				map<Declaration::Ptr, AstDeclaration::Ptr> decls;
				for (auto sdp : module->symbolDeclarations)
				{
					auto it = decls.find(sdp.second);
					if (it == decls.end())
					{
						auto ast = sdp.second->GenerateAst(module);
						assembly->declarations.push_back(ast);
						decls.insert(make_pair(sdp.second, ast));
						scope->readAsts.insert(make_pair(sdp.first, ast));

						auto itfunc = module->declarationFunctions.find(sdp.second);
						if (itfunc != module->declarationFunctions.end())
						{
							auto func = dynamic_pointer_cast<AstFunctionDeclaration>(ast);
							functionAsts.insert(make_pair(itfunc->second, func));
							scope->functionPrototypes.insert(make_pair(sdp.first, func));
						}
					}
					else
					{
						scope->readAsts.insert(make_pair(sdp.first, it->second));
					}
				}
			}
			
			for (auto module : symbolAssembly->symbolModules)
			{
				for (auto sdp : module->symbolDeclarations)
				{
					if (auto typeDecl = dynamic_pointer_cast<TypeDeclaration>(sdp.second))
					{
						if (typeDecl->parent)
						{
							auto type = scope->readAsts.find(sdp.first)->second;
							auto baseType = scope->GetType(module->baseTypes.find(typeDecl)->second, type);
						}
					}
				}
			}
		}

		void FillMultipleDispatchRedirectAst(
			AstFunctionDeclaration::Ptr ast,
			AstFunctionDeclaration::Ptr function
			)
		{
			auto astBlock = make_shared<AstBlockStatement>();
			ast->statement = astBlock;

			auto astExprStat = make_shared<AstExpressionStatement>();
			astBlock->statements.push_back(astExprStat);

			auto astInvoke = make_shared<AstInvokeExpression>();
			astExprStat->expression = astInvoke;

			auto astFunction = make_shared<AstReferenceExpression>();
			astFunction->reference = function;
			astInvoke->function = astFunction;

			for (auto argument : ast->arguments)
			{
				auto astArgument = make_shared<AstReferenceExpression>();
				astArgument->reference = argument;
				astInvoke->arguments.push_back(astArgument);
			}
		}

		void FillMultipleDispatchStepAst(
			AstFunctionDeclaration::Ptr ast,
			string functionName,
			int dispatch
			)
		{
			auto astBlock = make_shared<AstBlockStatement>();
			ast->statement = astBlock;

			auto astExprStat = make_shared<AstExpressionStatement>();
			astBlock->statements.push_back(astExprStat);

			auto astInvoke = make_shared<AstInvokeExpression>();
			astExprStat->expression = astInvoke;

			auto astFieldAccess = make_shared<AstFieldAccessExpression>();
			astFieldAccess->composedFieldName = functionName;
			astInvoke->function = astFieldAccess;

			auto astTargetObject = make_shared<AstReferenceExpression>();
			astTargetObject->reference = ast->arguments[dispatch];
			astFieldAccess->target = astTargetObject;

			for (auto argument : ast->arguments)
			{
				auto astArgument = make_shared<AstReferenceExpression>();
				astArgument->reference = argument;
				astInvoke->arguments.push_back(astArgument);
			}
		}

		void GenerateMultipleDispatchAsts(
			SymbolAssembly::Ptr symbolAssembly,
			AstAssembly::Ptr assembly,
			SymbolAstScope::Ptr scope,
			MultipleDispatchMap& mdc,
			FunctionModuleMap& functionModules,
			FunctionAstMap& functionAsts
			)
		{
			auto it = mdc.begin();
			while (it != mdc.end())
			{
				auto lower = mdc.lower_bound(it->first);
				auto upper = mdc.lower_bound(it->second);
				auto module = functionModules.find(it->first)->second;
				auto rootFunc = it->first;
				auto rootAst = functionAsts.find(rootFunc)->second;

				set<int> dispatches;
				AstFunctionDeclaration::Ptr dispatchFailAst;
				for (it = lower; it != upper; it++)
				{
					auto func = it->second;
					for (auto ita = func->function->name.begin(); ita != func->function->name.end(); ita++)
					{
						if (func->argumentTypes.find(*ita) != func->argumentTypes.end())
						{
							dispatches.insert(ita - func->function->name.begin());
						}
					}
				}
				
				FillMultipleDispatchStepAst(rootAst, "$dispatch<>" + rootAst->composedName, rootAst->readArgumentAstMap.find(*dispatches.begin())->second);
				{
					auto ast = rootFunc->function->GenerateAst(module);
					ast->composedName = "$dispatch_fail<>" + rootAst->composedName;
					assembly->declarations.push_back(ast);
					dispatchFailAst = dynamic_pointer_cast<AstFunctionDeclaration>(ast);
				}

				set<string> createdFunctions, typeFunctions, objectFunctions;
				for (it = lower; it != upper; it++)
				{
					auto func = it->second;
					auto module = functionModules.find(func)->second;
					string signature;
					for (auto itd = dispatches.begin(); itd != dispatches.end(); itd++)
					{
						string methodName = "$dispatch<" + signature + ">" + rootAst->composedName;
						auto ast = dynamic_pointer_cast<AstFunctionDeclaration>(func->function->GenerateAst(module));
						ast->composedName = methodName;

						auto ita = func->argumentTypes.find(func->function->name[*itd]);
						if (ita == func->argumentTypes.end())
						{
							auto type = make_shared<AstPredefinedType>();
							type->typeName = AstPredefinedTypeName::Object;
							ast->ownerType = type;
							objectFunctions.insert(methodName);
						}
						else
						{
							ast->ownerType = scope->GetType(ita->second, ast);
							typeFunctions.insert(methodName);
						}

						if (itd != dispatches.begin())
						{
							signature += ", ";
						}
						{
							stringstream o;
							ast->ownerType->Print(o, 0);
							signature += o.str();
						}

						string functionName = "$dispatch<" + signature + ">" + rootAst->composedName;
						if (createdFunctions.insert(functionName).second)
						{
							assembly->declarations.push_back(ast);
							auto itd2 = itd;
							if (++itd2 == dispatches.end())
							{
								FillMultipleDispatchRedirectAst(ast, functionAsts.find(func)->second);
							}
							else
							{
								FillMultipleDispatchStepAst(ast, functionName, ast->readArgumentAstMap.find(*itd)->second);
							}
						}
					}
				}

				for (auto name : typeFunctions)
				{
					if (objectFunctions.find(name) == objectFunctions.end())
					{
						auto ast = dynamic_pointer_cast<AstFunctionDeclaration>(rootFunc->function->GenerateAst(module));
						ast->composedName = name;
						{
							auto type = make_shared<AstPredefinedType>();
							type->typeName = AstPredefinedTypeName::Object;
							ast->ownerType = type;
						}
						assembly->declarations.push_back(ast);
						FillMultipleDispatchRedirectAst(ast, dispatchFailAst);
					}
				}

				functionAsts.find(rootFunc)->second = dispatchFailAst;
				it = upper;
			}
		}

		ast::AstAssembly::Ptr GenerateAst(SymbolAssembly::Ptr symbolAssembly)
		{
			auto assembly = make_shared<AstAssembly>();
			auto scope = make_shared<SymbolAstScope>();

			multimap<SymbolFunction::Ptr, SymbolFunction::Ptr> multipleDispatchChildren;
			map<SymbolFunction::Ptr, SymbolModule::Ptr> functionModules;
			map<SymbolFunction::Ptr, AstFunctionDeclaration::Ptr> functionAsts;
			GenerateStaticAst(symbolAssembly, assembly, scope, multipleDispatchChildren, functionModules, functionAsts);
			GenerateMultipleDispatchAsts(symbolAssembly, assembly, scope, multipleDispatchChildren, functionModules, functionAsts);

			for (auto fap : functionAsts)
			{
				auto func = fap.first;
				auto ast = fap.second;
				auto module = functionModules.find(func)->second;

				SymbolAstContext context;
				context.function = ast;
				auto itdecl = ast->arguments.begin();
				if (func->cpsStateVariable)
				{
					context.createdVariables.push_back(func->cpsStateVariable);
					scope->readAsts.insert(make_pair(func->cpsStateVariable, ast->stateArgument));
				}
				itdecl++;
				if (func->categorySignalVariable)
				{
					context.createdVariables.push_back(func->categorySignalVariable);
					scope->readAsts.insert(make_pair(func->categorySignalVariable, ast->signalArgument));
					itdecl++;
				}
				if (func->cpsContinuationVariable)
				{
					context.createdVariables.push_back(func->cpsContinuationVariable);
					scope->readAsts.insert(make_pair(func->cpsContinuationVariable, ast->continuationArgument));
				}
				if (func->resultVariable)
				{
					context.createdVariables.push_back(func->resultVariable);
					scope->readAsts.insert(make_pair(func->resultVariable, ast->resultVariable));
				}

				for (auto arg : func->arguments)
				{
					if (auto var = dynamic_pointer_cast<VariableArgumentFragment>(arg.second))
					{
						if (var->type == FunctionArgumentType::Argument)
						{
							continue;
						}
						else if (var->type == FunctionArgumentType::Assignable)
						{
							context.createdVariables.push_back(arg.first);
							scope->readAsts.insert(make_pair(arg.first, *itdecl++));
							scope->writeAsts.insert(make_pair(arg.first, *itdecl++));
							continue;
						}
						else if (var->type == FunctionArgumentType::Expression)
						{
							context.createdVariables.push_back(arg.first);
							scope->readAsts.insert(make_pair(arg.first, *itdecl++));
							scope->writeAsts.insert(make_pair(arg.first, nullptr));
							continue;
						}
					}
					context.createdVariables.push_back(arg.first);
					scope->readAsts.insert(make_pair(arg.first, *itdecl++));

					if (auto func = dynamic_pointer_cast<FunctionArgumentFragment>(arg.second))
					{
						auto ast = dynamic_pointer_cast<AstFunctionDeclaration>(func->declaration->GenerateAst(module));
						scope->functionPrototypes.insert(make_pair(arg.first, ast));
					}
				}

				ast->statement = func->statement->GenerateBodyAst(scope, context, ast->stateArgument, nullptr, true).statement;
				if (!dynamic_pointer_cast<AstBlockStatement>(ast->statement))
				{
					auto block = make_shared<AstBlockStatement>();
					block->statements.push_back(ast->statement);
					ast->statement = block;
				}

				for (auto var : context.createdVariables)
				{
					scope->readAsts.erase(var);
					scope->writeAsts.erase(var);
					scope->functionPrototypes.erase(var);
				}
			}

			assembly->SetParent();
			return assembly;
		}
	}
}