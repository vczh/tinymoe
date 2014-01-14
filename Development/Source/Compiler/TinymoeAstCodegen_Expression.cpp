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
				ref->composedName = "$state" + context.GetUniquePostfix();
				lambda->arguments.push_back(ref);
			}
			{
				auto ref = make_shared<AstSymbolDeclaration>();
				ref->composedName = "$result" + context.GetUniquePostfix();
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
					char* endptr = 0;
					int result = strtol(token.value.c_str(), &endptr, 10);
					auto ast = make_shared<AstIntegerExpression>();
					ast->value = result;
					return SymbolAstResult(ast);
				}
			case CodeTokenType::Float:
				{
					char* endptr = 0;
					double result = strtod(token.value.c_str(), &endptr);
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
			Expression::Ptr func;
			Expression::List args;

			if (auto ref = dynamic_pointer_cast<ReferenceExpression>(function))
			{
				switch (ref->symbol->target)
				{
				case GrammarSymbolTarget::Invoke:
					func = arguments[0];
					break;
				case GrammarSymbolTarget::InvokeWith:
					func = arguments[0];
					args = dynamic_pointer_cast<ListExpression>(arguments[1])->elements;
					break;
				case GrammarSymbolTarget::NewType:
					{
						auto ast = make_shared<AstNewTypeExpression>();
						ast->type = scope->GetType(dynamic_pointer_cast<ReferenceExpression>(arguments[0])->symbol, ast);
						return SymbolAstResult(ast);
					}
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
						ast->type = scope->GetType(dynamic_pointer_cast<ReferenceExpression>(arguments[0])->symbol, ast);
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
						ast->type = scope->GetType(dynamic_pointer_cast<ReferenceExpression>(arguments[1])->symbol, ast);
						return result.ReplaceValue(ast);
					}
				case GrammarSymbolTarget::IsNotType:
					{
						SymbolAstResult result = arguments[0]->GenerateAst(scope, context, state);
						auto ast = make_shared<AstTestTypeExpression>();
						ast->target = result.value;
						ast->type = scope->GetType(dynamic_pointer_cast<ReferenceExpression>(arguments[1])->symbol, ast);

						auto unary = make_shared<AstUnaryExpression>();
						unary->op = AstUnaryOperator::Not;
						unary->operand = ast;
						return result.ReplaceValue(unary);
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
			if (!func)
			{
				func = function;
				args = arguments;
			}

			SymbolAstResult result;
			vector<AstExpression::Ptr> exprs;
			int exprStart = 0;

			result.MergeForExpression(func->GenerateAst(scope, context, state), context, exprs, exprStart, state);
			for (auto arg : args)
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

			auto lambda = GenerateContinuationLambdaAst(scope, context, state);
			invoke->arguments.push_back(lambda);
			
			auto ast = make_shared<AstReferenceExpression>();
			ast->reference = lambda->arguments[1];

			result.AppendStatement(stat);
			return result.ReplaceValue(ast, lambda);
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
			AstUnaryOperator astOp = AstUnaryOperator::Not;
			switch (op)
			{
			case UnaryOperator::Not:
				astOp = AstUnaryOperator::Not;
				break;
			case UnaryOperator::Positive:
				astOp = AstUnaryOperator::Positive;
				break;
			case UnaryOperator::Negative:
				astOp = AstUnaryOperator::Negative;
				break;
			}

			SymbolAstResult result;
			vector<AstExpression::Ptr> exprs;
			int exprStart = 0;

			result.MergeForExpression(operand->GenerateAst(scope, context, state), context, exprs, exprStart, state);

			auto ast = make_shared<AstUnaryExpression>();
			ast->op = astOp;
			ast->operand = exprs[0];
			return result.ReplaceValue(ast);
		}

		SymbolAstResult BinaryExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state)
		{
			AstBinaryOperator astOp = AstBinaryOperator::Concat;
			
			switch (op)
			{
			case BinaryOperator::Concat:
				astOp = AstBinaryOperator::Concat;
				break;
			case BinaryOperator::Add:
				astOp = AstBinaryOperator::Add;
				break;
			case BinaryOperator::Sub:
				astOp = AstBinaryOperator::Sub;
				break;
			case BinaryOperator::Mul:
				astOp = AstBinaryOperator::Mul;
				break;
			case BinaryOperator::Div:
				astOp = AstBinaryOperator::Div;
				break;
			case BinaryOperator::IntDiv:
				astOp = AstBinaryOperator::IntDiv;
				break;
			case BinaryOperator::Mod:
				astOp = AstBinaryOperator::Mod;
				break;
			case BinaryOperator::LT:
				astOp = AstBinaryOperator::LT;
				break;
			case BinaryOperator::GT:
				astOp = AstBinaryOperator::GT;
				break;
			case BinaryOperator::LE:
				astOp = AstBinaryOperator::LE;
				break;
			case BinaryOperator::GE:
				astOp = AstBinaryOperator::GE;
				break;
			case BinaryOperator::EQ:
				astOp = AstBinaryOperator::EQ;
				break;
			case BinaryOperator::NE:
				astOp = AstBinaryOperator::NE;
				break;
			case BinaryOperator::And:
				astOp = AstBinaryOperator::And;
				break;
			case BinaryOperator::Or:
				astOp = AstBinaryOperator::Or;
				break;
			}

			SymbolAstResult result;
			vector<AstExpression::Ptr> exprs;
			int exprStart = 0;
			
			result.MergeForExpression(first->GenerateAst(scope, context, state), context, exprs, exprStart, state);
			result.MergeForExpression(second->GenerateAst(scope, context, state), context, exprs, exprStart, state);

			auto ast = make_shared<AstBinaryExpression>();
			ast->op = astOp;
			ast->first = exprs[0];
			ast->second = exprs[1];
			return result.ReplaceValue(ast);
		}
	}
}