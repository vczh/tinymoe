#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;
using namespace tinymoe::compiler;
using namespace tinymoe::ast;

extern void WriteAnsiFile(string_t fileName, stringstream_t& ss);
extern string_t ReadAnsiFile(string_t fileName);
extern string_t GetCodeForStandardLibrary();
extern void GenerateCSharpCode(AstAssembly::Ptr assembly, ostream_t& o);

void CodeGen(vector<string_t>& codes, string_t name)
{
	CodeError::List errors;
	auto assembly = SymbolAssembly::Parse(codes, errors);
	TEST_ASSERT(errors.size() == 0);
	TEST_ASSERT(assembly->symbolModules.size() == codes.size());

	auto ast = GenerateAst(assembly);
	{
		stringstream_t o;
		Print(ast, o, 0);
		WriteAnsiFile(T("../CSharpCodegenTest/") + name + T("/GeneratedAst.txt"), o);
	}
	{
		stringstream_t o;
		GenerateCSharpCode(ast, o);
		WriteAnsiFile(T("../CSharpCodegenTest/") + name + T("/TinymoeProgram.cs"), o);
	}
}

/*************************************************************
Hello World
*************************************************************/

TEST_CASE(TestStandardLibraryAstCodegen)
{
	vector<string_t> codes;
	codes.push_back(GetCodeForStandardLibrary());
	codes.push_back(ReadAnsiFile(T("../TestCases/HelloWorld.txt")));
	CodeGen(codes, T("StandardLibraryAst"));
}

/*************************************************************
Multiple Dispatch
*************************************************************/

TEST_CASE(TestMultipleDispatchAstCodegen)
{
	vector<string_t> codes;
	codes.push_back(GetCodeForStandardLibrary());
	codes.push_back(ReadAnsiFile(T("../TestCases/MultipleDispatch.txt")));
	CodeGen(codes, T("MultipleDispatchAst"));
}

/*************************************************************
Yield Return
*************************************************************/

TEST_CASE(TestYieldReturnAstCodegen)
{
	vector<string_t> codes;
	codes.push_back(GetCodeForStandardLibrary());
	codes.push_back(ReadAnsiFile(T("../TestCases/Coroutine.txt")));
	CodeGen(codes, T("YieldReturnAst"));
}

/*************************************************************
Unit Test
*************************************************************/

TEST_CASE(TestUnitTestAstCodegen)
{
	vector<string_t> codes;
	codes.push_back(GetCodeForStandardLibrary());
	codes.push_back(ReadAnsiFile(T("../TestCases/UnitTest.txt")));
	CodeGen(codes, T("UnitTestAst"));
}