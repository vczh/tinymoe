#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstExpression::CollectUsedVariables
		*************************************************************/

		void AstLiteralExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
		}

		void AstIntegerExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
		}

		void AstFloatExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
		}

		void AstStringExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
		}

		void AstExternalSymbolExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
		}

		void AstReferenceExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			if (rightValue)
			{
				used.insert(reference.lock());
			}
		}

		void AstNewTypeExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			for (auto field : fields)
			{
				field->CollectUsedVariables(true, defined, used);
			}
		}

		void AstTestTypeExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			target->CollectUsedVariables(true, defined, used);
		}

		void AstNewArrayExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			length->CollectUsedVariables(true, defined, used);
		}

		void AstNewArrayLiteralExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			for (auto element : elements)
			{
				element->CollectUsedVariables(true, defined, used);
			}
		}

		void AstArrayLengthExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			target->CollectUsedVariables(true, defined, used);
		}

		void AstArrayAccessExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			target->CollectUsedVariables(rightValue, defined, used);
			index->CollectUsedVariables(true, defined, used);
		}

		void AstFieldAccessExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			if (rightValue)
			{
				target->CollectUsedVariables(true, defined, used);
			}
		}

		void AstInvokeExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			function->CollectUsedVariables(true, defined, used);
			for (auto argument : arguments)
			{
				argument->CollectUsedVariables(true, defined, used);
			}
		}

		void AstLambdaExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			statement->CollectUsedVariables(defined, used);
		}

		/*************************************************************
		AstStatement::CollectUsedVariables
		*************************************************************/

		void AstBlockStatement::CollectUsedVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			for (auto stat : statements)
			{
				stat->CollectUsedVariables(defined, used);
			}
		}

		void AstExpressionStatement::CollectUsedVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			expression->CollectUsedVariables(true, defined, used);
		}

		void AstDeclarationStatement::CollectUsedVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			defined.insert(declaration);
		}

		void AstAssignmentStatement::CollectUsedVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			target->CollectUsedVariables(false, defined, used);
			value->CollectUsedVariables(true, defined, used);
		}

		void AstIfStatement::CollectUsedVariables(set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used)
		{
			condition->CollectUsedVariables(true, defined, used);
			trueBranch->CollectUsedVariables(defined, used);
			if (falseBranch)
			{
				falseBranch->CollectUsedVariables(defined, used);
			}
		}
	}
}