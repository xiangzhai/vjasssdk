library AStructCoreGeneralAsl requires ALibraryCoreDebugMisc, ALibraryCoreStringConversion

	struct Asl
		public static constant string version = "0.1"
		public static constant string maintainer = "Tamino Dauth"
		public static constant string website = "http://sourceforge.net/projects/vjasssdk/"
		//static members
		private static boolean m_usingRtc
		private static boolean m_usingJapi

		private static method create takes nothing returns thistype
			return 0
		endmethod
		
		private method onDestroy takes nothing returns nothing
		endmethod
		
		public static method init takes boolean usingRtc, boolean usingJapi returns nothing
			//static members
			set thistype.m_usingRtc = usingRtc
			set thistype.m_usingJapi = usingJapi
			
			debug call Print(tr("Initializing Advanced Script Library"))
			debug call Print(StringArg(tr("Version: %s"), thistype.version))
			debug call Print(StringArg(tr("Maintainer: %s"), thistype.maintainer))
			debug call Print(StringArg(tr("Website: %s"), thistype.website))
		endmethod
		
		//static members
		
		public static method usingRtc takes nothing returns boolean
			return thistype.m_usingRtc
		endmethod
		
		public static method usingJapi takes nothing returns boolean
			return thistype.m_usingJapi
		endmethod
	endstruct

endlibrary