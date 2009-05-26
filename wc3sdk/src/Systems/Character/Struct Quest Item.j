library AStructSystemsCharacterQuestItem requires ALibraryCoreDebugMisc, AStructSystemsCharacterAbstractQuest, AStructSystemsCharacterQuest

	struct AQuestItem extends AAbstractQuest
		//start members
		private AQuest m_quest
		//members
		private questitem questLogQuestItem

		//! runtextmacro A_STRUCT_DEBUG("\"AQuestItem\"")
		
		//start members
		
		public method quest takes nothing returns AQuest
			return this.m_quest
		endmethod

		public stub method setState takes integer state returns nothing
			local boolean result
			call super.setState(state)
			set result = not this.m_quest.checkQuestItemsForState(state)
			if (AQuest.isQuestLogUsed()) then
				//call QuestItemSetDescription(this.questLogQuestItem, this.getTitle())
				call QuestItemSetCompleted(this.questLogQuestItem, state == AAbstractQuest.stateCompleted)
				if (result) then
					call FlashQuestDialogButton()
					call ForceQuestDialogUpdate() //required?
				endif
			endif
		endmethod

		private method createQuestLogQuestItem takes nothing returns nothing
			if (AQuest.isQuestLogUsed()) then
				set this.questLogQuestItem = QuestCreateItem(this.m_quest.getQuestLogQuest())
				call QuestItemSetDescription(this.questLogQuestItem, this.getTitle())
			endif
		endmethod

		public static method create takes AQuest usedQuest, string description returns AQuestItem
			local AQuestItem this = AQuestItem.allocate(usedQuest.getCharacter(), description)
			debug if (usedQuest == 0) then
				debug call this.print("usedQuest is 0.")
			debug endif
			debug call Print("THISSSSSS: " + I2S(this))
			//start members
			set this.m_quest = usedQuest

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
