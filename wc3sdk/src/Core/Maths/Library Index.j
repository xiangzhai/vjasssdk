library ALibraryCoreMathsIndex requires optional ALibraryCoreDebugMisc

	/// @todo private causes an error
	debug function DebugCheckIndex takes integer index returns nothing
		debug if (index > JASS_MAX_ARRAY_SIZE) then
			debug call Print("Index is too high: " + I2S(index))
		debug endif
	debug endfunction

	function Index2D takes integer value0, integer value1, integer maxValue1 returns integer
		local integer index = (value0 * maxValue1) + value1
		debug call DebugCheckIndex(index)
		return index
	endfunction

	function Index3D takes integer value0, integer value1, integer value2, integer maxValue1, integer maxValue2 returns integer
		local integer index = (value0 * (maxValue1 * maxValue2)) + (value1 * maxValue2) + value2
		debug call DebugCheckIndex(index)
		return index
	endfunction

	function Index4D takes integer value0, integer value1, integer value2, integer value3, integer maxValue1, integer maxValue2, integer maxValue3 returns integer
		local integer index = (value0 * (maxValue1 * maxValue2 * maxValue3)) + (value1 * (maxValue2 * maxValue3)) + (value2 * maxValue3) + value3
		debug call DebugCheckIndex(index)
		return index
	endfunction

endlibrary