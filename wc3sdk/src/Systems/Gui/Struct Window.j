library AStructSystemsGuiWindow requires AStructSystemsGuiFrame, AStructSystemsGuiImage, AStructSystemsGuiWidget

	struct AWindow extends AFrame
		private AImage m_backgroundImage

		public static method create takes AMainWindow mainWindow, real x, real y, real width, real height, string backgroundImageFilePath returns thistype
			local thistype this = thistype.allocate(mainWindow, x, y, width, height)
			if (backgroundImageFilePath != null) then
				set this.m_backgroundImage = AImage.create(mainWindow, x, y, width, height, null, 0, 0)
				call this.m_backgroundImage.setFile(backgroundImageFilePath)
			endif
			return this
		endmethod
	endstruct

endlibrary