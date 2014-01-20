#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;
using namespace tinymoe::compiler;
using namespace tinymoe::ast;

extern void GenerateCSharpCode(AstAssembly::Ptr assembly, ostream& o);

void CodeGen(vector<string>& codes, string name)
{
	CodeError::List errors;
	auto assembly = SymbolAssembly::Parse(codes, errors);
	TEST_ASSERT(errors.size() == 0);
	TEST_ASSERT(assembly->symbolModules.size() == codes.size());

	auto ast = GenerateAst(assembly);
	{
		ofstream o("../CSharpCodegenTest/" + name + "/GeneratedAst.txt");
		ast->Print(o, 0);
	}
	{
		ofstream o("../CSharpCodegenTest/" + name + "/TinymoeProgram.cs");
		GenerateCSharpCode(ast, o);
	}
}

/*************************************************************
Hello World
*************************************************************/

TEST_CASE(TestStandardLibraryAstCodegen)
{
	vector<string> codes;
	codes.push_back(GetCodeForStandardLibrary());
	codes.push_back(R"tinymoe(
module hello world
using standard library

sentence print (message)
	redirect to "Print"
end

phrase sum from (first number) to (last number)
    set the result to 0
    repeat with the current number from first number to last number
        add the current number to the result
    end
end

phrase main
    print "1+ ... +10 = " & sum from 1 to 10

	try
		print "I will raise an exception."
		raise "exception"
		print "The world is mad!"
	catch exception
		print "So the exception will be caught"
	end

	try
		print "I will not raise an exception."
		print "So there is no exception to catch"
	catch exception
		print "The world is mad!"
	end
end

)tinymoe");
	CodeGen(codes, "StandardLibraryAst");
}

/*************************************************************
Multiple Dispatch
*************************************************************/

TEST_CASE(TestMultipleDispatchAstCodegen)
{
	vector<string> codes;
	codes.push_back(GetCodeForStandardLibrary());

	codes.push_back(R"tinymoe(
module geometry
using standard library

phrase square root of (number)
	redirect to "Sqrt"
end

sentence print (message)
	redirect to "Print"
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

phrase (a) and (b) are the same shape
	set the result to false
end

phrase (a : rectangle) and (b : rectangle) are the same shape
	set the result to true
end

phrase (a : triangle) and (b : triangle) are the same shape
	set the result to true
end

phrase (a : circle) and (b : circle) are the same shape
	set the result to true
end

phrase main
	set shape one to new triangle of (2, 3, 4)
	set shape two to new rectangle of (1, 2)
	if shape one and shape two are the same shape
		print "This world is mad!"
	else
		print "Triangle and rectangle are not the same shape!"
	end
end

)tinymoe");

	CodeGen(codes, "MultipleDispatchAst");
}

/*************************************************************
Yield Return
*************************************************************/

TEST_CASE(TestYieldReturnAstCodegen)
{
	vector<string> codes;
	codes.push_back(GetCodeForStandardLibrary());

	codes.push_back(R"tinymoe(
module enumerable
using standard library

symbol yielding return
symbol yielding break
 
type enumerable collection
    body
end
 
type collection enumerator
    value
	coroutine
end
 
cps (state) (continuation)
category
    inside ENUMERATING
sentence yield return (value)
	pause coroutine to yielding return with continuation and value
end

cps (state) (continuation)
category
    inside ENUMERATING
sentence yield break
	pause coroutine to yielding break with continuation and null
end
 
phrase new enumerator from (enumerable)
	set coroutine to new coroutine from (field body of enumerable)
    set the result to new collection enumerator of (null, coroutine)
end

cps (state) (continuation)
sentence move (enumerator) to the next
	set coroutine to field coroutine of enumerator
	if not (coroutine coroutine stopped)
		run coroutine coroutine

		select field flag of state
			case yielding return
				set field value of enumerator to field argument of state
				continue coroutine coroutine using state state
			case yielding break
				set field value of enumerator to null
				stop coroutine coroutine
			case null
				set field value of enumerator to null
				stop coroutine coroutine
			case else
				fall into the previous trap
		end
	end
end
 
phrase (enumerator) reaches the end
    set the result to coroutine field coroutine of enumerator stopped
end

phrase current value of (enumerator)
	set the result to field value of (field value of enumerator)
end
 
cps (state)
category
    start ENUMERATING
    closable
block (body) create enumerable to (assignable receiver)
    set receiver to new enumerable collection of ()
    set field body of receiver to body
end

block (sentence deal with (item)) repeat with (argument item) in (items : enumerable collection)
    set enumerator to new enumerator from items
    repeat while not (enumerator reaches the end)
        move enumerator to the next
        deal with current value of enumerator
    end
end

sentence print (message)
	redirect to "Print"
end
 
phrase main
    create enumerable to numbers
        repeat with i from 1 to 10
            print "Enumerating " & i
            yield return i
        end
    end
    
	repeat with number in numbers
		if number >= 5
			break
		end
		print "Printing " & number
	end
end

)tinymoe");

	CodeGen(codes, "YieldReturnAst");
}

/*************************************************************
Unit Test
*************************************************************/

TEST_CASE(TestUnitTestAstCodegen)
{
	vector<string> codes;
	codes.push_back(GetCodeForStandardLibrary());
	codes.push_back(R"tinymoe(
module unit test
using standard library

sentence print (message)
	redirect to "Print"
end

block (sentence run the test case) test case (name)
	try
		run the test case
		print "PASS: " & name
	catch exception
		select exception
			case "AssertionFailure"
				print "FAIL: " & name
			case else
				print "HALT: " & name
		end
	end
end

sentence assert (actual value) should be (expected value)
	if actual value <> expected value
		assert fail
	end
end

sentence assert fail
	raise "AssertionFailure"
end

phrase main
	test case "1+1=2"
		assert (1 + 1) should be 2
	end

	test case "Break should stop the repeating (1)"
		set sum to 0
		repeat with i from 1 to 100
			if i > 10
				break
			end
			add i to sum
		end
		assert sum should be 55
	end

	test case "Break should stop the repeating (2)"
		set sum to 0
		repeat with i from 1 to 100
			add i to sum
			if i >= 10
				break
			end
		end
		assert sum should be 55
	end

	test case "Continue should restart the next repeating"
		set sum to 0
		repeat with i from 1 to 100
			if i > 10
				continue
			end
			add i to sum
		end
		assert sum should be 55
	end

	test case "Only one branch of the if-else statement will be executed"
		set a to 0
		set b to 0
		set c to 0

		if true
			set a to 1
		else if true
			set a to 2
		else
			set a to 3
		end

		if false
			set b to 1
		else if true
			set b to 2
		else
			set b to 3
		end

		if false
			set c to 1
		else if false
			set c to 2
		else
			set c to 3
		end

		assert a should be 1
		assert b should be 2
		assert c should be 3
	end

	test case "A raised exception should be able to catch"
		set e to 0
		try
			raise "exception"
			set e to 0
		catch exception
			set e to exception
		end

		assert e should be "exception"
	end

	test case "Finally should always be executed"
		set e to 0
		try
			add 1 to e
			raise "exception"
			add 2 to e
		catch exception
			add 4 to e
		finally
			add 8 to e
		end

		assert e should be 13
	end

	test case "Only one branch of the try-else statement will be executed"
		set a to 0
		set b to 0
		set c to 0

		try
			set a to 1
		else try
			set a to 2
		catch exception
			set a to 3
		end

		try
			set b to 1
			raise "exception"
		else try
			set b to 2
		catch exception
			set b to 3
		end

		try
			set c to 1
			raise "exception"
		else try
			set c to 2
			raise "exception"
		catch exception
			set c to 3
		end

		assert a should be 1
		assert b should be 2
		assert c should be 3
	end

	test case "Exit block should jump out of the correct box"
		set e to 0
		named block one
			add 1 to e
			named block two
				add 2 to e
				named block three
					add 4 to e
					exit block two
					add 8 to e
				end
				add 16 to e
			end
			add 32 to e
		end
		add 64 to e

		assert e should be 103
	end
end

)tinymoe");
	CodeGen(codes, "UnitTestAst");
}