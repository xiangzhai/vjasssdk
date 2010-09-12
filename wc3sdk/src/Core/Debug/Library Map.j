library ALibraryCoreDebugMap requires AStructCoreDebugBenchmark, ALibraryCoreDebugMisc, ACoreString, AStructCoreGeneralMap

	function AMapDebug takes nothing returns nothing
static if (DEBUG_MODE) then
		local AUnitMap unitMap = AUnitMap.create()
		local AUnitMapIterator iterator
		call Print("Map debug:")
		set unitMap["Test"] = null
		call Print("Pushed back one element, map size " + I2S(unitMap.size()) + ".")
		call Print("Unit name " + GetUnitName(unitMap["Test"]) + ".")
		set iterator = unitMap.end()
		call unitMap.erase(iterator)
		call iterator.destroy()
		call Print("Popped back one element, map size " + I2S(unitMap.size()) + ".")

		// set new elements
		loop
			exitwhen(unitMap.size() == 10)
			set unitMap["I" + I2S(unitMap.size() - 1)] = null
		endloop

		call Print("Unit map size " + I2S(unitMap.size()) + " after setting new elements.")

		// clean up
		loop
			exitwhen (unitMap.empty())
			set iterator = unitMap.end()
			call unitMap.erase(iterator)
			call iterator.destroy()
		endloop

		call Print("Unit map size " + I2S(unitMap.size()) + " after cleaning up.")

		call unitMap.destroy()

endif
	endfunction


endlibrary