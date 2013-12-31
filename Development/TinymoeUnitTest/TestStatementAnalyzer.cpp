#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;

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