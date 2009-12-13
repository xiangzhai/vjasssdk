library ALibraryCoreDebugList requires ALibraryCoreDebugMisc, AStructCoreGeneralList, AStructCoreGeneralVector

	globals
		private constant integer insertions = 1000000
	endglobals

	private function listInsertionsSpeedTest takes nothing returns ABenchmark
		local ABenchmark benchmark = ABenchmark.create("List insertions speed test")
		local AIntegerList list = AIntegerList.create()
		loop
			exitwhen (list.size() == insertions)
			call list.pushBack(10)
		endloop
		call list.destroy()
		return benchmark
	endfunction

	private function vectorInsertionsSpeedTest takes nothing returns ABenchmark
		local ABenchmark benchmark = ABenchmark.create("List insertions speed test")
		local AIntegerVector vector = AIntegerVector.create()
		loop
			exitwhen (vector.size() == insertions)
			call vector.pushBack(10)
		endloop
		call vector.destroy()
		return benchmark
	endfunction

	function AListDebug takes nothing returns nothing
		local ABenchmark listBenchmark = listInsertionsSpeedTest()
		local ABenchmark vectorBenchmark = vectorInsertionsSpeedTest()

	endfunction

endlibrary