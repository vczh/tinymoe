#include "TinymoeAstCodegen.h"

using namespace tinymoe::ast;

namespace tinymoe
{
	namespace compiler
	{

		/*************************************************************
		Expression::GenerateAst
		*************************************************************/

		shared_ptr<ast::AstLambdaExpression> Expression::GenerateContinuationLambdaAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state)
		{
			auto lambda = make_shared<AstLambdaExpression>();
			{
				auto ref = make_shared<AstSymbolDeclaration>();
				ref->composedName = T("$state") + context.GetUniquePostfix();
				lambda->arguments.push_back(ref);
			}
			{
				auto ref = make_shared<AstSymbolDeclaration>();
				ref->composedName = T("$result") + context.GetUniquePostfix();
				lambda->arguments.push_back(ref);
			}
			return lambda;
		}

		SymbolAstResult LiteralExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state)
		{
			switch (token.type)
			{
			case CodeTokenType::Integer:
				{
					char_t* endptr = 0;
					int result = strtol_t(token.value.c_str(), &endptr, 10);
					auto ast = make_shared<AstIntegerExpression>();
					ast->value = result;
					return SymbolAstResult(ast);
				}
			case CodeTokenType::Float:
				{
					char_t* endptr = 0;
					double result = strtod_t(token.value.c_str(), &endptr);
					auto ast = make_shared<AstFloatExpression>();
					ast->value = result;
					return SymbolAstResult(ast);
				}
			case CodeTokenType::String:
				{
					auto ast = make_shared<AstStringExpression>();
					ast->value = token.value;
					return SymbolAstResult(ast);
				}
			}
			return SymbolAstResult();
		}

		SymbolAstResult ArgumentExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state)
		{
			return SymbolAstResult();
		}

		SymbolAstResult ReferenceExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state)
		{
			switch (symbol->target)
			{
			case GrammarSymbolTarget::True:
				{
					auto ast = make_shared<AstLiteralExpression>();
					ast->literalName = AstLiteralName::True;
					return SymbolAstResult(ast);
				}
			case GrammarSymbolTarget::False:
				{
					auto ast = make_shared<AstLiteralExpression>();
					ast->literalName = AstLiteralName::False;
					return SymbolAstResult(ast);
				}
			case GrammarSymbolTarget::Null:
				{
					auto ast = make_shared<AstLiteralExpression>();
					ast->literalName = AstLiteralName::Null;
					return SymbolAstResult(ast);
				}
			}

			if (scope->writeAsts.find(symbol) != scope->writeAsts.end())
			{
				auto stat = make_shared<AstExpressionStatement>();

				auto invoke = make_shared<AstInvokeExpression>();
				stat->expression = invoke;
				
				auto function = make_shared<AstReferenceExpression>();
				function->reference = scope->readAsts.find(symbol)->second;
				invoke->function = function;
				{
					auto arg = make_shared<AstReferenceExpression>();
					arg->reference = state;
					invoke->arguments.push_back(arg);
				}
				auto lambda = GenerateContinuationLambdaAst(scope, context, state);
				invoke->arguments.push_back(lambda);

				auto ref = make_shared<AstReferenceExpression>();
				ref->reference = lambda->arguments[1];

				return SymbolAstResult(ref, stat, lambda);
			}
			else
			{
				auto ast = make_shared<AstReferenceExpression>();
				ast->reference = scope->readAsts.find(symbol)->second;
				return SymbolAstResult(ast);
			}
		}

		SymbolAstResult InvokeExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state)
		{
			Expression::Ptr realFunction;
			Expression::List realArguments;
			bool invokeContinuation = false;
			if (auto ref = dynamic_pointer_cast<ReferenceExpression>(function))
			{
				switch (ref->symbol->target)
				{
				case GrammarSymbolTarget::Invoke:
					{
						realFunction = arguments[0];
						realArguments = dynamic_pointer_cast<ListExpression>(arguments[1])->elements;
					}
					break;
				case GrammarSymbolTarget::InvokeContinuation:
					{
						realFunction = arguments[0];
						realArguments = dynamic_pointer_cast<ListExpression>(arguments[1])->elements;
						invokeContinuation = true;
					}
					break;
				case GrammarSymbolTarget::NewTypeOfFields:
					{
						SymbolAstResult result;
						vector<AstExpression::Ptr> exprs;
						int exprStart = 0;
						for (auto expr : dynamic_pointer_cast<ListExpression>(arguments[1])->elements)
						{
							result.MergeForExpression(expr->GenerateAst(scope, context, state), context, exprs, exprStart, state);
						}

						auto ast = make_shared<AstNewTypeExpression>();
						ast->type = scope->GetType(dynamic_pointer_cast<ReferenceExpression>(arguments[0])->symbol);
						ast->fields = exprs;
						return result.ReplaceValue(ast);
					}
				case GrammarSymbolTarget::NewArray:
					{
						SymbolAstResult result = arguments[0]->GenerateAst(scope, context, state);
						auto ast = make_shared<AstNewArrayExpression>();
						ast->length = result.value;
						return result.ReplaceValue(ast);
					}
				case GrammarSymbolTarget::GetArrayItem:
					{
						SymbolAstResult result;
						vector<AstExpression::Ptr> exprs;
						int exprStart = 0;
						for (auto expr : arguments)
						{
							result.MergeForExpression(expr->GenerateAst(scope, context, state), context, exprs, exprStart, state);
						}

						auto ast = make_shared<AstArrayAccessExpression>();
						ast->target = exprs[1];
						ast->index = exprs[0];
						return result.ReplaceValue(ast);
					}
				case GrammarSymbolTarget::GetArrayLength:
					{
						SymbolAstResult result = arguments[0]->GenerateAst(scope, context, state);
						auto ast = make_shared<AstArrayLengthExpression>();
						ast->target = result.value;
						return result.ReplaceValue(ast);
					}
				case GrammarSymbolTarget::IsType:
					{
						SymbolAstResult result = arguments[0]->GenerateAst(scope, context, state);
						auto ast = make_shared<AstTestTypeExpression>();
						ast->target = result.value;
						ast->type = scope->GetType(dynamic_pointer_cast<ReferenceExpression>(arguments[1])->symbol);
						return result.ReplaceValue(ast);
					}
				case GrammarSymbolTarget::IsNotType:
					{
						SymbolAstResult result = arguments[0]->GenerateAst(scope, context, state);
						
						auto stat = make_shared<AstExpressionStatement>();

						auto invoke = make_shared<AstInvokeExpression>();
						stat->expression = invoke;
						{
							auto ref = make_shared<AstReferenceExpression>();
							ref->reference = scope->opNot;
							invoke->function = ref;
						}
						{
							auto arg = make_shared<AstReferenceExpression>();
							arg->reference = state;
							invoke->arguments.push_back(arg);
						}
						{
							auto arg = make_shared<AstTestTypeExpression>();
							arg->target = result.value;
							arg->type = scope->GetType(dynamic_pointer_cast<ReferenceExpression>(arguments[1])->symbol);
							invoke->arguments.push_back(arg);
						}

						auto lambda = Expression::GenerateContinuationLambdaAst(scope, context, state);
						invoke->arguments.push_back(lambda);
			
						auto ast = make_shared<AstReferenceExpression>();
						ast->reference = lambda->arguments[1];

						result.AppendStatement(stat);
						return result.ReplaceValue(ast, lambda);
					}
				case GrammarSymbolTarget::GetField:
					{
						SymbolAstResult result = arguments[1]->GenerateAst(scope, context, state);
						auto ast = make_shared<AstFieldAccessExpression>();
						ast->target = result.value;
						ast->composedFieldName = dynamic_pointer_cast<ArgumentExpression>(arguments[0])->name->GetComposedName();
						return result.ReplaceValue(ast);
					}
				}
			}

			if (!realFunction)
			{
				realFunction = function;
				realArguments = arguments;
			}

			SymbolAstResult result;
			vector<AstExpression::Ptr> exprs;
			int exprStart = 0;

			result.MergeForExpression(realFunction->GenerateAst(scope, context, state), context, exprs, exprStart, state);
			for (auto arg : realArguments)
			{
				result.MergeForExpression(arg->GenerateAst(scope, context, state), context, exprs, exprStart, state);
			}

			auto stat = make_shared<AstExpressionStatement>();

			auto invoke = make_shared<AstInvokeExpression>();
			stat->expression = invoke;
			auto it = exprs.begin();
			invoke->function = *it++;
			{
				auto ref = make_shared<AstReferenceExpression>();
				ref->reference = state;
				invoke->arguments.push_back(ref);
			}
			while (it != exprs.end())
			{
				invoke->arguments.push_back(*it++);
			}

			if (invokeContinuation)
			{
				auto ast = make_shared<AstLiteralExpression>();
				ast->literalName = AstLiteralName::Null;
				
				auto lambda = GenerateContinuationLambdaAst(scope, context, state);
				auto lambdaStat = make_shared<AstExpressionStatement>();
				lambdaStat->expression = lambda;
				
				result.AppendStatement(stat);
				result.AppendStatement(lambdaStat);
				return result.ReplaceValue(ast, lambda);
			}
			else
			{
				auto lambda = GenerateContinuationLambdaAst(scope, context, state);
				invoke->arguments.push_back(lambda);
			
				auto ast = make_shared<AstReferenceExpression>();
				ast->reference = lambda->arguments[1];

				result.AppendStatement(stat);
				return result.ReplaceValue(ast, lambda);
			}
		}

		SymbolAstResult ListExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state)
		{
			SymbolAstResult result;
			vector<AstExpression::Ptr> exprs;
			int exprStart = 0;

			for (auto element : elements)
			{
				result.MergeForExpression(element->GenerateAst(scope, context, state), context, exprs, exprStart, state);
			}

			auto ast = make_shared<AstNewArrayLiteralExpression>();
			for (auto expr : exprs)
			{
				ast->elements.push_back(expr);
			}
			return result.ReplaceValue(ast);
		}

		SymbolAstResult UnaryExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state)
		{
			AstDeclaration::Ptr astOp;
			switch (op)
			{
			case UnaryOperator::Not:
				astOp = scope->opNot;
				break;
			case UnaryOperator::Positive:
				astOp = scope->opPos;
				break;
			case UnaryOperator::Negative:
				astOp = scope->opNeg;
				break;
			}

			SymbolAstResult result;
			vector<AstExpression::Ptr> exprs;
			int exprStart = 0;
			result.MergeForExpression(operand->GenerateAst(scope, context, state), context, exprs, exprStart, state);

			auto stat = make_shared<AstExpressionStatement>();

			auto invoke = make_shared<AstInvokeExpression>();
			stat->expression = invoke;
			{
				auto ref = make_shared<AstReferenceExpression>();
				ref->reference = astOp;
				invoke->function = ref;
			}
			{
				auto arg = make_shared<AstReferenceExpression>();
				arg->reference = state;
				invoke->arguments.push_back(arg);
			}
			invoke->arguments.push_back(exprs[0]);

			auto lambda = Expression::GenerateContinuationLambdaAst(scope, context, state);
			invoke->arguments.push_back(lambda);
			
			auto ast = make_shared<AstReferenceExpression>();
			ast->reference = lambda->arguments[1];

			result.AppendStatement(stat);
			return result.ReplaceValue(ast, lambda);
		}

		SymbolAstResult BinaryExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state)
		{
			AstDeclaration::Ptr astOp;
			switch (op)
			{
			case BinaryOperator::Concat:
				astOp = scope->opConcat;
				break;
			case BinaryOperator::Add:
				astOp = scope->opAdd;
				break;
			case BinaryOperator::Sub:
				astOp = scope->opSub;
				break;
			case BinaryOperator::Mul:
				astOp = scope->opMul;
				break;
			case BinaryOperator::Div:
				astOp = scope->opDiv;
				break;
			case BinaryOperator::IntDiv:
				astOp = scope->opIntDiv;
				break;
			case BinaryOperator::Mod:
				astOp = scope->opMod;
				break;
			case BinaryOperator::LT:
				astOp = scope->opLT;
				break;
			case BinaryOperator::GT:
				astOp = scope->opGT;
				break;
			case BinaryOperator::LE:
				astOp = scope->opLE;
				break;
			case BinaryOperator::GE:
				astOp = scope->opGE;
				break;
			case BinaryOperator::EQ:
				astOp = scope->opEQ;
				break;
			case BinaryOperator::NE:
				astOp = scope->opNE;
				break;
			case BinaryOperator::And:
				astOp = scope->opAnd;
				break;
			case BinaryOperator::Or:
				astOp = scope->opOr;
				break;
			}

			SymbolAstResult result;
			vector<AstExpression::Ptr> exprs;
			int exprStart = 0;
			
			result.MergeForExpression(first->GenerateAst(scope, context, state), context, exprs, exprStart, state);
			result.MergeForExpression(second->GenerateAst(scope, context, state), context, exprs, exprStart, state);

			auto stat = make_shared<AstExpressionStatement>();

			auto invoke = make_shared<AstInvokeExpression>();
			stat->expression = invoke;
			{
				auto ref = make_shared<AstReferenceExpression>();
				ref->reference = astOp;
				invoke->function = ref;
			}
			{
				auto arg = make_shared<AstReferenceExpression>();
				arg->reference = state;
				invoke->arguments.push_back(arg);
			}
			invoke->arguments.push_back(exprs[0]);
			invoke->arguments.push_back(exprs[1]);

			auto lambda = Expression::GenerateContinuationLambdaAst(scope, context, state);
			invoke->arguments.push_back(lambda);
			
			auto ast = make_shared<AstReferenceExpression>();
			ast->reference = lambda->arguments[1];

			result.AppendStatement(stat);
			return result.ReplaceValue(ast, lambda);
		}
	}
}