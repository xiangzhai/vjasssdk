library ALibraryCoreDebugExecution requires ALibraryCoreDebugMisc, AStructCoreDebugBenchmark

	globals
		private integer tests = 1000
	endglobals

	private function Test takes integer a, integer b, integer c returns nothing
		local integer result = 0
		local integer i = 0
		loop
			exitwhen (i == tests)
			set result = result + a + b + c
			set i = i + 1
		endloop
	endfunction

	function AExecution takes nothing returns nothing
		local ABenchmark executeBenchmark = ABenchmark.create("execute")
		local ABenchmark evaluateBenchmark = ABenchmark.create("evaluate")
		local ABenchmark callBenchmark = ABenchmark.create("call")
		call executeBenchmark.start()
		call Test.execute(10, 10, 10)
		call executeBenchmark.stop()
		call evaluateBenchmark.start()
		call Test.evaluate(10, 10, 10)
		call evaluateBenchmark.stop()
		call callBenchmark.start()
		call Test(10, 10, 10)
		call callBenchmark.stop()
		call executeBenchmark.show()
		call evaluateBenchmark.show()
		call callBenchmark.show()
		call executeBenchmark.destroy()
		call evaluateBenchmark.destroy()
		call callBenchmark.destroy()
	endfunction

endlibrary