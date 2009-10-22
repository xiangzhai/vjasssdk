library AStructSystemsCharacterQuest requires optional ALibraryCoreDebugMisc, ALibraryCoreEnvironmentSound, AStructCoreGeneralVector, ALibraryCoreStringConversion, AStructSystemsCharacterAbstractQuest

	struct AQuest extends AAbstractQuest
		//static start members
		private static boolean useQuestLog
		private static string updateSoundPath
		//dynamic members
		private AIntegerVector m_questItems
		private string m_iconPath
		private string m_description
		//members
		private quest m_questLogQuest

		///! runtextmacro optional A_STRUCT_DEBUG("\"AQuest\"")

		//dynamic members

		public method questItem takes integer index returns AQuestItem
			return this.m_questItems[index]
		endmethod
		
		public method setIconPath takes string iconPath returns nothing
			debug if (not thistype.useQuestLog) then
				debug call this.print("setIconPath() was called (quest log is disabled).")
			debug endif
			set this.m_iconPath = iconPath
			call QuestSetIconPath(this.m_questLogQuest, iconPath)
		endmethod
		
		public method iconPath takes nothing returns string
			return this.m_iconPath
		endmethod

		/// No flash, just when you change the state!
		/// Description also is not used as start property because you do not always use the quest log.
		public method setDescription takes string description returns nothing
			debug if (not thistype.useQuestLog) then
				debug call this.print("setDescription() was called (quest log is disabled).")
			debug endif
			set this.m_description = description
			call QuestSetDescription(this.m_questLogQuest, description)
		endmethod

		public method description takes nothing returns string
			return this.m_description
		endmethod

		//members

		/// Used by AQuestItem, do not use.
		public method questLogQuest takes nothing returns quest
			return this.m_questLogQuest
		endmethod

		//methods

		public stub method setState takes integer state returns nothing
			local integer i
			local player user
			local playercolor playerColor
			local string title
			call super.setState(state)
			if (state == AAbstractQuest.stateCompleted or state == AAbstractQuest.stateFailed) then
				set i = 0
				loop
					exitwhen (i == this.m_questItems.size())
					if (AQuestItem(this.m_questItems[i]).state() == AAbstractQuest.stateNew) then
						call AQuestItem(this.m_questItems[i]).setState(state)
					endif
					set i = i + 1
				endloop
			endif
			if (thistype.useQuestLog) then
				if (this.character() == 0) then
					set title = this.title()
				else
					set user = this.character().user()
					set playerColor = GetPlayerColor(user)
					set title = "|cff" + PlayerColorToString(playerColor) + this.title() + "|r"
					set user = null
					set playerColor = null
				endif
				call QuestSetTitle(this.m_questLogQuest, title)
				//call QuestSetDescription(this.questLogQuest, this.description)
				if (state == AAbstractQuest.stateNotUsed) then
					call QuestSetDiscovered(this.m_questLogQuest, false)
				elseif (state == AAbstractQuest.stateNew) then
					call QuestSetDiscovered(this.m_questLogQuest, true)
					call FlashQuestDialogButton()
					call ForceQuestDialogUpdate() //required?
				elseif (state == AAbstractQuest.stateCompleted) then
					call QuestSetCompleted(this.m_questLogQuest, true)
					call FlashQuestDialogButton()
					call ForceQuestDialogUpdate() //required?
				elseif (state == AAbstractQuest.stateFailed) then
					call QuestSetFailed(this.m_questLogQuest, true)
					call FlashQuestDialogButton()
					call ForceQuestDialogUpdate() //required?
				endif
			endif
		endmethod

		public method displayUpdateMessage takes string message returns nothing
			local player user = this.character().user()
			call DisplayTimedTextToPlayer(user, 0.0, 0.0, 20.0, this.title())
			call DisplayTimedTextToPlayer(user, 0.0, 0.0, 20.0, message)
			call PlaySoundPathForPlayer(user, thistype.updateSoundPath)
			set user = null
		endmethod

		//Wenn alle QuestItems den gleichen State haben, erhält das Quest ebenfalls diesen State
		public method checkQuestItemsForState takes integer state returns boolean
			local integer i
			local boolean result = true
			//Hat noch nicht den State
			if (this.state() != state) then
				set i = 0
				loop
					exitwhen(i == this.m_questItems.size())
					if (AQuestItem(this.m_questItems[i]).state() != state) then
						set result = false
					endif
					set i = i + 1
				endloop
				if (result) then
					call this.setState(state)
				endif
			endif
			return result
		endmethod
		
		/// Friend relationship to @struct AQuestItem, do not use.
		public method addQuestItem takes AQuestItem questItem returns integer
			call this.m_questItems.pushBack(questItem)
			return this.m_questItems.backIndex()
		endmethod
		
		/// Friend relationship to @struct AQuestItem, do not use.
		public method removeQuestItemByIndex takes integer index returns nothing
			call this.m_questItems.erase(index)
		endmethod

		private method createQuestLogQuest takes nothing returns nothing
			if (thistype.useQuestLog) then
				set this.m_questLogQuest = CreateQuest()
				call QuestSetDiscovered(this.m_questLogQuest, false) //hide quest before setting state
				call QuestSetRequired(this.m_questLogQuest, this.character() == 0)
			endif
		endmethod

		public static method create takes ACharacter character, string title returns thistype
			local thistype this = thistype.allocate(character, title)
			//dynamic members
			set this.m_questItems = AIntegerVector.create()

			call this.createQuestLogQuest()
			return this
		endmethod

		private method destroyQuestLogQuest takes nothing returns nothing
			if (thistype.useQuestLog) then
				call DestroyQuest(this.m_questLogQuest)
				set this.m_questLogQuest = null
			endif
		endmethod
	
		//Alle QuestItems werden auch zerstört
		private method destroyQuestItems takes nothing returns nothing
			loop
				exitwhen (this.m_questItems.empty())
				call AQuestItem(this.m_questItems.back()).destroy()
			endloop
			call this.m_questItems.destroy()
		endmethod

		public method onDestroy takes nothing returns nothing
			call this.destroyQuestLogQuest()
			call this.destroyQuestItems()
		endmethod

		//init is already used
		public static method init0 takes boolean useQuestLog, string updateSoundPath returns nothing
			//static start members
			set thistype.useQuestLog = useQuestLog
			set thistype.updateSoundPath = updateSoundPath
		endmethod

		//static start members

		//AQuestItem need access
		public static method isQuestLogUsed takes nothing returns boolean
			return thistype.useQuestLog
		endmethod
	endstruct

endlibrary