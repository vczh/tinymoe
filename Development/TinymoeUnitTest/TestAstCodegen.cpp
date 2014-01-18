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
end

)tinymoe");
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
    current yielding result
	body
    continuation
end

type yielding result
	value
	trap
	fall back counter
end
 
cps (state) (continuation)
category
    inside ENUMERATING
sentence yield return (value)
    set field flag of state to yielding return
    set field continuation of state to continuation
    set field argument of state to new yielding result of (value, field trap of state)
	fall into the previous trap
end
 
cps (state) (continuation)
category
    inside ENUMERATING
sentence yield break
    reset continuation state state to yielding break
	fall into the previous trap
end
 
phrase new enumerator from (enumerable)
    set the result to new collection enumerator of (null, field body of enumerable, null)
end

phrase fall back counter between (top trap) and (current trap) from (counter)
	select top trap <> current trap
		case true
			set the result to fall back counter between (field previous trap of top trap) and current trap from (counter + 1)
		case false
			set the result to counter
	end
end

sentence set fall back counter of (state)
	set top trap to field trap of (field argument of state)
	set current trap to field trap of state
	set counter to (fall back counter between top trap and current trap from -1)
	set field fall back counter of (field argument of state) to counter
end

cps (state) (continuation)
sentence trap (expression value) with fall back enabled internal
	set the current trap to new continuation trap of (continuation, field trap of state)
	set field trap of state to the current trap
	set the result to value
end

cps (state) (continuation)
sentence trap (expression value) with fall back enabled
	trap value with fall back enabled internal
	set field trap of state to field previous trap of (field trap of state)
	set fall back counter of state
end

phrase restored trap (trap) with fall back (fall back trap) and counter (counter)
	select counter
		case 0
			set the result to trap
		case else
			set previous trap to restored trap trap with fall back (field previous trap of fall back trap) and counter (counter - 1)
			set the result to new continuation trap of (field continuation of fall back trap, previous trap)
	end
end

cps (state) (continuation)
sentence trap (expression value) with fall back (fall back) restored internal
	set the current trap to new continuation trap of (continuation, field trap of state)
	set new trap to restored trap (the current trap) with fall back (field trap of fall back) and counter (field fall back counter of fall back)
	set field trap of state to new trap
	set the result to value
end

cps (state) (continuation)
sentence trap (expression value) with fall back (fall back) restored
	trap value with fall back (fall back) restored internal
	set field trap of state to field previous trap of (field trap of state)
	set fall back counter of state
end

cps (state) (continuation)
sentence move (enumerator) to the next
	named block moving to the next
		if field body of enumerator <> null
			trap field body of enumerator of () with fall back enabled
			set field body of enumerator to null
		else if field continuation of enumerator <> null
			trap continuation field continuation of enumerator of (null) with fall back field current yielding result of enumerator restored
		else
			exit block moving to the next
		end

		select field flag of state
			case yielding return
				set field current yielding result of enumerator to field argument of state
				set field continuation of enumerator to field continuation of state
				reset continuation state state to null
			case yielding break
				set field current yielding result of enumerator to null
				set field continuation of enumerator to null
				reset continuation state state to null
			case null
				set field current yielding result of enumerator to null
				set field continuation of enumerator to null
			case else
				fall into the previous trap
		end
	end
end
 
phrase (enumerator) reaches the end
    set the result to field body of enumerator = null and field continuation of enumerator = null
end

phrase current value of (enumerator)
	select enumerator reaches the end
		case true
			set the result to null
		case false
			set the result to field value of (field current yielding result of enumerator)
	end
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
    repeat
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