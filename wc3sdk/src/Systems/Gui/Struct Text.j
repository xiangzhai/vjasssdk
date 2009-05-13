library AStructSystemsGuiText requires ALibraryCoreInterfaceTextTag, AStructSystemsGuiWidget

	struct AText extends AWidget
		//dynamic members
		private real heightOffset
		private string text
		private real size
		private integer red
		private integer green
		private integer blue
		private integer alpha
		private real speed
		private real angle
		private real fadepoint
		private boolean suspended //Unterbrochen
		//private boolean permanent //Textes are always permanent
		//private real lifespan //Textes are always permanent
		//private real age
		//Don't change permanent
		//members
		private texttag textTag

		//dynamic members

		//Do not change x and y
		//These values are always static!
		public method setHeightOffset takes real heightOffset returns nothing
			set this.heightOffset = heightOffset
			call SetTextTagPos(this.textTag, this.getMainWindow().getX(this.getX()), this.getMainWindow().getY(this.getY()), heightOffset)
		endmethod

		//We don't need SetTextTagPosUnit.
		//It's a GUI...

		public method setTextAndSize takes string text, real size returns nothing
			set this.text = text
			set this.size = size
			call SetTextTagTextBJ(this.textTag, text, size)
		endmethod

		public method setColour takes integer red, integer green, integer blue, integer alpha returns nothing
			set this.red = red
			set this.green = green
			set this.blue = blue
			set this.alpha = alpha
			call SetTextTagColor(this.textTag, red, green, blue, alpha)
		endmethod

		public method setSpeedAndAngle takes real speed, real angle returns nothing
			set this.speed = speed
			set this.angle = angle
			call SetTextTagVelocityBJ(this.textTag, speed, angle)
		endmethod

		public method setFadepoint takes real fadepoint returns nothing
			set this.fadepoint = fadepoint
			call SetTextTagFadepoint(this.textTag, fadepoint)
		endmethod

		//Unterbricht die Bewegung.
		public method setSuspended takes boolean suspended returns nothing
			set this.suspended = suspended
			call SetTextTagSuspended(this.textTag, suspended)
		endmethod

		//I wanted to implement slow methods which just change one element.
		//But using these methods isn't very useful.

		public method getHeightOffset takes nothing returns real
			return this.heightOffset
		endmethod

		public method getText takes nothing returns string
			return this.text
		endmethod

		public method getSize takes nothing returns real
			return this.size
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

		public method getSpeed takes nothing returns real
			return this.speed
		endmethod

		public method getAngle takes nothing returns real
			return this.angle
		endmethod

		public method getFadepoint takes nothing returns real
			return this.fadepoint
		endmethod

		public method getSuspended takes nothing returns boolean
			return this.suspended
		endmethod

		//methods

		public stub method show takes nothing returns nothing
			call super.show()
			call ShowTextTagForPlayer(this.getUser(), this.textTag, true) //ALibraryInterfaceTextTag
		endmethod

		public stub method hide takes nothing returns nothing
			call super.hide()
			call ShowTextTagForPlayer(this.getUser(), this.textTag, false) //ALibraryInterfaceTextTag
		endmethod

		public static method create takes AMainWindow mainWindow, real x, real y, real sizeX, real sizeY, AWidgetOnHitAction onHitAction, AWidgetOnTrackAction onTrackAction returns AText
			local AText this = AText.allocate(mainWindow, x, y, sizeX, sizeY, onHitAction, onTrackAction)
			//dynamic members
			set this.heightOffset = 0.0
			//members
			set this.textTag = CreateTextTag()
			call SetTextTagPos(this.textTag, mainWindow.getX(x), mainWindow.getY(y), this.heightOffset) //Elements x and y were set in the allocate method.

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			//members
			call DestroyTextTag(this.textTag)
			set this.textTag = null
		endmethod
	endstruct

endlibrary