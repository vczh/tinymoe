tinymoe
=======

###English-like programming language
######with dynamic typing and build-in continuation

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
