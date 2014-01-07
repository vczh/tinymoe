tinymoe
=======

###English-like programming language
######with dynamic typing and build-in continuation

Here are two samples to introduce this language:

*Hello world

	module hello world
	using standard library

	sentence print (message)
		redirect to "printf"
	end
	
	phrase sum from (start) to (end)
		set the result to 0
		repeat with the current number from start to end
			add the current number to the result
		end
	end
	
	phrase main
		print "1+ ... +100 = " & sum from 1 to 100
	end

*Geometry

	module geometry
	using standard library

	phrase square root of (number)
		redirect to "sqrt"
	end

	sentence print (message)
		redirect to "printf"
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

	phrase main
		set shape one to new triangle of (2, 3, 4)
		set shape two to new rectangle of (1, 2)
		if shape one and shape two is the same shape
			print "This world is mad!"
		end
	end