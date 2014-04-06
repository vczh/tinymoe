#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstExpression::GetRootLeftValue
		*************************************************************/

		shared_ptr<AstDeclaration> AstExpression::GetRootLeftValue()
		{
			return nullptr;
		}

		shared_ptr<AstDeclaration> AstReferenceExpression::GetRootLeftValue()
		{
			return reference.lock();
		}

		shared_ptr<AstDeclaration> AstArrayAccessExpression::GetRootLeftValue()
		{
			return target->GetRootLeftValue();
		}

		shared_ptr<AstDeclaration> AstFieldAccessExpression::GetRootLeftValue()
		{
			return target->GetRootLeftValue();
		}
	}
}