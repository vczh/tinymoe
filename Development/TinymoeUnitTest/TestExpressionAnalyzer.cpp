#include "UnitTest.h"
#include "..\Source\TinymoeExpressionAnalyzer.h"

using namespace tinymoe;

TEST_CASE(TestGrammarUniqueId)
{
}

TEST_CASE(TestGrammarStack)
{
	auto stack = make_shared<GrammarStack>();
	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();
	TEST_ASSERT(item->symbols.size() == 24);

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
}

TEST_CASE(TestParseTypeExpression)
{
}

TEST_CASE(TestParseArgumentExpression)
{
}

TEST_CASE(TestParseAssignableExpression)
{
}

TEST_CASE(TestParsePrimitiveExpression)
{
}

TEST_CASE(TestParseUnaryExpression)
{
}

TEST_CASE(TestParseBinaryExpression)
{
}

TEST_CASE(TestParseListExpression)
{
}

TEST_CASE(TestParseInvokeExpression)
{
}

TEST_CASE(TestParseStatement)
{
}