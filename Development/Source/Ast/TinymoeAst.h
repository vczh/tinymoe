#ifndef VCZH_AST_TINYMOEAST
#define VCZH_AST_TINYMOEAST

#include "../TinymoeSTL.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		Common
		*************************************************************/

		class AstType;
		class AstExpression;
		class AstStatement;
		class AstDeclaration;

		class AstNode : protected enable_shared_from_this<AstNode>
		{
		public:
			typedef shared_ptr<AstNode>				Ptr;
			typedef weak_ptr<AstNode>				WeakPtr;

			WeakPtr									parent;

			AstNode();
			virtual ~AstNode();

			string									Indent(int indentation);
			void									Print(ostream& o, int indentation, AstNode::WeakPtr _parent = AstNode::WeakPtr());
			void									SetParent(AstNode::WeakPtr _parent = AstNode::WeakPtr());
		protected:
			virtual void							PrintInternal(ostream& o, int indentation) = 0;
			virtual void							SetParentInternal() = 0;
		};

		class AstType : public AstNode
		{
		public:
			typedef shared_ptr<AstType>				Ptr;
			typedef vector<Ptr>						List;
		};

		class AstExpression : public AstNode
		{
		public:
			typedef shared_ptr<AstExpression>		Ptr;
			typedef vector<Ptr>						List;

			virtual void							CollectSideEffectExpressions(AstExpression::List& exprs) = 0;
			virtual void							RoughlyOptimize(AstExpression::Ptr& replacement) = 0;
			virtual void							CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables) = 0;
			virtual void							RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables) = 0;
			virtual shared_ptr<AstDeclaration>		GetRootLeftValue();
		};

		class AstStatement : public AstNode
		{
		public:
			typedef shared_ptr<AstStatement>		Ptr;
			typedef vector<Ptr>						List;

			virtual void							RoughlyOptimize(AstStatement::Ptr& replacement) = 0;
			virtual void							ExpandBlock(AstStatement::List& stats, bool lastStatement) = 0;
			virtual void							CollectUsedVariables(set<shared_ptr<AstDeclaration>>& variables) = 0;
			virtual void							RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables, AstStatement::Ptr& replacement) = 0;
		};

		class AstDeclaration : public AstNode
		{
		public:
			typedef shared_ptr<AstDeclaration>		Ptr;
			typedef weak_ptr<AstDeclaration>		WeakPtr;
			typedef vector<Ptr>						List;

			string									composedName;

			virtual void							RoughlyOptimize();
		};

		/*************************************************************
		Declaration
		*************************************************************/

		class AstSymbolDeclaration : public AstDeclaration
		{
		public:
			typedef shared_ptr<AstSymbolDeclaration>			Ptr;
			typedef vector<Ptr>									List;
			
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstTypeDeclaration : public AstDeclaration
		{
		public:
			typedef shared_ptr<AstTypeDeclaration>				Ptr;

			AstType::WeakPtr						baseType;
			AstSymbolDeclaration::List				fields;
			
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstFunctionDeclaration : public AstDeclaration
		{
		public:
			typedef shared_ptr<AstFunctionDeclaration>			Ptr;

			AstType::Ptr							ownerType;			// (optional) if ownerType is not null, this function is added to this type as a virtual function.
			AstSymbolDeclaration::List				arguments;
			map<int, int>							readArgumentAstMap;
			map<int, int>							writeArgumentAstMap;
			AstStatement::Ptr						statement;

			AstSymbolDeclaration::Ptr				resultVariable;

			AstSymbolDeclaration::Ptr				cpsStateArgument;			// for function
			AstSymbolDeclaration::Ptr				cpsContinuationArgument;	// (optional) for cps sentence
			AstSymbolDeclaration::Ptr				signalArgument;				// (optional) for block
			AstSymbolDeclaration::Ptr				blockBodyArgument;			// (optional) for block
			AstSymbolDeclaration::Ptr				continuationArgument;		// for function
			
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
			void									RoughlyOptimize()override;
		};

		/*************************************************************
		Expression
		*************************************************************/

		enum class AstLiteralName
		{
			Null,
			True,
			False,
		};

		enum class AstUnaryOperator
		{
			Positive,
			Negative,
			Not,
		};

		enum class AstBinaryOperator
		{
			Concat,
			Add,
			Sub,
			Mul,
			Div,
			IntDiv,
			Mod,
			LT,
			GT,
			LE,
			GE,
			EQ,
			NE,
			And,
			Or,
		};

		class AstLiteralExpression : public AstExpression
		{
		public:
			AstLiteralName							literalName;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstIntegerExpression : public AstExpression
		{
		public:
			int64_t									value;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstFloatExpression : public AstExpression
		{
		public:
			double									value;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstStringExpression : public AstExpression
		{
		public:
			string									value;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstExternalSymbolExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						name;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstReferenceExpression : public AstExpression
		{
		public:
			AstDeclaration::WeakPtr					reference;			// could be
																		//     AstSymbolDeclaration
																		//     AstFunctionDeclaration
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
			shared_ptr<AstDeclaration>				GetRootLeftValue()override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstUnaryExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						operand;
			AstUnaryOperator						op;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstBinaryExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						first;
			AstExpression::Ptr						second;
			AstBinaryOperator						op;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstNewTypeExpression : public AstExpression
		{
		public:
			AstType::Ptr							type;
			AstExpression::List						fields;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstTestTypeExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						target;
			AstType::Ptr							type;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstNewArrayExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						length;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstNewArrayLiteralExpression : public AstExpression
		{
		public:
			AstExpression::List						elements;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstArrayLengthExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						target;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstArrayAccessExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						target;
			AstExpression::Ptr						index;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
			shared_ptr<AstDeclaration>				GetRootLeftValue()override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstFieldAccessExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						target;
			string									composedFieldName;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
			shared_ptr<AstDeclaration>				GetRootLeftValue()override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstInvokeExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						function;
			AstExpression::List						arguments;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstLambdaExpression : public AstExpression
		{
		public:
			AstSymbolDeclaration::List				arguments;
			AstStatement::Ptr						statement;
			
			void									CollectSideEffectExpressions(AstExpression::List& exprs)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
			void									CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		/*************************************************************
		Statement
		*************************************************************/

		class AstBlockStatement : public AstStatement
		{
		public:
			AstStatement::List						statements;
			
			void									RoughlyOptimize(AstStatement::Ptr& replacement)override;
			void									ExpandBlock(AstStatement::List& stats, bool lastStatement)override;
			void									CollectUsedVariables(set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables, AstStatement::Ptr& replacement)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstExpressionStatement : public AstStatement
		{
		public:
			AstExpression::Ptr						expression;
			
			void									RoughlyOptimize(AstStatement::Ptr& replacement)override;
			void									ExpandBlock(AstStatement::List& stats, bool lastStatement)override;
			void									CollectUsedVariables(set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables, AstStatement::Ptr& replacement)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstDeclarationStatement : public AstStatement
		{
		public:
			AstDeclaration::Ptr						declaration;
			
			void									RoughlyOptimize(AstStatement::Ptr& replacement)override;
			void									ExpandBlock(AstStatement::List& stats, bool lastStatement)override;
			void									CollectUsedVariables(set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables, AstStatement::Ptr& replacement)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstAssignmentStatement : public AstStatement
		{
		public:
			AstExpression::Ptr						target;				// could be
																		//     AstReferenceExpression
																		//     AstFieldAccessExpression
																		//     AstArrayAccessExpression
			AstExpression::Ptr						value;
			
			void									RoughlyOptimize(AstStatement::Ptr& replacement)override;
			void									ExpandBlock(AstStatement::List& stats, bool lastStatement)override;
			void									CollectUsedVariables(set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables, AstStatement::Ptr& replacement)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		
		class AstIfStatement : public AstStatement
		{
		public:
			AstExpression::Ptr						condition;
			AstStatement::Ptr						trueBranch;
			AstStatement::Ptr						falseBranch;		// (optional)
			
			void									RoughlyOptimize(AstStatement::Ptr& replacement)override;
			void									ExpandBlock(AstStatement::List& stats, bool lastStatement)override;
			void									CollectUsedVariables(set<shared_ptr<AstDeclaration>>& variables)override;
			void									RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables, AstStatement::Ptr& replacement)override;
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		/*************************************************************
		Type
		*************************************************************/

		enum class AstPredefinedTypeName
		{
			Object,
			Symbol,
			Array,
			Boolean,
			Integer,
			Float,
			String,
		};

		class AstPredefinedType : public AstType
		{
		public:
			AstPredefinedTypeName					typeName;
			
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		class AstReferenceType : public AstType
		{
		public:
			weak_ptr<AstTypeDeclaration>			typeDeclaration;
			
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};

		/*************************************************************
		Assembly
		*************************************************************/

		class AstAssembly : public AstNode
		{
		public:
			typedef shared_ptr<AstAssembly>			Ptr;

			AstDeclaration::List					declarations;
			
			void									RoughlyOptimize();
		protected:
			void									PrintInternal(ostream& o, int indentation)override;
			void									SetParentInternal()override;
		};
	}
}

#endif