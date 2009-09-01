library AStructSystemsGuiDialog requires ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, AStructCoreGeneralVector, ALibraryCoreStringConversion

	/**
	* Provides a kind of wrapper struct for common Warcraft 3 dialogs.
	* Actually it's not only a wrapper since it's expands the whole functionality of dialogs by using @struct ADialogButton instances.
	* Besides it allows user to add more than 12 (or 16? Warcraft 3 limit) buttons by adding next and previous page buttons and splitting
	* all 10-button groups into pages.
	*/
	struct ADialog
		//static constant members
		public static constant integer maxDialogButtons = 100
		private static constant integer maxPageButtons = 10
		//static start members
		private static integer shortcutPreviousPage
		private static integer shortcutNextPage
		private static string textPreviousPage
		private static string textNextPage
		//dynamic members
		private string m_message
		private boolean m_isDisplayed
		//start members
		private player m_player
		//members
		private dialog m_dialog
		private AIntegerVector m_dialogButtons
		private integer m_currentPage
		private integer m_maxPageNumber
		private button m_previousPageButton
		private trigger m_nextPageTrigger
		private button m_nextPageButton
		private trigger m_previousPageTrigger
		
		//! runtextmacro A_STRUCT_DEBUG("\"ADialog\"")
		
		//dynamic members
		
		public method setMessage takes string message returns nothing
			set this.m_message = message
			call DialogSetMessage(this.m_dialog, message) //InsertLineBreaks(message, 10)
		endmethod
		
		public method message takes nothing returns string
			return this.m_message
		endmethod
		
		public method setDisplayed takes boolean displayed returns nothing
			local integer i
			local integer exitValue
			set this.m_isDisplayed = displayed
			if (displayed) then
				set i = this.m_currentPage * thistype.maxPageButtons
				set exitValue = (this.m_currentPage + 1) * thistype.maxPageButtons
				loop
					exitwhen (i == exitValue or i == this.m_dialogButtons.size())
					call ADialogButton(this.m_dialogButtons[i]).addButton()
					set i = i + 1
				endloop
				if (this.m_maxPageNumber > 0) then
					call this.createPreviousPageButton()
					call this.createNextPageButton()
				endif
			else
				set i = this.m_currentPage * thistype.maxPageButtons
				set exitValue = (this.m_currentPage + 1) * ADialog.maxPageButtons
				loop
					exitwhen (i == exitValue or i == this.m_dialogButtons.size())
					call ADialogButton(this.m_dialogButtons[i]).removeButton()
					set i = i + 1
				endloop
				if (this.m_maxPageNumber > 0) then
					call this.removePreviousPageButton()
					call this.removeNextPageButton()
				endif
				call DialogClear(this.m_dialog)
			endif
			call DialogDisplay(this.m_player, this.m_dialog, displayed)
		endmethod
		
		/// @return Returns if the dialog is displayed to player.
		public method isDisplayed takes nothing returns boolean
			return this.m_isDisplayed
		endmethod
		
		//start members
		
		public method player takes nothing returns player
			return this.m_player
		endmethod
		
		//members
		
		public method dialog takes nothing returns dialog
			return this.m_dialog
		endmethod
		
		public method currentPage takes nothing returns integer
			return this.m_currentPage
		endmethod
		
		/// @return Returns the number of the last page (starting from 0).
		/// For example: If there are three pages this method will return 2.
		public method maxPageNumber takes nothing returns integer
			return this.m_maxPageNumber
		endmethod
		
		//convenience methos
		
		public method show takes nothing returns nothing
			call this.setDisplayed(true)
		endmethod
		
		public method hide takes nothing returns nothing
			call this.setDisplayed(false)
		endmethod
		
		//methods
		
		public method addExtendedDialogButton takes string text, integer shortcut, boolean isQuitButton, boolean doScoreScreen, ADialogButtonAction action returns ADialogButton
			return ADialogButton.create(this, text, shortcut, isQuitButton, doScoreScreen, action)
		endmethod
		
		public method addDialogButton takes string text, integer shortcut, ADialogButtonAction action returns ADialogButton
			return this.addExtendedDialogButton(text, shortcut, false, false, action)
		endmethod
		
		public method addSimpleDialogButton takes string text, integer shortcut returns ADialogButton
			return this.addExtendedDialogButton(text, shortcut, false, false, 0)
		endmethod
		
		public method addDialogButtonInstance takes ADialogButton instance returns integer
			if (this.m_dialogButtons.size() - this.m_maxPageNumber * ADialog.maxPageButtons == ADialog.maxPageButtons) then
				set this.m_maxPageNumber = this.m_maxPageNumber + 1
			endif
			call this.m_dialogButtons.pushBack(instance)
			return this.m_dialogButtons.backIndex()
		endmethod
		
		public method removeDialogButtonInstance takes ADialogButton instance returns nothing
			if (this.m_dialogButtons.size() - this.m_maxPageNumber * ADialog.maxPageButtons == 0) then
				set this.m_maxPageNumber = this.m_maxPageNumber - 1
			endif
			call this.m_dialogButtons.remove(instance)
		endmethod
		
		public method removeDialogButtonByIndex takes integer index returns nothing
			if (this.m_dialogButtons.size() - this.m_maxPageNumber * ADialog.maxPageButtons == 0) then
				set this.m_maxPageNumber = this.m_maxPageNumber - 1
			endif
			call this.m_dialogButtons.erase(index)
		endmethod
		
		public method dialogButtons takes nothing returns integer
			return this.m_dialogButtons.size()
		endmethod
		
		/// Next and previous page buttons are in vector!
		public method clear takes nothing returns nothing
			if (this.m_isDisplayed) then
				//before page number is changed
				if (this.m_maxPageNumber > 0) then
					call this.removePreviousPageButton()
					call this.removeNextPageButton()
				endif
				set this.m_isDisplayed = false
			endif
			loop
				exitwhen (this.m_dialogButtons.empty())
				call ADialogButton(this.m_dialogButtons.back()).destroy() //removes itself from the vector!
			endloop
			set this.m_currentPage = 0
			call DialogClear(this.m_dialog)
		endmethod
		
		/// Friend relation to @struct ADialogButton.
		public method setDisplayedByButton takes boolean displayed returns nothing
			set this.m_isDisplayed = displayed
		endmethod
		
		private method changeToPreviousPage takes nothing returns nothing
			call this.setDisplayed(false)
			if (this.m_currentPage == 0) then
				set this.m_currentPage = this.m_maxPageNumber
			else
				set this.m_currentPage = this.m_currentPage - 1
			endif
			call this.setDisplayed(true)
		endmethod
		
		private method changeToNextPage takes nothing returns nothing
			call this.setDisplayed(false)
			if (this.m_currentPage == this.m_maxPageNumber) then
				set this.m_currentPage = 0
			else
				set this.m_currentPage = this.m_currentPage + 1
			endif
			call this.setDisplayed(true)
		endmethod
		
		private static method triggerActionPreviousPage takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call this.changeToPreviousPage()
			set triggeringTrigger = null
		endmethod
		
		private method createPreviousPageButton takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_previousPageButton = DialogAddButton(this.m_dialog, thistype.textPreviousPage, thistype.shortcutPreviousPage)
			set this.m_previousPageTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterDialogButtonEvent(this.m_previousPageTrigger, this.m_previousPageButton)
			set triggerAction = TriggerAddAction(this.m_previousPageTrigger, function thistype.triggerActionPreviousPage)
			call AHashTable.global().setHandleInteger(this.m_previousPageTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod
		
		private static method triggerActionNextPage takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call this.changeToNextPage()
			set triggeringTrigger = null
		endmethod
		
		private method removePreviousPageButton takes nothing returns nothing
			set this.m_previousPageButton = null
			call AHashTable.global().destroyTrigger(this.m_previousPageTrigger)
			set this.m_previousPageTrigger = null
		endmethod
		
		private method createNextPageButton takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_nextPageButton = DialogAddButton(this.m_dialog, thistype.textNextPage, thistype.shortcutNextPage)
			set this.m_nextPageTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterDialogButtonEvent(this.m_nextPageTrigger, this.m_nextPageButton)
			set triggerAction = TriggerAddAction(this.m_nextPageTrigger, function thistype.triggerActionNextPage)
			call AHashTable.global().setHandleInteger(this.m_nextPageTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod
		
		private method removeNextPageButton takes nothing returns nothing
			set this.m_nextPageButton = null
			call AHashTable.global().destroyTrigger(this.m_nextPageTrigger)
			set this.m_nextPageTrigger = null
		endmethod
		
		public static method create takes player usedPlayer returns thistype
			local thistype this = thistype.allocate()
			//dynamic members
			set this.m_isDisplayed = false
			//start members
			set this.m_player = usedPlayer
			//members
			set this.m_dialog = DialogCreate()
			set this.m_dialogButtons = AIntegerVector.create()
			set this.m_currentPage = 0
			set this.m_maxPageNumber = 0
			
			return this
		endmethod
		
		public method onDestroy takes nothing returns nothing
			call this.clear()
			set this.m_player = null
			call DialogDestroy(this.m_dialog)
			set this.m_dialog = null
			call this.m_dialogButtons.destroy()
		endmethod
		
		public static method init takes integer shortcutPreviousPage, integer shortcutNextPage, string textPreviousPage, string textNextPage returns nothing
			//static start members
			set thistype.shortcutPreviousPage = shortcutPreviousPage
			set thistype.shortcutNextPage = shortcutNextPage
			set thistype.textPreviousPage = textPreviousPage
			set thistype.textNextPage = textNextPage
		endmethod
	endstruct

endlibrary