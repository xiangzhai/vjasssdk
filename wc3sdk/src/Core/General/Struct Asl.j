library AStructCoreGeneralAsl requires ALibraryCoreDebugMisc, ALibraryCoreStringConversion

	struct Asl
		public static constant string version = "0.1"
		public static constant string maintainer = "Tamino Dauth"
		public static constant string website = "http://sourceforge.net/projects/vjasssdk/"

		private static method create takes nothing returns thistype
			return 0
		endmethod
		
		private method onDestroy takes nothing returns nothing
		endmethod
		
		public static method init takes nothing returns nothing
			debug call Print(tr("Initializing Advanced Script Library"))
			debug call Print(StringArg(tr("Version: %s"), thistype.version))
			debug call Print(StringArg(tr("Maintainer: %s"), thistype.maintainer))
			debug call Print(StringArg(tr("Website: %s"), thistype.website))
		endmethod
		
		//static members
		
		public static constant method useRtc takes nothing returns boolean
			return A_RTC
		endmethod
		
		public static constant method useJapi takes nothing returns boolean
			return A_JAPI
		endmethod

		public static constant method useObjects takes nothing returns boolean
			return A_OBJECTS
		endmethod

		public static constant method useDebug takes nothing returns boolean
			return A_DEBUG
		endmethod

		public static constant method useDebugHandles takes nothing returns boolean
			return A_DEBUG_HANDLES
		endmethod
	endstruct

endlibrary