#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstStatement::ExpandBlock
		*************************************************************/

		class AstStatement_ExpandBlock : public AstStatementVisitor
		{
		private:
			AstStatement::List&			stats;
			bool						lastStatement;

		public:
			AstStatement_ExpandBlock(AstStatement::List& _stats, bool _lastStatement)
				:stats(_stats), lastStatement(_lastStatement)
			{

			}

			void Visit(AstBlockStatement* node)override
			{
				if (!lastStatement)
				{
					for (auto stat : node->statements)
					{
						if (dynamic_pointer_cast<AstDeclarationStatement>(stat))
						{
							stats.push_back(dynamic_pointer_cast<AstStatement>(node->shared_from_this()));
							return;
						}
					}
				}

				for (auto stat : node->statements)
				{
					ExpandBlock(stat, stats, stat == *(node->statements.end() - 1));
				}
			}

			void Visit(AstExpressionStatement* node)override
			{
				stats.push_back(dynamic_pointer_cast<AstStatement>(node->shared_from_this()));
			}

			void Visit(AstDeclarationStatement* node)override
			{
				stats.push_back(dynamic_pointer_cast<AstStatement>(node->shared_from_this()));
			}

			void Visit(AstAssignmentStatement* node)override
			{
				stats.push_back(dynamic_pointer_cast<AstStatement>(node->shared_from_this()));
			}

			void Visit(AstIfStatement* node)override
			{
				stats.push_back(dynamic_pointer_cast<AstStatement>(node->shared_from_this()));
			}
		};

		/*************************************************************
		ExpandBlock
		*************************************************************/

		void ExpandBlock(AstStatement::Ptr node, AstStatement::List& stats, bool lastStatement)
		{
			AstStatement_ExpandBlock visitor(stats, lastStatement);
			node->Accept(&visitor);
		}
	}
}