library ALibraryCoreDebugList requires ALibraryCoreDebugMisc, AStructCoreDebugBenchmark, AStructCoreGeneralList, AStructCoreGeneralMap, AStructCoreGeneralVector

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

	private function mapInsertionsSpeedTest takes nothing returns ABenchmark
		local ABenchmark benchmark = ABenchmark.create("Map insertions speed test")
		local AIntegerStringMap map = AIntegerStringMap.create()
		local AIntegerStringMapIterator iterator
		call benchmark.start()
		loop
			exitwhen (map.size() == insertions)
			set iterator = map.begin()
			call map.insert(iterator, "Key", 10)
			call iterator.destroy()
		endloop
		call benchmark.stop()
		call map.destroy()
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
		local ABenchmark mapBenchmark = mapInsertionsSpeedTest()
		local ABenchmark vectorBenchmark = vectorInsertionsSpeedTest()
		call listBenchmark.show()
		call mapBenchmark.show()
		call vectorBenchmark.show()
		call listBenchmark.destroy()
		call mapBenchmark.destroy()
		call vectorBenchmark.destroy()
	endfunction

endlibrary