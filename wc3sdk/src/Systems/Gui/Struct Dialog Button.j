library AStructSystemsGuiDialogButton requires ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, ALibraryCoreStringConversion

	/// @todo Should be a part of @struct ADialogButton, vJass bug.
	function interface ADialogButtonAction takes ADialogButton dialogButton returns nothing

	struct ADialogButton
		//start members
		private ADialog m_dialog
		private string m_text
		private integer m_shortcut
		private boolean m_isQuitButton
		private boolean m_doScoreScreen
		private ADialogButtonAction m_action
		//members
		private integer m_index
		private button m_button
		private trigger m_trigger
		
		//! runtextmacro A_STRUCT_DEBUG("\"ADialogButton\"")
		
		//start members
		
		public method dialog takes nothing returns ADialog
			return this.m_dialog
		endmethod
		
		public method text takes nothing returns string
			return this.m_text
		endmethod
		
		public method shortcut takes nothing returns integer
			return this.m_shortcut
		endmethod
		
		public method isQuitButton takes nothing returns boolean
			return this.m_isQuitButton
		endmethod
		
		public method doScoreScreen takes nothing returns boolean
			return this.m_doScoreScreen
		endmethod
		
		public method action takes nothing returns ADialogButtonAction
			return this.m_action
		endmethod
		
		//members
		
		public method index takes nothing returns integer
			return this.m_index
		endmethod
		
		//methods
		
		/// Usually you do not need this method. It is used by @struct ADialog.
		public method addButton takes nothing returns nothing
			if (not this.m_isQuitButton) then
				set this.m_button = DialogAddButton(this.m_dialog.dialog(), HighlightShortcut(this.m_text, this.m_shortcut, "ffffcc00"),  this.m_shortcut)
			else
				 set this.m_button = DialogAddQuitButton(this.m_dialog.dialog(), this.m_doScoreScreen, HighlightShortcut(this.m_text, this.m_shortcut, "ffffcc00"),  this.m_shortcut)
			endif
			call this.createTrigger()
		endmethod
		
		public method removeButton takes nothing returns nothing
			call this.destroyTrigger()
		endmethod
		
		private static method triggerActionRunDialogButtonAction takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ADialogButton this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call this.dialog().setDisplayedByButton(false) // Do not clear the dialog automatically (dialog().clear()) since it can be shown again with same buttons.
			call this.m_action.execute(this)
			set triggeringTrigger = null
		endmethod

		private method createTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			//create always since it sends the dialog that it's hidden again.
			//if (this.m_action != 0) then
				set this.m_trigger = CreateTrigger()
				set triggerEvent = TriggerRegisterDialogButtonEvent(this.m_trigger, this.m_button)
				set triggerAction = TriggerAddAction(this.m_trigger, function thistype.triggerActionRunDialogButtonAction)
				call AHashTable.global().setHandleInteger(this.m_trigger, "this", this)
				set triggerEvent = null
				set triggerAction = null
			//endif
		endmethod
		
		public static method create takes ADialog usedDialog, string text, integer shortcut, boolean isQuitButton, boolean doScoreScreen, ADialogButtonAction action returns thistype
			local thistype this = thistype.allocate()
			//start members
			set this.m_dialog = usedDialog
			set this.m_text = text
			set this.m_shortcut = shortcut
			set this.m_isQuitButton = isQuitButton
			set this.m_doScoreScreen = doScoreScreen
			set this.m_action = action
			//members
			set this.m_index = usedDialog.addDialogButtonInstance(this)
			
			return this
		endmethod
		
		private method destroyTrigger takes nothing returns nothing
			//if (this.m_action != 0) then
				call AHashTable.global().destroyTrigger(this.m_trigger)
				set this.m_trigger = null
			//endif
		endmethod
		
		public method onDestroy takes nothing returns nothing
			call this.m_dialog.removeDialogButtonByIndex(this.m_index)
			if (this.m_button != null) then
				set this.m_button = null //can not be destroyed
				call this.destroyTrigger()
			endif
		endmethod
	endstruct

endlibrary