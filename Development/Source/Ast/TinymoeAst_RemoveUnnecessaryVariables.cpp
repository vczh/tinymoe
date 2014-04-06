#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstExpression::RemoveUnnecessaryVariables
		*************************************************************/

		class AstExpression_RemoveUnnecessaryVariables : public AstExpressionVisitor
		{
		private:
			set<shared_ptr<AstDeclaration>>&	defined;
			set<shared_ptr<AstDeclaration>>&	used;
		public:
			AstExpression_RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& _defined, set<shared_ptr<AstDeclaration>>& _used)
				:defined(_defined), used(_used)
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
					RemoveUnnecessaryVariables(field, defined, used);
				}
			}

			void Visit(AstTestTypeExpression* node)override
			{
				RemoveUnnecessaryVariables(node->target, defined, used);
			}

			void Visit(AstNewArrayExpression* node)override
			{
				RemoveUnnecessaryVariables(node->length, defined, used);
			}

			void Visit(AstNewArrayLiteralExpression* node)override
			{
				for (auto element : node->elements)
				{
					RemoveUnnecessaryVariables(element, defined, used);
				}
			}

			void Visit(AstArrayLengthExpression* node)override
			{
				RemoveUnnecessaryVariables(node->target, defined, used);
			}

			void Visit(AstArrayAccessExpression* node)override
			{
				RemoveUnnecessaryVariables(node->target, defined, used);
				RemoveUnnecessaryVariables(node->index, defined, used);
			}

			void Visit(AstFieldAccessExpression* node)override
			{
				RemoveUnnecessaryVariables(node->target, defined, used);
			}

			void Visit(AstInvokeExpression* node)override
			{
				RemoveUnnecessaryVariables(node->function, defined, used);
				for (auto argument : node->arguments)
				{
					RemoveUnnecessaryVariables(argument, defined, used);
				}
			}

			void Visit(AstLambdaExpression* node)override
			{
				RemoveUnnecessaryVariables(node->statement, defined, used, node->statement);
			}
		};

		/*************************************************************
		AstStatement::RemoveUnnecessaryVariables
		*************************************************************/

		class AstStatement_RemoveUnnecessaryVariables : public AstStatementVisitor
		{
		private:
			set<shared_ptr<AstDeclaration>>&	defined;
			set<shared_ptr<AstDeclaration>>&	used;
			AstStatement::Ptr&					replacement;
		public:
			AstStatement_RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& _defined, set<shared_ptr<AstDeclaration>>& _used, AstStatement::Ptr& _replacement)
				:defined(_defined), used(_used), replacement(_replacement)
			{
			}

			void Visit(AstBlockStatement* node)override
			{
				for (int i = node->statements.size() - 1; i >= 0; i--)
				{
					RemoveUnnecessaryVariables(node->statements[i], defined, used, node->statements[i]);
				}
			}

			void Visit(AstExpressionStatement* node)override
			{
				RemoveUnnecessaryVariables(node->expression, defined, used);
			}

			void Visit(AstDeclarationStatement* node)override
			{
				if (defined.find(node->declaration) != defined.end() && used.find(node->declaration) == used.end())
				{
					replacement = make_shared<AstBlockStatement>();
				}
			}

			void Visit(AstAssignmentStatement* node)override
			{
				auto leftValue = GetRootLeftValue(node->target);
				if (defined.find(leftValue) != defined.end() && used.find(leftValue) == used.end())
				{
					AstExpression::List exprs;
					CollectSideEffectExpressions(node->target, exprs);
					CollectSideEffectExpressions(node->value, exprs);

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

			void Visit(AstIfStatement* node)override
			{
				RemoveUnnecessaryVariables(node->trueBranch, defined, used, node->trueBranch);
				if (node->falseBranch)
				{
					RemoveUnnecessaryVariables(node->falseBranch, defined, used, node->falseBranch);
				}
			}
		};
		
		/*************************************************************
		RemoveUnnecessaryVariables
		*************************************************************/

		void RemoveUnnecessaryVariables(AstExpression::Ptr node, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			AstExpression_RemoveUnnecessaryVariables visitor(defined, used);
			node->Accept(&visitor);
		}

		void RemoveUnnecessaryVariables(AstStatement::Ptr node, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used, AstStatement::Ptr& replacement)
		{
			AstStatement_RemoveUnnecessaryVariables visitor(defined, used, replacement);
			node->Accept(&visitor);
		}
	}
}