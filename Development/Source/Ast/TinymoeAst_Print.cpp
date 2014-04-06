#include "TinymoeAst.h"

namespace tinymoe
{
	namespace ast
	{
		/*************************************************************
		AstDeclaration::Print
		*************************************************************/

		void AstSymbolDeclaration::PrintInternal(ostream_t& o, int indentation)
		{
			o << Indent(indentation) << T("$symbol ") << composedName << T(";");
		}

		void AstTypeDeclaration::PrintInternal(ostream_t& o, int indentation)
		{
			o << Indent(indentation) << T("$type ") << composedName;
			if (!baseType.expired())
			{
				o << T(" : ");
				baseType.lock()->Print(o, indentation);
			}
			o << endl << Indent(indentation) << T("{") << endl;
			for (auto field : fields)
			{
				field->Print(o, indentation + 1, shared_from_this());
				o << endl;
			}
			o << Indent(indentation) << T("}");
		}

		void AstFunctionDeclaration::PrintInternal(ostream_t& o, int indentation)
		{
			o << Indent(indentation) << T("$procedure ");
			if (ownerType)
			{
				o << T("(");
				ownerType->Print(o, indentation, shared_from_this());
				o << T(").");
			}
			o << composedName << T("(");
			for (auto it = arguments.begin(); it != arguments.end(); it++)
			{
				o << (*it)->composedName;
				if (it + 1 != arguments.end())
				{
					o << T(", ");
				}
			}

			o << T(")") << endl;
			statement->Print(o, indentation, shared_from_this());
		}

		/*************************************************************
		AstExpression::Print
		*************************************************************/

		void AstLiteralExpression::PrintInternal(ostream_t& o, int indentation)
		{
			switch (literalName)
			{
			case AstLiteralName::Null:
				o << T("$null");
				break;
			case AstLiteralName::True:
				o << T("$true");
				break;
			case AstLiteralName::False:
				o << T("$false");
				break;
			}
		}

		void AstIntegerExpression::PrintInternal(ostream_t& o, int indentation)
		{
			o << value;
		}

		void AstFloatExpression::PrintInternal(ostream_t& o, int indentation)
		{
			o << value;
		}

		void AstStringExpression::PrintInternal(ostream_t& o, int indentation)
		{
			o << T("\"") << value << T("\"");
		}

		void AstExternalSymbolExpression::PrintInternal(ostream_t& o, int indentation)
		{
			o << T("$external (\"") << name << T("\")");
		}

		void AstReferenceExpression::PrintInternal(ostream_t& o, int indentation)
		{
			o << reference.lock()->composedName;
		}

		void AstNewTypeExpression::PrintInternal(ostream_t& o, int indentation)
		{
			o << T("new ");
			type->Print(o, indentation, shared_from_this());
			o << T("(");
			for (auto it = fields.begin(); it != fields.end(); it++)
			{
				(*it)->Print(o, indentation, shared_from_this());
				if (it + 1 != fields.end())
				{
					o << T(", ");
				}
			}
			o << T(")");
		}

		void AstTestTypeExpression::PrintInternal(ostream_t& o, int indentation)
		{
			o << T("(");
			target->Print(o, indentation, shared_from_this());
			o << T(" is ");
			type->Print(o, indentation, shared_from_this());
			o << T(")");
		}

		void AstNewArrayExpression::PrintInternal(ostream_t& o, int indentation)
		{
			o << T("new $Array(");
			length->Print(o, indentation, shared_from_this());
			o << T(")");
		}

		void AstNewArrayLiteralExpression::PrintInternal(ostream_t& o, int indentation)
		{
			o << T("[");
			for (auto it = elements.begin(); it != elements.end(); it++)
			{
				(*it)->Print(o, indentation, shared_from_this());
				if (it + 1 != elements.end())
				{
					o << T(", ");
				}
			}
			o << T("]");
		}

		void AstArrayLengthExpression::PrintInternal(ostream_t& o, int indentation)
		{
			o << T("$ArrayLength(");
			target->Print(o, indentation, shared_from_this());
			o << T(")");
		}

		void AstArrayAccessExpression::PrintInternal(ostream_t& o, int indentation)
		{
			target->Print(o, indentation, shared_from_this());
			o << T("[");
			index->Print(o, indentation, shared_from_this());
			o << T("]");
		}

		void AstFieldAccessExpression::PrintInternal(ostream_t& o, int indentation)
		{
			target->Print(o, indentation, shared_from_this());
			o << T(".") << composedFieldName;
		}

		void AstInvokeExpression::PrintInternal(ostream_t& o, int indentation)
		{
			function->Print(o, indentation, shared_from_this());
			o << T("(") << endl;
			for (auto it = arguments.begin(); it != arguments.end(); it++)
			{
				o << Indent(indentation + 1);
				(*it)->Print(o, indentation + 1, shared_from_this());
				if (it + 1 != arguments.end())
				{
					o << T(", ");
				}
				o << endl;
			}
			o << Indent(indentation + 1) << T(")");
		}

		void AstLambdaExpression::PrintInternal(ostream_t& o, int indentation)
		{
			o << T("$lambda (");
			for (auto it = arguments.begin(); it != arguments.end(); it++)
			{
				o << (*it)->composedName;
				if (it + 1 != arguments.end())
				{
					o << T(", ");
				}
			}
			o << T(")") << endl;
			statement->Print(o, indentation + 1, shared_from_this());
		}

		/*************************************************************
		AstStatement::Print
		*************************************************************/

		void AstBlockStatement::PrintInternal(ostream_t& o, int indentation)
		{
			o << Indent(indentation) << T("{") << endl;
			for (auto statement : statements)
			{
				statement->Print(o, indentation + 1, shared_from_this());
				o << endl;
			}
			o << Indent(indentation) << T("}");
		}

		void AstExpressionStatement::PrintInternal(ostream_t& o, int indentation)
		{
			o << Indent(indentation);
			expression->Print(o, indentation, shared_from_this());
			o << T(";");
		}

		void AstDeclarationStatement::PrintInternal(ostream_t& o, int indentation)
		{
			declaration->Print(o, indentation, shared_from_this());
		}

		void AstAssignmentStatement::PrintInternal(ostream_t& o, int indentation)
		{
			o << Indent(indentation);
			target->Print(o, indentation, shared_from_this());
			o << T(" = ");
			value->Print(o, indentation, shared_from_this());
			o << T(";");
		}

		void AstIfStatement::PrintInternal(ostream_t& o, int indentation)
		{
			o << Indent(indentation) << T("if (");
			condition->Print(o, indentation, shared_from_this());
			o << endl;
			trueBranch->Print(o, indentation + 1, shared_from_this());
			if (falseBranch)
			{
				o << endl << Indent(indentation) << T("else") << endl;
				falseBranch->Print(o, indentation + 1, shared_from_this());
			}
		}

		/*************************************************************
		AstType::Print
		*************************************************************/

		void AstPredefinedType::PrintInternal(ostream_t& o, int indentation)
		{
			switch (typeName)
			{
			case AstPredefinedTypeName::Object:
				o << T("$Object");
				break;
			case AstPredefinedTypeName::Symbol:
				o << T("$Symbol");
				break;
			case AstPredefinedTypeName::Array:
				o << T("$Array");
				break;
			case AstPredefinedTypeName::Boolean:
				o << T("$Boolean");
				break;
			case AstPredefinedTypeName::Integer:
				o << T("$Integer");
				break;
			case AstPredefinedTypeName::Float:
				o << T("$Float");
				break;
			case AstPredefinedTypeName::String:
				o << T("$Function");
				break;
			}
		}

		void AstReferenceType::PrintInternal(ostream_t& o, int indentation)
		{
			o << typeDeclaration.lock()->composedName;
		}

		/*************************************************************
		AstAssembly::Print
		*************************************************************/

		void AstAssembly::PrintInternal(ostream_t& o, int indentation)
		{
			for (auto decl : declarations)
			{
				decl->Print(o, indentation, shared_from_this());
				o << endl << endl;
			}
		}
	}
}