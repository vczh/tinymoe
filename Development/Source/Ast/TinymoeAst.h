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

		class AstNode : protected enable_shared_from_this<AstNode>
		{
		public:
			typedef shared_ptr<AstNode>				Ptr;
			typedef weak_ptr<AstNode>				WeakPtr;

			WeakPtr							parent;

			AstNode();
			virtual ~AstNode();

			string							Indent(int indentation);
			void							Print(ostream& o, int indentation, AstNode::WeakPtr parent = AstNode::WeakPtr());
		protected:
			virtual void					PrintInternal(ostream& o, int indentation) = 0;
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
		};

		class AstStatement : public AstNode
		{
		public:
			typedef shared_ptr<AstStatement>		Ptr;
			typedef vector<Ptr>						List;
		};

		class AstDeclaration : public AstNode
		{
		public:
			typedef shared_ptr<AstDeclaration>		Ptr;
			typedef weak_ptr<AstDeclaration>		WeakPtr;
			typedef vector<Ptr>						List;

			string							composedName;
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
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstTypeDeclaration : public AstDeclaration
		{
		public:
			typedef shared_ptr<AstTypeDeclaration>				Ptr;

			AstType::WeakPtr				baseType;
			AstSymbolDeclaration::List		fields;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstFunctionDeclaration : public AstDeclaration
		{
		public:
			typedef shared_ptr<AstFunctionDeclaration>			Ptr;

			AstType::Ptr					ownerType;			// (optional) if ownerType is not null, this function is added to this type as a virtual function.
			AstSymbolDeclaration::List		arguments;
			map<int, int>					readArgumentAstMap;
			map<int, int>					writeArgumentAstMap;
			AstStatement::Ptr				statement;

			AstSymbolDeclaration::Ptr		resultVariable;

			AstSymbolDeclaration::Ptr		stateArgument;				// for function
			AstSymbolDeclaration::Ptr		signalArgument;				// (optional) for block
			AstSymbolDeclaration::Ptr		blockBodyArgument;			// (optional) for block
			AstSymbolDeclaration::Ptr		continuationArgument;		// for function
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
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
			AstLiteralName					literalName;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstIntegerExpression : public AstExpression
		{
		public:
			int64_t							value;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstFloatExpression : public AstExpression
		{
		public:
			double							value;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstStringExpression : public AstExpression
		{
		public:
			string							value;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstReferenceExpression : public AstExpression
		{
		public:
			AstDeclaration::WeakPtr			reference;			// could be
																//     AstSymbolDeclaration
																//     AstFunctionDeclaration
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstUnaryExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				operand;
			AstUnaryOperator				op;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstBinaryExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				first;
			AstExpression::Ptr				second;
			AstBinaryOperator				op;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstNewTypeExpression : public AstExpression
		{
		public:
			AstType::Ptr					type;
			AstExpression::List				fields;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstTestTypeExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				target;
			AstType::Ptr					type;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstNewArrayExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				length;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstNewArrayLiteralExpression : public AstExpression
		{
		public:
			AstExpression::List				elements;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstArrayLengthExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				target;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstArrayAccessExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				target;
			AstExpression::Ptr				index;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstFieldAccessExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				target;
			string							composedFieldName;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstInvokeExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				function;
			AstExpression::List				arguments;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstLambdaExpression : public AstExpression
		{
		public:
			AstSymbolDeclaration::List		arguments;
			AstStatement::Ptr				statement;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		/*************************************************************
		Statement
		*************************************************************/

		class AstBlockStatement : public AstStatement
		{
		public:
			AstStatement::List				statements;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstExpressionStatement : public AstStatement
		{
		public:
			AstExpression::Ptr				expression;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstDeclarationStatement : public AstStatement
		{
		public:
			AstDeclaration::Ptr				declaration;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstAssignmentStatement : public AstStatement
		{
		public:
			AstExpression::Ptr				target;				// could be
																//     AstReferenceExpression
																//     AstFieldAccessExpression
																//     AstArrayAccessExpression
			AstExpression::Ptr				value;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		
		class AstIfStatement : public AstStatement
		{
		public:
			AstExpression::Ptr				condition;
			AstStatement::Ptr				trueBranch;
			AstStatement::Ptr				falseBranch;		// (optional)
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
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
			AstPredefinedTypeName			typeName;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		class AstReferenceType : public AstType
		{
		public:
			weak_ptr<AstTypeDeclaration>	typeDeclaration;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};

		/*************************************************************
		Assembly
		*************************************************************/

		class AstAssembly : public AstNode
		{
		public:
			typedef shared_ptr<AstAssembly>			Ptr;

			AstDeclaration::List			declarations;
			
		protected:
			void							PrintInternal(ostream& o, int indentation)override;
		};
	}
}

#endif