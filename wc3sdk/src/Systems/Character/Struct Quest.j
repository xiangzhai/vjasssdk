library AStructSystemsCharacterQuest requires ALibraryCoreDebugMisc, ALibraryCoreEnvironmentSound, AStructSystemsCharacterAbstractQuest

	struct AQuest extends AAbstractQuest
		private static constant integer maxQuestItems = 5
		//static start members
		private static boolean useQuestLog = true //test
		private static string updateSoundPath = "Sound\\Interface\\QuestLog.wav" //test
		//dynamic members
		private AQuestItem array questItem[AQuest.maxQuestItems]
		private string iconPath
		private string description
		//properties
		private integer questItems
		private quest questLogQuest

		//! runtextmacro A_STRUCT_DEBUG("\"AQuest\"")

		//dynamic members

		public method addQuestItem takes AQuestItem questItem returns integer
			local integer id = this.questItems
			if (id < AQuest.maxQuestItems) then
				set this.questItems = this.questItems + 1
				set this.questItem[id] = questItem
				return id
			endif
			return -1
		endmethod

		public method getQuestItem takes integer id returns AQuestItem
			return this.questItem[id]
		endmethod
		
		public method setIconPath takes string iconPath returns nothing
			debug if (not AQuest.useQuestLog) then
				debug call this.print("setIconPath() was called (quest log is disabled).")
			debug endif
			set this.iconPath = iconPath
			call QuestSetIconPath(this.questLogQuest, iconPath)
		endmethod
		
		public method getIconPath takes nothing returns string
			return this.iconPath
		endmethod

		/// No flash, just when you change the state!
		/// Description also is not used as start property because you do not always use the quest log.
		public method setDescription takes string description returns nothing
			debug if (not AQuest.useQuestLog) then
				debug call this.print("setDescription() was called (quest log is disabled).")
			debug endif
			set this.description = description
			call QuestSetDescription(this.questLogQuest, description)
		endmethod

		public method getDescription takes nothing returns string
			return this.description
		endmethod

		//members

		/// Used by AQuestItem, do not use.
		public method getQuestLogQuest takes nothing returns quest
			return this.questLogQuest
		endmethod

		//methods

		public stub method setState takes integer state returns nothing
			//local integer i
			call super.setState(state)
			//set i = 0
			//loop
				//exitwhen (i == this.questItems)
				//if (this.questItem[i].getState() != state) then
					//call this.questItem[i].setState(state)  //AClassCharacterAbstractQuest
				//endif
				//set i = i + 1
			//endloop
			if (AQuest.useQuestLog) then
				call QuestSetTitle(this.questLogQuest, this.getTitle())
				//call QuestSetDescription(this.questLogQuest, this.description)
				if (state == AAbstractQuest.stateNotUsed) then
					call QuestSetDiscovered(this.questLogQuest, false)
				elseif (state == AAbstractQuest.stateNew) then
					call QuestSetDiscovered(this.questLogQuest, true)
					call FlashQuestDialogButton()
					call ForceQuestDialogUpdate() //required?
				elseif (state == AAbstractQuest.stateCompleted) then
					call QuestSetCompleted(this.questLogQuest, true)
					call FlashQuestDialogButton()
					call ForceQuestDialogUpdate() //required?
				elseif (state == AAbstractQuest.stateFailed) then
					call QuestSetFailed(this.questLogQuest, true)
					call FlashQuestDialogButton()
					call ForceQuestDialogUpdate() //required?
				endif
			endif
		endmethod

		public method displayUpdateMessage takes string message returns nothing
			call DisplayTimedTextToPlayer(this.getCharacter().getUser(), 0.0, 0.0, 20.0, this.getTitle())
			call DisplayTimedTextToPlayer(this.getCharacter().getUser(), 0.0, 0.0, 20.0, message)
			call PlaySoundPathForPlayer(this.getCharacter().getUser(), AQuest.updateSoundPath) //ALibraryEnvironmentSound
		endmethod

		//Wenn alle QuestItems den gleichen State haben, erhlt das Quest ebenfalls diesen State
		public method checkQuestItemsForState takes integer state returns boolean
			local integer i
			local boolean result
			//Hat noch nicht den State
			if (this.getState() != state) then
				set result = true
				set i = 0
				loop
					exitwhen(i == this.questItems)
					if (this.questItem[i].getState() != state) then
						set result = false
					endif
					set i = (i + 1)
				endloop
				if (result) then
					call this.setState(state)
				endif
			endif
			return result
		endmethod

		private method createQuestLogQuest takes nothing returns nothing
			if (AQuest.useQuestLog) then
				set this.questLogQuest = CreateQuest()
				call QuestSetDiscovered(this.questLogQuest, false) //hide quest before setting state
				call QuestSetRequired(this.questLogQuest, this.getCharacter() == 0)
			endif
		endmethod

		public static method create takes ACharacter character, string title returns AQuest
			local AQuest this = AQuest.allocate(character, title)
			//members
			set this.questItems = 0

			call this.createQuestLogQuest()
			return this
		endmethod

		private method destroyQuestLogQuest takes nothing returns nothing
			if (AQuest.useQuestLog) then
				call DestroyQuest(this.questLogQuest)
				set this.questLogQuest = null
			endif
		endmethod
	
		//Alle QuestItems werden auch zerstrt
		private method destroyQuestItems takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.questItems)
				call AQuestItem.destroy(this.questItem)
				set i = i + 1
			endloop
		endmethod

		public method onDestroy takes nothing returns nothing
			call this.destroyQuestLogQuest()
			call this.destroyQuestItems()
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