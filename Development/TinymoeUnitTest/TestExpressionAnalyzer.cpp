#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;
using namespace tinymoe::compiler;

void Tokenize(const string_t& code, CodeToken::List& tokens)
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
		Tokenize(T("true"), tokens);
		result.clear();
		stack->ParsePrimitive(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == T("true"));
	}
	{
		Tokenize(T("false"), tokens);
		result.clear();
		stack->ParsePrimitive(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == T("false"));
	}
	{
		Tokenize(T("null"), tokens);
		result.clear();
		stack->ParsePrimitive(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == T("null"));
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
		Tokenize(T("array"), tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == T("array"));
	}
	{
		Tokenize(T("integer"), tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == T("integer"));
	}
	{
		Tokenize(T("float"), tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == T("float"));
	}
	{
		Tokenize(T("string"), tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == T("string"));
	}
	{
		Tokenize(T("symbol"), tokens);
		result.clear();
		stack->ParseType(tokens.begin(), tokens.end(), result);
		TEST_ASSERT(result.size() == 1);

		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == T("symbol"));
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

	Tokenize(T("true end"), tokens);
	result.clear();
	stack->ParseArgument(tokens.begin(), tokens.end(), result);
	TEST_ASSERT(result.size() == 2);
	{
		auto expr = dynamic_pointer_cast<ArgumentExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->name->identifiers.size() == 1);
		TEST_ASSERT(expr->name->identifiers[0].value == T("true"));
	}
	{
		auto expr = dynamic_pointer_cast<ArgumentExpression>(result[1].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->name->identifiers.size() == 2);
		TEST_ASSERT(expr->name->identifiers[0].value == T("true"));
		TEST_ASSERT(expr->name->identifiers[1].value == T("end"));
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

	Tokenize(T("true end"), tokens);
	stack->ParseAssignable(tokens.begin(), tokens.end(), result);
	TEST_ASSERT(result.size() == 2);
	{
		auto expr = dynamic_pointer_cast<ReferenceExpression>(result[0].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->symbol->uniqueId == T("true"));
	}
	{
		auto expr = dynamic_pointer_cast<ArgumentExpression>(result[1].second);
		TEST_ASSERT(expr);
		TEST_ASSERT(expr->name->identifiers.size() == 2);
		TEST_ASSERT(expr->name->identifiers[0].value == T("true"));
		TEST_ASSERT(expr->name->identifiers[1].value == T("end"));
	}
}

Expression::Ptr ParseNonAmbiguousExpression(const string_t& code)
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
	auto expr = ParseNonAmbiguousExpression(T("item 1 of array new array of 10 items is not integer"));
	auto log = expr->ToLog();
	auto code = expr->ToCode();
	TEST_ASSERT(log == T("<primitive> is not <type>(item <expression> of array <primitive>(1, new array of <expression> items(10)), integer)"));
	TEST_ASSERT(code == T("((item 1 of array (new array of 10 items)) is not (integer))"));
}

TEST_CASE(TestParseUnaryExpression)
{
	auto expr = ParseNonAmbiguousExpression(T("not (-1 is string)"));
	auto log = expr->ToLog();
	auto code = expr->ToCode();
	TEST_ASSERT(log == T("not(<primitive> is <type>(-(1), string))"));
	TEST_ASSERT(code == T("(not((-1) is (string)))"));
}

TEST_CASE(TestParseBinaryExpression)
{
	auto expr = ParseNonAmbiguousExpression(T("1+2*3+4"));
	auto log = expr->ToLog();
	auto code = expr->ToCode();
	TEST_ASSERT(log == T("+(+(1, *(2, 3)), 4)"));
	TEST_ASSERT(code == T("((1 + (2 * 3)) + 4)"));
}

TEST_CASE(TestParseListExpression)
{
	auto expr = ParseNonAmbiguousExpression(T("new object of (1, 2, 3)"));
	auto log = expr->ToLog();
	auto code = expr->ToCode();
	TEST_ASSERT(log == T("new <type> of <list>(object, (list: 1, 2, 3))"));
	TEST_ASSERT(code == T("(new (object) of (1, 2, 3))"));
}

Expression::Ptr ParseNonAmbiguousStatement(const string_t& code, GrammarStack::Ptr stack = nullptr)
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

GrammarStack::Ptr ParseAmbiguousStatement(const string_t& code, Expression::List& expressionResult, GrammarStack::Ptr stack = nullptr)
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
		auto stat = ParseNonAmbiguousStatement(T("end"));
		auto log = stat->ToLog();
		auto code = stat->ToCode();
		TEST_ASSERT(log == T("end()"));
		TEST_ASSERT(code == T("(end)"));
	}
	{
		auto stat = ParseNonAmbiguousStatement(T("select item 1 of array new array of 10 items + 1"));
		auto log = stat->ToLog();
		auto code = stat->ToCode();
		TEST_ASSERT(log == T("select <expression>(+(item <expression> of array <primitive>(1, new array of <expression> items(10)), 1))"));
		TEST_ASSERT(code == T("(select ((item 1 of array (new array of 10 items)) + 1))"));
	}
	{
		auto stat = ParseNonAmbiguousStatement(T("set true to false"));
		auto log = stat->ToLog();
		auto code = stat->ToCode();
		TEST_ASSERT(log == T("set <assignable> to <expression>(true, false)"));
		TEST_ASSERT(code == T("(set (true) to (false))"));
	}
	{
		auto stat = ParseNonAmbiguousStatement(T("set new variable to false"));
		auto log = stat->ToLog();
		auto code = stat->ToCode();
		TEST_ASSERT(log == T("set <assignable> to <expression>((argument: new variable), false)"));
		TEST_ASSERT(code == T("(set (new variable) to (false))"));
	}
}

TEST_CASE(TestParseAmbiguousStatement)
{
	{
		Expression::List stats;
		auto stack = ParseAmbiguousStatement(T("set field something wrong of null to true"), stats);
		TEST_ASSERT(stats.size() == 2);
		{
			auto stat = stats[0];
			auto log = stat->ToLog();
			auto code = stat->ToCode();
			
			Expression::List assignables, arguments, modifiedAssignables;
			stat->CollectNewAssignable(assignables, arguments, modifiedAssignables);
			auto assignable = stack->CountStatementAssignables(assignables);

			TEST_ASSERT(log == T("set <assignable> to <expression>((argument: field something wrong of null), true)"));
			TEST_ASSERT(code == T("(set (field something wrong of null) to (true))"));
			TEST_ASSERT(assignable == -1);
		}
		{
			auto stat = stats[1];
			auto log = stat->ToLog();
			auto code = stat->ToCode();
			
			Expression::List assignables, arguments, modifiedAssignables;
			stat->CollectNewAssignable(assignables, arguments, modifiedAssignables);
			auto assignable = stack->CountStatementAssignables(assignables);

			TEST_ASSERT(log == T("set field <argument> of <expression> to <expression>((argument: something wrong), null, true)"));
			TEST_ASSERT(code == T("(set field (something wrong) of (null) to (true))"));
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
			+ T("print") + GrammarFragmentType::Expression
			);

		item->symbols.push_back(
			make_shared<GrammarSymbol>(GrammarSymbolType::Phrase)
			+ T("sum") + T("from") + GrammarFragmentType::Expression + T("to") + GrammarFragmentType::Primitive
			);

		stack->Push(item);
	}
	{
		auto stat = ParseNonAmbiguousStatement(T("print \"1+ ... +100 = \" & sum from 1 to 100"), stack);
		auto log = stat->ToLog();
		auto code = stat->ToCode();
		TEST_ASSERT(log == T("print <expression>(&(\"1+ ... +100 = \", sum from <expression> to <primitive>(1, 100)))"));
		TEST_ASSERT(code == T("(print (\"1+ ... +100 = \" & (sum from 1 to 100)))"));
	}
}