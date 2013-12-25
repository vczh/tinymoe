#include "UnitTest.h"
#include "..\Source\Tinymoe.h"

using namespace tinymoe;

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
}