#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe::compiler;

#define FIRST_LINE(COUNT)					TEST_ASSERT(codeFile->lines.size() == COUNT); auto lineIterator = codeFile->lines.begin(); {
#define NEXT_LINE							} lineIterator++; {
#define LAST_LINE							} lineIterator++; TEST_ASSERT(lineIterator == codeFile->lines.end())
#define FIRST_TOKEN(COUNT)					auto line = *lineIterator; TEST_ASSERT(line->tokens.size() == COUNT); auto tokenIterator = line->tokens.begin()
#define TOKEN(ROW, COLUMN, VALUE, TYPE)		TEST_ASSERT(tokenIterator->row == ROW); TEST_ASSERT(tokenIterator->column == COLUMN); TEST_ASSERT(tokenIterator->value == VALUE); TEST_ASSERT(tokenIterator->type == TYPE); tokenIterator++
#define LAST_TOKEN							TEST_ASSERT(tokenIterator == line->tokens.end())

TEST_CASE(TestLexerIdentifier)
{
	string code = R"tinymoe(
module test

phrase main
	print "Hello, world!"
end
)tinymoe";

	CodeError::List errors;
	auto codeFile = CodeFile::Parse(code, 0, errors);

	FIRST_LINE(4);
		FIRST_TOKEN(2);
		TOKEN(2, 1, "module", CodeTokenType::Module);
		TOKEN(2, 8, "test", CodeTokenType::Identifier);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(2);
		TOKEN(4, 1, "phrase", CodeTokenType::Phrase);
		TOKEN(4, 8, "main", CodeTokenType::Identifier);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(2);
		TOKEN(5, 2, "print", CodeTokenType::Identifier);
		TOKEN(5, 9, "Hello, world!", CodeTokenType::String);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(1);
		TOKEN(6, 1, "end", CodeTokenType::End);
		LAST_TOKEN;
	LAST_LINE;
}

TEST_CASE(TestLexerNumber)
{
	string code = R"tinymoe(
11 22. 33.44
)tinymoe";

	CodeError::List errors;
	auto codeFile = CodeFile::Parse(code, 0, errors);

	FIRST_LINE(1);
		FIRST_TOKEN(3);
		TOKEN(2, 1, "11", CodeTokenType::Integer);
		TOKEN(2, 4, "22", CodeTokenType::Integer);
		TOKEN(2, 8, "33.44", CodeTokenType::Float);
		LAST_TOKEN;
	LAST_LINE;

	TEST_ASSERT(errors.size() == 1);
	TEST_ASSERT(errors[0].position.value == ".");
}

TEST_CASE(TestLexerOperators)
{
	string code = R"tinymoe(
(),:&+-*/<<=>>==<>
)tinymoe";

	CodeError::List errors;
	auto codeFile = CodeFile::Parse(code, 0, errors);

	FIRST_LINE(1);
		FIRST_TOKEN(15);
		TOKEN(2, 1, "(", CodeTokenType::OpenBracket);
		TOKEN(2, 2, ")", CodeTokenType::CloseBracket);
		TOKEN(2, 3, ",", CodeTokenType::Comma);
		TOKEN(2, 4, ":", CodeTokenType::Colon);
		TOKEN(2, 5, "&", CodeTokenType::Concat);
		TOKEN(2, 6, "+", CodeTokenType::Add);
		TOKEN(2, 7, "-", CodeTokenType::Sub);
		TOKEN(2, 8, "*", CodeTokenType::Mul);
		TOKEN(2, 9, "/", CodeTokenType::Div);
		TOKEN(2, 10, "<", CodeTokenType::LT);
		TOKEN(2, 11, "<=", CodeTokenType::LE);
		TOKEN(2, 13, ">", CodeTokenType::GT);
		TOKEN(2, 14, ">=", CodeTokenType::GE);
		TOKEN(2, 16, "=", CodeTokenType::EQ);
		TOKEN(2, 17, "<>", CodeTokenType::NE);
		LAST_TOKEN;
	LAST_LINE;
}

TEST_CASE(TestLexerKeywords)
{
	string code = R"tinymoe(
module using
phrase sentence block symbol type
cps category expression argument
end and or not
)tinymoe";

	CodeError::List errors;
	auto codeFile = CodeFile::Parse(code, 0, errors);

	FIRST_LINE(4);
		FIRST_TOKEN(2);
		TOKEN(2, 1, "module", CodeTokenType::Module);
		TOKEN(2, 8, "using", CodeTokenType::Using);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(5);
		TOKEN(3, 1, "phrase", CodeTokenType::Phrase);
		TOKEN(3, 8, "sentence", CodeTokenType::Sentence);
		TOKEN(3, 17, "block", CodeTokenType::Block);
		TOKEN(3, 23, "symbol", CodeTokenType::Symbol);
		TOKEN(3, 30, "type", CodeTokenType::Type);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(4);
		TOKEN(4, 1, "cps", CodeTokenType::CPS);
		TOKEN(4, 5, "category", CodeTokenType::Category);
		TOKEN(4, 14, "expression", CodeTokenType::Expression);
		TOKEN(4, 25, "argument", CodeTokenType::Argument);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(4);
		TOKEN(5, 1, "end", CodeTokenType::End);
		TOKEN(5, 5, "and", CodeTokenType::And);
		TOKEN(5, 9, "or", CodeTokenType::Or);
		TOKEN(5, 12, "not", CodeTokenType::Not);
		LAST_TOKEN;
	LAST_LINE;
}

TEST_CASE(TestLexerString)
{
	string code = R"tinymoe(
"First line\r\nSecond line"
"Unfinished line
"Unfinished escaping\
"Third line\r\nFourth line"
)tinymoe";

	CodeError::List errors;
	auto codeFile = CodeFile::Parse(code, 0, errors);

	FIRST_LINE(2);
		FIRST_TOKEN(1);
		TOKEN(2, 2, "First line\r\nSecond line", CodeTokenType::String);
		LAST_TOKEN;
	NEXT_LINE;
		FIRST_TOKEN(1);
		TOKEN(5, 2, "Third line\r\nFourth line", CodeTokenType::String);
		LAST_TOKEN;
	LAST_LINE;

	TEST_ASSERT(errors.size() == 2);
	TEST_ASSERT(errors[0].position.value == "\"Unfinished line");
	TEST_ASSERT(errors[1].position.value == "\"Unfinished escaping\\");
}