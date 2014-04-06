#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstExpression::GetRootLeftValue
		*************************************************************/

		class AstExpression_GetRootLeftValue : public AstExpressionVisitor
		{
		public:
			shared_ptr<AstDeclaration>			result;

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
				result = node->reference.lock();
			}

			void Visit(AstNewTypeExpression* node)override
			{
			}

			void Visit(AstTestTypeExpression* node)override
			{
			}

			void Visit(AstNewArrayExpression* node)override
			{
			}

			void Visit(AstNewArrayLiteralExpression* node)override
			{
			}

			void Visit(AstArrayLengthExpression* node)override
			{
			}

			void Visit(AstArrayAccessExpression* node)override
			{
				result = GetRootLeftValue(node->target);
			}

			void Visit(AstFieldAccessExpression* node)override
			{
				result = GetRootLeftValue(node->target);
			}

			void Visit(AstInvokeExpression* node)override
			{
			}

			void Visit(AstLambdaExpression* node)override
			{
			}
		};
		
		/*************************************************************
		GetRootLeftValue
		*************************************************************/

		AstDeclaration::Ptr GetRootLeftValue(AstExpression::Ptr node)
		{
			AstExpression_GetRootLeftValue visitor;
			node->Accept(&visitor);
			return visitor.result;
		}
	}
}