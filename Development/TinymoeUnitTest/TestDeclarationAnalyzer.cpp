#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;
using namespace tinymoe::compiler;

TEST_CASE(TestParseCorrectSymbol)
{
	string_t code = T(R"tinymoe(
symbol symbol name
)tinymoe");
	CodeError::List errors;

	auto codeFile = CodeFile::Parse(code, 0, errors);
	TEST_ASSERT(errors.size() == 0);

	int lineIndex = 0;
	auto decl = SymbolDeclaration::Parse(codeFile, errors, lineIndex);
	TEST_ASSERT(decl);
	TEST_ASSERT(lineIndex == 1);
	TEST_ASSERT(errors.size() == 0);

	TEST_ASSERT(decl->keywordToken.value == T("symbol"));
	TEST_ASSERT(decl->name->identifiers.size() == 2);
	TEST_ASSERT(decl->name->identifiers[0].value == T("symbol"));
	TEST_ASSERT(decl->name->identifiers[1].value == T("name"));
}

TEST_CASE(TestParseWrongSymbol)
{
	{
		string_t code = T(R"tinymoe(
symbol
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = SymbolDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("symbol"));
	}
	{
		string_t code = T(R"tinymoe(
symbol illegal + name
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = SymbolDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("+"));
	}
}

TEST_CASE(TestParseCorrectType)
{
	{
		string_t code = T(R"tinymoe(
type empty type
end
	)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = TypeDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 0);
		
		TEST_ASSERT(decl->keywordToken.value == T("type"));
		TEST_ASSERT(decl->name->identifiers.size() == 2);
		TEST_ASSERT(decl->name->identifiers[0].value == T("empty"));
		TEST_ASSERT(decl->name->identifiers[1].value == T("type"));
		TEST_ASSERT(!decl->parent);
		TEST_ASSERT(decl->fields.size() == 0);
	}
	{
		string_t code = T(R"tinymoe(
type pair
	first
	second
end
	)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = TypeDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 4);
		TEST_ASSERT(errors.size() == 0);
		
		TEST_ASSERT(decl->keywordToken.value == T("type"));
		TEST_ASSERT(decl->name->identifiers.size() == 1);
		TEST_ASSERT(decl->name->identifiers[0].value == T("pair"));
		TEST_ASSERT(!decl->parent);

		TEST_ASSERT(decl->fields.size() == 2);
		TEST_ASSERT(decl->fields[0]->identifiers.size() == 1);
		TEST_ASSERT(decl->fields[0]->identifiers[0].value == T("first"));
		TEST_ASSERT(decl->fields[1]->identifiers.size() == 1);
		TEST_ASSERT(decl->fields[1]->identifiers[0].value == T("second"));
	}
	{
		string_t code = T(R"tinymoe(
type derived : base
	first
	second
end
	)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = TypeDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 4);
		TEST_ASSERT(errors.size() == 0);
		
		TEST_ASSERT(decl->keywordToken.value == T("type"));
		TEST_ASSERT(decl->name->identifiers.size() == 1);
		TEST_ASSERT(decl->name->identifiers[0].value == T("derived"));
		TEST_ASSERT(decl->parent->identifiers.size() == 1);
		TEST_ASSERT(decl->parent->identifiers[0].value == T("base"));

		TEST_ASSERT(decl->fields.size() == 2);
		TEST_ASSERT(decl->fields[0]->identifiers.size() == 1);
		TEST_ASSERT(decl->fields[0]->identifiers[0].value == T("first"));
		TEST_ASSERT(decl->fields[1]->identifiers.size() == 1);
		TEST_ASSERT(decl->fields[1]->identifiers[0].value == T("second"));
	}
}

TEST_CASE(TestParseWrongType)
{
	{
		string_t code = T(R"tinymoe(
type
end
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = TypeDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("type"));
	}
	{
		string_t code = T(R"tinymoe(
type illegal + name
end
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = TypeDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("+"));
	}
	{
		string_t code = T(R"tinymoe(
type type name
	illegal + name
end
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = TypeDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 3);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("+"));
	}
}

TEST_CASE(TestParseCorrectCps)
{
	{
		string_t code = T(R"tinymoe(
cps (state)
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 0);

		TEST_ASSERT(decl->stateName);
		TEST_ASSERT(decl->stateName->identifiers.size() == 1);
		TEST_ASSERT(decl->stateName->identifiers[0].value == T("state"));
		TEST_ASSERT(!decl->continuationName);
	}
	{
		string_t code = T(R"tinymoe(
cps (state) (continuation)
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 0);

		TEST_ASSERT(decl->stateName);
		TEST_ASSERT(decl->stateName->identifiers.size() == 1);
		TEST_ASSERT(decl->stateName->identifiers[0].value == T("state"));
		TEST_ASSERT(decl->continuationName);
		TEST_ASSERT(decl->continuationName->identifiers.size() == 1);
		TEST_ASSERT(decl->continuationName->identifiers[0].value == T("continuation"));
	}
}

TEST_CASE(TestParseWrongCps)
{
	{
		string_t code = T(R"tinymoe(
cps
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("cps"));
	}
	{
		string_t code = T(R"tinymoe(
cps +
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("+"));
	}
	{
		string_t code = T(R"tinymoe(
cps ()
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("cps"));
	}
	{
		string_t code = T(R"tinymoe(
cps (state
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("cps"));
	}
	{
		string_t code = T(R"tinymoe(
cps (state) (
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 2);
		TEST_ASSERT(errors[0].position.value == T("cps"));
		TEST_ASSERT(errors[1].position.value == T("cps"));
	}
	{
		string_t code = T(R"tinymoe(
cps (state) )
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T(")"));
	}
	{
		string_t code = T(R"tinymoe(
cps (state) (continuation) (too many parameters)
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCps::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("("));
	}
}

TEST_CASE(TestParseCorrectCategory)
{
	{
		string_t code = T(R"tinymoe(
category
	closable
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
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
		string_t code = T(R"tinymoe(
category (signal)
	follow SEH try
	follow SEH catch
	start SEH catch
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCategory::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 4);
		TEST_ASSERT(errors.size() == 0);
		
		TEST_ASSERT(decl->signalName);
		TEST_ASSERT(decl->signalName->identifiers.size() == 1);
		TEST_ASSERT(decl->signalName->identifiers[0].value == T("signal"));
		TEST_ASSERT(decl->categoryName);
		TEST_ASSERT(decl->categoryName->identifiers.size() == 2);
		TEST_ASSERT(decl->categoryName->identifiers[0].value == T("SEH"));
		TEST_ASSERT(decl->categoryName->identifiers[1].value == T("catch"));
		TEST_ASSERT(decl->followCategories.size() == 2);
		TEST_ASSERT(decl->followCategories[0]->identifiers.size() == 2);
		TEST_ASSERT(decl->followCategories[0]->identifiers[0].value == T("SEH"));
		TEST_ASSERT(decl->followCategories[0]->identifiers[1].value == T("try"));
		TEST_ASSERT(decl->followCategories[1]->identifiers.size() == 2);
		TEST_ASSERT(decl->followCategories[1]->identifiers[0].value == T("SEH"));
		TEST_ASSERT(decl->followCategories[1]->identifiers[1].value == T("catch"));
		TEST_ASSERT(!decl->closable);
	}
}

TEST_CASE(TestParseWrongCategory)
{
	{
		string_t code = T(R"tinymoe(
category (state
	closable
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCategory::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 2);
		TEST_ASSERT(errors[0].position.value == T("category"));
		TEST_ASSERT(errors[1].position.value == T("category"));
	}
	{
		string_t code = T(R"tinymoe(
category (state)
	start illegal + name
	closable
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCategory::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 3);
		TEST_ASSERT(errors.size() == 2);
		TEST_ASSERT(errors[0].position.value == T("+"));
		TEST_ASSERT(errors[1].position.value == T("category"));
	}
	{
		string_t code = T(R"tinymoe(
category (state)
	start SEH try
	follow
	closable
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCategory::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 4);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("follow"));
	}
	{
		string_t code = T(R"tinymoe(
category (signal)
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCategory::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 2);
		TEST_ASSERT(errors[0].position.value == T("category"));
		TEST_ASSERT(errors[1].position.value == T("category"));
	}
	{
		string_t code = T(R"tinymoe(
category (signal)
	closable
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCategory::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("category"));
	}
	{
		string_t code = T(R"tinymoe(
category
	follow a category
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionCategory::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(errors[0].position.value == T("category"));
	}
}

TEST_CASE(TestParseCorrectFunction)
{
	{
		string_t code = T(R"tinymoe(
sentence abort
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 0);

		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);
		TEST_ASSERT(!decl->cps);
		TEST_ASSERT(!decl->category);
		TEST_ASSERT(!decl->bodyName);
		TEST_ASSERT(!decl->alias);
		TEST_ASSERT(decl->type == FunctionDeclarationType::Sentence);
		
		TEST_ASSERT(decl->keywordToken.value == T("sentence"));
		TEST_ASSERT(decl->name.size() == 1);
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[0]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("abort"));
		}
	}
	{
		string_t code = T(R"tinymoe(
sentence abort
	do something
end

sentence halt
	do something
end
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 3);
		TEST_ASSERT(errors.size() == 0);
		
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == 1);
		TEST_ASSERT(decl->endLineIndex == 2);
		TEST_ASSERT(!decl->cps);
		TEST_ASSERT(!decl->category);
		TEST_ASSERT(!decl->bodyName);
		TEST_ASSERT(!decl->alias);
		TEST_ASSERT(decl->type == FunctionDeclarationType::Sentence);
		
		TEST_ASSERT(decl->keywordToken.value == T("sentence"));
		TEST_ASSERT(decl->name.size() == 1);
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[0]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("abort"));
		}
	}
	{
		string_t code = T(R"tinymoe(
sentence abort : an alias
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 0);
		
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);
		TEST_ASSERT(!decl->cps);
		TEST_ASSERT(!decl->category);
		TEST_ASSERT(!decl->bodyName);
		TEST_ASSERT(decl->alias);
		TEST_ASSERT(decl->alias->identifiers.size() == 2);
		TEST_ASSERT(decl->alias->identifiers[0].value == T("an"));
		TEST_ASSERT(decl->alias->identifiers[1].value == T("alias"));
		TEST_ASSERT(decl->type == FunctionDeclarationType::Sentence);
		
		TEST_ASSERT(decl->keywordToken.value == T("sentence"));
		TEST_ASSERT(decl->name.size() == 1);
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[0]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("abort"));
		}
	}
	{
		string_t code = T(R"tinymoe(
block (body) a (x) b (expression y) c (argument z) d (phrase o) e (sentence p) f (assignable q) g (list r) : an alias
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 0);
		
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);
		TEST_ASSERT(!decl->cps);
		TEST_ASSERT(!decl->category);
		TEST_ASSERT(decl->bodyName);
		{
			auto name = dynamic_pointer_cast<VariableArgumentFragment>(decl->bodyName);
			TEST_ASSERT(name->type == FunctionArgumentType::Normal);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("body"));
			TEST_ASSERT(!name->receivingType);
		}
		TEST_ASSERT(decl->alias);
		TEST_ASSERT(decl->alias->identifiers.size() == 2);
		TEST_ASSERT(decl->alias->identifiers[0].value == T("an"));
		TEST_ASSERT(decl->alias->identifiers[1].value == T("alias"));
		TEST_ASSERT(decl->type == FunctionDeclarationType::Block);
		
		TEST_ASSERT(decl->keywordToken.value == T("block"));
		TEST_ASSERT(decl->name.size() == 14);
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[0]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("a"));
		}
		{
			auto name = dynamic_pointer_cast<VariableArgumentFragment>(decl->name[1]);
			TEST_ASSERT(name->type == FunctionArgumentType::Normal);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("x"));
			TEST_ASSERT(!name->receivingType);
		}
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[2]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("b"));
		}
		{
			auto name = dynamic_pointer_cast<VariableArgumentFragment>(decl->name[3]);
			TEST_ASSERT(name->type == FunctionArgumentType::Expression);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("y"));
			TEST_ASSERT(!name->receivingType);
		}
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[4]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("c"));
		}
		{
			auto name = dynamic_pointer_cast<VariableArgumentFragment>(decl->name[5]);
			TEST_ASSERT(name->type == FunctionArgumentType::Argument);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("z"));
			TEST_ASSERT(!name->receivingType);
		}
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[6]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("d"));
		}
		{
			auto name = dynamic_pointer_cast<FunctionArgumentFragment>(decl->name[7]);
			TEST_ASSERT(name->declaration->type == FunctionDeclarationType::Phrase);
			TEST_ASSERT(name->declaration->name.size() == 1);
			
			auto argument = dynamic_pointer_cast<NameFragment>(name->declaration->name[0]);
			TEST_ASSERT(argument->name->identifiers.size() == 1);
			TEST_ASSERT(argument->name->identifiers[0].value == T("o"));
		}
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[8]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("e"));
		}
		{
			auto name = dynamic_pointer_cast<FunctionArgumentFragment>(decl->name[9]);
			TEST_ASSERT(name->declaration->type == FunctionDeclarationType::Sentence);
			TEST_ASSERT(name->declaration->name.size() == 1);
			
			auto argument = dynamic_pointer_cast<NameFragment>(name->declaration->name[0]);
			TEST_ASSERT(argument->name->identifiers.size() == 1);
			TEST_ASSERT(argument->name->identifiers[0].value == T("p"));
		}
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[10]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("f"));
		}
		{
			auto name = dynamic_pointer_cast<VariableArgumentFragment>(decl->name[11]);
			TEST_ASSERT(name->type == FunctionArgumentType::Assignable);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("q"));
			TEST_ASSERT(!name->receivingType);
		}
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[12]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("g"));
		}
		{
			auto name = dynamic_pointer_cast<VariableArgumentFragment>(decl->name[13]);
			TEST_ASSERT(name->type == FunctionArgumentType::List);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("r"));
			TEST_ASSERT(!name->receivingType);
		}
	}
	{
		string_t code = T(R"tinymoe(
block (body) abort
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 0);
		
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);
		TEST_ASSERT(!decl->cps);
		TEST_ASSERT(!decl->category);
		TEST_ASSERT(decl->bodyName);
		{
			auto name = dynamic_pointer_cast<VariableArgumentFragment>(decl->bodyName);
			TEST_ASSERT(name->type == FunctionArgumentType::Normal);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("body"));
			TEST_ASSERT(!name->receivingType);
		}
		TEST_ASSERT(!decl->alias);
		TEST_ASSERT(decl->type == FunctionDeclarationType::Block);
		
		TEST_ASSERT(decl->keywordToken.value == T("block"));
		TEST_ASSERT(decl->name.size() == 1);
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[0]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("abort"));
		}
	}
	{
		string_t code = T(R"tinymoe(
sentence remove (item) from (items : collection)
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 0);
		
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);
		TEST_ASSERT(!decl->cps);
		TEST_ASSERT(!decl->category);
		TEST_ASSERT(!decl->bodyName);
		TEST_ASSERT(!decl->alias);
		TEST_ASSERT(decl->type == FunctionDeclarationType::Sentence);
		
		TEST_ASSERT(decl->keywordToken.value == T("sentence"));
		TEST_ASSERT(decl->name.size() == 4);
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[0]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("remove"));
		}
		{
			auto name = dynamic_pointer_cast<VariableArgumentFragment>(decl->name[1]);
			TEST_ASSERT(name->type == FunctionArgumentType::Normal);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("item"));
			TEST_ASSERT(!name->receivingType);
		}
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[2]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("from"));
		}
		{
			auto name = dynamic_pointer_cast<VariableArgumentFragment>(decl->name[3]);
			TEST_ASSERT(name->type == FunctionArgumentType::Normal);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("items"));
			TEST_ASSERT(name->receivingType);
			TEST_ASSERT(name->receivingType->identifiers.size() == 1);
			TEST_ASSERT(name->receivingType->identifiers[0].value == T("collection"));
		}
	}
}

TEST_CASE(TestParseWrongFunction)
{
	{
		string_t code = T(R"tinymoe(
cps (state)
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);

		TEST_ASSERT(errors[0].position.value == T("cps"));
	}
	{
		string_t code = T(R"tinymoe(
category
	closable
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 1);

		TEST_ASSERT(errors[0].position.value == T("category"));
	}
	{
		string_t code = T(R"tinymoe(
cps (state)
category
	closable
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 3);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 2);

		TEST_ASSERT(errors[0].position.value == T("cps"));
	}
	{
		string_t code = T(R"tinymoe(
category
	closable
cps (state)
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 1);

		TEST_ASSERT(errors[0].position.value == T("cps"));
	}
	{
		string_t code = T(R"tinymoe(
cps (state)
phrase whatever
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 2);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 1);

		TEST_ASSERT(errors[0].position.value == T("phrase"));
	}
	{
		string_t code = T(R"tinymoe(
category
	closable
phrase whatever
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 3);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 2);

		TEST_ASSERT(errors[0].position.value == T("phrase"));
	}
	{
		string_t code = T(R"tinymoe(
category
	closable
sentence whatever
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 3);
		TEST_ASSERT(errors.size() == 2);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 2);

		TEST_ASSERT(errors[0].position.value == T("sentence"));
	}
	{
		string_t code = T(R"tinymoe(
block whatever
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);

		TEST_ASSERT(errors[0].position.value == T("block"));
	}
	{
		string_t code = T(R"tinymoe(
sentence (x) y
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);

		TEST_ASSERT(errors[0].position.value == T("sentence"));
	}
	{
		string_t code = T(R"tinymoe(
block (body) (x) y
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);

		TEST_ASSERT(errors[0].position.value == T("block"));
	}
	{
		string_t code = T(R"tinymoe(
phrase
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);

		TEST_ASSERT(errors[0].position.value == T("phrase"));
	}
	{
		string_t code = T(R"tinymoe(
phrase (x)
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);

		TEST_ASSERT(errors[0].position.value == T("phrase"));
	}
	{
		string_t code = T(R"tinymoe(
phrase (x) (y)
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 2);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);

		TEST_ASSERT(errors[0].position.value == T("phrase"));
		TEST_ASSERT(errors[1].position.value == T("phrase"));
	}
	{
		string_t code = T(R"tinymoe(
phrase a : b )
)tinymoe");
		CodeError::List errors;

		auto codeFile = CodeFile::Parse(code, 0, errors);
		TEST_ASSERT(errors.size() == 0);

		int lineIndex = 0;
		auto decl = FunctionDeclaration::Parse(codeFile, errors, lineIndex);
		TEST_ASSERT(decl);
		TEST_ASSERT(lineIndex == 1);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(decl->beginLineIndex == 0);
		TEST_ASSERT(decl->codeLineIndex == -1);
		TEST_ASSERT(decl->endLineIndex == 0);

		TEST_ASSERT(errors[0].position.value == T(")"));
	}
}

TEST_CASE(TestParseCorrectModule)
{
	string_t code = T(R"tinymoe(
module hello world
using standard library
using another module

phrase print (message)
	redirect to T("TheNameOfAnExternalPrintFunction"

phrase main
	print T("Hello, world!"
end
)tinymoe");
	CodeError::List errors;

	auto codeFile = CodeFile::Parse(code, 0, errors);
	TEST_ASSERT(errors.size() == 0);

	auto module = Module::Parse(codeFile, errors);
	TEST_ASSERT(module);
	TEST_ASSERT(errors.size() == 0);

	TEST_ASSERT(module->name->identifiers.size() == 2);
	TEST_ASSERT(module->name->identifiers[0].value == T("hello"));
	TEST_ASSERT(module->name->identifiers[1].value == T("world"));

	TEST_ASSERT(module->usings.size() == 2);
	TEST_ASSERT(module->usings[0]->identifiers.size() == 2);
	TEST_ASSERT(module->usings[0]->identifiers[0].value == T("standard"));
	TEST_ASSERT(module->usings[0]->identifiers[1].value == T("library"));
	TEST_ASSERT(module->usings[1]->identifiers.size() == 2);
	TEST_ASSERT(module->usings[1]->identifiers[0].value == T("another"));
	TEST_ASSERT(module->usings[1]->identifiers[1].value == T("module"));

	TEST_ASSERT(module->declarations.size() == 2);
	{
		auto decl = dynamic_pointer_cast<FunctionDeclaration>(module->declarations[0]);
		TEST_ASSERT(decl->name.size() == 2);
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[0]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("print"));
		}
		{
			auto name = dynamic_pointer_cast<VariableArgumentFragment>(decl->name[1]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("message"));
		}
	}
	{
		auto decl = dynamic_pointer_cast<FunctionDeclaration>(module->declarations[1]);
		TEST_ASSERT(decl->name.size() == 1);
		{
			auto name = dynamic_pointer_cast<NameFragment>(decl->name[0]);
			TEST_ASSERT(name->name->identifiers.size() == 1);
			TEST_ASSERT(name->name->identifiers[0].value == T("main"));
		}
	}
}