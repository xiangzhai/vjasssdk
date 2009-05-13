library AStructSystemsGuiImage requires ALibraryCoreInterfaceImage, AStructSystemsGuiWidget

	struct AImage extends AWidget
		//dynamic members
		private string m_file
		private integer red
		private integer green
		private integer blue
		private integer alpha
		//members
		private image usedImage

		//dynamic members
		
		public method setFile takes string file returns nothing
			set this.m_file = file
			if (this.usedImage != null) then //file is not dynamic :-(
				call DestroyImage(this.usedImage)
				set this.usedImage = null
			endif
			set this.usedImage = CreateImageForPlayer(this.getUser(), file, this.getMainWindow().getX(this.getX()), this.getMainWindow().getY(this.getY()), 0.0, this.getSizeX(), this.getSizeY())
		endmethod
		
		public method file takes nothing returns string
			return this.m_file
		endmethod
			
		public method setColour takes integer red, integer green, integer blue, integer alpha returns nothing
			call SetImageColor(this.usedImage, red, green, blue, alpha)
		endmethod

		public method getRed takes nothing returns integer
			return this.red
		endmethod

		public method getGreen takes nothing returns integer
			return this.green
		endmethod

		public method getBlue takes nothing returns integer
			return this.blue
		endmethod

		public method getAlpha takes nothing returns integer
			return this.alpha
		endmethod

		//methods

		public stub method show takes nothing returns nothing
			call super.show()
			if (this.usedImage != null) then
				call ShowImage(this.usedImage, true)
			endif
		endmethod

		public stub method hide takes nothing returns nothing
			call super.hide()
			if (this.usedImage != null) then
				call ShowImage(this.usedImage, false)
			endif
		endmethod

		public static method create takes AMainWindow mainWindow, real x, real y, real sizeX, real sizeY, AWidgetOnHitAction onHitAction, AWidgetOnTrackAction onTrackAction returns AImage
			local AImage this = AImage.allocate(mainWindow, x, y, sizeX, sizeY, onHitAction, onTrackAction)
			//members
			set this.usedImage = null

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			if (this.usedImage != null) then
				call DestroyImage(this.usedImage)
				set this.usedImage = null
			endif
		endmethod
	endstruct

endlibrary