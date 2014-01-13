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

		void AstNode::Print(ostream& o, int indentation, AstNode::WeakPtr _parent)
		{
			ASSERT(_parent.expired() || parent.lock() == _parent.lock());
			PrintInternal(o, indentation);
		}

		void AstNode::SetParent(AstNode::WeakPtr _parent)
		{
			ASSERT(parent.expired());
			parent = _parent;
			SetParentInternal();
		}

		/*************************************************************
		AstDeclaration::Print
		*************************************************************/

		void AstSymbolDeclaration::PrintInternal(ostream& o, int indentation)
		{
			o << Indent(indentation) << "$symbol " << composedName << ";";
		}

		void AstTypeDeclaration::PrintInternal(ostream& o, int indentation)
		{
			o << Indent(indentation) << "$type " << composedName;
			if (!baseType.expired())
			{
				o << " : ";
				baseType.lock()->Print(o, indentation);
			}
			o << endl << Indent(indentation) << "{" << endl;
			for (auto field : fields)
			{
				field->Print(o, indentation + 1, shared_from_this());
				o << endl;
			}
			o << Indent(indentation) << "}";
		}

		void AstFunctionDeclaration::PrintInternal(ostream& o, int indentation)
		{
			o << Indent(indentation) << "$procedure ";
			if (ownerType)
			{
				o << "(";
				ownerType->Print(o, indentation, shared_from_this());
				o << ").";
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

			if (statement)
			{
				o << ")" << endl;
				statement->Print(o, indentation, shared_from_this());
			}
			else
			{
				o << ");";
			}
		}

		/*************************************************************
		AstExpression::Print
		*************************************************************/

		void AstLiteralExpression::PrintInternal(ostream& o, int indentation)
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

		void AstIntegerExpression::PrintInternal(ostream& o, int indentation)
		{
			o << value;
		}

		void AstFloatExpression::PrintInternal(ostream& o, int indentation)
		{
			o << value;
		}

		void AstStringExpression::PrintInternal(ostream& o, int indentation)
		{
			o << "\"" << value << "\"";
		}

		void AstExternalSymbolExpression::PrintInternal(ostream& o, int indentation)
		{
			o << "$external (";
			name->Print(o, indentation);
			o << ")";
		}

		void AstReferenceExpression::PrintInternal(ostream& o, int indentation)
		{
			o << reference.lock()->composedName;
		}

		void AstUnaryExpression::PrintInternal(ostream& o, int indentation)
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
			operand->Print(o, indentation, shared_from_this());
		}

		void AstBinaryExpression::PrintInternal(ostream& o, int indentation)
		{
			o << "(";
			first->Print(o, indentation, shared_from_this());
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
			case AstBinaryOperator::IntDiv:
				o << " \\ ";
				break;
			case AstBinaryOperator::Mod:
				o << " % ";
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
			second->Print(o, indentation, shared_from_this());
			o << ")";
		}

		void AstNewTypeExpression::PrintInternal(ostream& o, int indentation)
		{
			o << "new ";
			type->Print(o, indentation, shared_from_this());
			o << "(";
			for (auto it = fields.begin(); it != fields.end(); it++)
			{
				(*it)->Print(o, indentation, shared_from_this());
				if (it + 1 != fields.end())
				{
					o << ", ";
				}
			}
			o << ")";
		}

		void AstTestTypeExpression::PrintInternal(ostream& o, int indentation)
		{
			o << "(";
			target->Print(o, indentation, shared_from_this());
			o << " is ";
			type->Print(o, indentation, shared_from_this());
			o << ")";
		}

		void AstNewArrayExpression::PrintInternal(ostream& o, int indentation)
		{
			o << "new $Array(";
			length->Print(o, indentation, shared_from_this());
			o << ")";
		}

		void AstNewArrayLiteralExpression::PrintInternal(ostream& o, int indentation)
		{
			o << "[";
			for (auto it = elements.begin(); it != elements.end(); it++)
			{
				(*it)->Print(o, indentation, shared_from_this());
				if (it + 1 != elements.end())
				{
					o << ", ";
				}
			}
			o << "]";
		}

		void AstArrayLengthExpression::PrintInternal(ostream& o, int indentation)
		{
			o << "$ArrayLength(";
			target->Print(o, indentation, shared_from_this());
			o << ")";
		}

		void AstArrayAccessExpression::PrintInternal(ostream& o, int indentation)
		{
			target->Print(o, indentation, shared_from_this());
			o << "[";
			index->Print(o, indentation, shared_from_this());
			o << "]";
		}

		void AstFieldAccessExpression::PrintInternal(ostream& o, int indentation)
		{
			target->Print(o, indentation, shared_from_this());
			o << "." << composedFieldName;
		}

		void AstInvokeExpression::PrintInternal(ostream& o, int indentation)
		{
			function->Print(o, indentation, shared_from_this());
			o << "(";
			for (auto it = arguments.begin(); it != arguments.end(); it++)
			{
				(*it)->Print(o, indentation, shared_from_this());
				if (it + 1 != arguments.end())
				{
					o << ", ";
				}
			}
			o << ")";
		}

		void AstLambdaExpression::PrintInternal(ostream& o, int indentation)
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
			statement->Print(o, indentation + 1, shared_from_this());
		}

		/*************************************************************
		AstStatement::Print
		*************************************************************/

		void AstBlockStatement::PrintInternal(ostream& o, int indentation)
		{
			o << Indent(indentation) << "{" << endl;
			for (auto statement : statements)
			{
				statement->Print(o, indentation + 1, shared_from_this());
				o << endl;
			}
			o << Indent(indentation) << "}";
		}

		void AstExpressionStatement::PrintInternal(ostream& o, int indentation)
		{
			o << Indent(indentation);
			expression->Print(o, indentation, shared_from_this());
			o << ";";
		}

		void AstDeclarationStatement::PrintInternal(ostream& o, int indentation)
		{
			declaration->Print(o, indentation, shared_from_this());
		}

		void AstAssignmentStatement::PrintInternal(ostream& o, int indentation)
		{
			o << Indent(indentation);
			target->Print(o, indentation, shared_from_this());
			o << " = ";
			value->Print(o, indentation, shared_from_this());
			o << ";";
		}

		void AstIfStatement::PrintInternal(ostream& o, int indentation)
		{
			o << Indent(indentation) << "if (";
			condition->Print(o, indentation, shared_from_this());
			o << endl;
			trueBranch->Print(o, indentation + 1, shared_from_this());
			if (falseBranch)
			{
				o << endl;
				falseBranch->Print(o, indentation + 1, shared_from_this());
			}
		}

		void AstReturnStatement::PrintInternal(ostream& o, int indentation)
		{
			o << Indent(indentation) << "return;";
		}

		/*************************************************************
		AstType::Print
		*************************************************************/

		void AstPredefinedType::PrintInternal(ostream& o, int indentation)
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

		void AstReferenceType::PrintInternal(ostream& o, int indentation)
		{
			o << typeDeclaration.lock()->composedName;
		}

		/*************************************************************
		AstAssembly::Print
		*************************************************************/

		void AstAssembly::PrintInternal(ostream& o, int indentation)
		{
			for (auto decl : declarations)
			{
				decl->Print(o, indentation, shared_from_this());
				o << endl << endl;
			}
		}

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

			if (statement)
			{
				statement->SetParent(shared_from_this());
			}
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
			name->SetParent(shared_from_this());
		}

		void AstReferenceExpression::SetParentInternal()
		{
		}

		void AstUnaryExpression::SetParentInternal()
		{
			operand->SetParent(shared_from_this());
		}

		void AstBinaryExpression::SetParentInternal()
		{
			first->SetParent(shared_from_this());
			second->SetParent(shared_from_this());
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

		void AstReturnStatement::SetParentInternal()
		{
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