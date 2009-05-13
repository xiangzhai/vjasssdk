library AStructCoreInterfaceInterfaceHashTable requires AStructCoreGeneralHashTable

	private struct AInterfaceHashTable
		private static AHashTable instance = 0

		private static method create takes nothing returns AInterfaceHashTable
			return 0
		endmethod

		private method onDestroy takes nothing returns nothing
		endmethod

		public static method getInstance takes nothing returns AHashTable
			if (AInterfaceHashTable.instance == 0) then
				set AInterfaceHashTable.instance = AHashTable.create("AInterfaceHashTable")
			endif
			return AInterfaceHashTable.instance
		endmethod
	endstruct

	function AGetInterfaceHashTable takes nothing returns AHashTable
		return AInterfaceHashTable.getInstance()
	endfunction

endlibrary