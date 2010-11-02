library AStructSystemsCharacterQuest requires optional ALibraryCoreDebugMisc, ALibraryCoreEnvironmentSound, AStructCoreGeneralVector, ALibraryCoreStringConversion, AStructSystemsCharacterAbstractQuest

	struct AQuest extends AAbstractQuest
		// static construction members
		private static boolean m_useQuestLog
		private static boolean m_likeWarcraft
		private static string m_updateSoundPath
		private static string m_textQuestNew
		private static string m_textQuestCompleted
		private static string m_textQuestFailed
		private static string m_textQuestUpdate
		private static string m_textListItem
		//dynamic members
		private AIntegerVector m_questItems
		private string m_iconPath
		private string m_description
		// members
		private quest m_questLogQuest

		///! runtextmacro optional A_STRUCT_DEBUG("\"AQuest\"")

		// dynamic members

		public method questItem takes integer index returns AQuestItem
			return this.m_questItems[index]
		endmethod

		public method setIconPath takes string iconPath returns nothing
			debug if (not thistype.m_useQuestLog) then
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
			debug if (not thistype.m_useQuestLog) then
				debug call this.print("setDescription() was called (quest log is disabled).")
			debug endif
			set this.m_description = description
			call QuestSetDescription(this.m_questLogQuest, description)
		endmethod

		public method description takes nothing returns string
			return this.m_description
		endmethod

		// members

		/// Used by AQuestItem, do not use.
		public method questLogQuest takes nothing returns quest
			return this.m_questLogQuest
		endmethod

		// methods

		private method displayStateMessage takes nothing returns nothing
			local integer i

			if (this.character() != 0) then

				call this.character().displayMessage(ACharacter.messageTypeInfo, this.title())
				if (not thistype.m_likeWarcraft or (not this.isCompleted() and not this.isFailed())) then
					set i = 0
					loop
						exitwhen (i == this.m_questItems.size())
						if (not AQuestItem(this.m_questItems[i]).isNotUsed()) then
							call this.character().displayMessage(ACharacter.messageTypeInfo, StringArg(thistype.m_textListItem, AQuestItem(this.m_questItems[i]).modifiedTitle()))
						endif
						set i = i + 1
					endloop
				endif
				call PlaySoundFileForPlayer(this.character().player(), this.soundPath())
			else
				call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, this.title())
				if (not thistype.m_likeWarcraft or (not this.isCompleted() and not this.isFailed())) then
					set i = 0
					loop
						exitwhen (i == this.m_questItems.size())
						if (not AQuestItem(this.m_questItems[i]).isNotUsed()) then
							call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, StringArg(thistype.m_textListItem, AQuestItem(this.m_questItems[i]).modifiedTitle()))
						endif
						set i = i + 1
					endloop
				endif
				call PlaySound(this.soundPath())
			endif

			/// @todo For corresponding player only.
			if (thistype.m_useQuestLog) then
				call FlashQuestDialogButton()
				call ForceQuestDialogUpdate() //required?
			endif
		endmethod

		public method displayState takes nothing returns nothing
			local string title = null

			if (this.state() == AAbstractQuest.stateNew and thistype.m_textQuestNew != null) then
				set title = thistype.m_textQuestNew
			elseif (this.state() == AAbstractQuest.stateCompleted and thistype.m_textQuestCompleted != null) then
				set title = thistype.m_textQuestCompleted
			elseif (this.state() == AAbstractQuest.stateFailed and thistype.m_textQuestFailed != null) then
				set title = thistype.m_textQuestFailed
			endif


			if (this.character() != 0) then
				if (thistype.m_likeWarcraft) then
					call this.character().displayMessage(ACharacter.messageTypeInfo, " ") // Warcraft like line break
				endif
				if (title != null) then
					call this.character().displayMessage(ACharacter.messageTypeInfo, title)
				endif
			else
				if (thistype.m_likeWarcraft) then
					call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, " ") // Warcraft like line break
				endif
				if (title != null) then
					call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, title)
				endif
			endif

			call this.displayStateMessage()
		endmethod

		public method displayUpdate takes nothing returns nothing
			if (this.character() != 0) then
				if (thistype.m_textQuestUpdate != null) then
					call this.character().displayMessage(ACharacter.messageTypeInfo, thistype.m_textQuestUpdate)
				endif
			else
				if (thistype.m_textQuestUpdate != null) then
					call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, thistype.m_textQuestUpdate)
				endif
			endif

			call this.displayStateMessage()
		endmethod

		public method displayUpdateMessage takes string message returns nothing
			if (this.character() != 0) then
				if (thistype.m_textQuestUpdate != null) then
					call this.character().displayMessage(ACharacter.messageTypeInfo, thistype.m_textQuestUpdate)
				endif
				call this.character().displayMessage(ACharacter.messageTypeInfo, this.title())
				call this.character().displayMessage(ACharacter.messageTypeInfo, message)
				if (thistype.m_updateSoundPath != null) then
					call PlaySoundFileForPlayer(this.character().player(), thistype.m_updateSoundPath)
				endif
			else
				if (thistype.m_textQuestUpdate != null) then
					call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, thistype.m_textQuestUpdate)
				endif
				call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, this.title())
				call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, message)
				if (thistype.m_updateSoundPath != null) then
					call PlaySound(thistype.m_updateSoundPath)
				endif
			endif
		endmethod

		/// Single call!
		public stub method enableUntil takes integer questItemIndex returns boolean
			local integer i = 0
			if (this.setState(AAbstractQuest.stateNew)) then
				set i = 0
				loop
					exitwhen (i == IMinBJ(this.m_questItems.size(), questItemIndex + 1))
					call AQuestItem(this.m_questItems[i]).setState(AAbstractQuest.stateNew)
					set i = i + 1
				endloop
				call this.displayState()
				return true
			endif
			return false
		endmethod

		/// Single call!
		/// Enables all items (checking conditions).
		public stub method enable takes nothing returns boolean
			return this.enableUntil(this.m_questItems.size() - 1)
		endmethod

		/// Single call!
		/// Should disable all items since setting state to AAbstractQuest.stateNotUsed will set all items to this state, too.
		public stub method disable takes nothing returns boolean
			return this.setState(AAbstractQuest.stateNotUsed)
		endmethod

		/// Single call!
		public stub method complete takes nothing returns boolean
			if (this.setState(AAbstractQuest.stateCompleted)) then
				call this.displayState()
				return true
			endif
			return false
		endmethod

		/// Single call!
		public stub method fail takes nothing returns boolean
			if (this.setState(AAbstractQuest.stateFailed)) then
				call this.displayState()
				return true
			endif
			return false
		endmethod

		/// Note that if quest's state is changed quest items states will be changed automatically without checking conditions.
		public stub method setStateWithoutCondition takes integer state returns nothing
			local integer i
			local player user
			local playercolor playerColor
			local string title = null

			if (state == AAbstractQuest.stateCompleted or state == AAbstractQuest.stateFailed or state == AAbstractQuest.stateNotUsed) then
				set i = 0
				loop
					exitwhen (i == this.m_questItems.size())
					if (AQuestItem(this.m_questItems[i]).state() == AAbstractQuest.stateNew) then
						call AQuestItem(this.m_questItems[i]).setStateWithoutConditionAndCheck(state) // if you call it with check it will be completed twice
					endif
					set i = i + 1
				endloop
			endif
			if (thistype.m_useQuestLog) then
				if (this.character() == 0) then
					set title = this.title()
				else
					set user = this.character().player()
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
				elseif (state == AAbstractQuest.stateCompleted) then
					call QuestSetCompleted(this.m_questLogQuest, true)
				elseif (state == AAbstractQuest.stateFailed) then
					call QuestSetFailed(this.m_questLogQuest, true)
				endif
			endif

			call super.setStateWithoutCondition(state)
		endmethod

		/// If all quest items have an equal state quest does also get their state (without checking its state condition!)
		public method checkQuestItemsForState takes integer state returns boolean
			local integer i
			local boolean result = true
			// does not already have the same state
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
					call this.setStateWithoutCondition(state)
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
			if (thistype.m_useQuestLog) then
				set this.m_questLogQuest = CreateQuest()
				call QuestSetDiscovered(this.m_questLogQuest, false) // hide quest before setting state
				call QuestSetRequired(this.m_questLogQuest, this.character() == 0)
			endif
		endmethod

		public static method create takes ACharacter character, string title returns thistype
			local thistype this = thistype.allocate(character, title)
			// dynamic members
			set this.m_questItems = AIntegerVector.create()

			call this.createQuestLogQuest()
			return this
		endmethod

		private method destroyQuestLogQuest takes nothing returns nothing
			if (thistype.m_useQuestLog) then
				call DestroyQuest(this.m_questLogQuest)
				set this.m_questLogQuest = null
			endif
		endmethod

		// all quest items will be destroyed, too
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

		/**
		* init is already used by struct @struct AAbstractQuest (@method AAbstractQuest.init)
		* @param likeWarcraft If this value is true quest fail messages won't list all quest items and there will be a line break before all quest state messages.
		*/
		public static method init0 takes boolean useQuestLog, boolean likeWarcraft, string updateSoundPath, string textQuestNew, string textQuestCompleted, string textQuestFailed, string textQuestUpdate, string textListItem returns nothing
			// static construction members
			set thistype.m_useQuestLog = useQuestLog
			set thistype.m_likeWarcraft = likeWarcraft
			set thistype.m_updateSoundPath = updateSoundPath
			set thistype.m_textQuestNew = textQuestNew
			set thistype.m_textQuestCompleted = textQuestCompleted
			set thistype.m_textQuestFailed = textQuestFailed
			set thistype.m_textQuestUpdate = textQuestUpdate
			set thistype.m_textListItem = textListItem
		endmethod

		// static construction members

		// AQuestItem need access
		public static method isQuestLogUsed takes nothing returns boolean
			return thistype.m_useQuestLog
		endmethod
	endstruct

endlibrary