library AStructSystemsCharacterQuest requires ALibraryCoreDebugMisc, ALibraryCoreEnvironmentSound, AStructCoreGeneralVector, AStructSystemsCharacterAbstractQuest

	/// @todo Size should be @member AQuest.maxQuestItems.
	//! runtextmacro A_VECTOR("private", "AQuestItemVector", "AQuestItem", "0", "5")

	struct AQuest extends AAbstractQuest
		private static constant integer maxQuestItems = 5
		//static start members
		private static boolean useQuestLog
		private static string updateSoundPath
		//dynamic members
		private AQuestItemVector m_questItems
		private string m_iconPath
		private string m_description
		//members
		private quest m_questLogQuest

		///! runtextmacro A_STRUCT_DEBUG("\"AQuest\"")

		//dynamic members

		public method addQuestItem takes AQuestItem questItem returns integer
			call this.m_questItems.pushBack(questItem)
			return this.m_questItems.backIndex()
		endmethod

		public method questItem takes integer index returns AQuestItem
			return this.m_questItems[index]
		endmethod
		
		public method setIconPath takes string iconPath returns nothing
			debug if (not AQuest.useQuestLog) then
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
			debug if (not AQuest.useQuestLog) then
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
			call super.setState(state)
			if (state == AAbstractQuest.stateCompleted or state == AAbstractQuest.stateFailed) then
				set i = 0
				loop
					exitwhen (i == this.m_questItems.size())
					if (this.m_questItems[i].state() == AAbstractQuest.stateNew) then
						call this.m_questItems[i].setState(state)
					endif
					set i = i + 1
				endloop
			endif
			if (AQuest.useQuestLog) then
				call QuestSetTitle(this.m_questLogQuest, this.title())
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
			call PlaySoundPathForPlayer(user, AQuest.updateSoundPath)
			set user = null
		endmethod

		//Wenn alle QuestItems den gleichen State haben, erhält das Quest ebenfalls diesen State
		public method checkQuestItemsForState takes integer state returns boolean
			local integer i
			local boolean result
			//Hat noch nicht den State
			if (this.state() != state) then
				set result = true
				set i = 0
				loop
					exitwhen(i == this.m_questItems.size())
					if (this.m_questItems[i].state() != state) then
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

		private method createQuestLogQuest takes nothing returns nothing
			if (AQuest.useQuestLog) then
				set this.m_questLogQuest = CreateQuest()
				call QuestSetDiscovered(this.m_questLogQuest, false) //hide quest before setting state
				call QuestSetRequired(this.m_questLogQuest, this.character() == 0)
			endif
		endmethod

		public static method create takes ACharacter character, string title returns AQuest
			local AQuest this = AQuest.allocate(character, title)
			//members
			set this.m_questItems = AQuestItemVector.create()

			call this.createQuestLogQuest()
			return this
		endmethod

		private method destroyQuestLogQuest takes nothing returns nothing
			if (AQuest.useQuestLog) then
				call DestroyQuest(this.m_questLogQuest)
				set this.m_questLogQuest = null
			endif
		endmethod
	
		//Alle QuestItems werden auch zerstört
		private method destroyQuestItems takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_questItems.size())
				call AQuestItem.destroy(this.m_questItems[i])
				set i = i + 1
			endloop
		endmethod

		public method onDestroy takes nothing returns nothing
			call this.destroyQuestLogQuest()
			call this.destroyQuestItems()
			call this.m_questItems.destroy()
		endmethod

		//init is already used
		public static method init0 takes boolean useQuestLog, string updateSoundPath returns nothing
			//static start members
			set AQuest.useQuestLog = useQuestLog
			set AQuest.updateSoundPath = updateSoundPath
		endmethod

		//static start members

		//AQuestItem need access
		public static method isQuestLogUsed takes nothing returns boolean
			return AQuest.useQuestLog
		endmethod
	endstruct

endlibrary