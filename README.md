tinymoe
=======

### English-like programming language
###### For DSL (Domain Specific Language) building and embedding, with dynamic typing, multiple dispatching and build-in continuation.

Here are several samples to introduce this language:

#### Hello world

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

#### Geometry (Multiple dispatching)

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

#### Make your own "yield return" (coroutine)

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
					continue coroutine coroutine
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

#### Make your tinymoe-unit (Unit test and assertion)

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
	end
