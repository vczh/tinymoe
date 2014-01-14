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

		//==============================================================================================================================================
		// Print
		//==============================================================================================================================================

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

			o << ")" << endl;
			statement->Print(o, indentation, shared_from_this());
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
			o << "(" << endl;
			for (auto it = arguments.begin(); it != arguments.end(); it++)
			{
				o << Indent(indentation + 1);
				(*it)->Print(o, indentation + 1, shared_from_this());
				if (it + 1 != arguments.end())
				{
					o << ", ";
				}
				o << endl;
			}
			o << Indent(indentation + 1) << ")";
		}

		void AstLambdaExpression::PrintInternal(ostream& o, int indentation)
		{
			o << "$lambda (";
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
				o << endl << Indent(indentation) << "else" << endl;
				falseBranch->Print(o, indentation + 1, shared_from_this());
			}
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

		//==============================================================================================================================================
		// SetParentInternal
		//==============================================================================================================================================

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

			statement->SetParent(shared_from_this());
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

		//==============================================================================================================================================
		// RoughlyOptimize
		//==============================================================================================================================================

		/*************************************************************
		AstDeclaration::RoughlyOptimize
		*************************************************************/

		void AstDeclaration::RoughlyOptimize()
		{
		}

		void AstFunctionDeclaration::RoughlyOptimize()
		{
			statement->RoughlyOptimize(statement);

			set<AstDeclaration::Ptr> variables;
			statement->CollectUsedVariables(variables);
			statement->RemoveUnnecessaryVariables(variables, statement);

			statement->RoughlyOptimize(statement);
		}

		void AstAssembly::RoughlyOptimize()
		{
			for (auto decl : declarations)
			{
				decl->RoughlyOptimize();
			}
		}

		/*************************************************************
		AstExpression::CollectSideEffectExpressions
		*************************************************************/

		void AstLiteralExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		void AstIntegerExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		void AstFloatExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		void AstStringExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		void AstExternalSymbolExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			name->CollectSideEffectExpressions(exprs);
		}

		void AstReferenceExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		void AstUnaryExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			operand->CollectSideEffectExpressions(exprs);
		}

		void AstBinaryExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			first->CollectSideEffectExpressions(exprs);
			second->CollectSideEffectExpressions(exprs);
		}

		void AstNewTypeExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			for (auto field : fields)
			{
				field->CollectSideEffectExpressions(exprs);
			}
		}

		void AstTestTypeExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			target->CollectSideEffectExpressions(exprs);
		}

		void AstNewArrayExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			length->CollectSideEffectExpressions(exprs);
		}

		void AstNewArrayLiteralExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			for (auto element : elements)
			{
				element->CollectSideEffectExpressions(exprs);
			}
		}

		void AstArrayLengthExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			target->CollectSideEffectExpressions(exprs);
		}

		void AstArrayAccessExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			target->CollectSideEffectExpressions(exprs);
			index->CollectSideEffectExpressions(exprs);
		}

		void AstFieldAccessExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			target->CollectSideEffectExpressions(exprs);
		}

		void AstInvokeExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
			exprs.push_back(dynamic_pointer_cast<AstExpression>(shared_from_this()));
		}

		void AstLambdaExpression::CollectSideEffectExpressions(AstExpression::List& exprs)
		{
		}

		/*************************************************************
		AstExpression::RoughlyOptimize
		*************************************************************/

		void AstLiteralExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
		}

		void AstIntegerExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
		}

		void AstFloatExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
		}

		void AstStringExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
		}

		void AstExternalSymbolExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			name->RoughlyOptimize(name);
		}

		void AstReferenceExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
		}

		void AstUnaryExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			operand->RoughlyOptimize(operand);
		}

		void AstBinaryExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			first->RoughlyOptimize(first);
			second->RoughlyOptimize(second);
		}

		void AstNewTypeExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			for (auto& field : fields)
			{
				field->RoughlyOptimize(field);
			}
		}

		void AstTestTypeExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			target->RoughlyOptimize(target);
		}

		void AstNewArrayExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			length->RoughlyOptimize(length);
		}

		void AstNewArrayLiteralExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			for (auto& element : elements)
			{
				element->RoughlyOptimize(element);
			}
		}

		void AstArrayLengthExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			target->RoughlyOptimize(target);
		}

		void AstArrayAccessExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			target->RoughlyOptimize(target);
			index->RoughlyOptimize(index);
		}

		void AstFieldAccessExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			target->RoughlyOptimize(target);
		}

		void AstInvokeExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			function->RoughlyOptimize(function);
			for (auto& argument : arguments)
			{
				argument->RoughlyOptimize(argument);
			}
		}

		void AstLambdaExpression::RoughlyOptimize(AstExpression::Ptr& replacement)
		{
			shared_ptr<AstExpressionStatement> stat;
			if (!(stat = dynamic_pointer_cast<AstExpressionStatement>(statement)))
			{
				if (auto block = dynamic_pointer_cast<AstBlockStatement>(statement))
				{
					if (block->statements.size() == 1)
					{
						stat = dynamic_pointer_cast<AstExpressionStatement>(block->statements[0]);
					}
				}
			}

			if (stat)
			{
				if (auto invoke = dynamic_pointer_cast<AstInvokeExpression>(stat->expression))
				{
					if (arguments.size() != invoke->arguments.size())
					{
						goto FAIL_TO_OPTIMIZE;
					}
					if (auto ref = dynamic_pointer_cast<AstReferenceExpression>(invoke->function))
					{
						auto decl = ref->reference.lock();
						for (auto argument : arguments)
						{
							if (decl == argument)
							{
								goto FAIL_TO_OPTIMIZE;
							}
						}

						for (int i = 0; (size_t)i < arguments.size(); i++)
						{
							if (auto arg = dynamic_pointer_cast<AstReferenceExpression>(invoke->arguments[i]))
							{
								if (arg->reference.lock() != arguments[i])
								{
									goto FAIL_TO_OPTIMIZE;
								}
							}
							else
							{
								goto FAIL_TO_OPTIMIZE;
							}
						}

						replacement = invoke->function;
						return;
					}
				}
			}

		FAIL_TO_OPTIMIZE:
			statement->RoughlyOptimize(statement);
		}

		/*************************************************************
		AstExpression::CollectUsedVariables
		*************************************************************/

		void AstLiteralExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
		}

		void AstIntegerExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
		}

		void AstFloatExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
		}

		void AstStringExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
		}

		void AstExternalSymbolExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			name->CollectUsedVariables(true, variables);
		}

		void AstReferenceExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			if (rightValue)
			{
				variables.insert(reference.lock());
			}
		}

		void AstUnaryExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			operand->CollectUsedVariables(true, variables);
		}

		void AstBinaryExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			first->CollectUsedVariables(true, variables);
			second->CollectUsedVariables(true, variables);
		}

		void AstNewTypeExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			for (auto field : fields)
			{
				field->CollectUsedVariables(true, variables);
			}
		}

		void AstTestTypeExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			target->CollectUsedVariables(true, variables);
		}

		void AstNewArrayExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			length->CollectUsedVariables(true, variables);
		}

		void AstNewArrayLiteralExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			for (auto element : elements)
			{
				element->CollectUsedVariables(true, variables);
			}
		}

		void AstArrayLengthExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			target->CollectUsedVariables(true, variables);
		}

		void AstArrayAccessExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			target->CollectUsedVariables(rightValue, variables);
			index->CollectUsedVariables(true, variables);
		}

		void AstFieldAccessExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			if (rightValue)
			{
				target->CollectUsedVariables(true, variables);
			}
		}

		void AstInvokeExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			function->CollectUsedVariables(true, variables);
			for (auto argument : arguments)
			{
				argument->CollectUsedVariables(true, variables);
			}
		}

		void AstLambdaExpression::CollectUsedVariables(bool rightValue, set<shared_ptr<AstDeclaration>>& variables)
		{
			statement->CollectUsedVariables(variables);
		}

		/*************************************************************
		AstExpression::RemoveUnnecessaryVariables
		*************************************************************/

		void AstLiteralExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
		}

		void AstIntegerExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
		}

		void AstFloatExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
		}

		void AstStringExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
		}

		void AstExternalSymbolExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			name->RemoveUnnecessaryVariables(variables);
		}

		void AstReferenceExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			variables.insert(reference.lock());
		}

		void AstUnaryExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			operand->RemoveUnnecessaryVariables(variables);
		}

		void AstBinaryExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			first->RemoveUnnecessaryVariables(variables);
			second->RemoveUnnecessaryVariables(variables);
		}

		void AstNewTypeExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			for (auto field : fields)
			{
				field->RemoveUnnecessaryVariables(variables);
			}
		}

		void AstTestTypeExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			target->RemoveUnnecessaryVariables(variables);
		}

		void AstNewArrayExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			length->RemoveUnnecessaryVariables(variables);
		}

		void AstNewArrayLiteralExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			for (auto element : elements)
			{
				element->RemoveUnnecessaryVariables(variables);
			}
		}

		void AstArrayLengthExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			target->RemoveUnnecessaryVariables(variables);
		}

		void AstArrayAccessExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			target->RemoveUnnecessaryVariables(variables);
			index->RemoveUnnecessaryVariables(variables);
		}

		void AstFieldAccessExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			target->RemoveUnnecessaryVariables(variables);
		}

		void AstInvokeExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			function->RemoveUnnecessaryVariables(variables);
			for (auto argument : arguments)
			{
				argument->RemoveUnnecessaryVariables(variables);
			}
		}

		void AstLambdaExpression::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			statement->RemoveUnnecessaryVariables(variables, statement);
		}

		/*************************************************************
		AstExpression::GetRootLeftValue
		*************************************************************/

		shared_ptr<AstDeclaration> AstExpression::GetRootLeftValue()
		{
			return nullptr;
		}

		shared_ptr<AstDeclaration> AstReferenceExpression::GetRootLeftValue()
		{
			return reference.lock();
		}

		shared_ptr<AstDeclaration> AstArrayAccessExpression::GetRootLeftValue()
		{
			return target->GetRootLeftValue();
		}

		shared_ptr<AstDeclaration> AstFieldAccessExpression::GetRootLeftValue()
		{
			return target->GetRootLeftValue();
		}

		/*************************************************************
		AstStatement::RoughlyOptimize
		*************************************************************/

		void AstBlockStatement::RoughlyOptimize(AstStatement::Ptr& replacement)
		{
			for (auto& stat : statements)
			{
				stat->RoughlyOptimize(stat);
			}

			AstStatement::List stats;
			for (auto stat : statements)
			{
				stat->ExpandBlock(stats, stat == *(statements.end() - 1));
			}
			statements = std::move(stats);
		}

		void AstExpressionStatement::RoughlyOptimize(AstStatement::Ptr& replacement)
		{
			expression->RoughlyOptimize(expression);
			if (auto invoke = dynamic_pointer_cast<AstInvokeExpression>(expression))
			{
				if (auto lambda = dynamic_pointer_cast<AstLambdaExpression>(invoke->function))
				{
					auto block = make_shared<AstBlockStatement>();
					for (int i = 0; (size_t)i < lambda->arguments.size(); i++)
					{
						{
							auto stat = make_shared<AstDeclarationStatement>();
							stat->declaration = lambda->arguments[i];
							block->statements.push_back(stat);
						}
						{
							auto ref = make_shared<AstReferenceExpression>();
							ref->reference = lambda->arguments[i];
						
							auto stat = make_shared<AstAssignmentStatement>();
							stat->target = ref;
							stat->value = invoke->arguments[i];

							block->statements.push_back(stat);
						}
					}
					block->statements.push_back(lambda->statement);

					replacement = block;
					goto END_OF_REPLACEMENT;
				}
			}

			{
				AstExpression::List exprs;
				expression->CollectSideEffectExpressions(exprs);
				if (exprs.size() == 0)
				{
					replacement = make_shared<AstBlockStatement>();
				}
				else if (exprs[0].get() != expression.get())
				{
					auto block = make_shared<AstBlockStatement>();
					for (auto expr : exprs)
					{
						auto stat = make_shared<AstExpressionStatement>();
						stat->expression = expr;
						block->statements.push_back(stat);
					}
					replacement = block;
				}
			}

		END_OF_REPLACEMENT:
			if (replacement.get() != this)
			{
				replacement->RoughlyOptimize(replacement);
			}
		}

		void AstDeclarationStatement::RoughlyOptimize(AstStatement::Ptr& replacement)
		{
		}

		void AstAssignmentStatement::RoughlyOptimize(AstStatement::Ptr& replacement)
		{
			target->RoughlyOptimize(target);
			value->RoughlyOptimize(value);
		}

		void AstIfStatement::RoughlyOptimize(AstStatement::Ptr& replacement)
		{
			condition->RoughlyOptimize(condition);
			trueBranch->RoughlyOptimize(trueBranch);
			if (falseBranch)
			{
				falseBranch->RoughlyOptimize(falseBranch);
			}
		}

		/*************************************************************
		AstStatement::ExpandBlock
		*************************************************************/

		void AstBlockStatement::ExpandBlock(AstStatement::List& stats, bool lastStatement)
		{
			if (!lastStatement)
			{
				for (auto stat : statements)
				{
					if (dynamic_pointer_cast<AstDeclarationStatement>(stat))
					{
						stats.push_back(dynamic_pointer_cast<AstStatement>(shared_from_this()));
						return;
					}
				}
			}
			
			for (auto stat : statements)
			{
				stat->ExpandBlock(stats, stat == *(statements.end() - 1));
			}
		}

		void AstExpressionStatement::ExpandBlock(AstStatement::List& stats, bool lastStatement)
		{
			stats.push_back(dynamic_pointer_cast<AstStatement>(shared_from_this()));
		}

		void AstDeclarationStatement::ExpandBlock(AstStatement::List& stats, bool lastStatement)
		{
			stats.push_back(dynamic_pointer_cast<AstStatement>(shared_from_this()));
		}

		void AstAssignmentStatement::ExpandBlock(AstStatement::List& stats, bool lastStatement)
		{
			stats.push_back(dynamic_pointer_cast<AstStatement>(shared_from_this()));
		}

		void AstIfStatement::ExpandBlock(AstStatement::List& stats, bool lastStatement)
		{
			stats.push_back(dynamic_pointer_cast<AstStatement>(shared_from_this()));
		}

		/*************************************************************
		AstStatement::CollectUsedVariables
		*************************************************************/

		void AstBlockStatement::CollectUsedVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			for (auto stat : statements)
			{
				stat->CollectUsedVariables(variables);
			}
		}

		void AstExpressionStatement::CollectUsedVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			expression->CollectUsedVariables(true, variables);
		}

		void AstDeclarationStatement::CollectUsedVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
		}

		void AstAssignmentStatement::CollectUsedVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			target->CollectUsedVariables(false, variables);
			value->CollectUsedVariables(true, variables);
		}

		void AstIfStatement::CollectUsedVariables(set<shared_ptr<AstDeclaration>>& variables)
		{
			condition->CollectUsedVariables(true, variables);
			trueBranch->CollectUsedVariables(variables);
			if (falseBranch)
			{
				falseBranch->CollectUsedVariables(variables);
			}
		}

		/*************************************************************
		AstStatement::RemoveUnnecessaryVariables
		*************************************************************/

		void AstBlockStatement::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables, AstStatement::Ptr& replacement)
		{
			for (int i = statements.size() - 1; i >= 0; i--)
			{
				statements[i]->RemoveUnnecessaryVariables(variables, statements[i]);
			}
		}

		void AstExpressionStatement::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables, AstStatement::Ptr& replacement)
		{
			expression->RemoveUnnecessaryVariables(variables);
		}

		void AstDeclarationStatement::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables, AstStatement::Ptr& replacement)
		{
			if (variables.find(declaration) == variables.end())
			{
				replacement = make_shared<AstBlockStatement>();
			}
		}

		void AstAssignmentStatement::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables, AstStatement::Ptr& replacement)
		{
			auto leftValue = target->GetRootLeftValue();
			if (variables.find(leftValue) == variables.end())
			{
				AstExpression::List exprs;
				target->CollectSideEffectExpressions(exprs);
				value->CollectSideEffectExpressions(exprs);

				auto block = make_shared<AstBlockStatement>();
				for (auto expr : exprs)
				{
					auto stat = make_shared<AstExpressionStatement>();
					stat->expression = expr;
					block->statements.push_back(stat);
				}
				replacement = block;
			}
		}

		void AstIfStatement::RemoveUnnecessaryVariables(set<shared_ptr<AstDeclaration>>& variables, AstStatement::Ptr& replacement)
		{
			trueBranch->RemoveUnnecessaryVariables(variables, trueBranch);
			if (falseBranch)
			{
				falseBranch->RemoveUnnecessaryVariables(variables, falseBranch);
			}
		}
	}
}