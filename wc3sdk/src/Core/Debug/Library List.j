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
		local AUnitMap map = AUnitMap.create()
		local AUnitMapIterator iterator
		local integer i = 0
		call benchmark.start()
		loop
			exitwhen (map.size() == insertions)
			set iterator = map.begin()
			call map.insert(iterator, "Key" + I2S(i), null)
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
		local AIntegerList list
		local ABenchmark listBenchmark = listInsertionsSpeedTest()
		local ABenchmark mapBenchmark = mapInsertionsSpeedTest()
		local ABenchmark vectorBenchmark = vectorInsertionsSpeedTest()
		call listBenchmark.show()
		call mapBenchmark.show()
		call vectorBenchmark.show()
		call listBenchmark.destroy()
		call mapBenchmark.destroy()
		call vectorBenchmark.destroy()
		debug call Print("A_FOREACH test:")
		set list = AIntegerList.createWithSize(10, 0)
		//! runtextmacro A_FOREACH("list")
			// no trigger sleep here since iterator could be changed
			debug call Print("Data: " + I2S(AIntegerListIterator(aIterator).data()))
		//! runtextmacro A_FOREACH_END()
		debug call Print("A_REVERSE_FOREACH test:")
		call list.clear()
		call list.pushBack(1)
		call list.pushBack(2)
		call list.pushBack(3)
		call list.pushBack(4)
		call list.pushBack(5)
		call list.pushBack(6)
		//! runtextmacro A_REVERSE_FOREACH("list")
			// no trigger sleep here since iterator could be changed
			debug call Print("Data: " + I2S(AIntegerListIterator(aIterator).data()))
		//! runtextmacro A_REVERSE_FOREACH_END()
	endfunction

endlibrary