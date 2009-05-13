library AStructCoreDebugDebugHashTable requires AStructCoreGeneralHashTable

	//Just one instance
	//Does not extend AHashTable!
	private struct ADebugHashTable
		private static AHashTable instance

		private static method create takes nothing returns ADebugHashTable
			return 0
		endmethod

		private method onDestroy takes nothing returns nothing
		endmethod

		private static method onInit takes nothing returns nothing
			set ADebugHashTable.instance = AHashTable.create("ADebugHashTable")
		endmethod

		public static method getInstance takes nothing returns AHashTable
			return ADebugHashTable.instance
		endmethod
	endstruct

	function AGetDebugHashTable takes nothing returns AHashTable
		return ADebugHashTable.getInstance()
	endfunction

endlibrary