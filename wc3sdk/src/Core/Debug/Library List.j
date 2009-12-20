library ALibraryCoreDebugList requires ALibraryCoreDebugMisc, AStructCoreDebugBenchmark, AStructCoreGeneralList, AStructCoreGeneralVector

	globals
		private constant integer insertions = 100000
	endglobals

	private function listInsertionsSpeedTest takes nothing returns ABenchmark
		local ABenchmark benchmark = ABenchmark.create("List insertions speed test")
		local AIntegerList list = AIntegerList.create()
		call benchmark.start()
		loop
			exitwhen (list.size() == insertions)
			call list.pushBack(10)
		endloop
		call benchmark.stop()
		call list.destroy()
		return benchmark
	endfunction

	private function vectorInsertionsSpeedTest takes nothing returns ABenchmark
		local ABenchmark benchmark = ABenchmark.create("Vector insertions speed test")
		local AIntegerVector vector = AIntegerVector.create()
		call benchmark.start()
		loop
			exitwhen (vector.size() == insertions)
			call vector.pushBack(10)
		endloop
		call benchmark.stop()
		call vector.destroy()
		return benchmark
	endfunction

	function AListDebug takes nothing returns nothing
		local ABenchmark listBenchmark = listInsertionsSpeedTest()
		local ABenchmark vectorBenchmark = vectorInsertionsSpeedTest()
		call listBenchmark.show()
		call vectorBenchmark.show()
		call listBenchmark.destroy()
		call vectorBenchmark.destroy()
	endfunction

endlibrary