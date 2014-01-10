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

		string GrammarFragment::GetUniqueIdFragment()
		{
			switch (type)
			{
			case GrammarFragmentType::Type:
				return "<type>";
			case GrammarFragmentType::Primitive:
				return "<primitive>";
			case GrammarFragmentType::Expression:
				return "<expression>";
			case GrammarFragmentType::List:
				return "<list>";
			case GrammarFragmentType::Assignable:
				return "<assignable>";
			case GrammarFragmentType::Argument:
				return "<argument>";
				break;
			}
		
			string uniqueId;
			for (auto i = identifiers.begin(); i != identifiers.end(); i++)
			{
				uniqueId += *i;
				if (i + 1 != identifiers.end())
				{
					uniqueId += " ";
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
			uniqueId = "";
			for (auto i = fragments.begin(); i != fragments.end(); i++)
			{
				uniqueId += (*i)->GetUniqueIdFragment();
				if (i + 1 != fragments.end())
				{
					uniqueId += " ";
				}
			}
		}

		GrammarSymbol::Ptr operator+(GrammarSymbol::Ptr symbol, const string& name)
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
				+ "object"
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Array)
				+ "array"
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Symbol)
				+ "symbol"
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Boolean)
				+ "boolean"
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Integer)
				+ "integer"
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Float)
				+ "float"
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::String)
				+ "string"
				);
		
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Symbol, GrammarSymbolTarget::True)
				+ "true"
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Symbol, GrammarSymbolTarget::False)
				+ "false"
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Symbol, GrammarSymbolTarget::Null)
				+ "null"
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Symbol, GrammarSymbolTarget::TheResult)
				+ "the" + "result"
				);

			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::NewType)
				+ "new" + GrammarFragmentType::Type
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::NewTypeOfFields)
				+ "new" + GrammarFragmentType::Type + "of" + GrammarFragmentType::List
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::NewArray)
				+ "new" + "array" + "of" + GrammarFragmentType::Expression + "items"
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::GetArrayItem)
				+ "item" + GrammarFragmentType::Expression + "of" + "array" + GrammarFragmentType::Primitive
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::GetArrayLength)
				+ "length" + "of" + "array" + GrammarFragmentType::Primitive
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::Invoke)
				+ "invoke" + GrammarFragmentType::Primitive
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::InvokeWith)
				+ "invoke" + GrammarFragmentType::Expression + "with" + GrammarFragmentType::List
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::IsType)
				+ GrammarFragmentType::Primitive + "is" + GrammarFragmentType::Type
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::IsNotType)
				+ GrammarFragmentType::Primitive + "is" + "not" + GrammarFragmentType::Type
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::GetField)
				+ "field" + GrammarFragmentType::Argument + "of" + GrammarFragmentType::Primitive
				);

			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::End)
				+ "end"
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Exit)
				+ "exit"
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Block, GrammarSymbolTarget::Select)
				+ "select" + GrammarFragmentType::Expression
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Case)
				+ "case" + GrammarFragmentType::Expression
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Call)
				+ "call" + GrammarFragmentType::Expression
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::CallContinuation)
				+ "call" + "continuation" + GrammarFragmentType::Expression + "with" + GrammarFragmentType::List
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::RedirectTo)
				+ "redirect" + "to" + GrammarFragmentType::Expression
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Assign)
				+ "set" + GrammarFragmentType::Assignable + "to" + GrammarFragmentType::Expression
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::SetArrayItem)
				+ "set" + "item" + GrammarFragmentType::Expression + "of" + "array" + GrammarFragmentType::Expression + "to" + GrammarFragmentType::Expression
				);
			symbols.push_back(
				make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::SetField)
				+ "set" + "field" + GrammarFragmentType::Argument + "of" + GrammarFragmentType::Expression + "to" + GrammarFragmentType::Expression
				);
		}

		/*************************************************************
		Expression::ToLog
		*************************************************************/

		string LiteralExpression::ToLog()
		{
			if (token.type == CodeTokenType::String)
			{
				return "\"" + CodeToken::EscapeString(token.value) + "\"";
			}
			else
			{
				return token.value;
			}
		}

		string ArgumentExpression::ToLog()
		{
			string result = "(argument: ";
			for (auto i = tokens.begin(); i != tokens.end(); i++)
			{
				result += i->value;
				if (i + 1 != tokens.end())
				{
					result += " ";
				}
			}
			result += ")";
			return result;
		}

		string ReferenceExpression::ToLog()
		{
			return symbol->uniqueId;
		}

		string InvokeExpression::ToLog()
		{
			string result = function->ToLog() + "(";
			for (auto i = arguments.begin(); i != arguments.end(); i++)
			{
				result += (*i)->ToLog();
				if (i + 1 != arguments.end())
				{
					result += ", ";
				}
			}
			result += ")";
			return result;
		}

		string ListExpression::ToLog()
		{
			string result = "(list: ";
			for (auto i = elements.begin(); i != elements.end(); i++)
			{
				result += (*i)->ToLog();
				if (i + 1 != elements.end())
				{
					result += ", ";
				}
			}
			result += ")";
			return result;
		}

		string UnaryExpression::ToLog()
		{
			string result;
			switch (op)
			{
			case UnaryOperator::Positive:
				result = "+(";
				break;
			case UnaryOperator::Negative:
				result = "-(";
				break;
			case UnaryOperator::Not:
				result = "not(";
				break;
			}
			result += operand->ToLog() + ")";
			return result;
		}

		string BinaryExpression::ToLog()
		{
			string result;
			switch (op)
			{
			case BinaryOperator::Concat:
				result += "&(";
				break;
			case BinaryOperator::Add:
				result += "+(";
				break;
			case BinaryOperator::Sub:
				result += "-(";
				break;
			case BinaryOperator::Mul:
				result += "*(";
				break;
			case BinaryOperator::Div:
				result += "/(";
				break;
			case BinaryOperator::IntDiv:
				result += "\\(";
				break;
			case BinaryOperator::Mod:
				result += "%(";
				break;
			case BinaryOperator::LT:
				result += "<(";
				break;
			case BinaryOperator::GT:
				result += ">(";
				break;
			case BinaryOperator::LE:
				result += "<=(";
				break;
			case BinaryOperator::GE:
				result += ">=(";
				break;
			case BinaryOperator::EQ:
				result += "=(";
				break;
			case BinaryOperator::NE:
				result += "<>(";
				break;
			case BinaryOperator::And:
				result += "and(";
				break;
			case BinaryOperator::Or:
				result += "or(";
				break;
			}
			result += first->ToLog() + ", " + second->ToLog() + ")";
			return result;
		}

		/*************************************************************
		Expression::ToCode
		*************************************************************/

		string LiteralExpression::ToCode()
		{
			if (token.type == CodeTokenType::String)
			{
				return "\"" + CodeToken::EscapeString(token.value) + "\"";
			}
			else
			{
				return token.value;
			}
		}

		string ArgumentExpression::ToCode()
		{
			string result = "(";
			for (auto i = tokens.begin(); i != tokens.end(); i++)
			{
				result += i->value;
				if (i + 1 != tokens.end())
				{
					result += " ";
				}
			}
			result += ")";
			return result;
		}

		string ReferenceExpression::ToCode()
		{
			return "(" + symbol->uniqueId + ")";
		}

		string InvokeExpression::ToCode()
		{
			auto reference = dynamic_pointer_cast<ReferenceExpression>(function);
			switch (reference->symbol->type)
			{
			case GrammarSymbolType::Phrase:
			case GrammarSymbolType::Sentence:
			case GrammarSymbolType::Block:
				{
					string result = "(";
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
							result += " ";
						}
					}
					result += ")";
					return result;
				}
			}

			string result = "(invoke " + function->ToCode() + " with (";
			for (auto i = arguments.begin(); i != arguments.end(); i++)
			{
				result += (*i)->ToCode();
				if (i + 1 != arguments.end())
				{
					result += ", ";
				}
			}
			result += "))";
			return result;
		}

		string ListExpression::ToCode()
		{
			string result = "(";
			for (auto i = elements.begin(); i != elements.end(); i++)
			{
				result += (*i)->ToCode();
				if (i + 1 != elements.end())
				{
					result += ", ";
				}
			}
			result += ")";
			return result;
		}

		string UnaryExpression::ToCode()
		{
			string result;
			switch (op)
			{
			case UnaryOperator::Positive:
				result = "(+";
				break;
			case UnaryOperator::Negative:
				result = "(-";
				break;
			case UnaryOperator::Not:
				result = "(not";
				break;
			}
			result += operand->ToCode() + ")";
			return result;
		}

		string BinaryExpression::ToCode()
		{
			string result = "(" + first->ToCode() + " ";
			switch (op)
			{
			case BinaryOperator::Concat:
				result += "&";
				break;
			case BinaryOperator::Add:
				result += "+";
				break;
			case BinaryOperator::Sub:
				result += "-";
				break;
			case BinaryOperator::Mul:
				result += "*";
				break;
			case BinaryOperator::Div:
				result += "/";
				break;
			case BinaryOperator::IntDiv:
				result += "\\";
				break;
			case BinaryOperator::Mod:
				result += "%";
				break;
			case BinaryOperator::LT:
				result += "<";
				break;
			case BinaryOperator::GT:
				result += ">";
				break;
			case BinaryOperator::LE:
				result += "<=";
				break;
			case BinaryOperator::GE:
				result += ">=";
				break;
			case BinaryOperator::EQ:
				result += "=";
				break;
			case BinaryOperator::NE:
				result += "<>";
				break;
			case BinaryOperator::And:
				result += "and";
				break;
			case BinaryOperator::Or:
				result += "or";
				break;
			}
			result += " " + second->ToCode() + ")";
			return result;
		}

		/*************************************************************
		Expression::CollectNewAssignables
		*************************************************************/

		void LiteralExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments)
		{
		}

		void ArgumentExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments)
		{
		}

		void ReferenceExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments)
		{
		}

		void InvokeExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments)
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
				function->CollectNewAssignable(newAssignables, newArguments);
				for (auto argument : arguments)
				{
					argument->CollectNewAssignable(newAssignables, newArguments);
				}
			}
		}

		void ListExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments)
		{
			for (auto element : elements)
			{
				element->CollectNewAssignable(newAssignables, newArguments);
			}
		}

		void UnaryExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments)
		{
			operand->CollectNewAssignable(newAssignables, newArguments);
		}

		void BinaryExpression::CollectNewAssignable(Expression::List& newAssignables, Expression::List& newArguments)
		{
			first->CollectNewAssignable(newAssignables, newArguments);
			second->CollectNewAssignable(newAssignables, newArguments);
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
				auto it = find_if(begin, end, [symbol](pair<string, GrammarSymbol::Ptr> x){return x.second == symbol; });
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

		CodeError GrammarStack::ParseToken(const string& token, Iterator input, Iterator end, vector<Iterator>& result)
		{
			if (input == end)
			{
				auto token = *(input - 1);
				CodeError error =
				{
					token,
					"Unexpected end of line.",
				};
				return error;
			}
			else if (input->value != token)
			{
				CodeError error =
				{
					*input,
					"\"" + token + "\" expected but \"" + input->value + "\" found.",
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
					"Unexpected end of line.",
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
				auto resultError = ParseToken("(", input, end, tokenResult);
				if (tokenResult.size() > 0)
				{
					ResultList expressionResult;
					auto error = ParseExpression(tokenResult[0], end, expressionResult);
					resultError = FoldError(resultError, error);

					for (auto er : expressionResult)
					{
						tokenResult.clear();
						error = ParseToken(")", er.first, end, tokenResult);
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
			auto resultError = ParseToken("(", input, end, tokenResult);
			if (tokenResult.size() == 0) return resultError;
			input = tokenResult[0];
		
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
						auto error = ParseToken(",", er.first, end, tokenResult);
						resultError = FoldError(resultError, error);
						if (tokenResult.size() != 0)
						{
							linkResult.push_back(make_pair(tokenResult[0], link));
						}
						else
						{
							tokenResult.clear();
							error = ParseToken(")", er.first, end, tokenResult);
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
					if (symbolSizes.find(argument->tokens.size()) != symbolSizes.end())
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
					"Unexpected end of line.",
				};
				return error;
			}

			CodeToken::List tokens;
			while (input != end)
			{
				tokens.push_back(*input++);

				auto argument = make_shared<ArgumentExpression>();
				argument->tokens = tokens;
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
				ParseExpression(argument->tokens.begin(), argument->tokens.end(), result);
				for (auto r : result)
				{
					if (r.first == argument->tokens.end())
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