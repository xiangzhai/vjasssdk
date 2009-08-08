library AStructSystemsGuiCheckBox requires AStructSystemsGuiWidget, AStructSystemsGuiImage

	private function onHitAction takes AWidget usedWidget returns nothing
		call ACheckBox(usedWidget).setChecked(not ACheckBox(usedWidget).isChecked())
	endfunction

	struct ACheckBox extends AWidget
		//static start members
		private static string checkedImageFilePath
		private static string uncheckedImageFilePath
		//dynamic members
		private boolean m_checked
		//members
		private AImage checkedImage

		//dynamic members

		public method setChecked takes boolean checked returns nothing
			set this.m_checked = checked
			if (checked) then
				call this.checkedImage.setFile(ACheckBox.checkedImageFilePath)
			else
				call this.checkedImage.setFile(ACheckBox.uncheckedImageFilePath)
			endif
		endmethod

		public method isChecked takes nothing returns boolean
			return this.m_checked
		endmethod

		//methods

		public static method create takes AMainWindow mainWindow, real x, real y, real sizeX, real sizeY, AWidgetOnTrackAction onTrackAction, boolean checked returns thistype
			local thistype this = thistype.allocate(mainWindow, x, y, sizeX, sizeY, onHitAction, onTrackAction)
			//dynamic members
			set this.m_checked = false
			//members
			set this.checkedImage = AImage.create(mainWindow, x, y, sizeX, sizeY, 0, 0)
			call this.checkedImage.setFile(ACheckBox.uncheckedImageFilePath)
			return this
		endmethod

		//static methods

		public static method init0 takes string checkedImageFilePath, string uncheckedImageFilePath returns nothing
			set ACheckBox.checkedImageFilePath = checkedImageFilePath
			set ACheckBox.uncheckedImageFilePath = uncheckedImageFilePath
		endmethod
	endstruct

endlibrary