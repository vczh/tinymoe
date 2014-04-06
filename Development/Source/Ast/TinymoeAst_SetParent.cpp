#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstDeclaration::SetParent
		*************************************************************/

		class AstDeclaration_SetParent : public AstDeclarationVisitor
		{
		public:

			void Visit(AstSymbolDeclaration* node)override
			{
			}

			void Visit(AstTypeDeclaration* node)override
			{
				for (auto field : node->fields)
				{
					SetParent(field, node->shared_from_this());
				}
			}

			void Visit(AstFunctionDeclaration* node)override
			{
				if (node->ownerType)
				{
					SetParent(node->ownerType, node->shared_from_this());
				}
				for (auto argument : node->arguments)
				{
					SetParent(argument, node->shared_from_this());
				}

				SetParent(node->statement, node->shared_from_this());
			}
		};

		/*************************************************************
		AstExpression::SetParent
		*************************************************************/

		class AstExpression_SetParent : public AstExpressionVisitor
		{
		public:

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
				SetParent(node->type, node->shared_from_this());
				for (auto field : node->fields)
				{
					SetParent(field, node->shared_from_this());
				}
			}

			void Visit(AstTestTypeExpression* node)override
			{
				SetParent(node->target, node->shared_from_this());
				SetParent(node->type, node->shared_from_this());
			}

			void Visit(AstNewArrayExpression* node)override
			{
				SetParent(node->length, node->shared_from_this());
			}

			void Visit(AstNewArrayLiteralExpression* node)override
			{
				for (auto element : node->elements)
				{
					SetParent(element, node->shared_from_this());
				}
			}

			void Visit(AstArrayLengthExpression* node)override
			{
				SetParent(node->target, node->shared_from_this());
			}

			void Visit(AstArrayAccessExpression* node)override
			{
				SetParent(node->target, node->shared_from_this());
				SetParent(node->index, node->shared_from_this());
			}

			void Visit(AstFieldAccessExpression* node)override
			{
				SetParent(node->target, node->shared_from_this());
			}

			void Visit(AstInvokeExpression* node)override
			{
				SetParent(node->function, node->shared_from_this());
				for (auto argument : node->arguments)
				{
					SetParent(argument, node->shared_from_this());
				}
			}

			void Visit(AstLambdaExpression* node)override
			{
				for (auto argument : node->arguments)
				{
					SetParent(argument, node->shared_from_this());
				}
				SetParent(node->statement, node->shared_from_this());
			}
		};

		/*************************************************************
		AstStatement::SetParentInternal
		*************************************************************/

		class AstStatement_SetParent : public AstStatementVisitor
		{
		public:

			void Visit(AstBlockStatement* node)override
			{
				for (auto statement : node->statements)
				{
					SetParent(statement, node->shared_from_this());
				}
			}

			void Visit(AstExpressionStatement* node)override
			{
				SetParent(node->expression, node->shared_from_this());
			}

			void Visit(AstDeclarationStatement* node)override
			{
				SetParent(node->declaration, node->shared_from_this());
			}

			void Visit(AstAssignmentStatement* node)override
			{
				SetParent(node->target, node->shared_from_this());
				SetParent(node->value, node->shared_from_this());
			}

			void Visit(AstIfStatement* node)override
			{
				SetParent(node->condition, node->shared_from_this());
				SetParent(node->trueBranch, node->shared_from_this());
				if (node->falseBranch)
				{
					SetParent(node->falseBranch, node->shared_from_this());
				}
			}
		};

		/*************************************************************
		AstType::SetParent
		*************************************************************/

		class AstType_SetParent : public AstTypeVisitor
		{
		public:

			void Visit(AstPredefinedType* node)override
			{
			}

			void Visit(AstReferenceType* node)override
			{
			}
		};

		/*************************************************************
		AstAssembly::SetParent
		*************************************************************/

		class AstNode_SetParent : public AstVisitor
		{
		public:

			void Visit(AstType* node)override
			{
				AstType_SetParent visitor;
				node->Accept(&visitor);
			}

			void Visit(AstExpression* node)override
			{
				AstExpression_SetParent visitor;
				node->Accept(&visitor);
			}

			void Visit(AstStatement* node)override
			{
				AstStatement_SetParent visitor;
				node->Accept(&visitor);
			}

			void Visit(AstDeclaration* node)override
			{
				AstDeclaration_SetParent visitor;
				node->Accept(&visitor);
			}

			void Visit(AstAssembly* node)override
			{
				for (auto decl : node->declarations)
				{
					SetParent(decl, node->shared_from_this());
				}
			}
		};

		/*************************************************************
		SetParent
		*************************************************************/

		void SetParent(AstNode::Ptr node, AstNode::WeakPtr _parent)
		{
			ASSERT(node->parent.expired());
			node->parent = _parent;
			AstNode_SetParent visitor;
			node->Accept(&visitor);
		}
	}
}