#include "UnitTest.h"
#include "..\Source\Tinymoe.h"

using namespace tinymoe;

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
	auto codeFile = CodeFile::Parse(code, errors);

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