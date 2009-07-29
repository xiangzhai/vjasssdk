library AStructSystemsCharacterQuestItem requires ALibraryCoreDebugMisc, AStructSystemsCharacterAbstractQuest, AStructSystemsCharacterQuest

	struct AQuestItem extends AAbstractQuest
		//start members
		private AQuest m_quest
		//members
		private questitem questLogQuestItem

		///! runtextmacro A_STRUCT_DEBUG("\"AQuestItem\"")
		
		//start members
		
		public method quest takes nothing returns AQuest
			return this.m_quest
		endmethod

		public stub method setState takes integer state returns nothing
			local boolean result
			call super.setState(state)
			set result = not this.m_quest.checkQuestItemsForState(state)
			if (AQuest.isQuestLogUsed()) then
				debug call Print("Set state of quest item " + I2S(this))
				if (this.questLogQuestItem == null) then
					debug call Print("Creating quest item")
					set this.questLogQuestItem = QuestCreateItem(this.m_quest.questLogQuest())
					call QuestItemSetDescription(this.questLogQuestItem, this.title())
				endif
				//call QuestItemSetDescription(this.questLogQuestItem, this.title())
				call QuestItemSetCompleted(this.questLogQuestItem, state == AAbstractQuest.stateCompleted)
				if (result) then
					call FlashQuestDialogButton()
					call ForceQuestDialogUpdate() //required?
				endif
			endif
		endmethod

		public static method create takes AQuest usedQuest, string description returns AQuestItem
			local AQuestItem this = AQuestItem.allocate(usedQuest.character(), description)
			debug if (usedQuest <= 0) then
				debug call this.print("Invalid used quest.")
			debug endif
			//start members
			set this.m_quest = usedQuest
			//members
			if (AQuest.isQuestLogUsed()) then
				set this.questLogQuestItem = null
			endif

			call usedQuest.addQuestItem(this)
			return this
		endmethod

		private method destroyQuestLogQuestItem takes nothing returns nothing
			if (AQuest.isQuestLogUsed()) then
				if (this.questLogQuestItem != null) then
					set this.questLogQuestItem = null
					//Could not destroy quest items!
				endif
			endif
		endmethod

		public method onDestroy takes nothing returns nothing
			call this.destroyQuestLogQuestItem()
		endmethod
	endstruct

endlibrary
