library AStructCoreGeneralAsl requires ALibraryCoreDebugMisc, ALibraryCoreStringConversion

	struct Asl
		public static constant string version = "0.1"
		public static constant string maintainer = "Tamino Dauth"
		public static constant string website = "http://sourceforge.net/projects/vjasssdk/"
		//static members
		private static boolean m_useRtc
		private static boolean m_useJapi

		private static method create takes nothing returns thistype
			return 0
		endmethod
		
		private method onDestroy takes nothing returns nothing
		endmethod
		
		public static method init takes boolean useRtc, boolean useJapi returns nothing
			//static members
			set thistype.m_useRtc = useRtc
			set thistype.m_useJapi = useJapi
			
			debug call Print(tr("Initializing Advanced Script Library"))
			debug call Print(StringArg(tr("Version: %s"), thistype.version))
			debug call Print(StringArg(tr("Maintainer: %s"), thistype.maintainer))
			debug call Print(StringArg(tr("Website: %s"), thistype.website))
		endmethod
		
		//static members
		
		public static method useRtc takes nothing returns boolean
			return thistype.m_useRtc
		endmethod
		
		public static method useJapi takes nothing returns boolean
			return thistype.m_useJapi
		endmethod
	endstruct

endlibrary