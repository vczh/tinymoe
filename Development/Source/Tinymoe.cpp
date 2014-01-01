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
symbol exiting function

type continuation state
	flag
	exception
	continuation
end

sentence reset continuation state (state) to (flag)
	set field flag of state to flag
	set field exception of state to null
	set field continuation of state to null
end

cps (state) (continuation)
sentence break
	reset continuation state state to breaking repeating
end

cps (state) (continuation)
sentence continue
	reset continuation state state to continuing repeating
end

cps (state) (continuation)
sentence raise (exception)
	reset continuation state state to raising exception
	set field exception of state to exception
end

cps (state) (continuation)
sentence exit
	reset continuation state state to exiting function
end

cps (state)
block (body) repeat : repeat statement
	call invoke body with (state)
	select field flag of state
		case breaking repeating
			reset continuation state state to null
		case continuing repeating
			reset continuation state state to null
			tail call invoke repeat statement with (state, body)
	end
end

block (sentence body) repeat while (expression condition)
	repeat
		if not condition
			break
		end
		body
	end
end

block (sentence deal with (item)) repeat with (argument item) from (lower bound) to (upper bound)
	set the current number to lower bound
	repeat while the current number <= upper bound
		deal with the current number
	end
end

block (phrase deal with (item)) repeat with (argument item) in (items)
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
block (sentence body) else if (condition)
	set the result to not signal and condition
	select the result
		case true
			body
	end
end

category (signal)
	follow IFELSE if
	closable
block (sentence body) else
	set the result to not signal
	select the result
		case true
			body
	end
end

cps (state)
category
	start SEH try
block (sentence body) try
	body
	if field flag of state = raising exception
		set the result to field exception of state
		reset continuation state state to null
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

sentence add (value) to (assignable variable)
	set variable to variable + value
end

sentence substract (value) from (assignable variable)
	set variable to variable - value
end
)tinymoe";
	}
}