#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
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
	}
}