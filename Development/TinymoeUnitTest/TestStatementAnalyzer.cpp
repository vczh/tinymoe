#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;
using namespace tinymoe::compiler;

TEST_CASE(TestParseStandardLibraryModule)
{
	vector<string> codes;
	CodeError::List errors;
	codes.push_back(GetCodeForStandardLibrary());
	auto assembly = SymbolAssembly::Parse(codes, errors);
	TEST_ASSERT(errors.size() == 0);
	TEST_ASSERT(assembly->symbolModules.size() == 1);
}

TEST_CASE(TestParseHelloWorldModule)
{
	string code = R"tinymoe(
module hello world
using standard library

sentence print (message)
	redirect to "printf"
end

phrase sum from (first number) to (last number)
    set the result to 0
    repeat with the current number from first number to last number
        add the current number to the result
    end
end

phrase main
    print "1+ ... +100 = " & sum from 1 to 100
end
)tinymoe";

	vector<string> codes;
	CodeError::List errors;
	codes.push_back(GetCodeForStandardLibrary());
	codes.push_back(code);
	auto assembly = SymbolAssembly::Parse(codes, errors);
	TEST_ASSERT(errors.size() == 0);
	TEST_ASSERT(assembly->symbolModules.size() == 2);
}

TEST_CASE(TestParsePotentialAmbiguousModule)
{
	string code = R"tinymoe(
module hello world
using standard library

type dog
	name
end

sentence print (message)
	redirect to "printf"
end

phrase console input
	redirect to "scanf"
end

phrase main
    set kula to new dog
	set field name of kula to console input
	print "kula's name is " & field name of kula & "."
end
)tinymoe";

	vector<string> codes;
	CodeError::List errors;
	codes.push_back(GetCodeForStandardLibrary());
	codes.push_back(code);
	auto assembly = SymbolAssembly::Parse(codes, errors);
	TEST_ASSERT(errors.size() == 0);
	TEST_ASSERT(assembly->symbolModules.size() == 2);

	auto decl = assembly->symbolModules[1]->module->declarations[3];
	auto func = assembly->symbolModules[1]->declarationFunctions.find(decl)->second;
	TEST_ASSERT(func->function->name.size() == 1);
	TEST_ASSERT(dynamic_pointer_cast<NameFragment>(func->function->name[0])->name->GetName() == "main");
	for (auto s : func->statement->statements)
	{
		cout << "  > " << s->statementExpression->ToCode() << endl;
	}
}

TEST_CASE(TestParseConnectedBlocksModule)
{
	string code = R"tinymoe(
module hello world
using standard library

sentence print (message)
	redirect to "printf"
end

phrase console input
	redirect to "scanf"
end

phrase main
	set the score to console input
	if the score < 0 or the score > 100
		print "Illegal score."
	else if the score = 100
		print "Perfect!"
	else if the score >= 90
		print "Good!"
	else if the score >= 70
		print "Normal."
	else if the score >- 60
		print "OK."
	else
		print "Unacceptable."
	end
end
)tinymoe";

	vector<string> codes;
	CodeError::List errors;
	codes.push_back(GetCodeForStandardLibrary());
	codes.push_back(code);
	auto assembly = SymbolAssembly::Parse(codes, errors);
	TEST_ASSERT(errors.size() == 0);
	TEST_ASSERT(assembly->symbolModules.size() == 2);
}

TEST_CASE(TestParseCategorizedSentenceModule)
{
	string code = R"tinymoe(
module hello world
using standard library

sentence print (message)
	redirect to "printf"
end

phrase console input
	redirect to "scanf"
end

phrase main
	set lower bound to console input
	set upper bound to console input
	set sum to 0
	set the current number to lower bound

	repeat
		add the current number to sum
		add 1 to the current number
		if the current number > upper bound
			break
		end
	end
	print "The sum is " & sum & "."
end
)tinymoe";

	vector<string> codes;
	CodeError::List errors;
	codes.push_back(GetCodeForStandardLibrary());
	codes.push_back(code);
	auto assembly = SymbolAssembly::Parse(codes, errors);
	TEST_ASSERT(errors.size() == 0);
	TEST_ASSERT(assembly->symbolModules.size() == 2);
}

TEST_CASE(TestParseWrongModule)
{
	{
		string code = R"tinymoe(
module hello world
using standard library

phrase main
	if true
	catch the exception
	end
end
)tinymoe";

		vector<string> codes;
		CodeError::List errors;
		codes.push_back(GetCodeForStandardLibrary());
		codes.push_back(code);
		auto assembly = SymbolAssembly::Parse(codes, errors);
		TEST_ASSERT(errors.size() == 2);
		TEST_ASSERT(assembly->symbolModules.size() == 2);
	}
	{
		string code = R"tinymoe(
module hello world
using standard library

phrase main
	if true
		break
	end
end
)tinymoe";

		vector<string> codes;
		CodeError::List errors;
		codes.push_back(GetCodeForStandardLibrary());
		codes.push_back(code);
		auto assembly = SymbolAssembly::Parse(codes, errors);
		TEST_ASSERT(errors.size() == 1);
		TEST_ASSERT(assembly->symbolModules.size() == 2);
	}
}