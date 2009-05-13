library AStructSystemsWorldWorldHashTable requires AStructCoreGeneralHashTable

	private struct AWorldHashTable
		private static AHashTable instance = 0

		private static method create takes nothing returns AWorldHashTable
			return 0
		endmethod

		private method onDestroy takes nothing returns nothing
		endmethod

		public static method getInstance takes nothing returns AHashTable
			if (AWorldHashTable.instance == 0) then
				set AWorldHashTable.instance = AHashTable.create("AWorldHashTable")
			endif
			return AWorldHashTable.instance
		endmethod
	endstruct

	function AGetWorldHashTable takes nothing returns AHashTable
		return AWorldHashTable.getInstance()
	endfunction

endlibrary