#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstExpression::CollectUsedVariables
		*************************************************************/

		class AstExpression_CollectUsedVariables : public AstExpressionVisitor
		{
		private:
			bool								rightValue;
			set<shared_ptr<AstDeclaration>>&	defined;
			set<shared_ptr<AstDeclaration>>&	used;
		public:
			AstExpression_CollectUsedVariables(bool _rightValue, set<shared_ptr<AstDeclaration>>& _defined, set<shared_ptr<AstDeclaration>>& _used)
				:rightValue(_rightValue), defined(_defined), used(_used)
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
				if (rightValue)
				{
					used.insert(node->reference.lock());
				}
			}

			void Visit(AstNewTypeExpression* node)override
			{
				for (auto field : node->fields)
				{
					CollectUsedVariables(field, true, defined, used);
				}
			}

			void Visit(AstTestTypeExpression* node)override
			{
				CollectUsedVariables(node->target, true, defined, used);
			}

			void Visit(AstNewArrayExpression* node)override
			{
				CollectUsedVariables(node->length, true, defined, used);
			}

			void Visit(AstNewArrayLiteralExpression* node)override
			{
				for (auto element : node->elements)
				{
					CollectUsedVariables(element, true, defined, used);
				}
			}

			void Visit(AstArrayLengthExpression* node)override
			{
				CollectUsedVariables(node->target, true, defined, used);
			}

			void Visit(AstArrayAccessExpression* node)override
			{
				CollectUsedVariables(node->target, rightValue, defined, used);
				CollectUsedVariables(node->index, true, defined, used);
			}

			void Visit(AstFieldAccessExpression* node)override
			{
				if (rightValue)
				{
					CollectUsedVariables(node->target, true, defined, used);
				}
			}

			void Visit(AstInvokeExpression* node)override
			{
				CollectUsedVariables(node->function, true, defined, used);
				for (auto argument : node->arguments)
				{
					CollectUsedVariables(argument, true, defined, used);
				}
			}

			void Visit(AstLambdaExpression* node)override
			{
				CollectUsedVariables(node->statement, defined, used);
			}
		};

		/*************************************************************
		AstStatement::CollectUsedVariables
		*************************************************************/

		class AstStatement_CollectUsedVariables : public AstStatementVisitor
		{
		private:
			set<shared_ptr<AstDeclaration>>&	defined;
			set<shared_ptr<AstDeclaration>>&	used;
		public:
			AstStatement_CollectUsedVariables(set<shared_ptr<AstDeclaration>>& _defined, set<shared_ptr<AstDeclaration>>& _used)
				:defined(_defined), used(_used)
			{

			}

			void Visit(AstBlockStatement* node)override
			{
				for (auto stat : node->statements)
				{
					CollectUsedVariables(stat, defined, used);
				}
			}

			void Visit(AstExpressionStatement* node)override
			{
				CollectUsedVariables(node->expression, true, defined, used);
			}

			void Visit(AstDeclarationStatement* node)override
			{
				defined.insert(node->declaration);
			}

			void Visit(AstAssignmentStatement* node)override
			{
				CollectUsedVariables(node->target, false, defined, used);
				CollectUsedVariables(node->value, true, defined, used);
			}

			void Visit(AstIfStatement* node)override
			{
				CollectUsedVariables(node->condition, true, defined, used);
				CollectUsedVariables(node->trueBranch, defined, used);
				if (node->falseBranch)
				{
					CollectUsedVariables(node->falseBranch, defined, used);
				}
			}
		};

		/*************************************************************
		CollectUsedVariables
		*************************************************************/

		void CollectUsedVariables(AstExpression::Ptr node, bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			AstExpression_CollectUsedVariables visitor(rightValue, defined, used);
			node->Accept(&visitor);
		}

		void CollectUsedVariables(AstStatement::Ptr node, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			AstStatement_CollectUsedVariables visitor(defined, used);
			node->Accept(&visitor);
		}
	}
}