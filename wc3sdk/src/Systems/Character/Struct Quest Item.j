library AStructSystemsCharacterQuestItem requires optional ALibraryCoreDebugMisc, AStructSystemsCharacterAbstractQuest, AStructSystemsCharacterQuest

	struct AQuestItem extends AAbstractQuest
		//start members
		private AQuest m_quest
		//members
		private questitem m_questItem
		private integer m_index

		///! runtextmacro optional A_STRUCT_DEBUG("\"AQuestItem\"")
		
		//start members
		
		public method quest takes nothing returns AQuest
			return this.m_quest
		endmethod

		public stub method setState takes integer state returns nothing
			local boolean result
			call super.setState(state)
			set result = not this.m_quest.checkQuestItemsForState(state)
			if (AQuest.isQuestLogUsed()) then
				if (this.m_questItem == null) then
					set this.m_questItem = QuestCreateItem(this.m_quest.questLogQuest())
					call QuestItemSetDescription(this.m_questItem, this.title())
				endif
				//call QuestItemSetDescription(this.questLogQuestItem, this.title())
				call QuestItemSetCompleted(this.m_questItem, state == AAbstractQuest.stateCompleted)
				if (result) then
					call FlashQuestDialogButton()
					call ForceQuestDialogUpdate() //required?
				endif
			endif
		endmethod

		public static method create takes AQuest usedQuest, string description returns thistype
			local thistype this = thistype.allocate(usedQuest.character(), description)
			debug if (usedQuest <= 0) then
				debug call this.print("Invalid used quest.")
			debug endif
			//start members
			set this.m_quest = usedQuest
			//members
			if (AQuest.isQuestLogUsed()) then
				set this.m_questItem = null
			endif
			set this.m_index = usedQuest.addQuestItem(this)
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			call this.m_quest.removeQuestItemByIndex(this.m_index)
			if (AQuest.isQuestLogUsed()) then
				if (this.m_questItem != null) then
					set this.m_questItem = null
					//Could not destroy quest items!
				endif
			endif
		endmethod
	endstruct

endlibrary
