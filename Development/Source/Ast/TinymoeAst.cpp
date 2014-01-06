#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstNode
		*************************************************************/

		AstNode::AstNode()
		{
		}

		AstNode::~AstNode()
		{
		}

		string AstNode::Indent(int indentation)
		{
			string s;
			for (int i = 0; i < indentation; i++)
			{
				s += "    ";
			}
			return s;
		}

		/*************************************************************
		AstDeclaration::Print
		*************************************************************/

		void AstSymbolDeclaration::Print(ostream& o, int indentation)
		{
			o << Indent(indentation) << "symbol " << composedName << ";";
		}

		void AstTypeDeclaration::Print(ostream& o, int indentation)
		{
			o << Indent(indentation) << "type " << composedName;
			if (!baseType.expired())
			{
				o << " : ";
				baseType.lock()->Print(o, indentation);
			}
			o << endl << Indent(indentation) << "{" << endl;
			for (auto field : fields)
			{
				field->Print(o, indentation + 1);
				o << endl;
			}
			o << Indent(indentation) << "}";
		}

		void AstFunctionDeclaration::Print(ostream& o, int indentation)
		{
			o << Indent(indentation) << "function ";
			if (ownerType)
			{
				ownerType->Print(o, indentation);
				o << ":";
			}
			o << composedName << "(";
			for (auto it = arguments.begin(); it != arguments.end(); it++)
			{
				o << (*it)->composedName;
				if (it + 1 != arguments.end())
				{
					o << ", ";
				}
			}
			o << ")" << endl;
			statement->Print(o, indentation + 1);
		}

		/*************************************************************
		AstExpression::Print
		*************************************************************/

		void AstThisExpression::Print(ostream& o, int indentation)
		{
			o << "$this";
		}

		void AstLiteralExpression::Print(ostream& o, int indentation)
		{
			switch (literalName)
			{
			case AstLiteralName::Null:
				o << "$null";
				break;
			case AstLiteralName::True:
				o << "$true";
				break;
			case AstLiteralName::False:
				o << "$false";
				break;
			}
		}

		void AstIntegerExpression::Print(ostream& o, int indentation)
		{
			o << value;
		}

		void AstFloatExpression::Print(ostream& o, int indentation)
		{
			o << value;
		}

		void AstStringExpression::Print(ostream& o, int indentation)
		{
			o << "\"" << value << "\"";
		}

		void AstReferenceExpression::Print(ostream& o, int indentation)
		{
			o << reference.lock()->composedName;
		}

		void AstUnaryExpression::Print(ostream& o, int indentation)
		{
			switch (op)
			{
			case AstUnaryOperator::Not:
				o << "!";
				break;
			case AstUnaryOperator::Positive:
				o << "+";
				break;
			case AstUnaryOperator::Negative:
				o << "-";
				break;
			}
			operand->Print(o, indentation);
		}

		void AstBinaryExpression::Print(ostream& o, int indentation)
		{
			o << "(";
			first->Print(o, indentation);
			switch (op)
			{
			case AstBinaryOperator::Concat:
				o << " & ";
				break;
			case AstBinaryOperator::Add:
				o << " + ";
				break;
			case AstBinaryOperator::Sub:
				o << " - ";
				break;
			case AstBinaryOperator::Mul:
				o << " * ";
				break;
			case AstBinaryOperator::Div:
				o << " / ";
				break;
			case AstBinaryOperator::LT:
				o << " < ";
				break;
			case AstBinaryOperator::GT:
				o << " > ";
				break;
			case AstBinaryOperator::LE:
				o << " <= ";
				break;
			case AstBinaryOperator::GE:
				o << " >= ";
				break;
			case AstBinaryOperator::EQ:
				o << " == ";
				break;
			case AstBinaryOperator::NE:
				o << " != ";
				break;
			case AstBinaryOperator::And:
				o << " && ";
				break;
			case AstBinaryOperator::Or:
				o << " || ";
				break;
			}
			second->Print(o, indentation);
			o << ")";
		}

		void AstNewTypeExpression::Print(ostream& o, int indentation)
		{
			o << "new ";
			type->Print(o, indentation);
			o << "()";
		}

		void AstTestTypeExpression::Print(ostream& o, int indentation)
		{
			o << "(";
			target->Print(o, indentation);
			o << " is ";
			type->Print(o, indentation);
			o << ")";
		}

		void AstNewArrayExpression::Print(ostream& o, int indentation)
		{
			o << "new $Array(";
			length->Print(o, indentation);
			o << ")";
		}

		void AstArrayLengthExpression::Print(ostream& o, int indentation)
		{
			o << "$ArrayLength(";
			target->Print(o, indentation);
			o << ")";
		}

		void AstArrayAccessExpression::Print(ostream& o, int indentation)
		{
			target->Print(o, indentation);
			o << "[";
			index->Print(o, indentation);
			o << "]";
		}

		void AstFieldAccessExpression::Print(ostream& o, int indentation)
		{
			target->Print(o, indentation);
			o << "." << composedFieldName;
		}

		void AstInvokeExpression::Print(ostream& o, int indentation)
		{
			function->Print(o, indentation);
			o << "(";
			for (auto it = arguments.begin(); it != arguments.end(); it++)
			{
				(*it)->Print(o, indentation);
				if (it + 1 != arguments.end())
				{
					o << ", ";
				}
			}
			o << ")";
		}

		void AstLambdaExpression::Print(ostream& o, int indentation)
		{
			o << "function (";
			for (auto it = arguments.begin(); it != arguments.end(); it++)
			{
				o << (*it)->composedName;
				if (it + 1 != arguments.end())
				{
					o << ", ";
				}
			}
			o << ")" << endl;
			statement->Print(o, indentation + 1);
		}

		/*************************************************************
		AstStatement::Print
		*************************************************************/

		void AstBlockStatement::Print(ostream& o, int indentation)
		{
			o << Indent(indentation - 1) << "{" << endl;
			for (auto statement : statements)
			{
				statement->Print(o, indentation + 1);
				o << endl;
			}
			o << Indent(indentation - 1) << "}" << endl;
		}

		void AstExpressionStatement::Print(ostream& o, int indentation)
		{
			o << Indent(indentation);
			expression->Print(o, indentation);
			o << ";";
		}

		void AstDeclarationStatement::Print(ostream& o, int indentation)
		{
			declaration->Print(o, indentation);
		}

		void AstAssignmentStatement::Print(ostream& o, int indentation)
		{
			o << Indent(indentation);
			target->Print(o, indentation);
			o << " = ";
			value->Print(o, indentation);
			o << ";";
		}

		void AstIfStatement::Print(ostream& o, int indentation)
		{
			o << Indent(indentation) << "if (";
			condition->Print(o, indentation);
			o << endl;
			trueBranch->Print(o, indentation + 1);
			if (falseBranch)
			{
				o << endl;
				falseBranch->Print(o, indentation + 1);
			}
		}

		/*************************************************************
		AstType::Print
		*************************************************************/

		void AstPredefinedType::Print(ostream& o, int indentation)
		{
			switch (typeName)
			{
			case AstPredefinedTypeName::Object:
				o << "$Object";
				break;
			case AstPredefinedTypeName::Symbol:
				o << "$Symbol";
				break;
			case AstPredefinedTypeName::Array:
				o << "$Array";
				break;
			case AstPredefinedTypeName::Boolean:
				o << "$Boolean";
				break;
			case AstPredefinedTypeName::Integer:
				o << "$Integer";
				break;
			case AstPredefinedTypeName::Float:
				o << "$Float";
				break;
			case AstPredefinedTypeName::String:
				o << "$String";
				break;
			}
		}

		void AstReferenceType::Print(ostream& o, int indentation)
		{
			o << typeDeclaration.lock()->composedName;
		}

		/*************************************************************
		AstAssembly::Print
		*************************************************************/

		void AstAssembly::Print(ostream& o, int indentation)
		{
			for (auto decl : declarations)
			{
				decl->Print(o, indentation);
				o << endl << endl;
			}
		}
	}
}