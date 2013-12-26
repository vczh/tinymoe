#include "UnitTest.h"
#include "..\Source\Tinymoe.h"

using namespace tinymoe;

TEST_CASE(TestParseCorrectSymbol)
{
	string code = R"tinymoe(
symbol symbol name
)tinymoe";
	CodeError::List errors;

	auto codeFile = CodeFile::Parse(code, errors);
	TEST_ASSERT(errors.size() == 0);

	int lineIndex = 0;
	auto decl = SymbolDeclaration::Parse(codeFile, errors, lineIndex);
	TEST_ASSERT(decl);
	TEST_ASSERT(lineIndex == 1);
	TEST_ASSERT(errors.size() == 0);

	TEST_ASSERT(decl->name->identifiers.size() == 2);
	TEST_ASSERT(decl->name->identifiers[0].value == "symbol");
	TEST_ASSERT(decl->name->identifiers[1].value == "name");
}

TEST_CASE(TestParseWrongSymbol)
{
	{
		string code = R"tinymoe(
symbol
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = SymbolDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].begin.value == "symbol");
		TEST_ASSERT(errors[0].end.value == "symbol");
	}
	{
		string code = R"tinymoe(
symbol illegal + name
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = SymbolDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].begin.value == "+");
		TEST_ASSERT(errors[0].end.value == "+");
	}
}