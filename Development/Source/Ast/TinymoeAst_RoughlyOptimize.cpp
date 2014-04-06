#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstDeclaration::RoughlyOptimize
		*************************************************************/

		void AstDeclaration::RoughlyOptimize()
		{
		}

		void AstFunctionDeclaration::RoughlyOptimize()
		{
			statement->RoughlyOptimize(statement);

			set<AstDeclaration::Ptr> defined, used;
			statement->CollectUsedVariables(defined, used);
			statement->RemoveUnnecessaryVariables(defined, used, statement);

			statement->RoughlyOptimize(statement);
		}

		void AstAssembly::RoughlyOptimize()
		{
			for (auto decl : declarations)
			{
				decl->RoughlyOptimize();
			}
		}

		/*************************************************************
		AstExpression::CollectSideEffectExpressions
		*************************************************************/

		void AstLiteralExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		void AstIntegerExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		void AstFloatExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		void AstStringExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		void AstExternalSymbolExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		void AstReferenceExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		void AstNewTypeExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			for (auto field : fields)
			{
				field->CollectSideEffectExpressions(exprs);
			}
		}

		void AstTestTypeExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			target->CollectSideEffectExpressions(exprs);
		}

		void AstNewArrayExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			length->CollectSideEffectExpressions(exprs);
		}

		void AstNewArrayLiteralExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			for (auto element : elements)
			{
				element->CollectSideEffectExpressions(exprs);
			}
		}

		void AstArrayLengthExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			target->CollectSideEffectExpressions(exprs);
		}

		void AstArrayAccessExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			target->CollectSideEffectExpressions(exprs);
			index->CollectSideEffectExpressions(exprs);
		}

		void AstFieldAccessExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			target->CollectSideEffectExpressions(exprs);
		}

		void AstInvokeExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			exprs.push_back(dynamic_pointer_cast<AstExpression>(shared_from_this()));
		}

		void AstLambdaExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		/*************************************************************
		AstExpression::RoughlyOptimize
		*************************************************************/

		void AstLiteralExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
		}

		void AstIntegerExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
		}

		void AstFloatExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
		}

		void AstStringExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
		}

		void AstExternalSymbolExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
		}

		void AstReferenceExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
		}

		void AstNewTypeExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			for (auto& field : fields)
			{
				field->RoughlyOptimize(field);
			}
		}

		void AstTestTypeExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			target->RoughlyOptimize(target);
		}

		void AstNewArrayExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			length->RoughlyOptimize(length);
		}

		void AstNewArrayLiteralExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			for (auto& element : elements)
			{
				element->RoughlyOptimize(element);
			}
		}

		void AstArrayLengthExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			target->RoughlyOptimize(target);
		}

		void AstArrayAccessExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			target->RoughlyOptimize(target);
			index->RoughlyOptimize(index);
		}

		void AstFieldAccessExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			target->RoughlyOptimize(target);
		}

		void AstInvokeExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			function->RoughlyOptimize(function);
			for (auto& argument : arguments)
			{
				argument->RoughlyOptimize(argument);
			}
		}

		void AstLambdaExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			shared_ptr<AstExpressionStatement> stat;
			if (!(stat = dynamic_pointer_cast<AstExpressionStatement>(statement)))
			{
				if (auto block = dynamic_pointer_cast<AstBlockStatement>(statement))
				{
					if (block->statements.size() == 1)
					{
						stat = dynamic_pointer_cast<AstExpressionStatement>(block->statements[0]);
					}
				}
			}

			if (stat)
			{
				if (auto invoke = dynamic_pointer_cast<AstInvokeExpression>(stat->expression))
				{
					if (arguments.size() != invoke->arguments.size())
					{
						goto FAIL_TO_OPTIMIZE;
					}
					if (auto ref = dynamic_pointer_cast<AstReferenceExpression>(invoke->function))
					{
						auto decl = ref->reference.lock();
						for (auto argument : arguments)
						{
							if (decl == argument)
							{
								goto FAIL_TO_OPTIMIZE;
							}
						}

						for (int i = 0; (size_t)i < arguments.size(); i++)
						{
							if (auto arg = dynamic_pointer_cast<AstReferenceExpression>(invoke->arguments[i]))
							{
								if (arg->reference.lock() != arguments[i])
								{
									goto FAIL_TO_OPTIMIZE;
								}
							}
							else
							{
								goto FAIL_TO_OPTIMIZE;
							}
						}

						replacement = invoke->function;
						return;
					}
				}
			}

		FAIL_TO_OPTIMIZE:
			statement->RoughlyOptimize(statement);
		}

		/*************************************************************
		AstStatement::RoughlyOptimize
		*************************************************************/

		void AstBlockStatement::RoughlyOptimize(AstStatement::Ptr& replacement)
		{
			for (auto& stat : statements)
			{
				stat->RoughlyOptimize(stat);
			}

			AstStatement::List stats;
			for (auto stat : statements)
			{
				stat->ExpandBlock(stats, stat == *(statements.end() - 1));
			}
			statements = std::move(stats);
		}

		void AstExpressionStatement::RoughlyOptimize(AstStatement::Ptr& replacement)
		{
			expression->RoughlyOptimize(expression);
			if (auto invoke = dynamic_pointer_cast<AstInvokeExpression>(expression))
			{
				if (auto lambda = dynamic_pointer_cast<AstLambdaExpression>(invoke->function))
				{
					auto block = make_shared<AstBlockStatement>();
					for (int i = 0; (size_t)i < lambda->arguments.size(); i++)
					{
						{
							auto stat = make_shared<AstDeclarationStatement>();
							stat->declaration = lambda->arguments[i];
							block->statements.push_back(stat);
						}
						{
							auto ref = make_shared<AstReferenceExpression>();
							ref->reference = lambda->arguments[i];
						
							auto stat = make_shared<AstAssignmentStatement>();
							stat->target = ref;
							stat->value = invoke->arguments[i];

							block->statements.push_back(stat);
						}
					}
					block->statements.push_back(lambda->statement);

					replacement = block;
					goto END_OF_REPLACEMENT;
				}
			}

			{
				AstExpression::List exprs;
				expression->CollectSideEffectExpressions(exprs);
				if (exprs.size() == 0)
				{
					replacement = make_shared<AstBlockStatement>();
				}
				else if (exprs[0].get() != expression.get())
				{
					auto block = make_shared<AstBlockStatement>();
					for (auto expr : exprs)
					{
						auto stat = make_shared<AstExpressionStatement>();
						stat->expression = expr;
						block->statements.push_back(stat);
					}
					replacement = block;
				}
			}

		END_OF_REPLACEMENT:
			if (replacement.get() != this)
			{
				replacement->RoughlyOptimize(replacement);
			}
		}

		void AstDeclarationStatement::RoughlyOptimize(AstStatement::Ptr& replacement)
		{
		}

		void AstAssignmentStatement::RoughlyOptimize(AstStatement::Ptr& replacement)
		{
			target->RoughlyOptimize(target);
			value->RoughlyOptimize(value);
		}

		void AstIfStatement::RoughlyOptimize(AstStatement::Ptr& replacement)
		{
			condition->RoughlyOptimize(condition);
			trueBranch->RoughlyOptimize(trueBranch);
			if (falseBranch)
			{
				falseBranch->RoughlyOptimize(falseBranch);
			}
		}
	}
}