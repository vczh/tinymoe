#include "TinymoeExpressionAnalyzer.h"

namespace tinymoe
{
	namespace compiler
	{
		/*************************************************************
		GrammarFragment
		*************************************************************/

		GrammarFragment::GrammarFragment(GrammarFragmentType _type)
			:type(_type)
		{
		}

		string_t GrammarFragment::GetUniqueIdFragment()
		{
			switch (type)
			{
			case GrammarFragmentType::Type:
				return T("<type>");
			case GrammarFragmentType::Primitive:
				return T("<primitive>");
			case GrammarFragmentType::Expression:
				return T("<expression>");
			case GrammarFragmentType::List:
				return T("<list>");
			case GrammarFragmentType::Assignable:
				return T("<assignable>");
			case GrammarFragmentType::Argument:
				return T("<argument>");
				break;
			}
		
			string_t uniqueId;
			for (auto i = identifiers.begin(); i != identifiers.end(); i++)
			{
				uniqueId += *i;
				if (i + 1 != identifiers.end())
				{
					uniqueId += T(" ");
				}
			}
			return uniqueId;
		}

		/*************************************************************
		GrammarSymbol
		*************************************************************/

		GrammarSymbol::GrammarSymbol(GrammarSymbolType _type, GrammarSymbolTarget _target)
			:type(_type)
			, target(_target)
		{
		}

		void GrammarSymbol::CalculateUniqueId()
		{
			uniqueId = T("");
			for (auto i = fragments.begin(); i != fragments.end(); i++)
			{
				uniqueId += (*i)->GetUniqueIdFragment();
				if (i + 1 != fragments.end())
				{
					uniqueId += T(" ");
				}
			}
		}

		GrammarSymbol::Ptr operator+(GrammarSymbol::Ptr symbol, const string_t& name)
		{
			GrammarFragment::Ptr fragment;
			if (symbol->fragments.size() == 0 || (fragment = symbol->fragments.back())->type != GrammarFragmentType::Name)
			{
				fragment = make_shared<GrammarFragment>(GrammarFragmentType::Name);
				symbol->fragments.push_back(fragment);
			}

			fragment->identifiers.push_back(name);
			return symbol;
		}

		GrammarSymbol::Ptr operator+(GrammarSymbol::Ptr symbol, GrammarFragmentType type)
		{
			auto fragment = make_shared<GrammarFragment>(type);
			symbol->fragments.push_back(fragment);
			return symbol;
		}

		/*************************************************************
		GrammarStackItem
		*************************************************************/

		void GrammarStackItem::FillPredefinedSymbols()
		{
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Object)
				+ T("object")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Array)
				+ T("array")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Symbol)
				+ T("symbol")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Boolean)
				+ T("boolean")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Integer)
				+ T("integer")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Float)
				+ T("float")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::String)
				+ T("string")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Function)
				+ T("function")
				);
		
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Symbol, GrammarSymbolTarget::True)
				+ T("true")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Symbol, GrammarSymbolTarget::False)
				+ T("false")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Symbol, GrammarSymbolTarget::Null)
				+ T("null")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Symbol, GrammarSymbolTarget::TheResult)
				+ T("the") + T("result")
				);
			
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::Invoke)
				+ GrammarFragmentType::Primitive + T("of") + GrammarFragmentType::List
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::InvokeContinuation)
				+ T("continuation") + GrammarFragmentType::Expression + T("of") + GrammarFragmentType::List
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::NewTypeOfFields)
				+ T("new") + GrammarFragmentType::Type + T("of") + GrammarFragmentType::List
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::NewArray)
				+ T("new") + T("array") + T("of") + GrammarFragmentType::Expression + T("items")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::GetArrayItem)
				+ T("item") + GrammarFragmentType::Expression + T("of") + T("array") + GrammarFragmentType::Primitive
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::GetArrayLength)
				+ T("length") + T("of") + T("array") + GrammarFragmentType::Primitive
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::IsType)
				+ GrammarFragmentType::Primitive + T("is") + GrammarFragmentType::Type
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::IsNotType)
				+ GrammarFragmentType::Primitive + T("is") + T("not") + GrammarFragmentType::Type
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::GetField)
				+ T("field") + GrammarFragmentType::Argument + T("of") + GrammarFragmentType::Primitive
				);

			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::End)
				+ T("end")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Block, GrammarSymbolTarget::Select)
				+ T("select") + GrammarFragmentType::Expression
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Case)
				+ T("case") + GrammarFragmentType::Expression
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::CaseElse)
				+ T("case") + T("else")
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::RedirectTo)
				+ T("redirect") + T("to") + GrammarFragmentType::Expression
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Assign)
				+ T("set") + GrammarFragmentType::Assignable + T("to") + GrammarFragmentType::Expression
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::SetArrayItem)
				+ T("set") + T("item") + GrammarFragmentType::Expression + T("of") + T("array") + GrammarFragmentType::Expression + T("to") + GrammarFragmentType::Expression
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::SetField)
				+ T("set") + T("field") + GrammarFragmentType::Argument + T("of") + GrammarFragmentType::Expression + T("to") + GrammarFragmentType::Expression
				);
		}

		/*************************************************************
		Expression::ToLog
		*************************************************************/

		string_t LiteralExpression::ToLog()
		{
			if (token.type == CodeTokenType::String)
			{
				return T("\"") + CodeToken::EscapeString(token.value) + T("\"");
			}
			else
			{
				return token.value;
			}
		}

		string_t ArgumentExpression::ToLog()
		{
			string_t result = T("(argument: ") + name->GetName() + T(")");
			return result;
		}

		string_t ReferenceExpression::ToLog()
		{
			return symbol->uniqueId;
		}

		string_t InvokeExpression::ToLog()
		{
			string_t result = function->ToLog() + T("(");
			for (auto i = arguments.begin(); i != arguments.end(); i++)
			{
				result += (*i)->ToLog();
				if (i + 1 != arguments.end())
				{
					result += T(", ");
				}
			}
			result += T(")");
			return result;
		}

		string_t ListExpression::ToLog()
		{
			string_t result = T("(list: ");
			for (auto i = elements.begin(); i != elements.end(); i++)
			{
				result += (*i)->ToLog();
				if (i + 1 != elements.end())
				{
					result += T(", ");
				}
			}
			result += T(")");
			return result;
		}

		string_t UnaryExpression::ToLog()
		{
			string_t result;
			switch (op)
			{
			case UnaryOperator::Positive:
				result = T("+(");
				break;
			case UnaryOperator::Negative:
				result = T("-(");
				break;
			case UnaryOperator::Not:
				result = T("not(");
				break;
			}
			result += operand->ToLog() + T(")");
			return result;
		}

		string_t BinaryExpression::ToLog()
		{
			string_t result;
			switch (op)
			{
			case BinaryOperator::Concat:
				result += T("&(");
				break;
			case BinaryOperator::Add:
				result += T("+(");
				break;
			case BinaryOperator::Sub:
				result += T("-(");
				break;
			case BinaryOperator::Mul:
				result += T("*(");
				break;
			case BinaryOperator::Div:
				result += T("/(");
				break;
			case BinaryOperator::IntDiv:
				result += T("\\(");
				break;
			case BinaryOperator::Mod:
				result += T("%(");
				break;
			case BinaryOperator::LT:
				result += T("<(");
				break;
			case BinaryOperator::GT:
				result += T(">(");
				break;
			case BinaryOperator::LE:
				result += T("<=(");
				break;
			case BinaryOperator::GE:
				result += T(">=(");
				break;
			case BinaryOperator::EQ:
				result += T("=(");
				break;
			case BinaryOperator::NE:
				result += T("<>(");
				break;
			case BinaryOperator::And:
				result += T("and(");
				break;
			case BinaryOperator::Or:
				result += T("or(");
				break;
			}
			result += first->ToLog() + T(", ") + second->ToLog() + T(")");
			return result;
		}

		/*************************************************************
		Expression::ToCode
		*************************************************************/

		string_t LiteralExpression::ToCode()
		{
			if (token.type == CodeTokenType::String)
			{
				return T("\"") + CodeToken::EscapeString(token.value) + T("\"");
			}
			else
			{
				return token.value;
			}
		}

		string_t ArgumentExpression::ToCode()
		{
			string_t result = T("(") + name->GetName() + T(")");
			return result;
		}

		string_t ReferenceExpression::ToCode()
		{
			return T("(") + symbol->uniqueId + T(")");
		}

		string_t InvokeExpression::ToCode()
		{
			auto reference = dynamic_pointer_cast<ReferenceExpression>(function);
			string_t result = T("(");
			auto it = arguments.begin();
			for (auto i = reference->symbol->fragments.begin(); i != reference->symbol->fragments.end(); i++)
			{
				if ((*i)->type == GrammarFragmentType::Name)
				{
					result += (*i)->GetUniqueIdFragment();
				}
				else
				{
					result += (*it++)->ToCode();
				}
				if (i + 1 != reference->symbol->fragments.end())
				{
					result += T(" ");
				}
			}
			result += T(")");
			return result;
		}

		string_t ListExpression::ToCode()
		{
			string_t result = T("(");
			for (auto i = elements.begin(); i != elements.end(); i++)
			{
				result += (*i)->ToCode();
				if (i + 1 != elements.end())
				{
					result += T(", ");
				}
			}
			result += T(")");
			return result;
		}

		string_t UnaryExpression::ToCode()
		{
			string_t result;
			switch (op)
			{
			case UnaryOperator::Positive:
				result = T("(+");
				break;
			case UnaryOperator::Negative:
				result = T("(-");
				break;
			case UnaryOperator::Not:
				result = T("(not");
				break;
			}
			result += operand->ToCode() + T(")");
			return result;
		}

		string_t BinaryExpression::ToCode()
		{
			string_t result = T("(") + first->ToCode() + T(" ");
			switch (op)
			{
			case BinaryOperator::Concat:
				result += T("&");
				break;
			case BinaryOperator::Add:
				result += T("+");
				break;
			case BinaryOperator::Sub:
				result += T("-");
				break;
			case BinaryOperator::Mul:
				result += T("*");
				break;
			case BinaryOperator::Div:
				result += T("/");
				break;
			case BinaryOperator::IntDiv:
				result += T("\\");
				break;
			case BinaryOperator::Mod:
				result += T("%");
				break;
			case BinaryOperator::LT:
				result += T("<");
				break;
			case BinaryOperator::GT:
				result += T(">");
				break;
			case BinaryOperator::LE:
				result += T("<=");
				break;
			case BinaryOperator::GE:
				result += T(">=");
				break;
			case BinaryOperator::EQ:
				result += T("=");
				break;
			case BinaryOperator::NE:
				result += T("<>");
				break;
			case BinaryOperator::And:
				result += T("and");
				break;
			case BinaryOperator::Or:
				result += T("or");
				break;
			}
			result += T(" ") + second->ToCode() + T(")");
			return result;
		}

		/*************************************************************
		Expression::CollectNewAssignables
		*************************************************************/

		void LiteralExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments, Expression::List& modifiedAssignables)
		{
		}

		void ArgumentExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments, Expression::List& modifiedAssignables)
		{
		}

		void ReferenceExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments, Expression::List& modifiedAssignables)
		{
		}

		void InvokeExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments, Expression::List& modifiedAssignables)
		{
			if (auto reference = dynamic_pointer_cast<ReferenceExpression>(function))
			{
				auto it = arguments.begin();
				for (auto fragment : reference->symbol->fragments)
				{
					if (fragment->type == GrammarFragmentType::Assignable)
					{
						if (auto argument = dynamic_pointer_cast<ArgumentExpression>(*it))
						{
							newAssignables.push_back(*it);
						}
						else if (auto ref = dynamic_pointer_cast<ReferenceExpression>(*it))
						{
							modifiedAssignables.push_back(*it);
						}
					}
					else if (fragment->type == GrammarFragmentType::Argument)
					{
						if (auto argument = dynamic_pointer_cast<ArgumentExpression>(*it))
						{
							newArguments.push_back(*it);
						}
					}
					if (fragment->type != GrammarFragmentType::Name)
					{
						it++;
					}
				}
			}
			else
			{
				function->CollectNewAssignable(newAssignables, newArguments, modifiedAssignables);
				for (auto argument : arguments)
				{
					argument->CollectNewAssignable(newAssignables, newArguments, modifiedAssignables);
				}
			}
		}

		void ListExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments, Expression::List& modifiedAssignables)
		{
			for (auto element : elements)
			{
				element->CollectNewAssignable(newAssignables, newArguments, modifiedAssignables);
			}
		}

		void UnaryExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments, Expression::List& modifiedAssignables)
		{
			operand->CollectNewAssignable(newAssignables, newArguments, modifiedAssignables);
		}

		void BinaryExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments, Expression::List& modifiedAssignables)
		{
			first->CollectNewAssignable(newAssignables, newArguments, modifiedAssignables);
			second->CollectNewAssignable(newAssignables, newArguments, modifiedAssignables);
		}

		/*************************************************************
		GrammarStack
		*************************************************************/

		void GrammarStack::Push(GrammarStackItem::Ptr stackItem)
		{
			for (auto symbol : stackItem->symbols)
			{
				symbol->CalculateUniqueId();
			}

			stackItems.push_back(stackItem);
			for (auto symbol : stackItem->symbols)
			{
				availableSymbols.insert(make_pair(symbol->uniqueId, symbol));
				if (symbol->target == GrammarSymbolTarget::TheResult)
				{
					resultSymbol = symbol;
				}
			}
		}

		GrammarStackItem::Ptr GrammarStack::Pop()
		{
			auto stackItem = stackItems.back();
			stackItems.pop_back();

			for (auto symbol : stackItem->symbols)
			{
				auto begin = availableSymbols.lower_bound(symbol->uniqueId);
				auto end = availableSymbols.upper_bound(symbol->uniqueId);
				auto it = find_if(begin, end, [symbol](pair<string_t, GrammarSymbol::Ptr> x){return x.second == symbol; });
				while (it != availableSymbols.end())
				{
					if (it->second == symbol) break;
					it++;
				}
				availableSymbols.erase(it);
			}

			return stackItem;
		}

		CodeError GrammarStack::SuccessError()
		{
			return CodeError();
		}

		CodeError GrammarStack::ParseToken(const string_t& token, Iterator input, Iterator end, vector<Iterator>& result)
		{
			if (input == end)
			{
				auto token = *(input - 1);
				CodeError error =
				{
					token,
					T("Unexpected end of line."),
				};
				return error;
			}
			else if (input->value != token)
			{
				CodeError error =
				{
					*input,
					T("\"") + token + T("\" expected but \"") + input->value + T("\" found."),
				};
				return error;
			}
			else
			{
				result.push_back(++input);
				return CodeError();
			}
		}

		CodeError GrammarStack::FoldError(CodeError error1, CodeError error2)
		{
			return error1.position.column > error2.position.column ? error1 : error2;
		}

		CodeError GrammarStack::ParseGrammarFragment(GrammarFragment::Ptr fragment, Iterator input, Iterator end, ResultList& result)
		{
			switch (fragment->type)
			{
			case GrammarFragmentType::Type:
				return ParseType(input, end, result);
			case GrammarFragmentType::Primitive:
				return ParseShortPrimitive(input, end, result);
			case GrammarFragmentType::Expression:
				return ParseExpression(input, end, result);
			case GrammarFragmentType::List:
				return ParseList(input, end, result);
			case GrammarFragmentType::Assignable:
				return ParseAssignable(input, end, result);
			case GrammarFragmentType::Argument:
				return ParseArgument(input, end, result);
			}

			vector<Iterator> nameResult;
			for (auto id : fragment->identifiers)
			{
				auto error = ParseToken(id, input, end, nameResult);
				if (nameResult.size() == 0)
				{
					return error;
				}
				else
				{
					input = nameResult[0];
					nameResult.clear();
				}
			}

			result.push_back(make_pair(input, Expression::Ptr(nullptr)));
			return SuccessError();
		}

		CodeError GrammarStack::ParseGrammarSymbolStep(GrammarSymbol::Ptr symbol, int fragmentIndex, ExpressionLink::Ptr previousExpression, Iterator input, Iterator end, vector<pair<Iterator, ExpressionLink::Ptr>>& result)
		{
			auto fragment = symbol->fragments[fragmentIndex];
			vector<pair<Iterator, Expression::Ptr>> fragmentResult;
			auto error = ParseGrammarFragment(fragment, input, end, fragmentResult);

			for (auto fr : fragmentResult)
			{
				if (fr.second)
					{
					auto link = make_shared<ExpressionLink>();
					link->expression = fr.second;
					link->previous = previousExpression;
					result.push_back(make_pair(fr.first, link));
				}
				else
				{
					result.push_back(make_pair(fr.first, previousExpression));
				}
			}
			return error;
		}

		CodeError GrammarStack::ParseGrammarSymbol(GrammarSymbol::Ptr symbol, int beginFragment, ExpressionLink::Ptr previousExpression, Iterator input, Iterator end, ResultList& result)
		{
			vector<pair<Iterator, ExpressionLink::Ptr>> stepResult;
			stepResult.push_back(make_pair(input, previousExpression));

			int stepResultBegin = 0;
			int stepResultEnd = stepResult.size();
			CodeError resultError;
			for (int i = beginFragment; (size_t)i < symbol->fragments.size(); i++)
			{
				for (int j = stepResultBegin; j < stepResultEnd; j++)
				{
					auto it = stepResult[j].first;
					auto expr = stepResult[j].second;
					auto error = ParseGrammarSymbolStep(symbol, i, expr, it, end, stepResult);
					resultError = FoldError(resultError, error);
				}

				stepResultBegin = stepResultEnd;
				stepResultEnd = stepResult.size();
			}

			for (int j = stepResultBegin; j < stepResultEnd; j++)
			{
				auto it = stepResult[j].first;
				auto expr = stepResult[j].second;
				Expression::List arguments;
				while (expr)
				{
					arguments.insert(arguments.begin(), expr->expression);
					expr = expr->previous;
				}

				bool invoke = arguments.size() > 0
					|| symbol->type == GrammarSymbolType::Phrase
					|| symbol->type == GrammarSymbolType::Sentence
					|| symbol->type == GrammarSymbolType::Block
					;

				Expression::Ptr resultExpr;
				if (invoke)
				{
					auto reference = make_shared<ReferenceExpression>();
					reference->symbol = symbol;

					auto invoke = make_shared<InvokeExpression>();
					invoke->function = reference;
					invoke->arguments = arguments;

					resultExpr = invoke;
				}
				else
				{
					auto reference = make_shared<ReferenceExpression>();
					reference->symbol = symbol;

					resultExpr = reference;
				}
				result.push_back(make_pair(it, resultExpr));
			}
			return resultError;
		}

		CodeError GrammarStack::ParseGrammarSymbol(GrammarSymbol::Ptr symbol, Iterator input, Iterator end, ResultList& result)
		{
			return ParseGrammarSymbol(symbol, 0, nullptr, input, end, result);
		}

		CodeError GrammarStack::ParseType(Iterator input, Iterator end, ResultList& result)
		{
			CodeError resultError;
			auto it = availableSymbols.begin();
			while (it != availableSymbols.end())
			{
				it = availableSymbols.upper_bound(it->first);
				it--;
				if (it->second->type == GrammarSymbolType::Type)
				{
					auto error = ParseGrammarSymbol(it->second, input, end, result);
					resultError = FoldError(resultError, error);
				}
				it++;
			}
			return resultError;
		}

		CodeError GrammarStack::ParseShortPrimitive(Iterator input, Iterator end, ResultList& result)
		{
			if (input == end)
			{
				auto token = *(input - 1);
				CodeError error =
				{
					token,
					T("Unexpected end of line."),
				};
				return error;
			}

			int resultBegin = result.size();
			switch (input->type)
			{
			case CodeTokenType::Integer:
			case CodeTokenType::Float:
			case CodeTokenType::String:
				{
					auto literal = make_shared<LiteralExpression>();
					literal->token = *input++;
					result.push_back(make_pair(input, static_pointer_cast<Expression>(literal)));
					return CodeError();
				}
			case CodeTokenType::Add:
			case CodeTokenType::Sub:
			case CodeTokenType::Not:
				{
					auto unaryType = input->type;
					ResultList primitiveResult;
					auto resultError = ParseShortPrimitive(++input, end, primitiveResult);
					for (auto pr : primitiveResult)
					{
						auto unary = make_shared<UnaryExpression>();
						unary->operand = pr.second;
						switch (unaryType)
						{
						case CodeTokenType::Add:
							unary->op = UnaryOperator::Positive;
							break;
						case CodeTokenType::Sub:
							unary->op = UnaryOperator::Negative;
							break;
						case CodeTokenType::Not:
							unary->op = UnaryOperator::Not;
							break;
						}

						result.push_back(make_pair(pr.first, static_pointer_cast<Expression>(unary)));
					}
					return resultError;
				}
			}

			{
				vector<Iterator> tokenResult;
				auto resultError = ParseToken(T("("), input, end, tokenResult);
				if (tokenResult.size() > 0)
				{
					ResultList expressionResult;
					auto error = ParseExpression(tokenResult[0], end, expressionResult);
					resultError = FoldError(resultError, error);

					for (auto er : expressionResult)
					{
						tokenResult.clear();
						error = ParseToken(T(")"), er.first, end, tokenResult);
						resultError = FoldError(resultError, error);
						if (tokenResult.size() > 0)
						{
							result.push_back(make_pair(tokenResult[0], er.second));
						}
					}

					return resultError;
				}
			}

			CodeError resultError;
			auto it = availableSymbols.begin();
			while (it != availableSymbols.end())
			{
				it = availableSymbols.upper_bound(it->first);
				it--;
				if (it->second->type == GrammarSymbolType::Symbol || it->second->type == GrammarSymbolType::Phrase)
				{
					auto symbol = it->second;
					switch (symbol->fragments[0]->type)
					{
					case GrammarFragmentType::Primitive:
					case GrammarFragmentType::Expression:
						break;
					default:
						auto error = ParseGrammarSymbol(symbol, input, end, result);
						resultError = FoldError(resultError, error);
					}
				}
				it++;
			}
			return resultError;
		}

		CodeError GrammarStack::ParsePrimitive(Iterator input, Iterator end, ResultList& result)
		{
			int resultBegin = result.size();
			auto resultError = ParseShortPrimitive(input, end, result);
			int resultEnd = result.size();

			while (resultBegin < resultEnd)
			{
				for (int i = resultBegin; i < resultEnd; i++)
				{
					if (result[i].first != end)
					{
						auto it = availableSymbols.begin();
						while (it != availableSymbols.end())
						{
							it = availableSymbols.upper_bound(it->first);
							it--;
							if (it->second->type == GrammarSymbolType::Phrase)
							{
								auto symbol = it->second;
								switch (symbol->fragments[0]->type)
								{
								case GrammarFragmentType::Primitive:
									{
										auto link = make_shared<ExpressionLink>();
										link->expression = result[i].second;
										auto error = ParseGrammarSymbol(symbol, 1, link, result[i].first, end, result);
										resultError = FoldError(resultError, error);
									}
									break;
								}
							}
							it++;
						}
					}
				}

				resultBegin = resultEnd;
				resultEnd = result.size();
			}

			return resultError;
		}

		CodeError GrammarStack::ParseList(Iterator input, Iterator end, ResultList& result)
		{
			vector<Iterator> tokenResult;
			auto resultError = ParseToken(T("("), input, end, tokenResult);
			if (tokenResult.size() == 0) return resultError;
			input = tokenResult[0];

			tokenResult.clear();
			{
				auto error = ParseToken(T(")"), input, end, tokenResult);
				resultError = FoldError(resultError, error);
				if (tokenResult.size() == 1)
				{
					result.push_back(make_pair(tokenResult[0], make_shared<ListExpression>()));
					return resultError;
				}
			}
		
			vector<pair<Iterator, ExpressionLink::Ptr>> linkResult, linkFinalResult;
			linkResult.push_back(make_pair(input, ExpressionLink::Ptr(nullptr)));
			int linkResultBegin = 0;
			int linkResultEnd = linkResult.size();

			while (linkResultBegin < linkResultEnd)
			{
				for (int i = linkResultBegin; i < linkResultEnd; i++)
				{
					auto it = linkResult[i].first;
					auto previousExpression = linkResult[i].second;
					ResultList expressionResult;
					ParseExpression(it, end, expressionResult);

					for (auto er : expressionResult)
					{
						auto link = make_shared<ExpressionLink>();
						link->expression = er.second;
						link->previous = previousExpression;

						tokenResult.clear();
						auto error = ParseToken(T(","), er.first, end, tokenResult);
						resultError = FoldError(resultError, error);
						if (tokenResult.size() != 0)
						{
							linkResult.push_back(make_pair(tokenResult[0], link));
						}
						else
						{
							tokenResult.clear();
							error = ParseToken(T(")"), er.first, end, tokenResult);
							resultError = FoldError(resultError, error);
							if (tokenResult.size() != 0)
							{
								linkFinalResult.push_back(make_pair(tokenResult[0], link));
							}
						}
					}
				}
				linkResultBegin = linkResultEnd;
				linkResultEnd = linkResult.size();
			}

			for (auto lfr : linkFinalResult)
			{
				auto it = lfr.first;
				auto expr = lfr.second;
				Expression::List elements;
				while (expr)
				{
					elements.insert(elements.begin(), expr->expression);
					expr = expr->previous;
				}

				auto list = make_shared<ListExpression>();
				list->elements = elements;
				result.push_back(make_pair(it, static_pointer_cast<Expression>(list)));
			}
			return resultError;
		}

		CodeError GrammarStack::ParseAssignable(Iterator input, Iterator end, ResultList& result)
		{
			int resultBegin = result.size();

			CodeError resultError;
			auto it = availableSymbols.begin();
			while (it != availableSymbols.end())
			{
				it = availableSymbols.upper_bound(it->first);
				it--;
				if (it->second->type == GrammarSymbolType::Symbol)
				{
					auto error = ParseGrammarSymbol(it->second, input, end, result);
					resultError = FoldError(resultError, error);
				}
				it++;
			}

			int resultEnd = result.size();
			set<int> symbolSizes;
			for (int i = resultBegin; i < resultEnd; i++)
			{
				if (auto reference = dynamic_pointer_cast<ReferenceExpression>(result[i].second))
				{
					symbolSizes.insert(reference->symbol->fragments[0]->identifiers.size());
				}
			}

			auto error = ParseArgument(input, end, result);
			for (int i = result.size() - 1; i >= resultEnd; i--)
			{
				if (auto argument = dynamic_pointer_cast<ArgumentExpression>(result[i].second))
				{
					if (symbolSizes.find(argument->name->identifiers.size()) != symbolSizes.end())
					{
						result.erase(result.begin() + i);
					}
				}
			}

			resultError = FoldError(resultError, error);
			return resultError;
		}

		CodeError GrammarStack::ParseArgument(Iterator input, Iterator end, ResultList& result)
		{
			if (input == end)
			{
				auto token = *(input - 1);
				CodeError error =
				{
					token,
					T("Unexpected end of line."),
				};
				return error;
			}

			CodeToken::List tokens;
			while (input != end && input->IsNameFragmentToken())
			{
				tokens.push_back(*input++);

				auto argument = make_shared<ArgumentExpression>();
				argument->name = make_shared<SymbolName>();
				argument->name->identifiers = tokens;
				result.push_back(make_pair(input, static_pointer_cast<Expression>(argument)));
			}

			return CodeError();
		}

		CodeError GrammarStack::ParseBinary(Iterator input, Iterator end, ParseFunctionType parser, CodeTokenType* tokenTypes, BinaryOperator* binaryOperators, int count, ResultList& result)
		{
			int resultBegin = result.size();
			auto resultError = (this->*parser)(input, end, result);
			int resultEnd = result.size();

			while (resultBegin < resultEnd)
			{
				for (int i = resultBegin; i < resultEnd; i++)
				{
					auto it = result[i].first;
					if (it != end)
					{
						for (int j = 0; j < count; j++)
						{
							if (it->type == tokenTypes[j])
							{
								ResultList expressionResult;
								auto error = (this->*parser)(++it, end, expressionResult);
								resultError = FoldError(resultError, error);
								for (auto er : expressionResult)
								{
									auto binary = make_shared<BinaryExpression>();
									binary->first = result[i].second;
									binary->second = er.second;
									binary->op = binaryOperators[j];
									result.push_back(make_pair(er.first, static_pointer_cast<Expression>(binary)));
								}
								break;
							}
						}
					}
				}
				resultBegin = resultEnd;
				resultEnd = result.size();
			}

			return resultError;
		}

		CodeError GrammarStack::ParseExp1(Iterator input, Iterator end, ResultList& result)
		{
			CodeTokenType tokenTypes[] = { CodeTokenType::Mul, CodeTokenType::Div, CodeTokenType::IntDiv, CodeTokenType::Mod };
			BinaryOperator binaryOperators[] = { BinaryOperator::Mul, BinaryOperator::Div, BinaryOperator::IntDiv, BinaryOperator::Mod };
			int count = sizeof(tokenTypes) / sizeof(*tokenTypes);
			return ParseBinary(input, end, &GrammarStack::ParsePrimitive, tokenTypes, binaryOperators, count, result);
		}

		CodeError GrammarStack::ParseExp2(Iterator input, Iterator end, ResultList& result)
		{
			CodeTokenType tokenTypes[] = { CodeTokenType::Add, CodeTokenType::Sub };
			BinaryOperator binaryOperators[] = { BinaryOperator::Add, BinaryOperator::Sub };
			int count = sizeof(tokenTypes) / sizeof(*tokenTypes);
			return ParseBinary(input, end, &GrammarStack::ParseExp1, tokenTypes, binaryOperators, count, result);
		}

		CodeError GrammarStack::ParseExp3(Iterator input, Iterator end, ResultList& result)
		{
			CodeTokenType tokenTypes[] = { CodeTokenType::Concat };
			BinaryOperator binaryOperators[] = { BinaryOperator::Concat };
			int count = sizeof(tokenTypes) / sizeof(*tokenTypes);
			return ParseBinary(input, end, &GrammarStack::ParseExp2, tokenTypes, binaryOperators, count, result);
		}

		CodeError GrammarStack::ParseExp4(Iterator input, Iterator end, ResultList& result)
		{
			CodeTokenType tokenTypes[] = { CodeTokenType::LT, CodeTokenType::GT, CodeTokenType::LE, CodeTokenType::GE, CodeTokenType::EQ, CodeTokenType::NE };
			BinaryOperator binaryOperators[] = { BinaryOperator::LT, BinaryOperator::GT, BinaryOperator::LE, BinaryOperator::GE, BinaryOperator::EQ, BinaryOperator::NE };
			int count = sizeof(tokenTypes) / sizeof(*tokenTypes);
			return ParseBinary(input, end, &GrammarStack::ParseExp3, tokenTypes, binaryOperators, count, result);
		}

		CodeError GrammarStack::ParseExp5(Iterator input, Iterator end, ResultList& result)
		{
			CodeTokenType tokenTypes[] = { CodeTokenType::And };
			BinaryOperator binaryOperators[] = { BinaryOperator::And };
			int count = sizeof(tokenTypes) / sizeof(*tokenTypes);
			return ParseBinary(input, end, &GrammarStack::ParseExp4, tokenTypes, binaryOperators, count, result);
		}

		CodeError GrammarStack::ParseExpression(Iterator input, Iterator end, ResultList& result)
		{
			CodeTokenType tokenTypes[] = { CodeTokenType::Or };
			BinaryOperator binaryOperators[] = { BinaryOperator::Or };
			int count = sizeof(tokenTypes) / sizeof(*tokenTypes);
			return ParseBinary(input, end, &GrammarStack::ParseExp5, tokenTypes, binaryOperators, count, result);
		}

		CodeError GrammarStack::ParseStatement(Iterator input, Iterator end, ResultList& result)
		{
			CodeError resultError;
			ResultList expressionResult;
			auto it = availableSymbols.begin();
			while (it != availableSymbols.end())
			{
				it = availableSymbols.upper_bound(it->first);
				it--;
				if (it->second->type == GrammarSymbolType::Sentence || it->second->type == GrammarSymbolType::Block)
				{
					auto symbol = it->second;
					switch (symbol->fragments[0]->type)
					{
					case GrammarFragmentType::Primitive:
					case GrammarFragmentType::Expression:
						break;
					default:
						auto error = ParseGrammarSymbol(symbol, input, end, expressionResult);
						resultError = FoldError(resultError, error);
					}
				}
				it++;
			}

			for (auto er : expressionResult)
			{
				if (er.first == end)
				{
					result.push_back(er);
				}
			}
			return resultError;
		}

		int GrammarStack::CountStatementAssignables(Expression::List& assignables)
		{
			Expression::Ptr illegalConvertedAssignable;
			return CountStatementAssignables(assignables, illegalConvertedAssignable);
		}

		int	 GrammarStack::CountStatementAssignables(Expression::List& assignables, Expression::Ptr& illegalConvertedAssignable)
		{
			for (auto assignable : assignables)
			{
				auto argument = dynamic_pointer_cast<ArgumentExpression>(assignable);

				ResultList result;
				ParseExpression(argument->name->identifiers.begin(), argument->name->identifiers.end(), result);
				for (auto r : result)
				{
					if (r.first == argument->name->identifiers.end())
					{
						illegalConvertedAssignable = r.second;
						return -1;
					}
				}
			}

			return assignables.size();
		}
	}
}