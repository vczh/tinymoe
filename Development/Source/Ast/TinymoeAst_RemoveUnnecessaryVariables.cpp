#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstExpression::RemoveUnnecessaryVariables
		*************************************************************/

		void AstLiteralExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
		}

		void AstIntegerExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
		}

		void AstFloatExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
		}

		void AstStringExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
		}

		void AstExternalSymbolExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
		}

		void AstReferenceExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
		}

		void AstNewTypeExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			for (auto field : fields)
			{
				field->RemoveUnnecessaryVariables(defined, used);
			}
		}

		void AstTestTypeExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			target->RemoveUnnecessaryVariables(defined, used);
		}

		void AstNewArrayExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			length->RemoveUnnecessaryVariables(defined, used);
		}

		void AstNewArrayLiteralExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			for (auto element : elements)
			{
				element->RemoveUnnecessaryVariables(defined, used);
			}
		}

		void AstArrayLengthExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			target->RemoveUnnecessaryVariables(defined, used);
		}

		void AstArrayAccessExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			target->RemoveUnnecessaryVariables(defined, used);
			index->RemoveUnnecessaryVariables(defined, used);
		}

		void AstFieldAccessExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			target->RemoveUnnecessaryVariables(defined, used);
		}

		void AstInvokeExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			function->RemoveUnnecessaryVariables(defined, used);
			for (auto argument : arguments)
			{
				argument->RemoveUnnecessaryVariables(defined, used);
			}
		}

		void AstLambdaExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			statement->RemoveUnnecessaryVariables(defined, used, statement);
		}

		/*************************************************************
		AstStatement::RemoveUnnecessaryVariables
		*************************************************************/

		void AstBlockStatement::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used, AstStatement::Ptr& replacement)
		{
			for (int i = statements.size() - 1; i >= 0; i--)
			{
				statements[i]->RemoveUnnecessaryVariables(defined, used, statements[i]);
			}
		}

		void AstExpressionStatement::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used, AstStatement::Ptr& replacement)
		{
			expression->RemoveUnnecessaryVariables(defined, used);
		}

		void AstDeclarationStatement::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used, AstStatement::Ptr& replacement)
		{
			if (defined.find(declaration) != defined.end() && used.find(declaration) == used.end())
			{
				replacement = make_shared<AstBlockStatement>();
			}
		}

		void AstAssignmentStatement::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used, AstStatement::Ptr& replacement)
		{
			auto leftValue = GetRootLeftValue(target);
			if (defined.find(leftValue) != defined.end() && used.find(leftValue) == used.end())
			{
				AstExpression::List exprs;
				CollectSideEffectExpressions(target, exprs);
				CollectSideEffectExpressions(value, exprs);

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

		void AstIfStatement::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used, AstStatement::Ptr& replacement)
		{
			trueBranch->RemoveUnnecessaryVariables(defined, used, trueBranch);
			if (falseBranch)
			{
				falseBranch->RemoveUnnecessaryVariables(defined, used, falseBranch);
			}
		}
	}
}