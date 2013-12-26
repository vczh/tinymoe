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

TEST_CASE(TestParseCorrectType)
{
	{
		string code = R"tinymoe(
type empty type
end
	)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = TypeDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 0);

		TEST_ASSERT(decl->name->identifiers.size() == 2);
		TEST_ASSERT(decl->name->identifiers[0].value == "empty");
		TEST_ASSERT(decl->name->identifiers[1].value == "type");
		TEST_ASSERT(!decl->parent);
		TEST_ASSERT(decl->fields.size() == 0);
	}
	{
		string code = R"tinymoe(
type pair
	first
	second
end
	)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = TypeDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 4);
		TEST_ASSERT(errors.size() == 0);

		TEST_ASSERT(decl->name->identifiers.size() == 1);
		TEST_ASSERT(decl->name->identifiers[0].value == "pair");
		TEST_ASSERT(!decl->parent);

		TEST_ASSERT(decl->fields.size() == 2);
		TEST_ASSERT(decl->fields[0]->identifiers.size() == 1);
		TEST_ASSERT(decl->fields[0]->identifiers[0].value == "first");
		TEST_ASSERT(decl->fields[1]->identifiers.size() == 1);
		TEST_ASSERT(decl->fields[1]->identifiers[0].value == "second");
	}
	{
		string code = R"tinymoe(
type derived : base
	first
	second
end
	)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = TypeDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 4);
		TEST_ASSERT(errors.size() == 0);

		TEST_ASSERT(decl->name->identifiers.size() == 1);
		TEST_ASSERT(decl->name->identifiers[0].value == "derived");
		TEST_ASSERT(decl->parent->identifiers.size() == 1);
		TEST_ASSERT(decl->parent->identifiers[0].value == "base");

		TEST_ASSERT(decl->fields.size() == 2);
		TEST_ASSERT(decl->fields[0]->identifiers.size() == 1);
		TEST_ASSERT(decl->fields[0]->identifiers[0].value == "first");
		TEST_ASSERT(decl->fields[1]->identifiers.size() == 1);
		TEST_ASSERT(decl->fields[1]->identifiers[0].value == "second");
	}
}

TEST_CASE(TestParseWrongType)
{
	{
		string code = R"tinymoe(
type
end
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = TypeDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].begin.value == "type");
		TEST_ASSERT(errors[0].end.value == "type");
	}
	{
		string code = R"tinymoe(
type illegal + name
end
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = TypeDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].begin.value == "+");
		TEST_ASSERT(errors[0].end.value == "+");
	}
	{
		string code = R"tinymoe(
type type name
	illegal + name
end
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = TypeDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 3);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].begin.value == "+");
		TEST_ASSERT(errors[0].end.value == "+");
	}
}

TEST_CASE(TestParseCorrectCps)
{
	{
		string code = R"tinymoe(
cps (state)
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 0);

		TEST_ASSERT(decl->stateName);
		TEST_ASSERT(decl->stateName->identifiers.size() == 1);
		TEST_ASSERT(decl->stateName->identifiers[0].value == "state");
		TEST_ASSERT(!decl->continuationName);
	}
	{
		string code = R"tinymoe(
cps (state) (continuation)
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 0);

		TEST_ASSERT(decl->stateName);
		TEST_ASSERT(decl->stateName->identifiers.size() == 1);
		TEST_ASSERT(decl->stateName->identifiers[0].value == "state");
		TEST_ASSERT(decl->continuationName);
		TEST_ASSERT(decl->continuationName->identifiers.size() == 1);
		TEST_ASSERT(decl->continuationName->identifiers[0].value == "continuation");
	}
}

TEST_CASE(TestParseWrongCps)
{
	{
		string code = R"tinymoe(
cps
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].begin.value == "cps");
		TEST_ASSERT(errors[0].end.value == "cps");
	}
	{
		string code = R"tinymoe(
cps +
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].begin.value == "+");
		TEST_ASSERT(errors[0].end.value == "+");
	}
	{
		string code = R"tinymoe(
cps ()
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].begin.value == "cps");
		TEST_ASSERT(errors[0].end.value == "cps");
	}
	{
		string code = R"tinymoe(
cps (state
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].begin.value == "cps");
		TEST_ASSERT(errors[0].end.value == "cps");
	}
	{
		string code = R"tinymoe(
cps (state) (
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 2);
		TEST_ASSERT(errors[0].begin.value == "cps");
		TEST_ASSERT(errors[0].end.value == "cps");
		TEST_ASSERT(errors[1].begin.value == "cps");
		TEST_ASSERT(errors[1].end.value == "cps");
	}
	{
		string code = R"tinymoe(
cps (state) )
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].begin.value == ")");
		TEST_ASSERT(errors[0].end.value == ")");
	}
	{
		string code = R"tinymoe(
cps (state) (continuation) (too many parameters)
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].begin.value == "(");
		TEST_ASSERT(errors[0].end.value == "(");
	}
}

TEST_CASE(TestParseCorrectCategory)
{
	{
		string code = R"tinymoe(
category
	closable
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCategory::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 0);
		
		TEST_ASSERT(!decl->signalName);
		TEST_ASSERT(!decl->categoryName);
		TEST_ASSERT(decl->followCategories.size() == 0);
		TEST_ASSERT(decl->closable);
	}
	{
		string code = R"tinymoe(
category (signal)
	follow SEH try
	start SEH catch
)tinymoe";
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCategory::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 3);
		TEST_ASSERT(errors.size() == 0);
		
		TEST_ASSERT(decl->signalName);
		TEST_ASSERT(decl->signalName->identifiers.size() == 1);
		TEST_ASSERT(decl->signalName->identifiers[0].value == "signal");
		TEST_ASSERT(decl->categoryName);
		TEST_ASSERT(decl->categoryName->identifiers.size() == 2);
		TEST_ASSERT(decl->categoryName->identifiers[0].value == "SEH");
		TEST_ASSERT(decl->categoryName->identifiers[1].value == "catch");
		TEST_ASSERT(decl->followCategories.size() == 1);
		TEST_ASSERT(decl->followCategories[0]->identifiers.size() == 2);
		TEST_ASSERT(decl->followCategories[0]->identifiers[0].value == "SEH");
		TEST_ASSERT(decl->followCategories[0]->identifiers[1].value == "try");
		TEST_ASSERT(!decl->closable);
	}
}