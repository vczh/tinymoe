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

		class AstVisitor;
		class AstTypeVisitor;
		class AstExpressionVisitor;
		class AstStatementVisitor;
		class AstDeclarationVisitor;

		class AstNode : public enable_shared_from_this<AstNode>
		{
		public:
			typedef shared_ptr<AstNode>				Ptr;
			typedef weak_ptr<AstNode>				WeakPtr;

			WeakPtr									parent;

			AstNode();
			virtual ~AstNode();
			
			virtual void							Accept(AstVisitor* visitor) = 0;
		};

		class AstType : public AstNode
		{
		public:
			typedef shared_ptr<AstType>				Ptr;
			typedef weak_ptr<AstType>				WeakPtr;
			typedef vector<Ptr>						List;

			void									Accept(AstVisitor* visitor)override;
			virtual void							Accept(AstTypeVisitor* visitor) = 0;
		};

		class AstExpression : public AstNode
		{
		public:
			typedef shared_ptr<AstExpression>		Ptr;
			typedef weak_ptr<AstExpression>			WeakPtr;
			typedef vector<Ptr>						List;
			
			void									Accept(AstVisitor* visitor)override;
			virtual void							Accept(AstExpressionVisitor* visitor) = 0;
			virtual void							RoughlyOptimize(AstExpression::Ptr& replacement) = 0;
		};

		class AstStatement : public AstNode
		{
		public:
			typedef shared_ptr<AstStatement>		Ptr;
			typedef weak_ptr<AstStatement>			WeakPtr;
			typedef vector<Ptr>						List;
			
			void									Accept(AstVisitor* visitor)override;
			virtual void							Accept(AstStatementVisitor* visitor) = 0;
			virtual void							RoughlyOptimize(AstStatement::Ptr& replacement) = 0;
		};

		class AstDeclaration : public AstNode
		{
		public:
			typedef shared_ptr<AstDeclaration>		Ptr;
			typedef weak_ptr<AstDeclaration>		WeakPtr;
			typedef vector<Ptr>						List;

			string_t								composedName;
			
			void									Accept(AstVisitor* visitor)override;
			virtual void							Accept(AstDeclarationVisitor* visitor) = 0;
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
			
			void									Accept(AstDeclarationVisitor* visitor)override;
		};

		class AstTypeDeclaration : public AstDeclaration
		{
		public:
			typedef shared_ptr<AstTypeDeclaration>				Ptr;

			AstType::WeakPtr						baseType;
			AstSymbolDeclaration::List				fields;
			
			void									Accept(AstDeclarationVisitor* visitor)override;
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

			AstSymbolDeclaration::Ptr				stateArgument;				// for function
			AstSymbolDeclaration::Ptr				signalArgument;				// (optional) for block
			AstSymbolDeclaration::Ptr				blockBodyArgument;			// (optional) for block
			AstSymbolDeclaration::Ptr				continuationArgument;		// for function
			
			void									Accept(AstDeclarationVisitor* visitor)override;
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

		class AstLiteralExpression : public AstExpression
		{
		public:
			AstLiteralName							literalName;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstIntegerExpression : public AstExpression
		{
		public:
			int64_t									value;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstFloatExpression : public AstExpression
		{
		public:
			double									value;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstStringExpression : public AstExpression
		{
		public:
			string_t								value;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstExternalSymbolExpression : public AstExpression
		{
		public:
			string_t								name;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstReferenceExpression : public AstExpression
		{
		public:
			AstDeclaration::WeakPtr					reference;			// could be
																		//     AstSymbolDeclaration
																		//     AstFunctionDeclaration
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstNewTypeExpression : public AstExpression
		{
		public:
			AstType::Ptr							type;
			AstExpression::List						fields;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstTestTypeExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						target;
			AstType::Ptr							type;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstNewArrayExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						length;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstNewArrayLiteralExpression : public AstExpression
		{
		public:
			AstExpression::List						elements;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstArrayLengthExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						target;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstArrayAccessExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						target;
			AstExpression::Ptr						index;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstFieldAccessExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						target;
			string_t								composedFieldName;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstInvokeExpression : public AstExpression
		{
		public:
			AstExpression::Ptr						function;
			AstExpression::List						arguments;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		class AstLambdaExpression : public AstExpression
		{
		public:
			AstSymbolDeclaration::List				arguments;
			AstStatement::Ptr						statement;
			
			void									Accept(AstExpressionVisitor* visitor)override;
			void									RoughlyOptimize(AstExpression::Ptr& replacement)override;
		};

		/*************************************************************
		Statement
		*************************************************************/

		class AstBlockStatement : public AstStatement
		{
		public:
			AstStatement::List						statements;
			
			void									Accept(AstStatementVisitor* visitor)override;
			void									RoughlyOptimize(AstStatement::Ptr& replacement)override;
		};

		class AstExpressionStatement : public AstStatement
		{
		public:
			AstExpression::Ptr						expression;
			
			void									Accept(AstStatementVisitor* visitor)override;
			void									RoughlyOptimize(AstStatement::Ptr& replacement)override;
		};

		class AstDeclarationStatement : public AstStatement
		{
		public:
			AstDeclaration::Ptr						declaration;
			
			void									Accept(AstStatementVisitor* visitor)override;
			void									RoughlyOptimize(AstStatement::Ptr& replacement)override;
		};

		class AstAssignmentStatement : public AstStatement
		{
		public:
			AstExpression::Ptr						target;				// could be
																		//     AstReferenceExpression
																		//     AstFieldAccessExpression
																		//     AstArrayAccessExpression
			AstExpression::Ptr						value;
			
			void									Accept(AstStatementVisitor* visitor)override;
			void									RoughlyOptimize(AstStatement::Ptr& replacement)override;
		};

		
		class AstIfStatement : public AstStatement
		{
		public:
			AstExpression::Ptr						condition;
			AstStatement::Ptr						trueBranch;
			AstStatement::Ptr						falseBranch;		// (optional)
			
			void									Accept(AstStatementVisitor* visitor)override;
			void									RoughlyOptimize(AstStatement::Ptr& replacement)override;
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
			Function,
		};

		class AstPredefinedType : public AstType
		{
		public:
			AstPredefinedTypeName					typeName;
			
			void									Accept(AstTypeVisitor* visitor)override;
		};

		class AstReferenceType : public AstType
		{
		public:
			weak_ptr<AstTypeDeclaration>			typeDeclaration;
			
			void									Accept(AstTypeVisitor* visitor)override;
		};

		/*************************************************************
		Assembly
		*************************************************************/

		class AstAssembly : public AstNode
		{
		public:
			typedef shared_ptr<AstAssembly>			Ptr;

			AstDeclaration::List					declarations;
			
			void									Accept(AstVisitor* visitor)override;
			void									RoughlyOptimize();
		};

		/*************************************************************
		Visitors
		*************************************************************/

		class AstVisitor
		{
		public:
			AstVisitor();
			virtual ~AstVisitor();
			
			virtual void							Visit(AstType* node) = 0;
			virtual void							Visit(AstExpression* node) = 0;
			virtual void							Visit(AstStatement* node) = 0;
			virtual void							Visit(AstDeclaration* node) = 0;
			virtual void							Visit(AstAssembly* node) = 0;
		};

		class AstTypeVisitor
		{
		public:
			AstTypeVisitor();
			virtual ~AstTypeVisitor();

			virtual void							Visit(AstPredefinedType* node) = 0;
			virtual void							Visit(AstReferenceType* node) = 0;
		};

		class AstExpressionVisitor
		{
		public:
			AstExpressionVisitor();
			virtual ~AstExpressionVisitor();
			
			virtual void							Visit(AstLiteralExpression* node) = 0;
			virtual void							Visit(AstIntegerExpression* node) = 0;
			virtual void							Visit(AstFloatExpression* node) = 0;
			virtual void							Visit(AstStringExpression* node) = 0;
			virtual void							Visit(AstExternalSymbolExpression* node) = 0;
			virtual void							Visit(AstReferenceExpression* node) = 0;
			virtual void							Visit(AstNewTypeExpression* node) = 0;
			virtual void							Visit(AstTestTypeExpression* node) = 0;
			virtual void							Visit(AstNewArrayExpression* node) = 0;
			virtual void							Visit(AstNewArrayLiteralExpression* node) = 0;
			virtual void							Visit(AstArrayLengthExpression* node) = 0;
			virtual void							Visit(AstArrayAccessExpression* node) = 0;
			virtual void							Visit(AstFieldAccessExpression* node) = 0;
			virtual void							Visit(AstInvokeExpression* node) = 0;
			virtual void							Visit(AstLambdaExpression* node) = 0;
		};

		class AstStatementVisitor
		{
		public:
			AstStatementVisitor();
			virtual ~AstStatementVisitor();
			
			virtual void							Visit(AstBlockStatement* node) = 0;
			virtual void							Visit(AstExpressionStatement* node) = 0;
			virtual void							Visit(AstDeclarationStatement* node) = 0;
			virtual void							Visit(AstAssignmentStatement* node) = 0;
			virtual void							Visit(AstIfStatement* node) = 0;
		};

		class AstDeclarationVisitor
		{
		public:
			AstDeclarationVisitor();
			virtual ~AstDeclarationVisitor();
			
			virtual void							Visit(AstSymbolDeclaration* node) = 0;
			virtual void							Visit(AstTypeDeclaration* node) = 0;
			virtual void							Visit(AstFunctionDeclaration* node) = 0;
		};

		/*************************************************************
		Helper Functions
		*************************************************************/
		
		extern void						SetParent(AstNode::Ptr node, AstNode::WeakPtr _parent = AstNode::WeakPtr());
		extern void						Print(AstNode::Ptr node, ostream_t& o, int indentation, AstNode::WeakPtr _parent = AstNode::WeakPtr());

		extern void						CollectSideEffectExpressions(AstExpression::Ptr node, AstExpression::List& exprs);
		extern void						CollectUsedVariables(AstExpression::Ptr node, bool rightValue, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used);
		extern void						CollectUsedVariables(AstStatement::Ptr node, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used);
		extern void						ExpandBlock(AstStatement::Ptr node, AstStatement::List& stats, bool lastStatement);
		extern AstDeclaration::Ptr		GetRootLeftValue(AstExpression::Ptr node);
		extern void						RemoveUnnecessaryVariables(AstExpression::Ptr node, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used);
		extern void						RemoveUnnecessaryVariables(AstStatement::Ptr node, set<shared_ptr<AstDeclaration>>& defined, set<shared_ptr<AstDeclaration>>& used, AstStatement::Ptr& replacement);
	}
}

#endif