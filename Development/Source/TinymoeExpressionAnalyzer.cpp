#include "TinymoeExpressionAnalyzer.h"

namespace tinymoe
{
	/*************************************************************
	GrammarFragment
	*************************************************************/

	GrammarFragment::GrammarFragment(GrammarFragmentType _type)
		:type(_type)
	{
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
		for (auto fragment : fragments)
		{
			switch (fragment->type)
			{
			case GrammarFragmentType::Type:
				uniqueId += "<type> ";
				break;
			case GrammarFragmentType::Primitive:
				uniqueId += "<primitive> ";
				break;
			case GrammarFragmentType::Expression:
				uniqueId += "<expression> ";
				break;
			case GrammarFragmentType::List:
				uniqueId += "<list> ";
				break;
			case GrammarFragmentType::Assignable:
				uniqueId += "<assignable> ";
				break;
			case GrammarFragmentType::Argument:
				uniqueId += "<argument> ";
				break;
			case GrammarFragmentType::Name:
				for (auto name : fragment->identifiers)
				{
					uniqueId += name + " ";
				}
				break;
			}
		}
	}

	/*************************************************************
	GrammarStackItem
	*************************************************************/

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

	void GrammarStackItem::FillPredefinedSymbols()
	{
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Array)
			+ "array"
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::String)
			+ "string"
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
			make_shared<GrammarSymbol>(GrammarSymbolType::Type, GrammarSymbolTarget::Symbol)
			+ "symbol"
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
			make_shared<GrammarSymbol>(GrammarSymbolType::Phrase, GrammarSymbolTarget::NewType)
			+ "new" + GrammarFragmentType::Type
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
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Select)
			+ "select" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::Case)
			+ "case" + GrammarFragmentType::Expression
			);
		symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence, GrammarSymbolTarget::TailCall)
			+ "tail" + "call" + GrammarFragmentType::Expression
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
		string result = "<argument>(";
		for (auto token : tokens)
		{
			result += token.value + " ";
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
		for (auto argument : arguments)
		{
			result += argument->ToLog();
			if (argument != arguments.back())
			{
				result += ", ";
			}
		}
		result += ")";
		return result;
	}

	string ListExpression::ToLog()
	{
		string result = "(";
		for (auto element : elements)
		{
			result += element->ToLog();
			if (element != elements.back())
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
			result = "+ (";
			break;
		case UnaryOperator::Negative:
			result = "- (";
			break;
		case UnaryOperator::Not:
			result = "not (";
			break;
		}
		result += operand->ToLog() + ")";
		return result;
	}

	string BinaryExpression::ToLog()
	{
		string result = "(" + first->ToLog() + " ";
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
		result += " " + second->ToLog() + ")";
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
		for (auto token : tokens)
		{
			result += token.value + " ";
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
				string result;
				auto it = arguments.begin();
				for (auto fragment : reference->symbol->fragments)
				{
					if (fragment->type == GrammarFragmentType::Name)
					{
						for (auto id : fragment->identifiers)
						{
							result += id + " ";
						}
					}
					else
					{
						result += (*it++)->ToCode() + " ";
					}
				}
				return result;
			}
		}

		string result = "invoke " + function->ToCode() + " with (";
		for (auto argument : arguments)
		{
			result += argument->ToCode();
			if (argument != arguments.back())
			{
				result += ", ";
			}
		}
		result += ")";
		return result;
	}

	string ListExpression::ToCode()
	{
		string result = "(";
		for (auto element : elements)
		{
			result += element->ToCode();
			if (element != elements.back())
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
			result = "+ (";
			break;
		case UnaryOperator::Negative:
			result = "- (";
			break;
		case UnaryOperator::Not:
			result = "not (";
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
			CodeError error = {
				token,
				token,
				"Unexpected end of line.",
			};
			return error;
		}
		else if (input->value != token)
		{
			auto token = *input;
			CodeError error = {
				token,
				token,
				"\"" + token.value + "\" expected but \"" + input->value + "\" found.",
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
		return error1.begin.column > error2.begin.column ? error1 : error2;
	}

	CodeError GrammarStack::ParseGrammarFragment(GrammarFragment::Ptr fragment, Iterator input, Iterator end, ResultList& result)
	{
		switch (fragment->type)
		{
		case GrammarFragmentType::Type:
			return ParseType(input, end, result);
		case GrammarFragmentType::Primitive:
			return ParsePrimitive(input, end, result);
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

	CodeError GrammarStack::ParseGrammarSymbol(GrammarSymbol::Ptr symbol, Iterator input, Iterator end, ResultList& result)
	{
		vector<pair<Iterator, ExpressionLink::Ptr>> stepResult;
		stepResult.push_back(make_pair(input, ExpressionLink::Ptr(nullptr)));

		int stepResultBegin = 0;
		int stepResultEnd = stepResult.size();
		CodeError resultError;
		for (int i = 0; (size_t)i < symbol->fragments.size(); i++)
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

	CodeError GrammarStack::ParsePrimitive(Iterator input, Iterator end, ResultList& result)
	{
		throw 0;
	}

	CodeError GrammarStack::ParseExpression(Iterator input, Iterator end, ResultList& result)
	{
		throw 0;
	}

	CodeError GrammarStack::ParseList(Iterator input, Iterator end, ResultList& result)
	{
		throw 0;
	}

	CodeError GrammarStack::ParseAssignable(Iterator input, Iterator end, ResultList& result)
	{
		throw 0;
	}

	CodeError GrammarStack::ParseArgument(Iterator input, Iterator end, ResultList& result)
	{
		throw 0;
	}
}