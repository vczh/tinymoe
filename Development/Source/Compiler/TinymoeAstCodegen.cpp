#include "TinymoeAstCodegen.h"

using namespace tinymoe::ast;

namespace tinymoe
{
	namespace compiler
	{
		class SymbolAstScope
		{
		public:
			typedef shared_ptr<SymbolAstScope>							Ptr;
			typedef map<GrammarSymbol::Ptr, AstDeclaration::Ptr>		SymbolAstDeclarationMap;

			SymbolAstDeclarationMap					readAsts;
			SymbolAstDeclarationMap					writeAsts;

			AstType::Ptr GetType(GrammarSymbol::Ptr symbol, AstNode::Ptr parent)
			{
				if (symbol->target == GrammarSymbolTarget::Custom)
				{
					auto type = make_shared<AstReferenceType>();
					type->typeDeclaration = dynamic_pointer_cast<AstTypeDeclaration>(readAsts.find(symbol)->second);
					return type;
				}
				else
				{
					AstPredefinedTypeName typeName = AstPredefinedTypeName::Object;
					switch (symbol->target)
					{
					case GrammarSymbolTarget::Object:
						typeName = AstPredefinedTypeName::Object;
						break;
					case GrammarSymbolTarget::Array:
						typeName = AstPredefinedTypeName::Array;
						break;
					case GrammarSymbolTarget::Symbol:
						typeName = AstPredefinedTypeName::Symbol;
						break;
					case GrammarSymbolTarget::Boolean:
						typeName = AstPredefinedTypeName::Boolean;
						break;
					case GrammarSymbolTarget::Integer:
						typeName = AstPredefinedTypeName::Integer;
						break;
					case GrammarSymbolTarget::Float:
						typeName = AstPredefinedTypeName::Float;
						break;
					case GrammarSymbolTarget::String:
						typeName = AstPredefinedTypeName::String;
						break;
					}

					auto type = make_shared<AstPredefinedType>();
					type->typeName = typeName;
					return type;
				}
			}
		};

		struct SymbolAstContext
		{
			int										uniqueId = 0;
			AstFunctionDeclaration::Ptr				function;
			GrammarSymbol::List						createdVariables;

			string GetUniquePostfix()
			{
				stringstream ss;
				ss << "_" << uniqueId++;
				return ss.str();
			}
		};

		struct SymbolAstResult
		{
			shared_ptr<AstExpression>				value;
			shared_ptr<AstStatement>				statement;
			shared_ptr<AstLambdaExpression>			continuation;

			SymbolAstResult()
			{
			}

			SymbolAstResult(shared_ptr<AstExpression> _value)
				:value(_value)
			{
			}

			SymbolAstResult(shared_ptr<AstStatement> _statement)
				:statement(_statement)
			{
			}

			SymbolAstResult(shared_ptr<AstExpression> _value, shared_ptr<AstStatement> _statement, shared_ptr<AstLambdaExpression> _continuation)
				:value(_value)
				, statement(_statement)
				, continuation(_continuation)
			{
			}

			bool RequireCps()const
			{
				return statement && continuation;
			}

			SymbolAstResult ReplaceValue(shared_ptr<AstExpression> _value)
			{
				return SymbolAstResult(_value, statement, continuation);
			}

			SymbolAstResult ReplaceValue(shared_ptr<AstExpression> _value, shared_ptr<AstLambdaExpression> _continuation)
			{
				return SymbolAstResult(_value, statement, _continuation);
			}

			void MergeForExpression(const SymbolAstResult& result, SymbolAstContext& context, vector<AstExpression::Ptr> exprs, int& exprStart, AstDeclaration::Ptr& state)
			{
				exprs.push_back(result.value);
				if (result.RequireCps())
				{
					auto block = make_shared<AstBlockStatement>();
					for (int i = exprStart; (size_t)i < exprs.size(); i++)
					{
						auto var = make_shared<AstDeclarationStatement>();
						{
							auto decl = make_shared<AstSymbolDeclaration>();
							decl->composedName = "$var" + context.GetUniquePostfix();
							var->declaration = decl;

							auto assign = make_shared<AstAssignmentStatement>();
							block->statements.push_back(assign);

							auto ref = make_shared<AstReferenceExpression>();
							ref->reference = decl;
							assign->target = ref;

							assign->value = exprs[i];
						}
					
						auto ref = make_shared<AstReferenceExpression>();
						ref->reference = var->declaration;
						exprs[i] = ref;
					}

					if (continuation)
					{
						continuation->statement = block;
					}
					else
					{
						statement = block;
					}
					continuation = result.continuation;
					exprStart = exprs.size();
					state = continuation->arguments[0];
				}
			}

			static void AppendStatement(AstStatement::Ptr& target, AstStatement::Ptr statement)
			{
				if (!target)
				{
					target = statement;
				}
				else if (auto block = dynamic_pointer_cast<AstBlockStatement>(target))
				{
					block->statements.push_back(statement);
				}
				else
				{
					block = make_shared<AstBlockStatement>();
					block->statements.push_back(target);
					block->statements.push_back(statement);
					target = block;
				}
			}

			void AppendStatement(AstStatement::Ptr statement)
			{
				value = nullptr;
				if (RequireCps())
				{
					AppendStatement(continuation->statement, statement);
				}
				else
				{
					AppendStatement(statement, statement);
				}
			}

			void MergeForStatement(const SymbolAstResult& result, AstDeclaration::Ptr& state)
			{
				AppendStatement(result.statement);
				if (result.RequireCps())
				{
					continuation = result.continuation;
				}

				if (continuation)
				{
					state = continuation->arguments[0];
				}
			}
		};

		/*************************************************************
		FunctionFragment::GetComposedName
		*************************************************************/

		string NameFragment::GetComposedName(bool primitive)
		{
			return name->GetComposedName();
		}

		string VariableArgumentFragment::GetComposedName(bool primitive)
		{
			string result;
			switch (type)
			{
			case FunctionArgumentType::Argument:
				result = "$argument";
				break;
			case FunctionArgumentType::Assignable:
				result = "assignable";
				break;
			case FunctionArgumentType::Expression:
				result = (primitive ? "$primitive" : "$expression");
				break;
			case FunctionArgumentType::List:
				result = "list";
				break;
			case FunctionArgumentType::Normal:
				result = (primitive ? "$primitive" : "$expression");
				break;
			}
			
			if (receivingType)
			{
				result += "<" + receivingType->GetComposedName() + ">";
			}
			return result;
		}

		string FunctionArgumentFragment::GetComposedName(bool primitive)
		{
			return (primitive ? "$primitive" : "$expression");
		}

		/*************************************************************
		FunctionFragment::GenerateAst
		*************************************************************/

		FunctionFragment::AstPair NameFragment::CreateAst(weak_ptr<ast::AstNode> parent)
		{
			return AstPair(nullptr, nullptr);
		}

		FunctionFragment::AstPair VariableArgumentFragment::CreateAst(weak_ptr<ast::AstNode> parent)
		{
			if (type == FunctionArgumentType::Argument)
			{
				return AstPair(nullptr, nullptr);
			}
			else if (type == FunctionArgumentType::Assignable)
			{
				auto read = make_shared<AstSymbolDeclaration>();
				read->composedName = "$read_" + name->GetComposedName();

				auto write = make_shared<AstSymbolDeclaration>();
				write->composedName = "$write_" + name->GetComposedName();
				return AstPair(read, write);
			}
			else
			{
				auto ast = make_shared<AstSymbolDeclaration>();
				ast->composedName = name->GetComposedName();
				return AstPair(ast, nullptr);
			}
		}

		FunctionFragment::AstPair FunctionArgumentFragment::CreateAst(weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstSymbolDeclaration>();
			ast->composedName = declaration->GetComposedName();
			return AstPair(ast, nullptr);
		}

		/*************************************************************
		Declaration::GenerateAst
		*************************************************************/

		shared_ptr<ast::AstDeclaration> SymbolDeclaration::GenerateAst(shared_ptr<SymbolAstScope> scope, shared_ptr<SymbolModule> symbolModule, weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstSymbolDeclaration>();
			ast->composedName = symbolModule->module->name->GetComposedName() + "::" + name->GetComposedName();
			return ast;
		}

		shared_ptr<ast::AstDeclaration> TypeDeclaration::GenerateAst(shared_ptr<SymbolAstScope> scope, shared_ptr<SymbolModule> symbolModule, weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstTypeDeclaration>();
			ast->composedName = symbolModule->module->name->GetComposedName() + "::" + name->GetComposedName();
			for (auto field : fields)
			{
				auto astField = make_shared<AstSymbolDeclaration>();
				astField->composedName = field->GetComposedName();
				ast->fields.push_back(astField);
			}
			return ast;
		}

		string FunctionDeclaration::GetComposedName()
		{
			string result;
			for (auto it = name.begin(); it != name.end(); it++)
			{
				result += (*it)->GetComposedName(type == FunctionDeclarationType::Phrase && (it == name.begin() || it + 1 == name.end()));
				if (it + 1 != name.end())
				{
					result += "_";
				}
			}
			return result;
		}

		shared_ptr<ast::AstDeclaration> FunctionDeclaration::GenerateAst(shared_ptr<SymbolAstScope> scope, shared_ptr<SymbolModule> symbolModule, weak_ptr<ast::AstNode> parent)
		{
			auto ast = make_shared<AstFunctionDeclaration>();
			ast->composedName = symbolModule->module->name->GetComposedName() + "::" + GetComposedName();
			
			{
				auto argument = make_shared<AstSymbolDeclaration>();
				argument->composedName = "$the_result";
				ast->resultVariable = argument;
			}
			{
				auto argument = make_shared<AstSymbolDeclaration>();
				if (cps && cps->stateName)
				{
					argument->composedName = cps->stateName->GetComposedName();
				}
				else
				{
					argument->composedName = "$state";
				}
				ast->arguments.push_back(argument);
				ast->stateArgument = argument;
			}
			if (category && category->signalName)
			{
				auto argument = make_shared<AstSymbolDeclaration>();
				argument->composedName = category->signalName->GetComposedName();
				ast->arguments.push_back(argument);
				ast->signalArgument = argument;
			}
			if (bodyName)
			{
				auto argument = bodyName->CreateAst(ast).first;
				ast->arguments.push_back(argument);
				ast->blockBodyArgument = argument;
			}

			for (auto it = name.begin(); it != name.end(); it++)
			{
				auto pair = (*it)->CreateAst(ast);
				if (pair.first)
				{
					ast->readArgumentAstMap.insert(make_pair(it - name.begin(), ast->arguments.size()));
					ast->arguments.push_back(pair.first);
				}
				if (pair.second)
				{
					ast->writeArgumentAstMap.insert(make_pair(it - name.begin(), ast->arguments.size()));
					ast->arguments.push_back(pair.second);
				}
			}

			{
				auto argument = make_shared<AstSymbolDeclaration>();
				if (cps && cps->continuationName)
				{
					argument->composedName = cps->continuationName->GetComposedName();
				}
				else
				{
					argument->composedName = "$continuation";
				}
				ast->arguments.push_back(argument);
				ast->continuationArgument = argument;
			}
			return ast;
		}

		/*************************************************************
		Expression::GenerateAst
		*************************************************************/

		shared_ptr<ast::AstLambdaExpression> Expression::GenerateContinuationLambdaAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<SymbolModule> module)
		{
			auto lambda = make_shared<AstLambdaExpression>();
			{
				auto ref = make_shared<AstSymbolDeclaration>();
				ref->composedName = "$state" + context.GetUniquePostfix();
				lambda->arguments.push_back(ref);
			}
			{
				auto ref = make_shared<AstSymbolDeclaration>();
				ref->composedName = "$result" + context.GetUniquePostfix();
				lambda->arguments.push_back(ref);
			}
			return lambda;
		}

		SymbolAstResult LiteralExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<SymbolModule> module)
		{
			switch (token.type)
			{
			case CodeTokenType::Integer:
				{
					char* endptr = 0;
					int result = strtol(token.value.c_str(), &endptr, 10);
					auto ast = make_shared<AstIntegerExpression>();
					ast->value = result;
					return SymbolAstResult(ast);
				}
			case CodeTokenType::Float:
				{
					char* endptr = 0;
					double result = strtod(token.value.c_str(), &endptr);
					auto ast = make_shared<AstFloatExpression>();
					ast->value = result;
					return SymbolAstResult(ast);
				}
			case CodeTokenType::String:
				{
					auto ast = make_shared<AstStringExpression>();
					ast->value = token.value;
					return SymbolAstResult(ast);
				}
			}
			return SymbolAstResult();
		}

		SymbolAstResult ArgumentExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<SymbolModule> module)
		{
			return SymbolAstResult();
		}

		SymbolAstResult ReferenceExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<SymbolModule> module)
		{
			switch (symbol->target)
			{
			case GrammarSymbolTarget::True:
				{
					auto ast = make_shared<AstLiteralExpression>();
					ast->literalName = AstLiteralName::True;
					return SymbolAstResult(ast);
				}
			case GrammarSymbolTarget::False:
				{
					auto ast = make_shared<AstLiteralExpression>();
					ast->literalName = AstLiteralName::False;
					return SymbolAstResult(ast);
				}
			case GrammarSymbolTarget::Null:
				{
					auto ast = make_shared<AstLiteralExpression>();
					ast->literalName = AstLiteralName::Null;
					return SymbolAstResult(ast);
				}
			}

			if (scope->writeAsts.find(symbol) != scope->writeAsts.end())
			{
				auto ast = make_shared<AstInvokeExpression>();
				
				auto function = make_shared<AstReferenceExpression>();
				function->reference = scope->readAsts.find(symbol)->second;
				ast->function = function;

				auto lambda = GenerateContinuationLambdaAst(scope, context, state, module);
				ast->arguments.push_back(lambda);

				return SymbolAstResult(ast, nullptr, lambda);
			}
			else
			{
				auto ast = make_shared<AstReferenceExpression>();
				ast->reference = scope->readAsts.find(symbol)->second;
				return SymbolAstResult(ast);
			}
		}

		SymbolAstResult InvokeExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<SymbolModule> module)
		{
			Expression::Ptr func;
			Expression::List args;

			if (auto ref = dynamic_pointer_cast<ReferenceExpression>(function))
			{
				switch (ref->symbol->target)
				{
				case GrammarSymbolTarget::Invoke:
					func = arguments[0];
					break;
				case GrammarSymbolTarget::InvokeWith:
					func = arguments[0];
					args = dynamic_pointer_cast<ListExpression>(arguments[1])->elements;
					break;
				case GrammarSymbolTarget::NewType:
					{
						auto ast = make_shared<AstNewTypeExpression>();
						ast->type = scope->GetType(dynamic_pointer_cast<ReferenceExpression>(arguments[0])->symbol, ast);
						return SymbolAstResult(ast);
					}
				case GrammarSymbolTarget::NewTypeOfFields:
					{
						SymbolAstResult result;
						vector<AstExpression::Ptr> exprs;
						int exprStart = 0;
						for (auto expr : dynamic_pointer_cast<ListExpression>(arguments[1])->elements)
						{
							result.MergeForExpression(expr->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);
						}

						auto ast = make_shared<AstNewTypeExpression>();
						ast->type = scope->GetType(dynamic_pointer_cast<ReferenceExpression>(arguments[0])->symbol, ast);
						ast->fields = exprs;
						return result.ReplaceValue(ast);
					}
				case GrammarSymbolTarget::NewArray:
					{
						SymbolAstResult result = arguments[0]->GenerateAst(scope, context, state, module);
						auto ast = make_shared<AstNewArrayExpression>();
						ast->length = result.value;
						return result.ReplaceValue(ast);
					}
				case GrammarSymbolTarget::GetArrayItem:
					{
						SymbolAstResult result;
						vector<AstExpression::Ptr> exprs;
						int exprStart = 0;
						for (auto expr : arguments)
						{
							result.MergeForExpression(expr->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);
						}

						auto ast = make_shared<AstArrayAccessExpression>();
						ast->target = exprs[1];
						ast->index = exprs[0];
						return result.ReplaceValue(ast);
					}
				case GrammarSymbolTarget::GetArrayLength:
					{
						SymbolAstResult result = arguments[0]->GenerateAst(scope, context, state, module);
						auto ast = make_shared<AstArrayLengthExpression>();
						ast->target = result.value;
						return result.ReplaceValue(ast);
					}
				case GrammarSymbolTarget::IsType:
					{
						SymbolAstResult result = arguments[0]->GenerateAst(scope, context, state, module);
						auto ast = make_shared<AstTestTypeExpression>();
						ast->target = result.value;
						ast->type = scope->GetType(dynamic_pointer_cast<ReferenceExpression>(arguments[1])->symbol, ast);
						return result.ReplaceValue(ast);
					}
				case GrammarSymbolTarget::IsNotType:
					{
						SymbolAstResult result = arguments[0]->GenerateAst(scope, context, state, module);
						auto ast = make_shared<AstTestTypeExpression>();
						ast->target = result.value;
						ast->type = scope->GetType(dynamic_pointer_cast<ReferenceExpression>(arguments[1])->symbol, ast);

						auto unary = make_shared<AstUnaryExpression>();
						unary->op = AstUnaryOperator::Not;
						unary->operand = ast;
						return result.ReplaceValue(unary);
					}
				case GrammarSymbolTarget::GetField:
					{
						SymbolAstResult result = arguments[1]->GenerateAst(scope, context, state, module);
						auto ast = make_shared<AstFieldAccessExpression>();
						ast->target = result.value;
						ast->composedFieldName = dynamic_pointer_cast<ArgumentExpression>(arguments[0])->name->GetComposedName();
						return result.ReplaceValue(ast);
					}
				}
			}
			if (!func)
			{
				func = function;
				args = arguments;
			}

			SymbolAstResult result;
			vector<AstExpression::Ptr> exprs;
			int exprStart = 0;

			result.MergeForExpression(func->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);
			for (auto arg : args)
			{
				result.MergeForExpression(arg->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);
			}

			auto ast = make_shared<AstInvokeExpression>();
			auto it = exprs.begin();
			ast->function = *it++;
			{
				auto ref = make_shared<AstReferenceExpression>();
				ref->reference = state;
				ast->arguments.push_back(ref);
			}
			while (it != exprs.end())
			{
				ast->arguments.push_back(*it++);
			}

			auto lambda = GenerateContinuationLambdaAst(scope, context, state, module);
			ast->arguments.push_back(lambda);

			return result.ReplaceValue(ast, lambda);
		}

		SymbolAstResult ListExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<SymbolModule> module)
		{
			SymbolAstResult result;
			vector<AstExpression::Ptr> exprs;
			int exprStart = 0;

			for (auto element : elements)
			{
				result.MergeForExpression(element->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);
			}

			auto ast = make_shared<AstNewArrayLiteralExpression>();
			for (auto expr : exprs)
			{
				ast->elements.push_back(expr);
			}
			return result.ReplaceValue(ast);
		}

		SymbolAstResult UnaryExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<SymbolModule> module)
		{
			AstUnaryOperator astOp = AstUnaryOperator::Not;
			switch (op)
			{
			case UnaryOperator::Not:
				astOp = AstUnaryOperator::Not;
				break;
			case UnaryOperator::Positive:
				astOp = AstUnaryOperator::Positive;
				break;
			case UnaryOperator::Negative:
				astOp = AstUnaryOperator::Negative;
				break;
			}

			SymbolAstResult result;
			vector<AstExpression::Ptr> exprs;
			int exprStart = 0;

			result.MergeForExpression(operand->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);

			auto ast = make_shared<AstUnaryExpression>();
			ast->op = astOp;
			ast->operand = result.value;
			return result.ReplaceValue(ast);
		}

		SymbolAstResult BinaryExpression::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<SymbolModule> module)
		{
			AstBinaryOperator astOp = AstBinaryOperator::Concat;
			
			switch (op)
			{
			case BinaryOperator::Concat:
				astOp = AstBinaryOperator::Concat;
				break;
			case BinaryOperator::Add:
				astOp = AstBinaryOperator::Add;
				break;
			case BinaryOperator::Sub:
				astOp = AstBinaryOperator::Sub;
				break;
			case BinaryOperator::Mul:
				astOp = AstBinaryOperator::Mul;
				break;
			case BinaryOperator::Div:
				astOp = AstBinaryOperator::Div;
				break;
			case BinaryOperator::IntDiv:
				astOp = AstBinaryOperator::IntDiv;
				break;
			case BinaryOperator::Mod:
				astOp = AstBinaryOperator::Mod;
				break;
			case BinaryOperator::LT:
				astOp = AstBinaryOperator::LT;
				break;
			case BinaryOperator::GT:
				astOp = AstBinaryOperator::GT;
				break;
			case BinaryOperator::LE:
				astOp = AstBinaryOperator::LE;
				break;
			case BinaryOperator::GE:
				astOp = AstBinaryOperator::GE;
				break;
			case BinaryOperator::EQ:
				astOp = AstBinaryOperator::EQ;
				break;
			case BinaryOperator::NE:
				astOp = AstBinaryOperator::NE;
				break;
			case BinaryOperator::And:
				astOp = AstBinaryOperator::And;
				break;
			case BinaryOperator::Or:
				astOp = AstBinaryOperator::Or;
				break;
			}

			SymbolAstResult result;
			vector<AstExpression::Ptr> exprs;
			int exprStart = 0;
			
			result.MergeForExpression(first->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);
			result.MergeForExpression(second->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);

			auto ast = make_shared<AstBinaryExpression>();
			ast->op = astOp;
			ast->first = exprs[0];
			ast->second = exprs[1];
			return result.ReplaceValue(ast);
		}

		/*************************************************************
		Statement::GenerateAst
		*************************************************************/

		SymbolAstResult Statement::GenerateExitAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<SymbolModule> module)
		{
			auto block = make_shared<AstBlockStatement>();
			{
				auto stat = make_shared<AstExpressionStatement>();
				block->statements.push_back(stat);

				auto invoke = make_shared<AstInvokeExpression>();
				stat->expression = invoke;

				auto cont = make_shared<AstReferenceExpression>();
				cont->reference = context.function->continuationArgument;
				invoke->function = cont;
				{
					auto arg = make_shared<AstReferenceExpression>();
					arg->reference = state;
					invoke->arguments.push_back(arg);
				}
				{
					auto arg = make_shared<AstReferenceExpression>();
					arg->reference = context.function->resultVariable;
					invoke->arguments.push_back(arg);
				}
			}
			{
				auto stat = make_shared<AstReturnStatement>();
				block->statements.push_back(stat);
			}
			return SymbolAstResult(block);
		}

		SymbolAstResult Statement::GenerateAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<ast::AstDeclaration> signal, shared_ptr<SymbolModule> module)
		{
			switch (statementSymbol->target)
			{
			case GrammarSymbolTarget::Exit:
				{
					return GenerateExitAst(scope, context, state, module);
				}
			case GrammarSymbolTarget::Select:
				{
					SymbolAstResult result = statementExpression->arguments[0]->GenerateAst(scope, context, state, module);
					auto selectedValue = result.value;

					auto selectLambda = Expression::GenerateContinuationLambdaAst(scope, context, state, module);

					auto selectContinuation = make_shared<AstSymbolDeclaration>();
					selectContinuation->composedName = "$select_continuation" + context.GetUniquePostfix();

					auto selectValue = make_shared<AstSymbolDeclaration>();
					selectValue->composedName = "$select_value" + context.GetUniquePostfix();

					{
						auto stat = make_shared<AstDeclarationStatement>();
						stat->declaration = selectContinuation;
						result.AppendStatement(stat);
					}
					{
						auto stat = make_shared<AstDeclarationStatement>();
						stat->declaration = selectValue;
						result.AppendStatement(stat);
					}
					{
						auto stat = make_shared<AstAssignmentStatement>();
						stat->value = selectLambda;

						auto access = make_shared<AstReferenceExpression>();
						access->reference = selectContinuation;
						stat->target = access;

						result.AppendStatement(stat);
					}
					{
						auto stat = make_shared<AstAssignmentStatement>();
						stat->value = selectedValue;

						auto access = make_shared<AstReferenceExpression>();
						access->reference = selectValue;
						stat->target = access;

						result.AppendStatement(stat);
					}

					if (result.continuation)
					{
						state = result.continuation->arguments[0];
					}

					shared_ptr<AstIfStatement> lastIfStat;
					Statement::Ptr caseElse;
					for (auto childCase : statements)
					{
						if (childCase->statementSymbol->target == GrammarSymbolTarget::CaseElse)
						{
							caseElse = childCase;
							continue;
						}
						SymbolAstResult caseResult = childCase->statementExpression->arguments[0]->GenerateAst(scope, context, state, module);
						if (caseResult.continuation)
						{
							state = caseResult.continuation->arguments[0];
						}

						auto ifstat = make_shared<AstIfStatement>();
						{
							auto binary = make_shared<AstBinaryExpression>();
							binary->op = AstBinaryOperator::EQ;
							
							auto value = make_shared<AstReferenceExpression>();
							value->reference = selectValue;
							binary->first = value;

							binary->second = caseResult.value;

							ifstat->condition = binary;
						}
						caseResult.AppendStatement(ifstat);

						SymbolAstResult bodyResult = childCase->GenerateBodyAst(scope, context, state, module, selectContinuation);
						ifstat->trueBranch = bodyResult.statement;

						if (lastIfStat)
						{
							lastIfStat->falseBranch = ifstat;
							result.continuation = caseResult.continuation;
						}
						else
						{
							result.AppendStatement(ifstat);
						}
						lastIfStat = ifstat;
					}

					if (caseElse)
					{
						SymbolAstResult bodyResult = caseElse->GenerateBodyAst(scope, context, state, module, selectContinuation);

						if (lastIfStat)
						{
							lastIfStat->falseBranch = bodyResult.statement;
						}
						else
						{
							result.AppendStatement(bodyResult.statement);
						}
					}

					result.continuation = selectLambda;
					return result;
				}
			case GrammarSymbolTarget::Call:
				{
					SymbolAstResult result = statementExpression->GenerateAst(scope, context, state, module);

					auto ast = make_shared<AstExpressionStatement>();
					ast->expression = result.value;

					result.AppendStatement(ast);
					return result;
				}
			case GrammarSymbolTarget::CallContinuation:
				{
					auto func = statementExpression->arguments[0];
					auto& args = dynamic_pointer_cast<ListExpression>(statementExpression->arguments[1])->elements;

					SymbolAstResult result;
					vector<AstExpression::Ptr> exprs;
					int exprStart = 0;

					result.MergeForExpression(func->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);
					for (auto arg : args)
					{
						result.MergeForExpression(arg->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);
					}

					auto invoke = make_shared<AstInvokeExpression>();
					auto it = exprs.begin();
					invoke->function = *it++;
					{
						auto ref = make_shared<AstReferenceExpression>();
						ref->reference = state;
						invoke->arguments.push_back(ref);
					}
					while (it != exprs.end())
					{
						invoke->arguments.push_back(*it++);
					}

					auto ast = make_shared<AstExpressionStatement>();
					ast->expression = invoke;

					result.AppendStatement(ast);
					return result;
				}
			case GrammarSymbolTarget::RedirectTo:
				{
					auto block = make_shared<AstBlockStatement>();
					{
						auto stat = make_shared<AstExpressionStatement>();
						block->statements.push_back(stat);

						auto invoke = make_shared<AstInvokeExpression>();
						stat->expression = invoke;

						for (auto decl : context.function->arguments)
						{
							auto arg = make_shared<AstReferenceExpression>();
							arg->reference = (decl == context.function->stateArgument ? state : decl);
							invoke->arguments.push_back(arg);
						}
					}
					{
						auto stat = make_shared<AstReturnStatement>();
						block->statements.push_back(stat);
					}
					return SymbolAstResult(block);
				}
			case GrammarSymbolTarget::Assign:
				{
					SymbolAstResult result = statementExpression->arguments[1]->GenerateAst(scope, context, state, module);
					auto assignValue = result.value;

					AstDeclaration::Ptr variable;
					bool invoke = false;
					if (auto ref = dynamic_pointer_cast<ReferenceExpression>(statementExpression->arguments[0]))
					{
						auto it = scope->writeAsts.find(ref->symbol);
						if (it == scope->writeAsts.end())
						{
							it = scope->readAsts.find(ref->symbol);
						}
						else
						{
							invoke = true;
						}
						variable = it->second;
					}
					else if (auto arg = dynamic_pointer_cast<ArgumentExpression>(statementExpression->arguments[0]))
					{
						variable = make_shared<AstSymbolDeclaration>();
						variable->composedName = arg->name->GetComposedName();

						auto varStat = make_shared<AstDeclarationStatement>();
						varStat->declaration = variable;
						result.AppendStatement(varStat);

						auto symbol = newVariables.begin()->first;
						context.createdVariables.push_back(symbol);
						scope->readAsts.insert(make_pair(symbol, variable));
					}
					
					if (invoke)
					{
						auto ast = make_shared<AstExpressionStatement>();

						auto invoke = make_shared<AstInvokeExpression>();
						ast->expression = invoke;

						auto access = make_shared<AstReferenceExpression>();
						access->reference = variable;
						invoke->function = access;

						invoke->arguments.push_back(assignValue);

						auto lambda = Expression::GenerateContinuationLambdaAst(scope, context, state, module);
						invoke->arguments.push_back(lambda);

						result.AppendStatement(ast);
						result.continuation = lambda;
						return result;
					}
					else
					{
						auto ast = make_shared<AstAssignmentStatement>();
						ast->value = assignValue;

						auto access = make_shared<AstReferenceExpression>();
						access->reference = variable;
						ast->target = access;

						result.AppendStatement(ast);
						return result;
					}
				}
			case GrammarSymbolTarget::SetArrayItem:
				{
					SymbolAstResult result;
					AstExpression::List exprs;
					int exprStart = 0;
					result.MergeForExpression(statementExpression->arguments[0]->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);
					result.MergeForExpression(statementExpression->arguments[1]->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);
					result.MergeForExpression(statementExpression->arguments[2]->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);

					auto ast = make_shared<AstAssignmentStatement>();

					auto access = make_shared<AstArrayAccessExpression>();
					ast->target = access;
					access->target = exprs[1];
					access->index = exprs[0];

					ast->value = exprs[2];

					result.AppendStatement(ast);
					return result;
				}
			case GrammarSymbolTarget::SetField:
				{
					SymbolAstResult result;
					AstExpression::List exprs;
					int exprStart = 0;
					result.MergeForExpression(statementExpression->arguments[1]->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);
					result.MergeForExpression(statementExpression->arguments[2]->GenerateAst(scope, context, state, module), context, exprs, exprStart, state);

					auto ast = make_shared<AstAssignmentStatement>();

					auto access = make_shared<AstFieldAccessExpression>();
					ast->target = access;
					access->target = exprs[0];
					access->composedFieldName = dynamic_pointer_cast<ArgumentExpression>(statementExpression->arguments[0])->name->GetComposedName();

					ast->value = exprs[1];

					result.AppendStatement(ast);
					return result;
				}
			}

			return SymbolAstResult();
		}

		SymbolAstResult Statement::GenerateBodyAst(shared_ptr<SymbolAstScope> scope, SymbolAstContext& context, shared_ptr<ast::AstDeclaration> state, shared_ptr<SymbolModule> module, shared_ptr<ast::AstDeclaration> continuation)
		{
			SymbolAstResult result;
			for (auto stat : statements)
			{
				AstDeclaration::Ptr signal;
				if (stat->connectToPreviousBlock)
				{
					signal = result.continuation->arguments[1];
				}
				result.MergeForStatement(stat->GenerateAst(scope, context, state, signal, module), state);
			}

			if (continuation)
			{
				auto stat = make_shared<AstExpressionStatement>();

				auto invoke = make_shared<AstInvokeExpression>();
				stat->expression = invoke;

				auto cont = make_shared<AstReferenceExpression>();
				cont->reference = continuation;
				invoke->function = cont;

				if (result.continuation)
				{
					{
						auto arg = make_shared<AstReferenceExpression>();
						arg->reference = result.continuation->arguments[0];
						invoke->arguments.push_back(arg);
					}
					{
						auto arg = make_shared<AstReferenceExpression>();
						arg->reference = result.continuation->arguments[1];
						invoke->arguments.push_back(arg);
					}
				}
				else
				{
					{
						auto arg = make_shared<AstReferenceExpression>();
						arg->reference = state;
						invoke->arguments.push_back(arg);
					}
					{
						auto arg = make_shared<AstLiteralExpression>();
						arg->literalName = AstLiteralName::Null;
						invoke->arguments.push_back(arg);
					}
				}
				result.AppendStatement(stat);
			}
			else
			{
				result.MergeForStatement(GenerateExitAst(scope, context, state, module), state);
			}
			return result;
		}

		/*************************************************************
		GenerateAst
		*************************************************************/

		typedef multimap<SymbolFunction::Ptr, SymbolFunction::Ptr>			MultipleDispatchMap;
		typedef map<SymbolFunction::Ptr, SymbolModule::Ptr>					FunctionModuleMap;
		typedef map<SymbolFunction::Ptr, AstFunctionDeclaration::Ptr>		FunctionAstMap;

		void GenerateStaticAst(
			SymbolAssembly::Ptr symbolAssembly,
			AstAssembly::Ptr assembly,
			SymbolAstScope::Ptr scope,
			MultipleDispatchMap& mdc,
			FunctionModuleMap& functionModules,
			FunctionAstMap& functionAsts
			)
		{
			for (auto module : symbolAssembly->symbolModules)
			{
				for (auto dfp : module->declarationFunctions)
				{
					functionModules.insert(make_pair(dfp.second, module));
					if (!dfp.second->multipleDispatchingRoot.expired())
					{
						mdc.insert(make_pair(dfp.second->multipleDispatchingRoot.lock(), dfp.second));
					}
				}
			}

			for (auto module : symbolAssembly->symbolModules)
			{
				map<Declaration::Ptr, AstDeclaration::Ptr> decls;
				for (auto sdp : module->symbolDeclarations)
				{
					auto it = decls.find(sdp.second);
					if (it == decls.end())
					{
						auto ast = sdp.second->GenerateAst(scope, module, AstNode::WeakPtr(assembly));
						assembly->declarations.push_back(ast);
						decls.insert(make_pair(sdp.second, ast));
						scope->readAsts.insert(make_pair(sdp.first, ast));

						auto itfunc = module->declarationFunctions.find(sdp.second);
						if (itfunc != module->declarationFunctions.end())
						{
							functionAsts.insert(make_pair(itfunc->second, dynamic_pointer_cast<AstFunctionDeclaration>(ast)));
						}
					}
					else
					{
						scope->readAsts.insert(make_pair(sdp.first, it->second));
					}
				}
			}
			
			for (auto module : symbolAssembly->symbolModules)
			{
				for (auto sdp : module->symbolDeclarations)
				{
					if (auto typeDecl = dynamic_pointer_cast<TypeDeclaration>(sdp.second))
					{
						if (typeDecl->parent)
						{
							auto type = scope->readAsts.find(sdp.first)->second;
							auto baseType = scope->GetType(module->baseTypes.find(typeDecl)->second, type);
						}
					}
				}
			}
		}

		void FillMultipleDispatchRedirectAst(
			AstFunctionDeclaration::Ptr ast,
			AstFunctionDeclaration::Ptr function
			)
		{
			auto astBlock = make_shared<AstBlockStatement>();
			ast->statement = astBlock;

			auto astExprStat = make_shared<AstExpressionStatement>();
			astBlock->statements.push_back(astExprStat);

			auto astInvoke = make_shared<AstInvokeExpression>();
			astExprStat->expression = astInvoke;

			auto astFunction = make_shared<AstReferenceExpression>();
			astFunction->reference = function;
			astInvoke->function = astFunction;

			for (auto argument : ast->arguments)
			{
				auto astArgument = make_shared<AstReferenceExpression>();
				astArgument->reference = argument;
				astInvoke->arguments.push_back(astArgument);
			}
		}

		void FillMultipleDispatchStepAst(
			AstFunctionDeclaration::Ptr ast,
			string functionName,
			int dispatch
			)
		{
			auto astBlock = make_shared<AstBlockStatement>();
			ast->statement = astBlock;

			auto astExprStat = make_shared<AstExpressionStatement>();
			astBlock->statements.push_back(astExprStat);

			auto astInvoke = make_shared<AstInvokeExpression>();
			astExprStat->expression = astInvoke;

			auto astFieldAccess = make_shared<AstFieldAccessExpression>();
			astFieldAccess->composedFieldName = functionName;
			astInvoke->function = astFieldAccess;

			auto astTargetObject = make_shared<AstReferenceExpression>();
			astTargetObject->reference = ast->arguments[dispatch];
			astFieldAccess->target = astTargetObject;

			for (auto argument : ast->arguments)
			{
				auto astArgument = make_shared<AstReferenceExpression>();
				astArgument->reference = argument;
				astInvoke->arguments.push_back(astArgument);
			}
		}

		void GenerateMultipleDispatchAsts(
			SymbolAssembly::Ptr symbolAssembly,
			AstAssembly::Ptr assembly,
			SymbolAstScope::Ptr scope,
			MultipleDispatchMap& mdc,
			FunctionModuleMap& functionModules,
			FunctionAstMap& functionAsts
			)
		{
			auto it = mdc.begin();
			while (it != mdc.end())
			{
				auto lower = mdc.lower_bound(it->first);
				auto upper = mdc.lower_bound(it->second);
				auto module = functionModules.find(it->first)->second;
				auto rootFunc = it->first;
				auto rootAst = functionAsts.find(rootFunc)->second;

				set<int> dispatches;
				AstFunctionDeclaration::Ptr dispatchFailAst;
				for (it = lower; it != upper; it++)
				{
					auto func = it->second;
					for (auto ita = func->function->name.begin(); ita != func->function->name.end(); ita++)
					{
						if (func->argumentTypes.find(*ita) != func->argumentTypes.end())
						{
							dispatches.insert(ita - func->function->name.begin());
						}
					}
				}
				
				FillMultipleDispatchStepAst(rootAst, "$dispatch<>" + rootAst->composedName, rootAst->readArgumentAstMap.find(*dispatches.begin())->second);
				{
					auto ast = rootFunc->function->GenerateAst(scope, module, assembly);
					ast->composedName = "$dispatch_fail<>" + rootAst->composedName;
					assembly->declarations.push_back(ast);
					dispatchFailAst = dynamic_pointer_cast<AstFunctionDeclaration>(ast);
				}

				set<string> createdFunctions, typeFunctions, objectFunctions;
				for (it = lower; it != upper; it++)
				{
					auto func = it->second;
					auto module = functionModules.find(func)->second;
					string signature;
					for (auto itd = dispatches.begin(); itd != dispatches.end(); itd++)
					{
						string methodName = "$dispatch<" + signature + ">" + rootAst->composedName;
						auto ast = dynamic_pointer_cast<AstFunctionDeclaration>(func->function->GenerateAst(scope, module, assembly));
						ast->composedName = methodName;

						auto ita = func->argumentTypes.find(func->function->name[*itd]);
						if (ita == func->argumentTypes.end())
						{
							auto type = make_shared<AstPredefinedType>();
							type->typeName = AstPredefinedTypeName::Object;
							ast->ownerType = type;
							objectFunctions.insert(methodName);
						}
						else
						{
							ast->ownerType = scope->GetType(ita->second, ast);
							typeFunctions.insert(methodName);
						}

						if (itd != dispatches.begin())
						{
							signature += ", ";
						}
						{
							stringstream o;
							ast->ownerType->Print(o, 0);
							signature += o.str();
						}

						string functionName = "$dispatch<" + signature + ">" + rootAst->composedName;
						if (createdFunctions.insert(functionName).second)
						{
							assembly->declarations.push_back(ast);
							auto itd2 = itd;
							if (++itd2 == dispatches.end())
							{
								FillMultipleDispatchRedirectAst(ast, functionAsts.find(func)->second);
							}
							else
							{
								FillMultipleDispatchStepAst(ast, functionName, ast->readArgumentAstMap.find(*itd)->second);
							}
						}
					}
				}

				for (auto name : typeFunctions)
				{
					if (objectFunctions.find(name) == objectFunctions.end())
					{
						auto ast = dynamic_pointer_cast<AstFunctionDeclaration>(rootFunc->function->GenerateAst(scope, module, assembly));
						ast->composedName = name;
						{
							auto type = make_shared<AstPredefinedType>();
							type->typeName = AstPredefinedTypeName::Object;
							ast->ownerType = type;
						}
						assembly->declarations.push_back(ast);
						FillMultipleDispatchRedirectAst(ast, dispatchFailAst);
					}
				}

				functionAsts.find(rootFunc)->second = dispatchFailAst;
				it = upper;
			}
		}

		ast::AstAssembly::Ptr GenerateAst(SymbolAssembly::Ptr symbolAssembly)
		{
			auto assembly = make_shared<AstAssembly>();
			auto scope = make_shared<SymbolAstScope>();

			multimap<SymbolFunction::Ptr, SymbolFunction::Ptr> multipleDispatchChildren;
			map<SymbolFunction::Ptr, SymbolModule::Ptr> functionModules;
			map<SymbolFunction::Ptr, AstFunctionDeclaration::Ptr> functionAsts;
			GenerateStaticAst(symbolAssembly, assembly, scope, multipleDispatchChildren, functionModules, functionAsts);
			GenerateMultipleDispatchAsts(symbolAssembly, assembly, scope, multipleDispatchChildren, functionModules, functionAsts);

			assembly->SetParent();
			return assembly;
		}
	}
}