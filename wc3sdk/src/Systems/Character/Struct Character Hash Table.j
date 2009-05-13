/// @todo Compare this library with the ALibraryInterfaceInterfaceHashTable library
library AStructSystemsCharacterCharacterHashTable requires AStructCoreGeneralHashTable

	private struct ACharacterHashTable
		private static AHashTable instance = 0

		private static method create takes nothing returns ACharacterHashTable
			return 0
		endmethod

		private method onDestroy takes nothing returns nothing
		endmethod

		public static method getInstance takes nothing returns AHashTable
			if (ACharacterHashTable.instance == 0) then
				set ACharacterHashTable.instance = AHashTable.create("ACharacterHashTable")
			endif
			return ACharacterHashTable.instance
		endmethod
	endstruct

	function AGetCharacterHashTable takes nothing returns AHashTable
		return ACharacterHashTable.getInstance()
	endfunction

endlibrary