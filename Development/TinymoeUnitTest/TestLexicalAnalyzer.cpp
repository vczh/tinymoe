#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;
using namespace tinymoe::compiler;

#define FIRST_LINE(COUNT)					TEST_ASSERT(codeFile->lines.size() == COUNT); auto lineIterator = codeFile->lines.begin(); {
#define NEXT_LINE							} lineIterator++; {
#define LAST_LINE							} lineIterator++; TEST_ASSERT(lineIterator == codeFile->lines.end())
#define FIRST_TOKEN(COUNT)					auto line = *lineIterator; TEST_ASSERT(line->tokens.size() == COUNT); auto tokenIterator = line->tokens.begin()
#define TOKEN(ROW, COLUMN, VALUE, TYPE)		TEST_ASSERT(tokenIterator->row == ROW); TEST_ASSERT(tokenIterator->column == COLUMN); TEST_ASSERT(tokenIterator->value == VALUE); TEST_ASSERT(tokenIterator->type == TYPE); tokenIterator++
#define LAST_TOKEN							TEST_ASSERT(tokenIterator == line->tokens.end())

TEST_CASE(TestLexerIdentifier)
{
	string_t code = T(R"tinymoe(
module test

phrase main
	print "Hello, world!"
end
)tinymoe");

	CodeError::List errors;
	auto codeFile = CodeFile::Parse(code, 0, errors);

	FIRST_LINE(4);
		FIRST_TOKEN(2);
		TOKEN(2, 1, T("module"), CodeTokenType::Module);
		TOKEN(2, 8, T("test"), CodeTokenType::Identifier);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(2);
		TOKEN(4, 1, T("phrase"), CodeTokenType::Phrase);
		TOKEN(4, 8, T("main"), CodeTokenType::Identifier);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(2);
		TOKEN(5, 2, T("print"), CodeTokenType::Identifier);
		TOKEN(5, 9, T("Hello, world!"), CodeTokenType::String);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(1);
		TOKEN(6, 1, T("end"), CodeTokenType::End);
		LAST_TOKEN;
	LAST_LINE;
}

TEST_CASE(TestLexerNumber)
{
	string_t code = T(R"tinymoe(
11 22. 33.44
)tinymoe");

	CodeError::List errors;
	auto codeFile = CodeFile::Parse(code, 0, errors);

	FIRST_LINE(1);
		FIRST_TOKEN(3);
		TOKEN(2, 1, T("11"), CodeTokenType::Integer);
		TOKEN(2, 4, T("22"), CodeTokenType::Integer);
		TOKEN(2, 8, T("33.44"), CodeTokenType::Float);
		LAST_TOKEN;
	LAST_LINE;

	TEST_ASSERT(errors.size() == 1);
	TEST_ASSERT(errors[0].position.value == T("."));
}

TEST_CASE(TestLexerOperators)
{
	string_t code = T(R"tinymoe(
(),:&+-*/<<=>>==<>--xxx
- -
)tinymoe");

	CodeError::List errors;
	auto codeFile = CodeFile::Parse(code, 0, errors);

	FIRST_LINE(2);
		FIRST_TOKEN(15);
		TOKEN(2, 1, T("("), CodeTokenType::OpenBracket);
		TOKEN(2, 2, T(")"), CodeTokenType::CloseBracket);
		TOKEN(2, 3, T(","), CodeTokenType::Comma);
		TOKEN(2, 4, T(":"), CodeTokenType::Colon);
		TOKEN(2, 5, T("&"), CodeTokenType::Concat);
		TOKEN(2, 6, T("+"), CodeTokenType::Add);
		TOKEN(2, 7, T("-"), CodeTokenType::Sub);
		TOKEN(2, 8, T("*"), CodeTokenType::Mul);
		TOKEN(2, 9, T("/"), CodeTokenType::Div);
		TOKEN(2, 10, T("<"), CodeTokenType::LT);
		TOKEN(2, 11, T("<="), CodeTokenType::LE);
		TOKEN(2, 13, T(">"), CodeTokenType::GT);
		TOKEN(2, 14, T(">="), CodeTokenType::GE);
		TOKEN(2, 16, T("="), CodeTokenType::EQ);
		TOKEN(2, 17, T("<>"), CodeTokenType::NE);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(2);
		TOKEN(3, 1, T("-"), CodeTokenType::Sub);
		TOKEN(3, 3, T("-"), CodeTokenType::Sub);
		LAST_TOKEN;
	LAST_LINE;
}

TEST_CASE(TestLexerKeywords)
{
	string_t code = T(R"tinymoe(
module using
phrase sentence block symbol type
cps category expression argument
end and or not
)tinymoe");

	CodeError::List errors;
	auto codeFile = CodeFile::Parse(code, 0, errors);

	FIRST_LINE(4);
		FIRST_TOKEN(2);
		TOKEN(2, 1, T("module"), CodeTokenType::Module);
		TOKEN(2, 8, T("using"), CodeTokenType::Using);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(5);
		TOKEN(3, 1, T("phrase"), CodeTokenType::Phrase);
		TOKEN(3, 8, T("sentence"), CodeTokenType::Sentence);
		TOKEN(3, 17, T("block"), CodeTokenType::Block);
		TOKEN(3, 23, T("symbol"), CodeTokenType::Symbol);
		TOKEN(3, 30, T("type"), CodeTokenType::Type);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(4);
		TOKEN(4, 1, T("cps"), CodeTokenType::CPS);
		TOKEN(4, 5, T("category"), CodeTokenType::Category);
		TOKEN(4, 14, T("expression"), CodeTokenType::Expression);
		TOKEN(4, 25, T("argument"), CodeTokenType::Argument);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(4);
		TOKEN(5, 1, T("end"), CodeTokenType::End);
		TOKEN(5, 5, T("and"), CodeTokenType::And);
		TOKEN(5, 9, T("or"), CodeTokenType::Or);
		TOKEN(5, 12, T("not"), CodeTokenType::Not);
		LAST_TOKEN;
	LAST_LINE;
}

TEST_CASE(TestLexerString)
{
	string_t code = T(R"tinymoe(
"First line\r\nSecond line"
"Unfinished line
"Unfinished escaping\
"Third line\r\nFourth line"
)tinymoe");

	CodeError::List errors;
	auto codeFile = CodeFile::Parse(code, 0, errors);

	FIRST_LINE(2);
		FIRST_TOKEN(1);
		TOKEN(2, 2, T("First line\r\nSecond line"), CodeTokenType::String);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(1);
		TOKEN(5, 2, T("Third line\r\nFourth line"), CodeTokenType::String);
		LAST_TOKEN;
	LAST_LINE;

	TEST_ASSERT(errors.size() == 2);
	TEST_ASSERT(errors[0].position.value == T("\"Unfinished line"));
	TEST_ASSERT(errors[1].position.value == T("\"Unfinished escaping\\"));
}