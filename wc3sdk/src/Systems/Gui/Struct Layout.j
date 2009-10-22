library AStructSystemsGuiLayout requires optional ALibraryCoreDebugMisc, AStructSystemsGuiWidget, AStructSystemsGuiButton, AStructSystemsGuiText, AStructSystemsGuiImage

	/// Note that this struct repesents a static layout!
	struct ALayout
		//private static constant members
		private static constant integer maxSizeX = 1000
		private static constant integer maxSizeY = 1000
		//start members
		private AMainWindow mainWindow
		private boolean horizontal
		//members
		private boolean array usedX[ALayout.maxSizeX]
		private boolean array usedY[ALayout.maxSizeY]
		private integer nextX
		private integer nextY

		//! runtextmacro optional A_STRUCT_DEBUG("\"ALayout\"")

		public method createWidget takes integer sizeX, integer sizeY, AWidgetOnHitAction onHitAction, AWidgetOnTrackAction onTrackAction returns AWidget
			if (not this.prepareForNext(sizeX, sizeY)) then
				debug call this.print("Was unable to get place for new widget.")
				return 0
			endif
			return AWidget.create(this.mainWindow, this.nextX, this.nextY, sizeX, sizeY, onHitAction, onTrackAction)
		endmethod

		public method createButton takes integer sizeX, integer sizeY, AWidgetOnHitAction onHitAction, AWidgetOnTrackAction onTrackAction returns AButton
			if (not this.prepareForNext(sizeX, sizeY)) then
				debug call this.print("Was unable to get place for new button.")
				return 0
			endif
			return AButton.create(this.mainWindow, this.nextX, this.nextY, sizeX, sizeY, onHitAction, onTrackAction)
		endmethod

		public method createText takes integer sizeX, integer sizeY, AWidgetOnHitAction onHitAction, AWidgetOnTrackAction onTrackAction returns AText
			if (not this.prepareForNext(sizeX, sizeY)) then
				debug call this.print("Was unable to get place for new text.")
				return 0
			endif
			return AButton.create(this.mainWindow, this.nextX, this.nextY, sizeX, sizeY, onHitAction, onTrackAction)
		endmethod

		public method createImage takes string file, integer sizeX, integer sizeY, AWidgetOnHitAction onHitAction, AWidgetOnTrackAction onTrackAction returns AImage
			if (not this.prepareForNext(sizeX, sizeY)) then
				debug call this.print("Was unable to get place for new image.")
				return 0
			endif
			return AImage.create(file, this.mainWindow, this.nextX, this.nextY, sizeX, sizeY, onHitAction, onTrackAction)
		endmethod

		private method checkForPlace takes integer x, integer y, integer sizeX, integer sizeY returns boolean
			local integer i = x
			local integer j
			local integer exitValue0 = x + sizeX
			local integer exitValue1 = y + sizeY
			loop
				exitwhen (i == exitValue0)
				set j = y
				loop
					exitwhen (j == exitValue1)
					if (this.usedX[i] or this.usedY[j]) then
						return false
					endif
					set j = j + 1
				endloop
				set i = i + 1
			endloop
			return true
		endmethod

		private method reservePlace takes integer x, integer y, integer sizeX, integer sizeY returns nothing
			local integer i = x
			loop
				exitwhen (i == sizeX)
				set j = y
				loop
					exitwhen (j == sizeY)
					set this.usedX[i] = true
					set this.usedY[j] = true
					set j = j + 1
				endloop
				set i = i + 1
			endloop
		endmethod

		private method prepareForNext takes integer sizeX, integer sizeY returns boolean
			local integer i = 0
			local integer j
			local boolean found = false
			if (this.horizontal) then
				loop
					exitwhen (found or i == this.mainWindow.getSizeX())
					set j = 0
					loop
						exitwhen (j == this.mainWindow.getSizeY())
						if (not this.usedX[i] and not this.usedY[j]) then
							set found = checkForPlace(i, j, sizeX, sizeY)
							exitwhen (found) //exiting boths loops?
						endif
						set j = j + 1
						endloop
					endif
					set i = i + 1
				endloop
			else
				loop
					exitwhen (found or i == this.mainWindow.getSizeY())
					set j = 0
					loop
						exitwhen (j == this.mainWindow.getSizeX())
						if (not this.usedX[j] and not this.usedY[i]) then
							set found = checkForPlace(i, j, sizeX, sizeY)
							exitwhen (found) //exiting boths loops?
						endif
						set j = j + 1
						endloop
					endif
					set i = i + 1
				endloop
			endif

			//if (not found) then
				//set this.nextX = -1.0
				//set this.nextY = -1.0
			if (this.horizontal) then
				set this.nextX = i
				set this.nextY = j
			else
				set this.nextX = j
				set this.nextY = i
			endif
			call reservePlace(this.nextX, this.nextY, sizeX, sizeY)
			return found
		endmethod


		public static method create takes AMainWindow mainWindow, boolean horizontal returns ALayout
			local ALayout this = ALayout.allocate()
			//start members
			set this.mainWindow = mainWindow
			set this.horizontal = horizontal

			debug if (mainWindow.getSizeX() > ALayout.maxSizeX) then
				debug call this.print("Size x is smaller than the size of its main window.")
			debug elseif (mainWindow.getSizeY() > ALayout.maxSizeY) then
				debug call this.print("Size y is smaller than the size of its main window.")
			debug endif

			return this
		endmethod
	endstruct

endlibrary