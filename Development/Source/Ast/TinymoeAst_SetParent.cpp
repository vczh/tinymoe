#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstDeclaration::SetParentInternal
		*************************************************************/

		void AstSymbolDeclaration::SetParentInternal()
		{
		}

		void AstTypeDeclaration::SetParentInternal()
		{
			for (auto field : fields)
			{
				field->SetParent(shared_from_this());
			}
		}

		void AstFunctionDeclaration::SetParentInternal()
		{
			if (ownerType)
			{
				ownerType->SetParent(shared_from_this());
			}
			for (auto argument : arguments)
			{
				argument->SetParent(shared_from_this());
			}

			statement->SetParent(shared_from_this());
		}

		/*************************************************************
		AstExpression::SetParentInternal
		*************************************************************/

		void AstLiteralExpression::SetParentInternal()
		{
		}

		void AstIntegerExpression::SetParentInternal()
		{
		}

		void AstFloatExpression::SetParentInternal()
		{
		}

		void AstStringExpression::SetParentInternal()
		{
		}

		void AstExternalSymbolExpression::SetParentInternal()
		{
		}

		void AstReferenceExpression::SetParentInternal()
		{
		}

		void AstNewTypeExpression::SetParentInternal()
		{
			type->SetParent(shared_from_this());
			for (auto field : fields)
			{
				field->SetParent(shared_from_this());
			}
		}

		void AstTestTypeExpression::SetParentInternal()
		{
			target->SetParent(shared_from_this());
			type->SetParent(shared_from_this());
		}

		void AstNewArrayExpression::SetParentInternal()
		{
			length->SetParent(shared_from_this());
		}

		void AstNewArrayLiteralExpression::SetParentInternal()
		{
			for (auto element : elements)
			{
				element->SetParent(shared_from_this());
			}
		}

		void AstArrayLengthExpression::SetParentInternal()
		{
			target->SetParent(shared_from_this());
		}

		void AstArrayAccessExpression::SetParentInternal()
		{
			target->SetParent(shared_from_this());
			index->SetParent(shared_from_this());
		}

		void AstFieldAccessExpression::SetParentInternal()
		{
			target->SetParent(shared_from_this());
		}

		void AstInvokeExpression::SetParentInternal()
		{
			function->SetParent(shared_from_this());
			for (auto argument : arguments)
			{
				argument->SetParent(shared_from_this());
			}
		}

		void AstLambdaExpression::SetParentInternal()
		{
			for (auto argument : arguments)
			{
				argument->SetParent(shared_from_this());
			}
			statement->SetParent(shared_from_this());
		}

		/*************************************************************
		AstStatement::SetParentInternal
		*************************************************************/

		void AstBlockStatement::SetParentInternal()
		{
			for (auto statement : statements)
			{
				statement->SetParent(shared_from_this());
			}
		}

		void AstExpressionStatement::SetParentInternal()
		{
			expression->SetParent(shared_from_this());
		}

		void AstDeclarationStatement::SetParentInternal()
		{
			declaration->SetParent(shared_from_this());
		}

		void AstAssignmentStatement::SetParentInternal()
		{
			target->SetParent(shared_from_this());
			value->SetParent(shared_from_this());
		}

		void AstIfStatement::SetParentInternal()
		{
			condition->SetParent(shared_from_this());
			trueBranch->SetParent(shared_from_this());
			if (falseBranch)
			{
				falseBranch->SetParent(shared_from_this());
			}
		}

		/*************************************************************
		AstType::SetParentInternal
		*************************************************************/

		void AstPredefinedType::SetParentInternal()
		{
		}

		void AstReferenceType::SetParentInternal()
		{
		}

		/*************************************************************
		AstAssembly::SetParentInternal
		*************************************************************/

		void AstAssembly::SetParentInternal()
		{
			for (auto decl : declarations)
			{
				decl->SetParent(shared_from_this());
			}
		}
	}
}