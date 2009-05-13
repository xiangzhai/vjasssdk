library AStructCoreEnvironmentEnvironmentHashTable requires AStructCoreGeneralHashTable

	private struct AEnvironmentHashTable
		private static AHashTable instance = 0

		private static method create takes nothing returns AEnvironmentHashTable
			return 0
		endmethod

		private method onDestroy takes nothing returns nothing
		endmethod

		public static method getInstance takes nothing returns AHashTable
			if (AEnvironmentHashTable.instance == 0) then
				set AEnvironmentHashTable.instance = AHashTable.create("AEnvironmentHashTable")
			endif
			return AEnvironmentHashTable.instance
		endmethod
	endstruct

	function AGetEnvironmentHashTable takes nothing returns AHashTable
		return AEnvironmentHashTable.getInstance()
	endfunction

endlibrary