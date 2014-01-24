#include "TinymoeAstCodegen.h"

using namespace tinymoe::ast;

namespace tinymoe
{
	namespace compiler
	{

		/*************************************************************
		Statement::GenerateAst
		*************************************************************/

		SymbolAstResult Statement::GenerateExitAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state)
		{
			auto stat = make_shared<AstExpressionStatement>();

			auto invoke = make_shared<AstInvokeExpression>();
			stat->expression = invoke;

			auto cont = make_shared<AstReferenceExpression>();
			cont->reference = context.function->continuationArgument;
			invoke->function = cont;
			{
				auto arg = make_shared<AstReferenceExpression>();
				arg->reference = state;
				invoke->arguments.push_back(arg);
			}
			{
				auto arg = make_shared<AstReferenceExpression>();
				arg->reference = context.function->resultVariable;
				invoke->arguments.push_back(arg);
			}
			return SymbolAstResult(stat);
		}

		void Statement::GenerateAssignableArgumentPairAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<ast::AstDeclaration> signal, shared_ptr<Expression> assignable, vector<shared_ptr<ast::AstSymbolDeclaration>>::iterator& itvar, shared_ptr<ast::AstExpression>& reader, shared_ptr<ast::AstExpression>& writer)
		{
			AstDeclaration::Ptr declRead, declWrite;
			bool invoke = false;
			if (auto ref = dynamic_pointer_cast<ReferenceExpression>(assignable))
			{
				auto itwrite = scope->writeAsts.find(ref->symbol);
				auto itread = scope->readAsts.find(ref->symbol);

				if (itwrite != scope->writeAsts.end())
				{
					declRead = itread->second;
					declWrite = itwrite->second;
					if (!declWrite)
					{
						declWrite = declRead;
					}
					invoke = true;
				}
				else
				{
					declRead = declWrite = itread->second;
				}
			}
			else if (auto arg = dynamic_pointer_cast<ArgumentExpression>(assignable))
			{
				declRead = declWrite = *itvar++;
			}

			if (invoke)
			{
				{
					auto expr = make_shared<AstReferenceExpression>();
					expr->reference = declRead;
					reader = expr;
				}
				{
					auto expr = make_shared<AstReferenceExpression>();
					expr->reference = declWrite;
					writer = expr;
				}
			}
			else
			{
				{
					auto lambda = make_shared<AstLambdaExpression>();
					{
						auto ref = make_shared<AstSymbolDeclaration>();
						ref->composedName = T("$state") + context.GetUniquePostfix();
						lambda->arguments.push_back(ref);
					}
					{
						auto ref = make_shared<AstSymbolDeclaration>();
						ref->composedName = T("$continuation") + context.GetUniquePostfix();
						lambda->arguments.push_back(ref);
					}
					reader = lambda;
								
					auto stat = make_shared<AstExpressionStatement>();
					lambda->statement = stat;

					auto invoke = make_shared<AstInvokeExpression>();
					stat->expression = invoke;

					auto cont = make_shared<AstReferenceExpression>();
					cont->reference = lambda->arguments[1];
					invoke->function = cont;
					{
						auto arg = make_shared<AstReferenceExpression>();
						arg->reference = lambda->arguments[0];
						invoke->arguments.push_back(arg);
					}
					{
						auto arg = make_shared<AstReferenceExpression>();
						arg->reference = declRead;
						invoke->arguments.push_back(arg);
					}
				}
				{
					auto lambda = make_shared<AstLambdaExpression>();
					{
						auto ref = make_shared<AstSymbolDeclaration>();
						ref->composedName = T("$state") + context.GetUniquePostfix();
						lambda->arguments.push_back(ref);
					}
					{
						auto ref = make_shared<AstSymbolDeclaration>();
						ref->composedName = T("$input") + context.GetUniquePostfix();
						lambda->arguments.push_back(ref);
					}
					{
						auto ref = make_shared<AstSymbolDeclaration>();
						ref->composedName = T("$continuation") + context.GetUniquePostfix();
						lambda->arguments.push_back(ref);
					}
					writer = lambda;
								
					auto block = make_shared<AstBlockStatement>();
					lambda->statement = block;
					{
						auto stat = make_shared<AstAssignmentStatement>();
						block->statements.push_back(stat);
						{
							auto arg = make_shared<AstReferenceExpression>();
							arg->reference = declWrite;
							stat->target = arg;
						}
						{
							auto arg = make_shared<AstReferenceExpression>();
							arg->reference = lambda->arguments[1];
							stat->value = arg;
						}
					}
					{
						auto stat = make_shared<AstExpressionStatement>();
						block->statements.push_back(stat);

						auto invoke = make_shared<AstInvokeExpression>();
						stat->expression = invoke;

						auto cont = make_shared<AstReferenceExpression>();
						cont->reference = lambda->arguments[2];
						invoke->function = cont;
						{
							auto arg = make_shared<AstReferenceExpression>();
							arg->reference = lambda->arguments[0];
							invoke->arguments.push_back(arg);
						}
						{
							auto arg = make_shared<AstLiteralExpression>();
							arg->literalName = AstLiteralName::Null;
							invoke->arguments.push_back(arg);
						}
					}
				}
			}
		}

		void Statement::GenerateExpressionArgumentAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<ast::AstDeclaration> signal, shared_ptr<Expression> expression, shared_ptr<ast::AstExpression>& reader)
		{
			if (auto ref = dynamic_pointer_cast<ReferenceExpression>(expression))
			{
				auto itwrite = scope->writeAsts.find(ref->symbol);
				auto itread = scope->readAsts.find(ref->symbol);

				if (itwrite != scope->writeAsts.end())
				{
					auto expr = make_shared<AstReferenceExpression>();
					expr->reference = itread->second;
					reader = expr;
					return;
				}
			}

			auto lambda = make_shared<AstLambdaExpression>();
			{
				auto decl = make_shared<AstSymbolDeclaration>();
				decl->composedName = T("$state") + context.GetUniquePostfix();
				lambda->arguments.push_back(decl);
			}
			{
				auto decl = make_shared<AstSymbolDeclaration>();
				decl->composedName = T("$continuation") + context.GetUniquePostfix();
				lambda->arguments.push_back(decl);
			}
			SymbolAstResult result = expression->GenerateAst(scope, context, lambda->arguments[0]);

			auto stat = make_shared<AstExpressionStatement>();

			auto invoke = make_shared<AstInvokeExpression>();
			stat->expression = invoke;
			{
				auto ref = make_shared<AstReferenceExpression>();
				ref->reference = lambda->arguments[1];
				invoke->function = ref;
			}
			{
				auto ref = make_shared<AstReferenceExpression>();
				ref->reference = lambda->arguments[0];
				invoke->arguments.push_back(ref);
			}
			invoke->arguments.push_back(result.value);
			result.AppendStatement(stat);
			lambda->statement = result.statement;

			reader = lambda;
		}

		void Statement::CreateNewVariableDeclarations(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<ast::AstDeclaration> signal, vector<shared_ptr<ast::AstSymbolDeclaration>>& newVariableDecls, SymbolAstResult& statResult)
		{
			for (auto symbol : newVariables)
			{
				auto decl = make_shared<AstSymbolDeclaration>();
				auto& ids = symbol.first->fragments[0]->identifiers;
				for (auto it = ids.begin(); it != ids.end(); it++)
				{
					decl->composedName += *it;
					if (it + 1 != ids.end())
					{
						decl->composedName += T("_");
					}
				}
				newVariableDecls.push_back(decl);

				auto stat = make_shared<AstDeclarationStatement>();
				stat->declaration = decl;
				statResult.AppendStatement(stat);

				context.createdVariables.push_back(symbol.first);
				scope->readAsts.insert(make_pair(symbol.first, decl));
			}
		}

		void Statement::CreateBlockArgumentDeclarations(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<ast::AstDeclaration> signal, vector<shared_ptr<ast::AstSymbolDeclaration>>& blockArgumentDecls)
		{
			for (auto symbol : blockArguments)
			{
				auto decl = make_shared<AstSymbolDeclaration>();
				auto& ids = symbol.first->fragments[0]->identifiers;
				for (auto it = ids.begin(); it != ids.end(); it++)
				{
					decl->composedName += *it;
					if (it + 1 != ids.end())
					{
						decl->composedName += T("_");
					}
				}
				blockArgumentDecls.push_back(decl);

				context.createdVariables.push_back(symbol.first);
				scope->readAsts.insert(make_pair(symbol.first, decl));
			}
		}

		SymbolAstResult Statement::GeneratePredefinedAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<ast::AstDeclaration> signal)
		{
			switch (statementSymbol->target)
			{
			case GrammarSymbolTarget::Select:
				{
					SymbolAstResult result = statementExpression->arguments[0]->GenerateAst(scope, context, state);
					auto selectedValue = result.value;

					auto selectLambda = Expression::GenerateContinuationLambdaAst(scope, context, state);

					auto selectContinuation = make_shared<AstSymbolDeclaration>();
					selectContinuation->composedName = T("$select_continuation") + context.GetUniquePostfix();

					auto selectValue = make_shared<AstSymbolDeclaration>();
					selectValue->composedName = T("$select_value") + context.GetUniquePostfix();

					{
						auto stat = make_shared<AstDeclarationStatement>();
						stat->declaration = selectContinuation;
						result.AppendStatement(stat);
					}
					{
						auto stat = make_shared<AstDeclarationStatement>();
						stat->declaration = selectValue;
						result.AppendStatement(stat);
					}
					{
						auto stat = make_shared<AstAssignmentStatement>();
						stat->value = selectLambda;

						auto access = make_shared<AstReferenceExpression>();
						access->reference = selectContinuation;
						stat->target = access;

						result.AppendStatement(stat);
					}
					{
						auto stat = make_shared<AstAssignmentStatement>();
						stat->value = selectedValue;

						auto access = make_shared<AstReferenceExpression>();
						access->reference = selectValue;
						stat->target = access;

						result.AppendStatement(stat);
					}

					if (result.continuation)
					{
						state = result.continuation->arguments[0];
					}

					shared_ptr<AstIfStatement> lastIfStat;
					Statement::Ptr caseElse;
					for (auto childCase : statements)
					{
						if (childCase->statementSymbol->target == GrammarSymbolTarget::CaseElse)
						{
							caseElse = childCase;
							continue;
						}
						SymbolAstResult caseResult = childCase->statementExpression->arguments[0]->GenerateAst(scope, context, state);
						if (caseResult.continuation)
						{
							state = caseResult.continuation->arguments[0];
						}

						{
							auto stat = make_shared<AstExpressionStatement>();

							auto invoke = make_shared<AstInvokeExpression>();
							stat->expression = invoke;
							{
								auto ref = make_shared<AstReferenceExpression>();
								ref->reference = scope->opEQ;
								invoke->function = ref;
							}
							{
								auto arg = make_shared<AstReferenceExpression>();
								arg->reference = state;
								invoke->arguments.push_back(arg);
							}
							{
								auto arg = make_shared<AstReferenceExpression>();
								arg->reference = selectValue;
								invoke->arguments.push_back(arg);
							}
							invoke->arguments.push_back(caseResult.value);

							auto lambda = Expression::GenerateContinuationLambdaAst(scope, context, state);
							invoke->arguments.push_back(lambda);
			
							auto ast = make_shared<AstReferenceExpression>();
							ast->reference = lambda->arguments[1];

							caseResult.AppendStatement(stat);
							caseResult = caseResult.ReplaceValue(ast, lambda);
						}

						auto ifstat = make_shared<AstIfStatement>();
						{
							ifstat->condition = caseResult.value;
						}
						caseResult.AppendStatement(ifstat);

						SymbolAstResult bodyResult = childCase->GenerateBodyAst(scope, context, state, selectContinuation);
						ifstat->trueBranch = bodyResult.statement;

						if (lastIfStat)
						{
							lastIfStat->falseBranch = caseResult.statement;
						}
						else
						{
							result.AppendStatement(caseResult.statement);
						}
						result.continuation = caseResult.continuation;
						lastIfStat = ifstat;
					}

					if (caseElse)
					{
						SymbolAstResult bodyResult = caseElse->GenerateBodyAst(scope, context, state, selectContinuation);

						if (lastIfStat)
						{
							lastIfStat->falseBranch = bodyResult.statement;
						}
						else
						{
							result.AppendStatement(bodyResult.statement);
						}
					}
					else
					{
						auto stat = make_shared<AstExpressionStatement>();

						auto invoke = make_shared<AstInvokeExpression>();
						stat->expression = invoke;
						{
							auto ref = make_shared<AstReferenceExpression>();
							ref->reference = selectContinuation;
							invoke->function = ref;
						}
						{
							auto arg = make_shared<AstReferenceExpression>();
							arg->reference = state;
							invoke->arguments.push_back(arg);
						}
						{
							auto arg = make_shared<AstLiteralExpression>();
							arg->literalName = AstLiteralName::Null;
							invoke->arguments.push_back(arg);
						}
						
						if (lastIfStat)
						{
							lastIfStat->falseBranch = stat;
						}
						else
						{
							result.AppendStatement(stat);
						}
					}

					result.continuation = selectLambda;
					return result;
				}
			case GrammarSymbolTarget::RedirectTo:
				{
					auto stat = make_shared<AstExpressionStatement>();

					auto invoke = make_shared<AstInvokeExpression>();
					stat->expression = invoke;

					auto external = make_shared<AstExternalSymbolExpression>();
					external->name = dynamic_pointer_cast<LiteralExpression>(statementExpression->arguments[0])->token.value;
					invoke->function = external;

					for (auto decl : context.function->arguments)
					{
						if (decl != context.function->continuationArgument)
						{
							auto arg = make_shared<AstReferenceExpression>();
							arg->reference = (decl == context.function->stateArgument ? state : decl);
							invoke->arguments.push_back(arg);
						}
					}

					auto lambda = Expression::GenerateContinuationLambdaAst(scope, context, state);
					invoke->arguments.push_back(lambda);
			
					auto ast = make_shared<AstReferenceExpression>();
					ast->reference = lambda->arguments[1];

					return SymbolAstResult(ast, stat, lambda);
				}
			case GrammarSymbolTarget::Assign:
				{
					SymbolAstResult result = statementExpression->arguments[1]->GenerateAst(scope, context, state);
					auto assignValue = result.value;
					if (result.continuation)
					{
						state = result.continuation->arguments[0];
					}

					AstDeclaration::Ptr variable;
					bool invoke = false;
					bool newVariable = false;
					if (auto ref = dynamic_pointer_cast<ReferenceExpression>(statementExpression->arguments[0]))
					{
						auto it = scope->writeAsts.find(ref->symbol);
						if (it == scope->writeAsts.end())
						{
							it = scope->readAsts.find(ref->symbol);
						}
						else
						{
							invoke = true;
							if (!it->second)
							{
								it = scope->readAsts.find(ref->symbol);
							}
						}
						variable = it->second;
					}
					else if (auto arg = dynamic_pointer_cast<ArgumentExpression>(statementExpression->arguments[0]))
					{
						newVariable = true;
						variable = make_shared<AstSymbolDeclaration>();
						variable->composedName = arg->name->GetComposedName();

						auto varStat = make_shared<AstDeclarationStatement>();
						varStat->declaration = variable;
						result.AppendStatement(varStat);

						auto symbol = newVariables.begin()->first;
						context.createdVariables.push_back(symbol);
						scope->readAsts.insert(make_pair(symbol, variable));
					}
					
					if (invoke)
					{
						auto ast = make_shared<AstExpressionStatement>();

						auto invoke = make_shared<AstInvokeExpression>();
						ast->expression = invoke;
						{
							auto access = make_shared<AstReferenceExpression>();
							access->reference = variable;
							invoke->function = access;
						}

						{
							auto arg = make_shared<AstReferenceExpression>();
							arg->reference = state;
							invoke->arguments.push_back(arg);
						}
						invoke->arguments.push_back(assignValue);

						auto lambda = Expression::GenerateContinuationLambdaAst(scope, context, state);
						invoke->arguments.push_back(lambda);

						result.AppendStatement(ast);
						result.continuation = lambda;
						return result;
					}
					else
					{
						auto ast = make_shared<AstAssignmentStatement>();
						ast->value = assignValue;

						auto access = make_shared<AstReferenceExpression>();
						access->reference = variable;
						ast->target = access;

						result.AppendStatement(ast);
						if (newVariable && !result.RequireCps())
						{
							auto stat = make_shared<AstExpressionStatement>();

							auto invoke = make_shared<AstInvokeExpression>();
							stat->expression = invoke;

							auto lambda = Expression::GenerateContinuationLambdaAst(scope, context, state);
							invoke->function = lambda;
							{
								auto arg = make_shared<AstReferenceExpression>();
								arg->reference = state;
								invoke->arguments.push_back(arg);
							}
							{
								auto arg = make_shared<AstLiteralExpression>();
								arg->literalName = AstLiteralName::Null;
								invoke->arguments.push_back(arg);
							}

							result.AppendStatement(stat);
							result.continuation = lambda;
						}
						return result;
					}
				}
			case GrammarSymbolTarget::SetArrayItem:
				{
					SymbolAstResult result;
					AstExpression::List exprs;
					int exprStart = 0;
					result.MergeForExpression(statementExpression->arguments[0]->GenerateAst(scope, context, state), context, exprs, exprStart, state);
					result.MergeForExpression(statementExpression->arguments[1]->GenerateAst(scope, context, state), context, exprs, exprStart, state);
					result.MergeForExpression(statementExpression->arguments[2]->GenerateAst(scope, context, state), context, exprs, exprStart, state);

					auto ast = make_shared<AstAssignmentStatement>();

					auto access = make_shared<AstArrayAccessExpression>();
					ast->target = access;
					access->target = exprs[1];
					access->index = exprs[0];

					ast->value = exprs[2];

					result.AppendStatement(ast);
					return result;
				}
			case GrammarSymbolTarget::SetField:
				{
					SymbolAstResult result;
					AstExpression::List exprs;
					int exprStart = 0;
					result.MergeForExpression(statementExpression->arguments[1]->GenerateAst(scope, context, state), context, exprs, exprStart, state);
					result.MergeForExpression(statementExpression->arguments[2]->GenerateAst(scope, context, state), context, exprs, exprStart, state);

					auto ast = make_shared<AstAssignmentStatement>();

					auto access = make_shared<AstFieldAccessExpression>();
					ast->target = access;
					access->target = exprs[0];
					access->composedFieldName = dynamic_pointer_cast<ArgumentExpression>(statementExpression->arguments[0])->name->GetComposedName();

					ast->value = exprs[1];

					result.AppendStatement(ast);
					return result;
				}
			}

			return SymbolAstResult();
		}

		SymbolAstResult Statement::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<ast::AstDeclaration> signal)
		{
			SymbolAstResult statResult = GeneratePredefinedAst(scope, context, state, signal);
			if (statResult.statement)
			{
				return statResult;
			}
			SymbolAstResult exprResult;
			AstExpression::List exprs;
			int exprStart = 0;

			AstSymbolDeclaration::List newVariableDecls, blockArgumentDecls;
			CreateNewVariableDeclarations(scope, context, state, signal, newVariableDecls, statResult);
			int contextVariableCount = context.createdVariables.size();
			CreateBlockArgumentDeclarations(scope, context, state, signal, blockArgumentDecls);

			auto itvar = newVariableDecls.begin();
			auto itarg = statementExpression->arguments.begin();
			for (auto name : statementSymbol->fragments)
			{
				if (auto var = dynamic_pointer_cast<VariableArgumentFragment>(name->functionFragment))
				{
					switch (var->type)
					{
					case FunctionArgumentType::Argument:
						itarg++;
						break;
					case FunctionArgumentType::Assignable:
						{
							AstExpression::Ptr reader, writer;
							GenerateAssignableArgumentPairAst(scope, context, state, signal, *itarg++, itvar, reader, writer);
							exprs.push_back(reader);
							exprs.push_back(writer);
						}
						break;
					case FunctionArgumentType::Expression:
						{
							AstExpression::Ptr reader;
							GenerateExpressionArgumentAst(scope, context, state, signal, *itarg++, reader);
							exprs.push_back(reader);
						}
						break;
					default:
						exprResult.MergeForExpression((*itarg)->GenerateAst(scope, context, state), context, exprs, exprStart, state);
						itarg++;
					}
				}
				else if (auto func = dynamic_pointer_cast<FunctionArgumentFragment>(name->functionFragment))
				{
					exprResult.MergeForExpression((*itarg)->GenerateAst(scope, context, state), context, exprs, exprStart, state);
					itarg++;
				}
			}

			auto invoke = make_shared<AstInvokeExpression>();
			{
				auto arg = make_shared<AstReferenceExpression>();
				arg->reference = scope->readAsts.find(statementSymbol)->second;
				invoke->function = arg;
			}
			auto targetFunction = scope->functionPrototypes.find(statementSymbol)->second;
			auto statementContinuation = Expression::GenerateContinuationLambdaAst(scope, context, state);;

			if (targetFunction->stateArgument)
			{
				auto arg = make_shared<AstReferenceExpression>();
				arg->reference = state;
				invoke->arguments.push_back(arg);
			}
			if (targetFunction->signalArgument)
			{
				auto arg = make_shared<AstReferenceExpression>();
				arg->reference = signal;
				invoke->arguments.push_back(arg);
			}
			if (targetFunction->blockBodyArgument)
			{
				auto lambda = make_shared<AstLambdaExpression>();
				{
					auto ref = make_shared<AstSymbolDeclaration>();
					ref->composedName = T("$state") + context.GetUniquePostfix();
					lambda->arguments.push_back(ref);
				}
				for (auto decl : blockArgumentDecls)
				{
					lambda->arguments.push_back(decl);
				}
				{
					auto ref = make_shared<AstSymbolDeclaration>();
					ref->composedName = T("$continuation") + context.GetUniquePostfix();
					lambda->arguments.push_back(ref);
				}

				auto oldContinuation = context.continuation;
				context.continuation = *(lambda->arguments.end() - 1);
				lambda->statement = GenerateBodyAst(scope, context, lambda->arguments[0], *(lambda->arguments.end() - 1)).statement;
				context.continuation = oldContinuation;
				invoke->arguments.push_back(lambda);

				for (int i = context.createdVariables.size() - 1; i >= contextVariableCount; i--)
				{
					scope->readAsts.erase(context.createdVariables[i]);
					context.createdVariables.erase(context.createdVariables.begin() + i);
				}
			}
			invoke->arguments.insert(invoke->arguments.end(), exprs.begin(), exprs.end());

			if (targetFunction->continuationArgument)
			{
				invoke->arguments.push_back(statementContinuation);
			}

			auto stat = make_shared<AstExpressionStatement>();
			stat->expression = invoke;
			exprResult.AppendStatement(stat);
			statResult.MergeForStatement(exprResult, state);
			statResult.continuation = statementContinuation;
			return statResult;
		}

		SymbolAstResult Statement::GenerateBodyAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<ast::AstDeclaration> continuation)
		{
			SymbolAstResult result;
			for (auto stat : statements)
			{
				AstDeclaration::Ptr signal;
				if (stat->connectToPreviousBlock)
				{
					signal = result.continuation->arguments[1];
				}
				result.MergeForStatement(stat->GenerateAst(scope, context, state, signal), state);
			}

			if (continuation)
			{
				auto stat = make_shared<AstExpressionStatement>();

				auto invoke = make_shared<AstInvokeExpression>();
				stat->expression = invoke;

				auto cont = make_shared<AstReferenceExpression>();
				cont->reference = continuation;
				invoke->function = cont;

				if (result.continuation)
				{
					{
						auto arg = make_shared<AstReferenceExpression>();
						arg->reference = result.continuation->arguments[0];
						invoke->arguments.push_back(arg);
					}
					{
						auto arg = make_shared<AstReferenceExpression>();
						arg->reference = result.continuation->arguments[1];
						invoke->arguments.push_back(arg);
					}
				}
				else
				{
					{
						auto arg = make_shared<AstReferenceExpression>();
						arg->reference = state;
						invoke->arguments.push_back(arg);
					}
					{
						auto arg = make_shared<AstLiteralExpression>();
						arg->literalName = AstLiteralName::Null;
						invoke->arguments.push_back(arg);
					}
				}
				result.AppendStatement(stat);
			}
			return result;
		}
	}
}