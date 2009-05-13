/// @todo Make a new test. Container system has changed …
/// Test completed.
/// 2008-02-26
/// 2009-03-08 Tamino Dauth <tamino@cdauth.de>
/// Revised test.
library ALibraryCoreDebugContainer requires ALibraryCoreDebugMisc

	//! runtextmacro A_CONTAINER("integer", "Integer", "1000")

	private function Compare takes integer element, integer comparedElement returns boolean
		return (element < comparedElement)
	endfunction

	private function Show takes integer element returns nothing
		debug call Print("Show " + I2S(element))
	endfunction

	private function SearchForTen takes integer element returns boolean
		return (element == 10)
	endfunction

	private function Operation takes integer element0, integer element1 returns integer
		return element0 + element1
	endfunction

	private function SearchForBiggerElement takes integer element0, integer element1 returns boolean
		return (element0 == element1)
	endfunction

	/// @todo Add tests for all container methods.
	function GetContainerDebug takes nothing returns nothing
		local integer i
		local AIntegerContainer Container1 = AIntegerContainer.create()
		local AIntegerContainer Container2 = AIntegerContainer.create()
		local AIntegerContainer Container3 = AIntegerContainer.create() //overloading
		//Operator overloading - Works
		debug call Print("operator overloading")
		set i = 0
		loop
			exitwhen (i == Container1.getSize())
			set Container1[i] = GetRandomInt(0, 1000)
			set i = (i + 1)
		endloop
		call Container1.forEach(0, Container1.getSize(), AIntegerContainerUnaryFunction.Show)
		//sort - Works
		debug call Print("sort")
		call Container1.sort(0, Container1.getSize(), AIntegerContainerBinaryPredicate.Compare)
		call Container1.forEach(0, Container1.getSize(), AIntegerContainerUnaryFunction.Show)
		//fill - Works
		debug call Print("fill")
		call Container2.fill(0, Container2.getSize(), 100)
		call Container2.forEach(0, Container2.getSize(), AIntegerContainerUnaryFunction.Show)
		//reverse - Works
		debug call Print("reverse")
		call Container1.reverse(5, (Container1.getSize() - 5))
		call Container1.forEach(5, (Container1.getSize() - 5), AIntegerContainerUnaryFunction.Show)
		//find - Works
		debug call Print("find")
		set Container1[0] = 10
		if (Container1.find(0, Container1.getSize(), 10) != -1) then
			debug call Print("find does work")
		endif
		//find_if - Works
		debug call Print("find_if")
		set Container2[0] = 10
		if (Container2.findIf(0, Container2.getSize(), AIntegerContainerUnaryPredicate.SearchForTen) != -1) then
			debug call Print("findIf does work.")
		endif
		//equal - Works
		debug call Print("equal")
		call Container1.fill(0, Container1.getSize(), 10)
		call Container2.fill(0, Container1.getSize(), 10) //Using the size of container1
		if (Container1.equal(0, Container1.getSize(), 0, Container2)) then
			debug call Print("equal does work.")
		endif
		//copy - Works
		debug call Print("copy")
		call Container2.fill(0, Container2.getSize(), 20)
		call Container2.copy(0, Container1.getSize(), 0, Container1)
		call Container1.forEach(0, Container1.getSize(), AIntegerContainerUnaryFunction.Show)
		//fill number test - Works
		debug call Print("fill number test")
		call Container2.fill(4, (Container2.getSize() - 4), 120)
		call Container2.forEach(4, (Container2.getSize() - 4), AIntegerContainerUnaryFunction.Show)
		//operation - Untested
		debug call Print("operation")
		debug call Print("Result: " + I2S(Container2.operation(0, Container2.getSize(), 10, AIntegerContainerOperation.Operation)) + ".")
		//adjacent find - Untested
		debug call Print("adjacent find")
		set Container2[0] = 10
		set Container2[1] = 10
		debug call Print("Result: " + I2S(Container2.adjacentFind(0, Container2.getSize())) + ".")
		//adjacent find if - Untested
		debug call Print("adjacent find if")
		debug call Print("Result: " + I2S(Container2.adjacentFindIf(0, Container2.getSize(), AIntegerContainerBinaryPredicate.SearchForBiggerElement)) + ".")
	endfunction

endlibrary