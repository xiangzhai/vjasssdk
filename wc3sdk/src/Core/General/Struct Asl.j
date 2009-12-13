library AStructCoreGeneralAsl requires optional ALibraryCoreDebugMisc, ALibraryCoreStringConversion

	struct Asl
		public static constant string version = "1.1"
		public static constant string maintainer = "Tamino Dauth"
		public static constant string website = "http://sourceforge.net/projects/vjasssdk/"

		private static method create takes nothing returns thistype
			return 0
		endmethod

		private method onDestroy takes nothing returns nothing
		endmethod

		public static method init takes nothing returns nothing
static if (DEBUG_MODE) then
			call thistype.showInformation()
endif
		endmethod

		// static constant members

		public static constant method useRtc takes nothing returns boolean
			return A_RTC
		endmethod

		public static constant method useJapi takes nothing returns boolean
			return A_JAPI
		endmethod

		public static constant method useObjects takes nothing returns boolean
			return A_OBJECTS
		endmethod

		public static constant method useDebugHandles takes nothing returns boolean
			return A_DEBUG_HANDLES
		endmethod

		// static methods

static if (DEBUG_MODE) then
		public static method showInformation takes nothing returns nothing
			call Print("Advanced Script Library")
			call Print(StringArg(tr("Version: %s"), thistype.version))
			call Print(StringArg(tr("Maintainer: %s"), thistype.maintainer))
			call Print(StringArg(tr("Website: %s"), thistype.website))
		endmethod
endif
	endstruct

endlibrary