#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;

SymbolAssembly::Ptr CreateAssemblyFromFiles(vector<string>& codes)
{
	Module::List modules;
	CodeFile::List codeFiles;
	CodeError::List errors;

	for (auto code : codes)
	{
		auto codeFile = CodeFile::Parse(code, errors);
		TEST_ASSERT(errors.size() == 0);

		auto module = Module::Parse(codeFile, errors);
		TEST_ASSERT(errors.size() == 0);

		codeFiles.push_back(codeFile);
		modules.push_back(module);
	}

	auto assembly = make_shared<SymbolAssembly>();
	assembly->InstallModules(modules, codeFiles, errors);
	TEST_ASSERT(errors.size() == 0);
	TEST_ASSERT(assembly->symbolModules.size() == codes.size());

	auto item = make_shared<GrammarStackItem>();
	item->FillPredefinedSymbols();
	auto stack = make_shared<GrammarStack>();
	stack->Push(item);

	for (auto module : assembly->symbolModules)
	{
		module->Build(stack, errors);
		TEST_ASSERT(errors.size() == 0);
		TEST_ASSERT(stack->stackItems.size() == 1);
	}

	return assembly;
}

TEST_CASE(TestParseStandardLibraryModule)
{
	vector<string> codes;
	codes.push_back(GetCodeForStandardLibrary());
	CreateAssemblyFromFiles(codes);
}

TEST_CASE(TestParseHelloWorldModule)
{
	string code = R"tinymoe(
module hello world
using standard library

phrase sum from (start) to (end)
    set the result to 0
    repeat with the current number from start to end
        add the current number to the result
    end
end

phrase main
    print "1+ ... +100 = " & sum from 1 to 100
end
)tinymoe";
	
	vector<string> codes;
	codes.push_back(GetCodeForStandardLibrary());
	codes.push_back(code);
	auto assembly = CreateAssemblyFromFiles(codes);
}