library AStructSystemsCharacterQuestItem requires ALibraryCoreDebugMisc, AStructSystemsCharacterAbstractQuest, AStructSystemsCharacterQuest

	struct AQuestItem extends AAbstractQuest
		//start members
		private AQuest usedQuest
		//members
		private questitem questLogQuestItem

		//! runtextmacro A_STRUCT_DEBUG("\"AQuestItem\"")

		public stub method setState takes integer state returns nothing
			local boolean result
			call super.setState(state) //AClassCharacterAbstractQuest
			set result = not this.usedQuest.checkQuestItemsForState(state) //AClassCharacterQuest
			if (AQuest.isQuestLogUsed()) then //AClassCharacterQuest
				//call QuestItemSetDescription(this.questLogQuestItem, this.getTitle()) //AClassCharacterAbstractQuest
				call QuestItemSetCompleted(this.questLogQuestItem, state == AAbstractQuest.stateCompleted) //AClassCharacterAbstractQuest
				if (result) then
					call FlashQuestDialogButton()
					call ForceQuestDialogUpdate() //required?
				endif
			endif
		endmethod

		private method createQuestLogQuestItem takes nothing returns nothing
			if (AQuest.isQuestLogUsed()) then //AClassCharacterQuest
				set this.questLogQuestItem = QuestCreateItem(this.usedQuest.getQuestLogQuest())
				call QuestItemSetDescription(this.questLogQuestItem, this.getTitle())
			endif
		endmethod

		public static method create takes AQuest usedQuest, string description returns AQuestItem
			local AQuestItem this = AQuestItem.allocate(usedQuest.getCharacter(), description)
			debug if (usedQuest == 0) then
				debug call this.print("usedQuest is 0.")
			debug endif
			//start members
			set this.usedQuest = usedQuest

			call this.createQuestLogQuestItem()
			call usedQuest.addQuestItem(this)
			return this
		endmethod

		private method destroyQuestLogQuestItem takes nothing returns nothing
			if (AQuest.isQuestLogUsed()) then
				set this.questLogQuestItem = null
				//Could not destroy quest items!
			endif
		endmethod

		public method onDestroy takes nothing returns nothing
			call this.destroyQuestLogQuestItem()
		endmethod
	endstruct

endlibrary
