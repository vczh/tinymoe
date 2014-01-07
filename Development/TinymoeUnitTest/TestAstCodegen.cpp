#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;
using namespace tinymoe::compiler;

void CodeGen(vector<string>& codes, string name)
{
	CodeError::List errors;
	auto assembly = SymbolAssembly::Parse(codes, errors);
	TEST_ASSERT(errors.size() == 0);
	TEST_ASSERT(assembly->symbolModules.size() == codes.size());

	auto ast = GenerateAst(assembly);
	ofstream o("TestResult." + name + ".test.txt");
	ast->Print(o, 0);
}

TEST_CASE(TestStandardLibraryAstCodegen)
{
	vector<string> codes;
	codes.push_back(GetCodeForStandardLibrary());
	CodeGen(codes, "StandardLibraryAst");
}

TEST_CASE(TestMultipleDispatchAstCodegen)
{
	vector<string> codes;
	codes.push_back(GetCodeForStandardLibrary());

	codes.push_back(R"tinymoe(
module geometry
using standard library

phrase square root of (number)
	redirect to "sqrt"
end

type rectangle
	width
	height
end

type triangle
	a
	b
	c
end

type circle
	radius
end

phrase area of (shape)
	raise "This is not a shape."
end

phrase area of (shape : rectangle)
	set the result to field width of shape * field height of shape
end

phrase area of (shape : triangle)
	set a to field a of shape
	set b to field b of shape
	set c to field c of shape
	set p to (a + b + c) / 2
	set the result to square root of (p * (p - a) * (p - b) * (p - c))
end

phrase area of (shape : circle)
	set r to field radius of shape
	set the result to r * r * 3.14
end

phrase (a) and (b) is the same shape
	set the result to false
end

phrase (a : rectangle) and (b : rectangle) is the same shape
	set the result to true
end

phrase (a : triangle) and (b : triangle) is the same shape
	set the result to true
end

phrase (a : circle) and (b : circle) is the same shape
	set the result to true
end

)tinymoe");

	CodeGen(codes, "MultipleDispatchAst");
}