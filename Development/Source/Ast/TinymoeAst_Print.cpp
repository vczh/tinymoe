#include "TinymoeAst.h"

// find:	void (\w+)::PrintInternal\(ostream_t& o, int indentation\)
// replace:	void Visit($1* node)override

namespace tinymoe
{
	namespace ast
	{
		string_t Indent(int indentation)
		{
			string_t s;
			for (int i = 0; i < indentation; i++)
			{
				s += T("    ");
			}
			return s;
		}

		/*************************************************************
		AstDeclaration::Print
		*************************************************************/

		class AstDeclaration_Print : public AstDeclarationVisitor
		{
		private:
			ostream_t&			o;
			int					indentation;
		public:
			AstDeclaration_Print(ostream_t& _o, int _indentation)
				:o(_o), indentation(_indentation)
			{
			}

			void Visit(AstSymbolDeclaration* node)override
			{
				o << Indent(indentation) << T("$symbol ") << node->composedName << T(";");
			}

			void Visit(AstTypeDeclaration* node)override
			{
				o << Indent(indentation) << T("$type ") << node->composedName;
				if (!node->baseType.expired())
				{
					o << T(" : ");
					Print(node->baseType.lock(), o, indentation);
				}
				o << endl << Indent(indentation) << T("{") << endl;
				for (auto field : node->fields)
				{
					Print(field, o, indentation + 1, node->shared_from_this());
					o << endl;
				}
				o << Indent(indentation) << T("}");
			}

			void Visit(AstFunctionDeclaration* node)override
			{
				o << Indent(indentation) << T("$procedure ");
				if (node->ownerType)
				{
					o << T("(");
					Print(node->ownerType, o, indentation, node->shared_from_this());
					o << T(").");
				}
				o << node->composedName << T("(");
				for (auto it = node->arguments.begin(); it != node->arguments.end(); it++)
				{
					o << (*it)->composedName;
					if (it + 1 != node->arguments.end())
					{
						o << T(", ");
					}
				}

				o << T(")") << endl;
				Print(node->statement, o, indentation, node->shared_from_this());
			}
		};

		/*************************************************************
		AstExpression::Print
		*************************************************************/

		class AstExpression_Print : public AstExpressionVisitor
		{
		private:
			ostream_t&			o;
			int					indentation;
		public:
			AstExpression_Print(ostream_t& _o, int _indentation)
				:o(_o), indentation(_indentation)
			{
			}

			void Visit(AstLiteralExpression* node)override
			{
				switch (node->literalName)
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

			void Visit(AstIntegerExpression* node)override
			{
				o << node->value;
			}

			void Visit(AstFloatExpression* node)override
			{
				o << node->value;
			}

			void Visit(AstStringExpression* node)override
			{
				o << T("\"") << node->value << T("\"");
			}

			void Visit(AstExternalSymbolExpression* node)override
			{
				o << T("$external (\"") << node->name << T("\")");
			}

			void Visit(AstReferenceExpression* node)override
			{
				o << node->reference.lock()->composedName;
			}

			void Visit(AstNewTypeExpression* node)override
			{
				o << T("new ");
				Print(node->type, o, indentation, node->shared_from_this());
				o << T("(");
				for (auto it = node->fields.begin(); it != node->fields.end(); it++)
				{
					Print((*it), o, indentation, node->shared_from_this());
					if (it + 1 != node->fields.end())
					{
						o << T(", ");
					}
				}
				o << T(")");
			}

			void Visit(AstTestTypeExpression* node)override
			{
				o << T("(");
				Print(node->target, o, indentation, node->shared_from_this());
				o << T(" is ");
				Print(node->type, o, indentation, node->shared_from_this());
				o << T(")");
			}

			void Visit(AstNewArrayExpression* node)override
			{
				o << T("new $Array(");
				Print(node->length, o, indentation, node->shared_from_this());
				o << T(")");
			}

			void Visit(AstNewArrayLiteralExpression* node)override
			{
				o << T("[");
				for (auto it = node->elements.begin(); it != node->elements.end(); it++)
				{
					Print((*it), o, indentation, node->shared_from_this());
					if (it + 1 != node->elements.end())
					{
						o << T(", ");
					}
				}
				o << T("]");
			}

			void Visit(AstArrayLengthExpression* node)override
			{
				o << T("$ArrayLength(");
				Print(node->target, o, indentation, node->shared_from_this());
				o << T(")");
			}

			void Visit(AstArrayAccessExpression* node)override
			{
				Print(node->target, o, indentation, node->shared_from_this());
				o << T("[");
				Print(node->index, o, indentation, node->shared_from_this());
				o << T("]");
			}

			void Visit(AstFieldAccessExpression* node)override
			{
				Print(node->target, o, indentation, node->shared_from_this());
				o << T(".") << node->composedFieldName;
			}

			void Visit(AstInvokeExpression* node)override
			{
				Print(node->function, o, indentation, node->shared_from_this());
				o << T("(") << endl;
				for (auto it = node->arguments.begin(); it != node->arguments.end(); it++)
				{
					o << Indent(indentation + 1);
					Print((*it), o, indentation + 1, node->shared_from_this());
					if (it + 1 != node->arguments.end())
					{
						o << T(", ");
					}
					o << endl;
				}
				o << Indent(indentation + 1) << T(")");
			}

			void Visit(AstLambdaExpression* node)override
			{
				o << T("$lambda (");
				for (auto it = node->arguments.begin(); it != node->arguments.end(); it++)
				{
					o << (*it)->composedName;
					if (it + 1 != node->arguments.end())
					{
						o << T(", ");
					}
				}
				o << T(")") << endl;
				Print(node->statement, o, indentation + 1, node->shared_from_this());
			}
		};

		/*************************************************************
		AstStatement::Print
		*************************************************************/

		class AstStatement_Print : public AstStatementVisitor
		{
		private:
			ostream_t&			o;
			int					indentation;
		public:
			AstStatement_Print(ostream_t& _o, int _indentation)
				:o(_o), indentation(_indentation)
			{
			}

			void Visit(AstBlockStatement* node)override
			{
				o << Indent(indentation) << T("{") << endl;
				for (auto statement : node->statements)
				{
					Print(statement, o, indentation + 1, node->shared_from_this());
					o << endl;
				}
				o << Indent(indentation) << T("}");
			}

			void Visit(AstExpressionStatement* node)override
			{
				o << Indent(indentation);
				Print(node->expression, o, indentation, node->shared_from_this());
				o << T(";");
			}

			void Visit(AstDeclarationStatement* node)override
			{
				Print(node->declaration, o, indentation, node->shared_from_this());
			}

			void Visit(AstAssignmentStatement* node)override
			{
				o << Indent(indentation);
				Print(node->target, o, indentation, node->shared_from_this());
				o << T(" = ");
				Print(node->value, o, indentation, node->shared_from_this());
				o << T(";");
			}

			void Visit(AstIfStatement* node)override
			{
				o << Indent(indentation) << T("if (");
				Print(node->condition, o, indentation, node->shared_from_this());
				o << endl;
				Print(node->trueBranch, o, indentation + 1, node->shared_from_this());
				if (node->falseBranch)
				{
					o << endl << Indent(indentation) << T("else") << endl;
					Print(node->falseBranch, o, indentation + 1, node->shared_from_this());
				}
			}
		};

		/*************************************************************
		AstType::Print
		*************************************************************/

		class AstType_Print : public AstTypeVisitor
		{
		private:
			ostream_t&			o;
			int					indentation;
		public:
			AstType_Print(ostream_t& _o, int _indentation)
				:o(_o), indentation(_indentation)
			{
			}

			void Visit(AstPredefinedType* node)override
			{
				switch (node->typeName)
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

			void Visit(AstReferenceType* node)override
			{
				o << node->typeDeclaration.lock()->composedName;
			}
		};

		/*************************************************************
		AstAssembly::Print
		*************************************************************/

		class AstNode_Print : public AstVisitor
		{
		private:
			ostream_t&			o;
			int					indentation;
		public:
			AstNode_Print(ostream_t& _o, int _indentation)
				:o(_o), indentation(_indentation)
			{
			}
			
			void Visit(AstType* node)override
			{
				AstType_Print visitor(o, indentation);
				node->Accept(&visitor);
			}

			void Visit(AstExpression* node)override
			{
				AstExpression_Print visitor(o, indentation);
				node->Accept(&visitor);
			}

			void Visit(AstStatement* node)override
			{
				AstStatement_Print visitor(o, indentation);
				node->Accept(&visitor);
			}

			void Visit(AstDeclaration* node)override
			{
				AstDeclaration_Print visitor(o, indentation);
				node->Accept(&visitor);
			}

			void Visit(AstAssembly* node)override
			{
				for (auto decl : node->declarations)
				{
					Print(decl, o, indentation, node->shared_from_this());
					o << endl << endl;
				}
			}
		};

		/*************************************************************
		Print
		*************************************************************/

		void Print(AstNode::Ptr node, ostream_t& o, int indentation, AstNode::WeakPtr _parent)
		{
			ASSERT(_parent.expired() || node->parent.lock() == _parent.lock());
			AstNode_Print visitor(o, indentation);
			node->Accept(&visitor);
		}
	}
}