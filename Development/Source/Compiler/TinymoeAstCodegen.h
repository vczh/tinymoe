#ifndef VCZH_COMPILER_TINYMOEASTCODEGEN
#define VCZH_COMPILER_TINYMOEASTCODEGEN

#include "TinymoeStatementAnalyzer.h"
#include "../Ast/TinymoeAst.h"

namespace tinymoe
{
	namespace compiler
	{
		class SymbolAstScope
		{
		public:
			typedef shared_ptr<SymbolAstScope>										Ptr;
			typedef map<GrammarSymbol::Ptr, ast::AstDeclaration::Ptr>				SymbolAstDeclarationMap;
			typedef map<GrammarSymbol::Ptr, ast::AstFunctionDeclaration::Ptr>		SymbolAstFunctionDeclarationMap;

			SymbolAstDeclarationMap					readAsts;
			SymbolAstDeclarationMap					writeAsts;
			SymbolAstFunctionDeclarationMap			functionPrototypes;

			ast::AstType::Ptr						GetType(GrammarSymbol::Ptr symbol, ast::AstNode::Ptr parent);
		};

		struct SymbolAstContext
		{
			int										uniqueId = 0;
			ast::AstFunctionDeclaration::Ptr		function;
			GrammarSymbol::List						createdVariables;

			string									GetUniquePostfix();
		};

		struct SymbolAstResult
		{
			shared_ptr<ast::AstExpression>			value;
			shared_ptr<ast::AstStatement>			statement;
			shared_ptr<ast::AstLambdaExpression>	continuation;

			SymbolAstResult();
			SymbolAstResult(shared_ptr<ast::AstExpression> _value);
			SymbolAstResult(shared_ptr<ast::AstStatement> _statement);
			SymbolAstResult(shared_ptr<ast::AstExpression> _value, shared_ptr<ast::AstStatement> _statement, shared_ptr<ast::AstLambdaExpression> _continuation);

			bool									RequireCps()const;
			SymbolAstResult							ReplaceValue(shared_ptr<ast::AstExpression> _value);
			SymbolAstResult							ReplaceValue(shared_ptr<ast::AstExpression> _value, shared_ptr<ast::AstLambdaExpression> _continuation);
			void									MergeForExpression(const SymbolAstResult& result, SymbolAstContext& context, vector<ast::AstExpression::Ptr>& exprs, int& exprStart, ast::AstDeclaration::Ptr& state);
			static void								AppendStatement(ast::AstStatement::Ptr& target, ast::AstStatement::Ptr statement);
			void									AppendStatement(ast::AstStatement::Ptr _statement);
			void									MergeForStatement(const SymbolAstResult& result, ast::AstDeclaration::Ptr& state);
		};

		extern ast::AstAssembly::Ptr				GenerateAst(SymbolAssembly::Ptr symbolAssembly);
	}
}

#endif