#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstDeclaration::RoughlyOptimize
		*************************************************************/

		class AstDeclaration_RoughlyOptimize : public AstDeclarationVisitor
		{
		public:
			void Visit(AstSymbolDeclaration* node)override
			{
			}
			
			void Visit(AstTypeDeclaration* node)override
			{
			}

			void Visit(AstFunctionDeclaration* node)override
			{
				RoughlyOptimize(node->statement, node->statement);

				set<AstDeclaration::Ptr> defined, used;
				CollectUsedVariables(node->statement, defined, used);
				RemoveUnnecessaryVariables(node->statement, defined, used, node->statement);

				RoughlyOptimize(node->statement, node->statement);
			}
		};

		/*************************************************************
		AstExpression::RoughlyOptimize
		*************************************************************/

		class AstExpression_RoughlyOptimize : public AstExpressionVisitor
		{
		private:
			AstExpression::Ptr&				replacement;

		public:
			AstExpression_RoughlyOptimize(AstExpression::Ptr& _replacement)
				:replacement(_replacement)
			{

			}

			void Visit(AstLiteralExpression* node)override
			{
			}

			void Visit(AstIntegerExpression* node)override
			{
			}

			void Visit(AstFloatExpression* node)override
			{
			}

			void Visit(AstStringExpression* node)override
			{
			}

			void Visit(AstExternalSymbolExpression* node)override
			{
			}

			void Visit(AstReferenceExpression* node)override
			{
			}

			void Visit(AstNewTypeExpression* node)override
			{
				for (auto& field : node->fields)
				{
					RoughlyOptimize(field, field);
				}
			}

			void Visit(AstTestTypeExpression* node)override
			{
				RoughlyOptimize(node->target, node->target);
			}

			void Visit(AstNewArrayExpression* node)override
			{
				RoughlyOptimize(node->length, node->length);
			}

			void Visit(AstNewArrayLiteralExpression* node)override
			{
				for (auto& element : node->elements)
				{
					RoughlyOptimize(element, element);
				}
			}

			void Visit(AstArrayLengthExpression* node)override
			{
				RoughlyOptimize(node->target, node->target);
			}

			void Visit(AstArrayAccessExpression* node)override
			{
				RoughlyOptimize(node->target, node->target);
				RoughlyOptimize(node->index, node->index);
			}

			void Visit(AstFieldAccessExpression* node)override
			{
				RoughlyOptimize(node->target, node->target);
			}

			void Visit(AstInvokeExpression* node)override
			{
				RoughlyOptimize(node->function, node->function);
				for (auto& argument : node->arguments)
				{
					RoughlyOptimize(argument, argument);
				}
			}

			void Visit(AstLambdaExpression* node)override
			{
				shared_ptr<AstExpressionStatement> stat;
				if (!(stat = dynamic_pointer_cast<AstExpressionStatement>(node->statement)))
				{
					if (auto block = dynamic_pointer_cast<AstBlockStatement>(node->statement))
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
						if (node->arguments.size() != invoke->arguments.size())
						{
							goto FAIL_TO_OPTIMIZE;
						}
						if (auto ref = dynamic_pointer_cast<AstReferenceExpression>(invoke->function))
						{
							auto decl = ref->reference.lock();
							for (auto argument : node->arguments)
							{
								if (decl == argument)
								{
									goto FAIL_TO_OPTIMIZE;
								}
							}

							for (int i = 0; (size_t)i < node->arguments.size(); i++)
							{
								if (auto arg = dynamic_pointer_cast<AstReferenceExpression>(invoke->arguments[i]))
								{
									if (arg->reference.lock() != node->arguments[i])
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
				RoughlyOptimize(node->statement, node->statement);
			}
		};

		/*************************************************************
		AstStatement::RoughlyOptimize
		*************************************************************/

		class AstStatement_RoughlyOptimize : public AstStatementVisitor
		{
		private:
			AstStatement::Ptr&				replacement;

		public:
			AstStatement_RoughlyOptimize(AstStatement::Ptr& _replacement)
				:replacement(_replacement)
			{

			}

			void Visit(AstBlockStatement* node)override
			{
				for (auto& stat : node->statements)
				{
					RoughlyOptimize(stat, stat);
				}

				AstStatement::List stats;
				for (auto stat : node->statements)
				{
					ExpandBlock(stat, stats, stat == *(node->statements.end() - 1));
				}
				node->statements = std::move(stats);
			}

			void Visit(AstExpressionStatement* node)override
			{
				RoughlyOptimize(node->expression, node->expression);
				if (auto invoke = dynamic_pointer_cast<AstInvokeExpression>(node->expression))
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
					CollectSideEffectExpressions(node->expression, exprs);
					if (exprs.size() == 0)
					{
						replacement = make_shared<AstBlockStatement>();
					}
					else if (exprs[0].get() != node->expression.get())
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
				if (replacement.get() != node)
				{
					RoughlyOptimize(replacement, replacement);
				}
			}

			void Visit(AstDeclarationStatement* node)override
			{
			}

			void Visit(AstAssignmentStatement* node)override
			{
				RoughlyOptimize(node->target, node->target);
				RoughlyOptimize(node->value, node->value);
			}

			void Visit(AstIfStatement* node)override
			{
				RoughlyOptimize(node->condition, node->condition);
				RoughlyOptimize(node->trueBranch, node->trueBranch);
				if (node->falseBranch)
				{
					RoughlyOptimize(node->falseBranch, node->falseBranch);
				}
			}
		};

		/*************************************************************
		RoughlyOptimize
		*************************************************************/

		void RoughlyOptimize(AstDeclaration::Ptr node)
		{
			AstDeclaration_RoughlyOptimize visitor;
			node->Accept(&visitor);
		}

		void RoughlyOptimize(AstExpression::Ptr node, AstExpression::Ptr& _replacement)
		{
			AstExpression_RoughlyOptimize visitor(_replacement);
			node->Accept(&visitor);
		}

		void RoughlyOptimize(AstStatement::Ptr node, AstStatement::Ptr& _replacement)
		{
			AstStatement_RoughlyOptimize visitor(_replacement);
			node->Accept(&visitor);
		}

		void RoughlyOptimize(AstAssembly::Ptr node)
		{
			for (auto decl : node->declarations)
			{
				RoughlyOptimize(decl);
			}
		}
	}
}