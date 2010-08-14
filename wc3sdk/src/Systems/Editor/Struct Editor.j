library AStructSystemsEditorEditor requires AStructCoreGeneralGroup, AStructCoreGeneralVector, AStructSystemsGuiGui

	/**
	* Provides a selection storage.
	* @todo Should store containers of different types.
	*/
	private struct AEditorSelection
		// construction members
		private AEditor m_editor
		// members
		private trigger m_waitTrigger
		private player m_player
		private location m_location
		private region m_region
		private unit m_unit
		private AGroup m_units
		private item m_item
		private AItemVector m_items
		private destructable m_destructable
		private ADestructableVector m_destructables

		public method clear takes nothing returns nothing
			set this.m_player = null
			if (this.m_location != null) then
				call RemoveLocation(this.m_location)
				set this.m_location = null
			endif
			if (this.m_region != null) then
				/// @todo Remove rects.
				call RemoveRegion(this.m_region)
				set this.m_region = null
			endif
			set this.m_unit = null
			if (this.m_units != 0) then
				call this.m_units.units().clear()
			endif
			set this.m_item = null
			if (this.m_items != 0) then
				call this.m_items.clear()
			endif
			set this.m_destructable = null
			if (this.m_destructables != 0) then
				call this.m_destructables.clear()
			endif
		endmethod

		public method waitForUnit takes nothing returns nothing
			if (this.m_waitTrigger != null) then // cancel old selection
				call DestroyTrigger(this.m_waitTrigger)
				set this.m_waitTrigger = null
			endif
			set this.m_waitTrigger = CreateTrigger()
			call TriggerRegisterPlayerEvent(
		endmethod

		public static method create takes AEditor editor returns thistype
			local thistype this = thistype.allocate()
			//start members
			set this.m_editor = editor
			//members
			set this.m_waitTrigger = null
			set this.m_player = null
			set this.m_location = null
			set this.m_region = null
			set this.m_unit = null
			set this.m_units = 0
			set this.m_item = null
			set this.m_items = 0
			set this.m_destructable = null
			set this.m_destructables = 0
			return this
		endmethod
	endstruct

	/**
	* Editor implementation for game runtime.
	* Could be useful for Dungeon Master-like maps.
	* @todo Completely unfinished.
	*/
	struct AEditor
		// static members
		private static thistype array m_playerEditor[bj_MAX_PLAYERS]
		// construction members
		private player m_player
		private AEditorSelection m_selection

		//start member

		public method player takes nothing returns player
			return this.m_player
		endmethod

		//methods

		private static method buttonFunctionClearSelection takes ADialogButton dialogButton returns nothing
			local thistype this = thistype.m_playerEditor[GetPlayerId(dialogButton.dialog().player())]
			call this.m_selection.clear()
		endmethod

		private static method buttonFunctionSelectUnit takes ADialogButton dialogButton returns nothing
			local thistype this = thistype.m_playerEditor[GetPlayerId(dialogButton.dialog().player())]
			call this.m_selection.waitForUnit()
		endmethod

		private static method buttonFunctionBackToMainMenu takes ADialogButton dialogButton returns nothing
			local thistype this = thistype.m_playerEditor[GetPlayerId(dialogButton.dialog().player())]
			call this.showMenu()
		endmethod

		private static method buttonFunctionShowSelectionMenu takes ADialogButton dialogButton returns nothing
			local thistype this = thistype.m_playerEditor[GetPlayerId(dialogButton.dialog().player())]
			call AGui.playerGui(this.m_player).dialog().clear()
			call AGui.playerGui(this.m_player).dialog().setMessage(tr("Auswahl"))
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Leeren"), thistype.buttonFunctionClearSelection)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Spieler"), thistype.buttonFunctionClearSelection)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Punkt"), thistype.buttonFunctionClearSelection)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Region"), thistype.buttonFunctionClearSelection)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Einheit"), thistype.buttonFunctionSelectUnit)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Einheitengruppe"), thistype.buttonFunctionClearSelection)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Gegenstand"), thistype.buttonFunctionClearSelection)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Gegenstandsgruppe"), thistype.buttonFunctionClearSelection)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Zerstörbares"), thistype.buttonFunctionClearSelection)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Zerstörbaresgruppe"), thistype.buttonFunctionClearSelection)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Zurück"), thistype.buttonFunctionBackToMainMenu)
			call AGui.playerGui(this.m_player).dialog().show()
		endmethod

		private static method buttonFunctionShowCreateMenu takes ADialogButton dialogButton returns nothing
			local thistype this = thistype.m_playerEditor[GetPlayerId(dialogButton.dialog().player())]
		endmethod

		private static method buttonFunctionShowEditMenu takes ADialogButton dialogButton returns nothing
			local thistype this = thistype.m_playerEditor[GetPlayerId(dialogButton.dialog().player())]
		endmethod

		public method showMenu takes nothing returns nothing
			call AGui.playerGui(this.m_player).dialog().clear()
			call AGui.playerGui(this.m_player).dialog().setMessage(tr("Menü"))
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Auswahl"), thistype.buttonFunctionShowSelectionMenu)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Erzeugen"), thistype.buttonFunctionShowCreateMenu)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Bearbeiten"), thistype.buttonFunctionShowEditMenu)
			call AGui.playerGui(this.m_player).dialog().addButton(tr("Zurück zum Spiel"), 0)
			call AGui.playerGui(this.m_player).dialog().show()
		endmethod

		public static method create takes player whichPlayer returns thistype
			local thistype this = thistype.allocate()
			//start members
			set this.m_player = whichPlayer
			set this.m_selection = AEditorSelection.create(this)

			return this
		endmethod

		private method onDestroy takes nothing returns nothing
			//start members
			set this.m_player = null
			call this.m_selection.destroy()
		endmethod

		public static method playerEditor takes player whichPlayer returns thistype
			if (thistype.m_playerEditor[GetPlayerId(whichPlayer)] == 0) then
				set thistype.m_playerEditor[GetPlayerId(whichPlayer)] = thistype.create(whichPlayer)
			endif
			return thistype.m_playerEditor[GetPlayerId(whichPlayer)]
		endmethod
	endstruct

endlibrary