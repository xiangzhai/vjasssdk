library AModuleCoreGeneralSystemStruct requires optional ALibraryCoreDebugMisc

	module ASystemStruct
		private static string m_name
		private static boolean m_initialized

		public static method initialized takes nothing returns boolean
			return thistype.m_initialized
		endmethod

		private static method onInit takes nothing returns nothing
			set thistype.m_name = null
			set thistype.m_initialized = false
		endmethod

		/// @todo Should only be accessable by struct, missing vJass feature.
		public static method setName takes string name returns nothing
			set thistype.m_name = name
		endmethod

		/// @todo Should only be accessable by struct, missing vJass feature.
		public static method initialize takes nothing returns nothing
static if (DEBUG_MODE) then
			if (thistype.m_initialized) then
				call Print("System struct \"" + thistype.m_name + "\" has been initialized although it was initialized before.")
			endif
endif
			set thistype.m_initialized = true
		endmethod

		/// @todo Should only be accessable by struct, missing vJass feature.
		public static method uninitialize takes nothing returns nothing
static if (DEBUG_MODE) then
			if (not thistype.m_initialized) then
				call Print("System struct \"" + thistype.m_name + "\" has been uninitialized although it was not initialized before.")
			endif
endif
			set thistype.m_initialized = false
		endmethod

static if (DEBUG_MODE) then
		/// @todo Should only be accessable by struct, missing vJass feature.
		public static method check takes string methodName returns boolean
			local boolean result = thistype.m_initialized
			if (not result) then
				call PrintFunctionError(methodName, "System struct \"" + thistype.m_name + "\" was not initialized.")
			endif
			return result
		endmethod
endif

	endmodule

endlibrary