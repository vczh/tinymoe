#include "Tinymoe.h"

namespace tinymoe
{
	/*************************************************************
	Helper Functions
	*************************************************************/

	string GetCodeForStandardLibrary()
	{
		return R"tinymoe(
module standard library

symbol breaking repeating
symbol continuing repeating
symbol raising exception
symbol exiting program
symbol exiting block

type continuation trap
	continuation
	previous trap
end

type continuation state
	flag
	argument
	continuation
	trap
end

sentence reset continuation state (state) to (flag)
	set field flag of state to flag
	set field argument of state to null
	set field continuation of state to null
end

cps (state) (continuation)
sentence trap (expression value)
	set the current trap to new continuation trap of (continuation, field trap of state)
	set field trap of state to the current trap
	set the result to value
end

cps (state) (continuation)
sentence fall into the previous trap
	set the current trap to field continuation of field trap of state
	set field trap of state to field previous trap of field trap of state
	call continuation the current trap of ()
end

cps (state) (continuation)
sentence copy continuation state (new state)
	set field flag of state to field flag of new state
	set field argument of state to field argument of new state
	set field continuation of state to field continuation of new state
end

cps (state) (continuation)
category
	inside REPEAT
sentence break
	reset continuation state state to breaking repeating
end

cps (state) (continuation)
category
	inside REPEAT
sentence continue
	reset continuation state state to continuing repeating
end

cps (state) (continuation)
sentence raise (exception)
	reset continuation state state to raising exception
	set field argument of state to exception
end

cps (state) (continuation)
sentence exit program
	reset continuation state state to exiting program
end

cps (state) (continuation)
category
	inside NAMEDBLOCK
sentence exit block (handle)
	reset continuation state state to exiting block
	set field argument of state to handle
end

cps (state)
category
	start REPEAT
	closable
block (body) repeat : repeat statement
	trap body of ()
	select field flag of state
		case breaking repeating
			reset continuation state state to null
		case continuing repeating
			reset continuation state state to null
			call repeat statement of (body)
		case null
			call repeat statement of (body)
		case else
			fall into the previous trap
	end
end

category
	start REPEAT
	closable
block (sentence body) repeat while (expression condition)
	repeat
		if not condition
			break
		end
		body
	end
end

category
	start REPEAT
	closable
block (sentence deal with (item)) repeat with (argument item) from (lower bound) to (upper bound)
	set the current number to lower bound
	repeat while the current number <= upper bound
		deal with the current number
		add 1 to the current number
	end
end

category
	start REPEAT
	closable
block (sentence deal with (item)) repeat with (argument item) in (items)
	raise "Only enumerable object (like containers) can be iterated using the repeat-with-in statement."
end

block (sentence deal with (item)) repeat with (argument item) in (items : array)
	repeat with index from 1 to length of array items
		deal with item index of array items
	end
end

category
	start IFELSE if
	closable
block (sentence body) if (condition)
	set the result to condition
	select the result
		case true
			body
	end
end

category (signal)
	follow IFELSE if
	start IFELSE if
	closable
block (sentence body) else if (expression condition)
	set the result to signal
	select the result
		case false
			set the result to condition
			select the result
				case true
					body
			end
	end
end

category (signal)
	follow IFELSE if
	closable
block (sentence body) else
	set the result to signal
	select the result
		case false
			body
	end
end

cps (state)
category
	start SEH try
block (body) try
	trap body of ()
	select field flag of state
		case raising exception
			set the result to field exception of state
			reset continuation state state to null
		case else
			fall into the previous trap
	end
end

cps (state)
category (signal)
	follow SEH try
	start SEH catch
	closable
block (sentence body) catch (argument exception)
	if signal <> null
		body
	end	
end

cps (state)
category (signal)
	follow SEH try
	follow SEH catch
	closable
block (sentence body) finally
	body
end

cps (state)
category
	start NAMEDBLOCK
	closable
block (body) named block (argument handle)
	set handle to new object of ()
	trap body of (handle)
	if field flag of state = exiting block and field argument of state = handle
		reset continuation state state to null
	else
		fall into the previous trap
	end
end

sentence add (value) to (assignable variable)
	set variable to variable + value
end

sentence substract (value) from (assignable variable)
	set variable to variable - value
end

phrase boolean of (value)
	redirect to "CastToBoolean"
end

phrase integer of (value)
	redirect to "CastToInteger"
end

phrase floating point of (value)
	redirect to "CastToFloat"
end

phrase string of (value)
	redirect to "CastToString"
end

)tinymoe";
	}
}