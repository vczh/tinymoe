#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe::compiler;

void Tokenize(const string& code, CodeToken::List& tokens)
{
	CodeError::List errors;
	auto codeFile = CodeFile::Parse(code, 0, errors);
	TEST_ASSERT(errors.size() == 0);
	TEST_ASSERT(codeFile->lines.size() == 1);
	tokens = codeFile->lines[0]->tokens;
}

TEST_CASE(TestGrammarStack)
{
	auto stack = make_shared<GrammarStack>();
	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();

	stack->Push(item);
	TEST_ASSERT(stack->stackItems.size() == 1);
	TEST_ASSERT(stack->availableSymbols.size() == item->symbols.size());
	for (auto symbol : item->symbols)
	{
		auto it = stack->availableSymbols.find(symbol->uniqueId);
		TEST_ASSERT(it != stack->availableSymbols.end());
		TEST_ASSERT(it->first == symbol->uniqueId);
		TEST_ASSERT(it->second == symbol);
	}

	TEST_ASSERT(stack->Pop() == item);
	TEST_ASSERT(stack->stackItems.size() == 0);
	TEST_ASSERT(stack->availableSymbols.size() == 0);
}

TEST_CASE(TestParseNameExpression)
{
	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();
	auto stack = make_shared<GrammarStack>();
	stack->Push(item);

	CodeToken::List tokens;
	GrammarStack::ResultList result;

	{
		Tokenize("true", tokens);
		result.clear();
		stack->ParsePrimitive(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "true");
	}
	{
		Tokenize("false", tokens);
		result.clear();
		stack->ParsePrimitive(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "false");
	}
	{
		Tokenize("null", tokens);
		result.clear();
		stack->ParsePrimitive(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "null");
	}
}

TEST_CASE(TestParseTypeExpression)
{
	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();
	auto stack = make_shared<GrammarStack>();
	stack->Push(item);

	CodeToken::List tokens;
	GrammarStack::ResultList result;

	{
		Tokenize("array", tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "array");
	}
	{
		Tokenize("integer", tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "integer");
	}
	{
		Tokenize("float", tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "float");
	}
	{
		Tokenize("string", tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "string");
	}
	{
		Tokenize("symbol", tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "symbol");
	}
}

TEST_CASE(TestParseArgumentExpression)
{
	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();
	auto stack = make_shared<GrammarStack>();
	stack->Push(item);

	CodeToken::List tokens;
	GrammarStack::ResultList result;

	Tokenize("true end", tokens);
	result.clear();
	stack->ParseArgument(tokens.begin(), tokens.end(), result);
	TEST_ASSERT(result.size() == 2);
	{
		auto expr = dynamic_pointer_cast<ArgumentExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->tokens.size() == 1);
		TEST_ASSERT(expr->tokens[0].value == "true");
	}
	{
		auto expr = dynamic_pointer_cast<ArgumentExpression>(result[1].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->tokens.size() == 2);
		TEST_ASSERT(expr->tokens[0].value == "true");
		TEST_ASSERT(expr->tokens[1].value == "end");
	}
}

TEST_CASE(TestParseAssignableExpression)
{
	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();
	auto stack = make_shared<GrammarStack>();
	stack->Push(item);

	CodeToken::List tokens;
	GrammarStack::ResultList result;

	Tokenize("true end", tokens);
	stack->ParseAssignable(tokens.begin(), tokens.end(), result);
	TEST_ASSERT(result.size() == 2);
	{
		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == "true");
	}
	{
		auto expr = dynamic_pointer_cast<ArgumentExpression>(result[1].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->tokens.size() == 2);
		TEST_ASSERT(expr->tokens[0].value == "true");
		TEST_ASSERT(expr->tokens[1].value == "end");
	}
}

Expression::Ptr ParseNonAmbiguousExpression(const string& code)
{
	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();
	auto stack = make_shared<GrammarStack>();
	stack->Push(item);

	CodeToken::List tokens;
	GrammarStack::ResultList result;

	Tokenize(code, tokens);
	stack->ParseExpression(tokens.begin(), tokens.end(), result);

	Expression::List fullExpressions;
	for (auto r : result)
	{
		if (r.first == tokens.end())
		{
			fullExpressions.push_back(r.second);
		}
	}

	TEST_ASSERT(fullExpressions.size() == 1);
	return fullExpressions[0];
}

TEST_CASE(TestParsePrimitiveExpression)
{
	auto expr = ParseNonAmbiguousExpression("item 1 of array new array of 10 items is not integer");
	auto log = expr->ToLog();
	auto code = expr->ToCode();
	TEST_ASSERT(log == "<primitive> is not <type>(item <expression> of array <primitive>(1, new array of <expression> items(10)), integer)");
	TEST_ASSERT(code == "((item 1 of array (new array of 10 items)) is not (integer))");
}

TEST_CASE(TestParseUnaryExpression)
{
	auto expr = ParseNonAmbiguousExpression("not (-1 is string)");
	auto log = expr->ToLog();
	auto code = expr->ToCode();
	TEST_ASSERT(log == "not(<primitive> is <type>(-(1), string))");
	TEST_ASSERT(code == "(not((-1) is (string)))");
}

TEST_CASE(TestParseBinaryExpression)
{
	auto expr = ParseNonAmbiguousExpression("1+2*3+4");
	auto log = expr->ToLog();
	auto code = expr->ToCode();
	TEST_ASSERT(log == "+(+(1, *(2, 3)), 4)");
	TEST_ASSERT(code == "((1 + (2 * 3)) + 4)");
}

TEST_CASE(TestParseListExpression)
{
	auto expr = ParseNonAmbiguousExpression("invoke null with (1, 2, 3)");
	auto log = expr->ToLog();
	auto code = expr->ToCode();
	TEST_ASSERT(log == "invoke <expression> with <list>(null, (list: 1, 2, 3))");
	TEST_ASSERT(code == "(invoke (null) with (1, 2, 3))");
}

Expression::Ptr ParseNonAmbiguousStatement(const string& code, GrammarStack::Ptr stack = nullptr)
{
	if (!stack)
	{
		auto item = make_shared<GrammarStackItem>();
		item->FillPredefinedSymbols();
		
		stack = make_shared<GrammarStack>();
		stack->Push(item);
	}

	CodeToken::List tokens;
	GrammarStack::ResultList result;

	Tokenize(code, tokens);
	stack->ParseStatement(tokens.begin(), tokens.end(), result);
	TEST_ASSERT(result.size() == 1);
	return result[0].second;
}

GrammarStack::Ptr ParseAmbiguousStatement(const string& code, Expression::List& expressionResult, GrammarStack::Ptr stack = nullptr)
{
	if (!stack)
	{
		auto item = make_shared<GrammarStackItem>();
		item->FillPredefinedSymbols();

		stack = make_shared<GrammarStack>();
		stack->Push(item);
	}

	CodeToken::List tokens;
	GrammarStack::ResultList result;

	Tokenize(code, tokens);
	stack->ParseStatement(tokens.begin(), tokens.end(), result);

	for (auto r : result)
	{
		expressionResult.push_back(r.second);
	}
	return stack;
}

TEST_CASE(TestParseStatement)
{
	{
		auto stat = ParseNonAmbiguousStatement("end");
		auto log = stat->ToLog();
		auto code = stat->ToCode();
		TEST_ASSERT(log == "end()");
		TEST_ASSERT(code == "(end)");
	}
	{
		auto stat = ParseNonAmbiguousStatement("select item 1 of array new array of 10 items + 1");
		auto log = stat->ToLog();
		auto code = stat->ToCode();
		TEST_ASSERT(log == "select <expression>(+(item <expression> of array <primitive>(1, new array of <expression> items(10)), 1))");
		TEST_ASSERT(code == "(select ((item 1 of array (new array of 10 items)) + 1))");
	}
	{
		auto stat = ParseNonAmbiguousStatement("set true to false");
		auto log = stat->ToLog();
		auto code = stat->ToCode();
		TEST_ASSERT(log == "set <assignable> to <expression>(true, false)");
		TEST_ASSERT(code == "(set (true) to (false))");
	}
	{
		auto stat = ParseNonAmbiguousStatement("set new variable to false");
		auto log = stat->ToLog();
		auto code = stat->ToCode();
		TEST_ASSERT(log == "set <assignable> to <expression>((argument: new variable), false)");
		TEST_ASSERT(code == "(set (new variable) to (false))");
	}
}

TEST_CASE(TestParseAmbiguousStatement)
{
	{
		Expression::List stats;
		auto stack = ParseAmbiguousStatement("set field something wrong of null to true", stats);
		TEST_ASSERT(stats.size() == 2);
		{
			auto stat = stats[0];
			auto log = stat->ToLog();
			auto code = stat->ToCode();
			
			Expression::List assignables, arguments;
			stat->CollectNewAssignable(assignables, arguments);
			auto assignable = stack->CountStatementAssignables(assignables);

			TEST_ASSERT(log == "set <assignable> to <expression>((argument: field something wrong of null), true)");
			TEST_ASSERT(code == "(set (field something wrong of null) to (true))");
			TEST_ASSERT(assignable == -1);
		}
		{
			auto stat = stats[1];
			auto log = stat->ToLog();
			auto code = stat->ToCode();
			
			Expression::List assignables, arguments;
			stat->CollectNewAssignable(assignables, arguments);
			auto assignable = stack->CountStatementAssignables(assignables);

			TEST_ASSERT(log == "set field <argument> of <expression> to <expression>((argument: something wrong), null, true)");
			TEST_ASSERT(code == "(set field (something wrong) of (null) to (true))");
			TEST_ASSERT(assignable == 0);
		}
	}
}

TEST_CASE(TestParseStatementInContext)
{
	auto stack = make_shared<GrammarStack>();
	{
		auto item = make_shared<GrammarStackItem>();
		item->FillPredefinedSymbols();
		stack->Push(item);
	}
	{
		auto item = make_shared<GrammarStackItem>();
		
		item->symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Sentence)
			+ "print" + GrammarFragmentType::Expression
			);

		item->symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Phrase)
			+ "sum" + "from" + GrammarFragmentType::Expression + "to" + GrammarFragmentType::Primitive
			);

		stack->Push(item);
	}
	{
		auto stat = ParseNonAmbiguousStatement("print \"1+ ... +100 = \" & sum from 1 to 100", stack);
		auto log = stat->ToLog();
		auto code = stat->ToCode();
		TEST_ASSERT(log == "print <expression>(&(\"1+ ... +100 = \", sum from <expression> to <primitive>(1, 100)))");
		TEST_ASSERT(code == "(print (\"1+ ... +100 = \" & (sum from 1 to 100)))");
	}
}