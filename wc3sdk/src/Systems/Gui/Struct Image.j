library AStructSystemsGuiImage requires ALibraryCoreInterfaceImage, AStructSystemsGuiWidget

	/// Uses the default Jass type @type image to treat images in GUIs.
	struct AImage extends AWidget
		//dynamic members
		private string m_file
		private integer m_red
		private integer m_green
		private integer m_blue
		private integer m_alpha
		//members
		private image m_image

		//dynamic members
		
		public method setFile takes string file returns nothing
			set this.m_file = file
			if (this.m_image != null) then //file is not dynamic :-(
				call DestroyImage(this.m_image)
				set this.m_image = null
			endif
			set this.m_image = CreateImageForPlayer(this.user(), file, this.mainWindow().getX(this.x()), this.mainWindow().getY(this.y()), 5.0, this.sizeX(), this.sizeY()) //0.0 instead of 5.0
			call ShowImage(this.m_image, this.isShown())
		endmethod
		
		public method file takes nothing returns string
			return this.m_file
		endmethod
			
		public method setColour takes integer red, integer green, integer blue, integer alpha returns nothing
			set this.m_red = red
			set this.m_green = green
			set this.m_blue = blue
			set this.m_alpha = alpha
			call SetImageColor(this.m_image, red, green, blue, alpha)
		endmethod

		public method red takes nothing returns integer
			return this.m_red
		endmethod

		public method green takes nothing returns integer
			return this.m_green
		endmethod

		public method blue takes nothing returns integer
			return this.m_blue
		endmethod

		public method alpha takes nothing returns integer
			return this.m_alpha
		endmethod

		//methods

		public stub method show takes nothing returns nothing
			call super.show()
			if (this.m_image != null) then
				call ShowImage(this.m_image, true)
			endif
		endmethod

		public stub method hide takes nothing returns nothing
			call super.hide()
			if (this.m_image != null) then
				call ShowImage(this.m_image, false)
			endif
		endmethod

		public static method create takes AMainWindow mainWindow, real x, real y, real sizeX, real sizeY, AWidgetOnHitAction onHitAction, AWidgetOnTrackAction onTrackAction returns AImage
			local AImage this = AImage.allocate(mainWindow, x, y, sizeX, sizeY, onHitAction, onTrackAction)
			//members
			set this.m_image = null

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			if (this.m_image != null) then
				call DestroyImage(this.m_image)
				set this.m_image = null
			endif
		endmethod
	endstruct

endlibrary