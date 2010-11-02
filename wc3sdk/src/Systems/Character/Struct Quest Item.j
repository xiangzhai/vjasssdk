library AStructSystemsCharacterQuestItem requires optional ALibraryCoreDebugMisc, AStructSystemsCharacterAbstractQuest, AStructSystemsCharacterQuest

	struct AQuestItem extends AAbstractQuest
		// construction members
		private AQuest m_quest
		// members
		private questitem m_questItem
		private integer m_index

		///! runtextmacro optional A_STRUCT_DEBUG("\"AQuestItem\"")

		// construction members

		public method quest takes nothing returns AQuest
			return this.m_quest
		endmethod

		// methods

		/// Single call!
		public stub method enable takes nothing returns boolean
			if (this.setState(AAbstractQuest.stateNew)) then
				call this.quest().displayUpdate()
				return true
			endif
			return false
		endmethod

		/// Single call!
		public stub method disable takes nothing returns boolean
			return this.setState(AAbstractQuest.stateNotUsed)
		endmethod

		/// Single call!
		public stub method complete takes nothing returns boolean
			local integer oldState = this.quest().state()
			if (this.setState(AAbstractQuest.stateCompleted)) then
				if (this.quest().state() == AAbstractQuest.stateCompleted and oldState != AAbstractQuest.stateCompleted) then
					call this.quest().displayState()
				else
					call this.quest().displayUpdate()
				endif
				return true
			endif
			return false
		endmethod

		/// Single call!
		public stub method fail takes nothing returns boolean
			local integer oldState = this.quest().state()
			if (this.setState(AAbstractQuest.stateFailed)) then
				if (this.quest().state() == AAbstractQuest.stateFailed and oldState != AAbstractQuest.stateFailed) then
					call this.quest().displayState()
				else
					call this.quest().displayUpdate()
				endif
				return true
			endif
			return false
		endmethod

		/// @note Required by structure AQuest, don't call manually.
		public method setStateWithoutConditionAndCheck takes integer state returns nothing
			if (AQuest.isQuestLogUsed()) then
				if (this.m_questItem == null) then
					set this.m_questItem = QuestCreateItem(this.m_quest.questLogQuest())
					call QuestItemSetDescription(this.m_questItem, this.title())
				endif
				//call QuestItemSetDescription(this.questLogQuestItem, this.title())
				call QuestItemSetCompleted(this.m_questItem, state == AAbstractQuest.stateCompleted)
			endif
			call super.setStateWithoutCondition(state)
		endmethod

		public stub method setStateWithoutCondition takes integer state returns nothing
			call this.setStateWithoutConditionAndCheck(state)
			call this.m_quest.checkQuestItemsForState(state)
		endmethod

		public static method create takes AQuest whichQuest, string description returns thistype
			local thistype this = thistype.allocate(whichQuest.character(), description)
			debug if (whichQuest <= 0) then
				debug call this.print("Invalid used quest.")
			debug endif
			// construction members
			set this.m_quest = whichQuest
			// members
			if (AQuest.isQuestLogUsed()) then
				set this.m_questItem = null
			endif
			set this.m_index = whichQuest.addQuestItem(this)
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
