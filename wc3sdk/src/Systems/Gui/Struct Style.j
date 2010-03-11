library AStructSystemsGuiStyle

	/**
	* Defines custom GUI style for a single main window.
	* Note that there're only pre-defined styles for some Warcraft 3 races at the moment.
	* GUI files of races "Demon", "Naga" and "Panda" are delivered with wc3sdk's data archives.
	* @see MainWindow
	*/
	struct AStyle
		public static constant integer typeHuman = 0
		public static constant integer typeOrc = 1
		public static constant integer typeUndead = 2
		public static constant integer typeNightElf = 3
		public static constant integer typeDemon = 4
		public static constant integer typeNaga = 5
		public static constant integer typeNerubian = 6
		public static constant integer typePanda = 7
		public static constant integer maxTypes = 8
		private string m_frameTopImageFilePath
		private string m_frameBottomImageFilePath
		private string m_frameLeftImageFilePath
		private string m_frameRightImageFilePath
		private string m_arrowTopImageFilePath
		private string m_arrowBottomImageFilePath
		private string m_arrowLeftImageFilePath
		private string m_arrowRightImageFilePath

		//! runtextmacro A_STRUCT_DEBUG("\"AStyle\"")

		public static method createHuman takes nothing returns thistype
			local thistype this = thistype.allocate()
			return this
		endmethod

		public static method createOrc takes nothing returns thistype
			local thistype this = thistype.allocate()
			return this
		endmethod

		public static method createUndead takes nothing returns thistype
			local thistype this = thistype.allocate()
			return this
		endmethod

		public static method createNightElf takes nothing returns thistype
			local thistype this = thistype.allocate()
			return this
		endmethod

		private static method create takes integer type returns thistype
			if (type == thistype.typeHuman) then
				return thistype.createHuman()
			elseif (type == thistype.typeOrc) then
				return thistype.createOrc()
			endif
			debug call thistype.staticPrint("Invalid style type: " + I2S(type) + ".")
			return 0
		endmethod
	endstruct

endlibrary