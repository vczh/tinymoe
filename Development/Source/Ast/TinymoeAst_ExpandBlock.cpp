#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstStatement::ExpandBlock
		*************************************************************/

		void AstBlockStatement::ExpandBlock(AstStatement::List& stats, bool lastStatement)
		{
			if (!lastStatement)
			{
				for (auto stat : statements)
				{
					if (dynamic_pointer_cast<AstDeclarationStatement>(stat))
					{
						stats.push_back(dynamic_pointer_cast<AstStatement>(shared_from_this()));
						return;
					}
				}
			}
			
			for (auto stat : statements)
			{
				stat->ExpandBlock(stats, stat == *(statements.end() - 1));
			}
		}

		void AstExpressionStatement::ExpandBlock(AstStatement::List& stats, bool lastStatement)
		{
			stats.push_back(dynamic_pointer_cast<AstStatement>(shared_from_this()));
		}

		void AstDeclarationStatement::ExpandBlock(AstStatement::List& stats, bool lastStatement)
		{
			stats.push_back(dynamic_pointer_cast<AstStatement>(shared_from_this()));
		}

		void AstAssignmentStatement::ExpandBlock(AstStatement::List& stats, bool lastStatement)
		{
			stats.push_back(dynamic_pointer_cast<AstStatement>(shared_from_this()));
		}

		void AstIfStatement::ExpandBlock(AstStatement::List& stats, bool lastStatement)
		{
			stats.push_back(dynamic_pointer_cast<AstStatement>(shared_from_this()));
		}
	}
}