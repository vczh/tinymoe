#ifndef VCZH_TINYMOEAST
#define VCZH_TINYMOEAST

#include "../TinymoeSTL.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		Common
		*************************************************************/

		class AstNode
		{
		public:
			typedef shared_ptr<AstNode>				Ptr;
			typedef weak_ptr<AstNode>				WeakPtr;

			WeakPtr							parent;

			AstNode();
			virtual ~AstNode();
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
			typedef weak_ptr<AstNode>				WeakPtr;
			typedef vector<Ptr>						List;

			string							moduleName;
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
		};

		class AstTypeDeclaration : public AstDeclaration
		{
		public:
			AstDeclaration::List			member;				// could be
																//     AstSymbolDeclaration (field)
																//     AstFunctionDeclaration (virtual function)
		};

		class AstFunctionDeclaration : public AstDeclaration
		{
		public:
			AstSymbolDeclaration::List		arguments;
			AstStatement::Ptr				statement;
		};

		/*************************************************************
		Expression
		*************************************************************/

		enum class AstLiteralName
		{
			Null,
			True,
			Falst,
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
		};

		class AstIntegerExpression : public AstExpression
		{
		public:
			int64_t							value;
		};

		class AstFloatExpression : public AstExpression
		{
		public:
			double							value;
		};

		class AstStringExpression : public AstExpression
		{
		public:
			string							value;
		};

		class AstReferenceExpression : public AstExpression
		{
		public:
			AstDeclaration::WeakPtr			reference;			// could be
																//     AstSymbolDeclaration
																//     AstFunctionDeclaration
		};

		class AstUnaryExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				operand;
			AstUnaryOperator				op;
		};

		class AstBinaryExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				first;
			AstExpression::Ptr				second;
			AstBinaryOperator				op;
		};

		class AstNewTypeExpression : public AstExpression
		{
		public:
			AstType::Ptr					type;
		};

		class AstTestTypeExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				target;
			AstType::Ptr					type;
		};

		class AstNewArrayExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				length;
		};

		class AstArrayLengthExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				target;
		};

		class AstArrayAccessExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				target;
			AstExpression::Ptr				index;
		};

		class AstFieldAccessExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				target;
			string							fieldName;
		};

		class AstInvokeExpression : public AstExpression
		{
		public:
			AstExpression::Ptr				function;
			AstExpression::List				arguments;
		};

		class AstLambdaArgument : public AstNode
		{
		public:
			typedef shared_ptr<AstLambdaArgument>	Ptr;
			typedef vector<Ptr>						List;

			string							composedName;
		};

		class AstLambdaExpression : public AstExpression
		{
		public:
			AstSymbolDeclaration::List		arguments;
			AstStatement::Ptr				statement;
		};

		/*************************************************************
		Statement
		*************************************************************/

		class AstBlockStatement : public AstStatement
		{
		public:
			AstStatement::List				statements;
		};

		class AstExpressionStatement : public AstStatement
		{
		public:
			bool							tailCall = false;
			AstExpression::Ptr				expression;
		};

		class AstDeclarationStatement : public AstStatement
		{
		public:
			AstDeclaration::Ptr				declaration;
		};

		class AstAssignmentStatement : public AstStatement
		{
		public:
			AstExpression::Ptr				target;
			AstExpression::Ptr				value;
		};

		class AstFieldAssignmentStatement : public AstStatement
		{
		public:
			AstExpression::Ptr				target;
			string							fieldName;
			AstExpression::Ptr				value;
		};

		class AstArrayAssignmentStatement : public AstStatement
		{
		public:
			AstExpression::Ptr				target;
			AstExpression::Ptr				index;
			AstExpression::Ptr				value;
		};
		
		class AstIfStatement : public AstStatement
		{
		public:
			AstExpression::Ptr				condition;
			AstStatement::Ptr				trueBranch;
			AstStatement::Ptr				falseBranch;		// (optional)
		};

		/*************************************************************
		Type
		*************************************************************/

		enum class AstPredefinedTypeName
		{
			Symbol,
			Array,
			Integer,
			Float,
			String,
		};

		class AstPredefinedType : public AstType
		{
		public:
			AstPredefinedTypeName			typeName;
		};

		class AstReferenceType : public AstType
		{
		public:
			weak_ptr<AstTypeDeclaration>	typeDeclaration;
		};

		/*************************************************************
		Assembly
		*************************************************************/

		class AstAssembly : public AstNode
		{
		public:
			typedef shared_ptr<AstAssembly>			Ptr;

			AstDeclaration::List			declarations;
		};
	}
}

#endif