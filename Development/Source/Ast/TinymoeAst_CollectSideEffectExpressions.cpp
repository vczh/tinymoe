#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstExpression::CollectSideEffectExpressions
		*************************************************************/

		class AstExpression_CollectSideEffectExpressions : public AstExpressionVisitor
		{
		private:
			AstExpression::List&			exprs;

		public:
			AstExpression_CollectSideEffectExpressions(AstExpression::List& _exprs)
				:exprs(_exprs)
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
				for (auto field : node->fields)
				{
					CollectSideEffectExpressions(field, exprs);
				}
			}

			void Visit(AstTestTypeExpression* node)override
			{
				CollectSideEffectExpressions(node->target, exprs);
			}

			void Visit(AstNewArrayExpression* node)override
			{
				CollectSideEffectExpressions(node->length, exprs);
			}

			void Visit(AstNewArrayLiteralExpression* node)override
			{
				for (auto element : node->elements)
				{
					CollectSideEffectExpressions(element, exprs);
				}
			}

			void Visit(AstArrayLengthExpression* node)override
			{
				CollectSideEffectExpressions(node->target, exprs);
			}

			void Visit(AstArrayAccessExpression* node)override
			{
				CollectSideEffectExpressions(node->target, exprs);
				CollectSideEffectExpressions(node->index, exprs);
			}

			void Visit(AstFieldAccessExpression* node)override
			{
				CollectSideEffectExpressions(node->target, exprs);
			}

			void Visit(AstInvokeExpression* node)override
			{
				exprs.push_back(dynamic_pointer_cast<AstExpression>(node->shared_from_this()));
			}

			void Visit(AstLambdaExpression* node)override
			{
			}
		};

		/*************************************************************
		AstExpression::CollectSideEffectExpressions
		*************************************************************/

		void CollectSideEffectExpressions(AstExpression::Ptr node, AstExpression::List& exprs)
		{
			AstExpression_CollectSideEffectExpressions visitor(exprs);
			node->Accept(&visitor);
		}
	}
}