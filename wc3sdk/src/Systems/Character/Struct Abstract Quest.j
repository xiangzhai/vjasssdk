library AStructSystemsCharacterAbstractQuest requires ALibraryCoreDebugMisc, ALibraryCoreEnvironmentSound, ALibraryCoreStringConversion, AStructSystemsCharacterCharacterHashTable, AStructSystemsCharacterCharacter

	/// @todo Should be a part of @struct AAbstractQuest, vJass bug.
	function interface AAbstractQuestStateEvent takes AAbstractQuest abstractQuest, trigger usedTrigger returns nothing

	/// @todo Should be a part of @struct AAbstractQuest, vJass bug.
	function interface AAbstractQuestStateCondition takes AAbstractQuest abstractQuest returns boolean

	/// @todo Should be a part of @struct AAbstractQuest, vJass bug.
	function interface AAbstractQuestStateAction takes AAbstractQuest abstractQuest returns nothing

	struct AAbstractQuest
		//static constant members
		public static constant integer stateNotUsed = -1
		public static constant integer stateNew = 0
		public static constant integer stateCompleted = 1
		public static constant integer stateFailed = 2
		public static constant integer rewardLevel = 0
		public static constant integer rewardSkillPoints = 1
		public static constant integer rewardExperience = 2
		public static constant integer rewardStrength = 3
		public static constant integer rewardAgility = 4
		public static constant integer rewardIntelligence = 5
		public static constant integer rewardGold = 6
		public static constant integer rewardLumber = 7
		private static constant integer maxStates = 3
		private static constant integer maxRewards = 8
		//static start members
		/// @todo Added default values for debugging.
		private static real pingRate
		private static string stateNewSoundPath = "Sound\\Interface\\QuestNew.wav"
		private static string stateCompletedSoundPath = "Sound\\Interface\\QuestCompleted.wav"
		private static string stateFailedSoundPath = "Sound\\Interface\\QuestFailed.wav"
		private static string textStateNew = "%s Neu"
		private static string textStateCompleted = "%s Abgeschlossen"
		private static string textStateFailed = "%s Fehlgeschlagen"
		private static string textRewardLevels = "+%i Stufe(n)"
		private static string textRewardSkillPoints = "%i Heldenfähigkeiten-Punkt(e)"
		private static string textRewardExperience = "+%i Erfahrung"
		private static string textRewardStrength = "+%i Kraft"
		private static string textRewardAgility = "+%i Beweglichkeit"
		private static string textRewardIntelligence = "+%i Intelligenz"
		private static string textRewardGold = "+%i Gold"
		private static string textRewardLumber = "+%i Holz"
		//static members
		private static AAbstractQuest array abstractQuests
		private static integer abstractQuestCount
		private static timer pingTimer
		//dynamic members
		private integer state /// Should be setted by state method.
		private AAbstractQuestStateCondition array stateCondition[AAbstractQuest.maxStates]
		private AAbstractQuestStateAction array stateAction[AAbstractQuest.maxStates]
		private integer array reward[AAbstractQuest.maxRewards]
		private boolean m_ping
		private real m_pingX
		private real m_pingY
		private real m_pingDuration
		private real m_pingRed
		private real m_pingGreen
		private real m_pingBlue
		//start members
		private ACharacter character
		private string title
		//members
		private trigger array stateTrigger[AAbstractQuest.maxStates]

		//! runtextmacro A_STRUCT_DEBUG("\"AAbstractQuest\"")

		//dynamic members

		public stub method setState takes integer state returns nothing
			debug if ((state >= AAbstractQuest.stateNotUsed) and (state < AAbstractQuest.maxStates)) then
				if (this.stateTrigger[state] == null) then
					if (this.stateCondition[state] != 0 and not this.stateCondition[state].evaluate(this)) then
						return
					endif
					if (this.stateAction[state] != 0) then
						call this.stateAction[state].execute(this) //call custom function
					endif
				endif
				set this.state = state
				debug call Print("Set state to " + I2S(state))
				debug call Print("After displaying message")
				if (state == AAbstractQuest.stateNotUsed) then
					call this.disable()
				elseif (state == AAbstractQuest.stateNew) then
					debug call Print("Enable")
					call this.displayMessage()
					call this.enable()
				elseif (state == AAbstractQuest.stateCompleted) then
					call this.displayMessage()
					call this.distributeRewards()
					call this.disable() //disable
				elseif (state == AAbstractQuest.stateFailed) then
					call this.displayMessage()
					call this.disable() //disable
				endif
			debug else
				debug call Print("Invalid quest state in quest " + I2S(this) + ".") //ALibraryGeneralDebug
			debug endif
		endmethod

		public method getState takes nothing returns integer
			return this.state
		endmethod

		//call first setStateEvent then setStateCondition and at least setStateAction
		public method setStateEvent takes integer state, AAbstractQuestStateEvent stateEvent returns nothing
			if (this.stateTrigger[state] == null) then
				set this.stateTrigger[state] = CreateTrigger()
				call AGetCharacterHashTable().storeHandleInteger(this.stateTrigger[state], "this", this) //AClassCharacterCharacterHashTable
				call AGetCharacterHashTable().storeHandleInteger(this.stateTrigger[state], "state", state) //AClassCharacterCharacterHashTable
				call DisableTrigger(this.stateTrigger[state])
				debug call Print("Create state trigger for state " + I2S(state))
				debug call Print("Saved id is " + I2S(AGetCharacterHashTable().getHandleInteger(this.stateTrigger[state], "this")) + " (has to be " + I2S(this) + " and state is " + I2S(AGetCharacterHashTable().getHandleInteger(this.stateTrigger[state], "state")))
				debug call Print("Handle id: " + I2S(H2I(this.stateTrigger[state])))
			endif
			call stateEvent.execute(this, this.stateTrigger[state]) //evaluate - returns the trigger event
			debug call Print("After state execution with id " + I2S(this))
		endmethod

		public method setStateCondition takes integer state, AAbstractQuestStateCondition stateCondition returns nothing
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			set this.stateCondition[state] = stateCondition
			debug call Print("After adding condition to state trigger: " + I2S(H2I(this.stateTrigger[state])) + " and state " + I2S(state))
			if (this.stateTrigger[state] != null) then
				set conditionFunction = Condition(function AAbstractQuest.triggerConditionRunQuestState)
				set triggerCondition = TriggerAddCondition(this.stateTrigger[state], conditionFunction)
				set conditionFunction = null
				set triggerCondition = null
			endif
			debug call Print("After adding condition with id " + I2S(this) + " and state " + I2S(state))
		endmethod

		public method setStateAction takes integer state, AAbstractQuestStateAction stateAction returns nothing
			local triggeraction triggerAction
			debug call Print("After adding action with state " + I2S(state))
			set this.stateAction[state] = stateAction
			if (this.stateTrigger[state] != null) then
				set triggerAction = TriggerAddAction(this.stateTrigger[state], function AAbstractQuest.triggerActionRunQuestState)
				set triggerAction = null
			debug else
				debug call this.print("TRIGGER IS NULL")
			endif
			debug call Print("After adding action with id " + I2S(this))
		endmethod

		public method setReward takes integer reward, integer value returns nothing
			set this.reward[reward] = value
		endmethod

		public method getReward takes integer reward returns integer
			return this.reward
		endmethod
		
		public method setPing takes boolean ping returns nothing
			set this.m_ping = ping
		endmethod
		
		public method ping takes nothing returns boolean
			return this.m_ping
		endmethod

		public method setPingX takes real pingX returns nothing
			set this.m_pingX = pingX
		endmethod

		public method pingX takes nothing returns real x
			return this.m_pingX
		endmethod

		public method setPingY takes real pingY returns nothing
			set this.m_pingY = pingY
		endmethod

		public method pingY takes nothing returns real
			return this.m_pingY
		endmethod

		public method setPingDuration takes real pingDuration returns nothing
			set this.m_pingDuration = pingDuration
		endmethod

		public method pingDuration takes nothing returns real
			return this.m_pingDuration
		endmethod

		/// @param pingRed Default is 100.0.
		public method setPingRed takes real pingRed returns nothing
			set this.m_pingRed = pingRed
		endmethod

		public method pingRed takes nothing returns real
			return this.m_pingRed
		endmethod

		/// @param pingGreen Default is 100.0.
		public method setPingGreen takes real pingGreen returns nothing
			set this.m_pingGreen = pingGreen
		endmethod

		public method pingGreen takes nothing returns real
			return this.m_pingGreen
		endmethod

		/// @param pingBlue Default is 100.0.
		public method setPingBlue takes real pingBlue returns nothing
			set this.m_pingBlue = pingBlue
		endmethod

		public method pingBlue takes nothing returns real
			return this.m_pingBlue
		endmethod

		//start members

		public method getCharacter takes nothing returns ACharacter
			return this.character
		endmethod

		//comfort methods
		
		public method setPingLocation takes location usedLocation returns nothing
			set this.m_pingX = GetLocationX(usedLocation)
			set this.m_pingY = GetLocationY(usedLocation)
		endmethod
		
		public method setPingRect takes rect usedRect returns nothing
			debug if (usedRect == null) then
				debug call this.print("usedRect is null.")
			debug else
				debug call this.print("usedRect is NOT null.")
			debug endif
			set this.m_pingX = GetRectCenterX(usedRect)
			set this.m_pingY = GetRectCenterY(usedRect)
		endmethod
		
		public method setPingWidget takes widget usedWidget returns nothing
			set this.m_pingX = GetWidgetX(usedWidget)
			set this.m_pingY = GetWidgetY(usedWidget)
		endmethod
		
		public method setPingUnit takes unit usedUnit returns nothing
			set this.m_pingX = GetUnitX(usedUnit)
			set this.m_pingY = GetUnitY(usedUnit)
		endmethod
		
		public method setPingDestructable takes destructable usedDestructable returns nothing
			set this.m_pingX = GetDestructableX(usedDestructable)
			set this.m_pingY = GetDestructableY(usedDestructable)
		endmethod
		
		public method setPingItem takes item usedItem returns nothing
			set this.m_pingX = GetItemX(usedItem)
			set this.m_pingY = GetItemY(usedItem)
		endmethod
		
		public method setPingColour takes real red, real green, real blue returns nothing
			set this.m_pingRed = red
			set this.m_pingGreen = green
			set this.m_pingBlue = blue
		endmethod
		
		//methods

		public method getTitle takes nothing returns string
			return this.title
		endmethod
		
		public method getModifiedTitle takes nothing returns string
			if (this.state == AAbstractQuest.stateNew) then
				return StringArg(AAbstractQuest.textStateNew, this.title) //ALibraryStringConversion
			elseif (this.state == AAbstractQuest.stateCompleted) then
				return StringArg(AAbstractQuest.textStateCompleted, this.title) //ALibraryStringConversion
			elseif (this.state == AAbstractQuest.stateFailed) then
				return StringArg(AAbstractQuest.textStateFailed, this.title) //ALibraryStringConversion
			debug else
				debug call this.print("Unknown state (in getModifiedTitle()): " + I2S(this.state))
			endif
			return this.title
		endmethod

		public method enable takes nothing returns nothing
			call this.enableStateTriggers()
		endmethod

		public method disable takes nothing returns nothing
			call this.disableStateTriggers()
		endmethod

		private method enableStateTriggers takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == AAbstractQuest.maxStates)
				if (this.stateTrigger[i] != null) then
					call EnableTrigger(this.stateTrigger[i])
					debug call this.print("Enable state trigger " + I2S(i))
				endif
				set i = i + 1
			endloop
		endmethod

		private method disableStateTriggers takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == AAbstractQuest.maxStates)
				if (this.stateTrigger[i] != null) then
					call DisableTrigger(this.stateTrigger[i])
				endif
				set i = i + 1
			endloop
		endmethod

		private method getSoundPath takes nothing returns string
			if (this.state == AAbstractQuest.stateNew) then
				return AAbstractQuest.stateNewSoundPath
			elseif (this.state == AAbstractQuest.stateCompleted) then
				return AAbstractQuest.stateCompletedSoundPath
			else
				return AAbstractQuest.stateFailedSoundPath
			endif
			return ""
		endmethod

		private method displayMessage takes nothing returns nothing
			if (this.character != 0) then
				call this.character.displayMessage(ACharacter.messageTypeInfo, this.getModifiedTitle()) //AClassCharacterCharacter
				call PlaySoundPathForPlayer(this.character.getUser(), this.getSoundPath()) //ALibraryEnvironmentSound
			else
				call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, this.getModifiedTitle()) //AClassCharacterCharacter
				call PlaySound(this.getSoundPath())
			endif
		endmethod

		private method displayRewardMessage takes integer reward returns nothing
			local string message
			if (reward == AAbstractQuest.rewardLevel) then
				set message = AAbstractQuest.textRewardLevels
			elseif (reward == AAbstractQuest.rewardSkillPoints) then
				set message = AAbstractQuest.textRewardSkillPoints
			elseif (reward == AAbstractQuest.rewardExperience) then
				set message = AAbstractQuest.textRewardExperience
			elseif (reward == AAbstractQuest.rewardStrength) then
				set message = AAbstractQuest.textRewardStrength
			elseif (reward == AAbstractQuest.rewardAgility) then
				set message = AAbstractQuest.textRewardAgility
			elseif (reward == AAbstractQuest.rewardIntelligence) then
				set message = AAbstractQuest.textRewardIntelligence
			elseif (reward == AAbstractQuest.rewardGold) then
				set message = AAbstractQuest.textRewardGold
			elseif (reward == AAbstractQuest.rewardLumber) then
				set message = AAbstractQuest.textRewardLumber
			debug else
				debug call this.print("Unknown reward: " + I2S(reward))
			endif

			set message = IntegerArg(message, this.reward[reward])

			if (this.character != 0) then
				call this.character.displayMessage(ACharacter.messageTypeInfo, message)
			else
				call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, message)
			endif
		endmethod

		private method distributeRewards takes nothing returns nothing
			local integer i
			if (this.character != 0) then
				if (this.reward[AAbstractQuest.rewardLevel] != 0) then
					call this.character.addLevels(this.reward[AAbstractQuest.rewardLevel], true)
				endif
				if (this.reward[AAbstractQuest.rewardSkillPoints] != 0) then
					call this.character.addSkillPoints(this.reward[AAbstractQuest.rewardSkillPoints])
				endif
				if (this.reward[AAbstractQuest.rewardExperience] != 0) then
					call this.character.addExperience(this.reward[AAbstractQuest.rewardExperience], true)
				endif
				if (this.reward[AAbstractQuest.rewardStrength] != 0) then
					call this.character.addStrength(this.reward[AAbstractQuest.rewardStrength])
				endif
				if (this.reward[AAbstractQuest.rewardAgility] != 0) then
					call this.character.addAgility(this.reward[AAbstractQuest.rewardAgility])
				endif
				if (this.reward[AAbstractQuest.rewardIntelligence] != 0) then
					call this.character.addIntelligence(this.reward[AAbstractQuest.rewardIntelligence])
				endif
				if (this.reward[AAbstractQuest.rewardGold] != 0) then
					call this.character.addGold(this.reward[AAbstractQuest.rewardGold])
				endif
				if (this.reward[AAbstractQuest.rewardLumber] != 0) then
					call this.character.addLumber(this.reward[AAbstractQuest.rewardLumber])
				endif
			else
				if (this.reward[AAbstractQuest.rewardLevel] != 0) then
					call ACharacter.addLevelsToAll(this.reward[AAbstractQuest.rewardLevel], true)
				endif
				if (this.reward[AAbstractQuest.rewardSkillPoints] != 0) then
					call ACharacter.addSkillPointsToAll(this.reward[AAbstractQuest.rewardSkillPoints])
				endif
				if (this.reward[AAbstractQuest.rewardExperience] != 0) then
					call ACharacter.addExperienceToAll(this.reward[AAbstractQuest.rewardExperience], true)
				endif
				if (this.reward[AAbstractQuest.rewardStrength] != 0) then
					call ACharacter.addStrengthToAll(this.reward[AAbstractQuest.rewardStrength])
				endif
				if (this.reward[AAbstractQuest.rewardAgility] != 0) then
					call ACharacter.addAgilityToAll(this.reward[AAbstractQuest.rewardAgility])
				endif
				if (this.reward[AAbstractQuest.rewardIntelligence] != 0) then
					call ACharacter.addIntelligenceToAll(this.reward[AAbstractQuest.rewardIntelligence])
				endif
				if (this.reward[AAbstractQuest.rewardGold] != 0) then
					call ACharacter.addGoldToAll(this.reward[AAbstractQuest.rewardGold])
				endif
				if (this.reward[AAbstractQuest.rewardLumber] != 0) then
					call ACharacter.addLumberToAll(this.reward[AAbstractQuest.rewardLumber])
				endif
			endif
			set i = AAbstractQuest.rewardLevel
			loop
				exitwhen(i == AAbstractQuest.maxRewards)
				if (this.reward[i] != 0) then
					call this.displayRewardMessage(i)
				endif
				set i = i + 1
			endloop
		endmethod

		private static method triggerConditionRunQuestState takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AAbstractQuest this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this") //AClassCharacterCharacterHashTable
			local integer state = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "state") //AClassCharacterCharacterHashTable
			local boolean result = this.stateCondition[state].evaluate(this) /// @todotest
			debug call Print("Handle id: " + I2S(H2I(triggeringTrigger)))
			set triggeringTrigger = null
			debug call Print("QUEST CONDITION for abstract quest " + I2S(this) + " with state " + I2S(state))
			if (this.stateCondition[state] == 0) then
				debug call Print("Condition is 0.")
			endif
			return this.stateCondition[state].evaluate(this)
		endmethod
	
		private static method triggerActionRunQuestState takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AAbstractQuest this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this")
			local integer state = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "state")
			debug call Print("ACTION for abstract quest " + I2S(this) + " with state " + I2S(state))
			call this.setState(state) //custom function will be called in this method
			set triggeringTrigger = null
		endmethod
		
		private method setupStateTriggers takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == AAbstractQuest.maxStates)
				set this.stateTrigger[i] = null
				set i = i + 1
			endloop
		endmethod

		public static method create takes ACharacter character, string title returns AAbstractQuest
			local AAbstractQuest this = AAbstractQuest.allocate()
			//start members
			set this.character = character
			set this.title = title
			//dynamic members
			set this.state = AAbstractQuest.stateNotUsed
			//static members
			set AAbstractQuest.abstractQuests[AAbstractQuest.abstractQuestCount] = this
			set AAbstractQuest.abstractQuestCount = AAbstractQuest.abstractQuestCount + 1

			call this.setupStateTriggers()
			return this
		endmethod

		private method destroyStateTriggers takes nothing returns nothing
			local integer i = AAbstractQuest.stateNew
			loop
				exitwhen (i == AAbstractQuest.maxStates)
				if (this.stateTrigger[i] != null) then
					call AGetCharacterHashTable().destroyTrigger(this.stateTrigger[i])
					set this.stateTrigger[i] = null
				endif
				set i = i + 1
			endloop
		endmethod

		public method onDestroy takes nothing returns nothing
		
			call this.destroyStateTriggers()
		endmethod
		
		private static method timerFunctionPing takes nothing returns nothing
			local AAbstractQuest abstractQuest
			local player user
			local integer i = 0
			loop
				exitwhen (i == AAbstractQuest.abstractQuestCount)
				set abstractQuest = AAbstractQuest.abstractQuests[i]
				if (abstractQuest.m_ping and abstractQuest.state == AAbstractQuest.stateNew) then
					if (abstractQuest.character != 0) then
						set user = abstractQuest.character.getUser()
						call PingMinimapExForPlayer(user, abstractQuest.m_pingX, abstractQuest.m_pingY, abstractQuest.m_pingDuration, abstractQuest.m_pingRed, abstractQuest.m_pingGreen, abstractQuest.m_pingBlue, true)
						set user = null
					else
						call PingMinimapEx(abstractQuest.m_pingX, abstractQuest.m_pingY, abstractQuest.m_pingDuration, PercentTo255(abstractQuest.m_pingRed), PercentTo255(abstractQuest.m_pingGreen), PercentTo255(abstractQuest.m_pingBlue), true)
					endif
				endif
				set i = i + 1
			endloop
		endmethod

		private static method createPingTimer takes nothing returns nothing
			if (AAbstractQuest.pingRate > 0.0) then
				set AAbstractQuest.pingTimer = CreateTimer()
				call TimerStart(AAbstractQuest.pingTimer, AAbstractQuest.pingRate, true, function AAbstractQuest.timerFunctionPing)
			endif
		endmethod

		/// @param pingRate If this value is 0.0 or smaller there won't be any pings.
		public static method init takes real pingRate, string stateNewSoundPath, string stateCompletedSoundPath, string stateFailedSoundPath, string textStateNew, string textStateCompleted, string textStateFailed, string textRewardLevels, string textRewardSkillPoints, string textRewardExperience, string textRewardStrength, string textRewardAgility, string textRewardIntelligence, string textRewardGold, string textRewardLumber returns nothing
			//static members
			set AAbstractQuest.abstractQuestCount = 0
			//static start members
			set AAbstractQuest.pingRate = pingRate
			set AAbstractQuest.stateNewSoundPath = stateNewSoundPath
			set AAbstractQuest.stateCompletedSoundPath = stateCompletedSoundPath
			set AAbstractQuest.stateFailedSoundPath = stateFailedSoundPath
			set AAbstractQuest.textStateNew = textStateNew
			set AAbstractQuest.textStateCompleted = textStateCompleted
			set AAbstractQuest.textStateFailed = textStateFailed
			set AAbstractQuest.textRewardLevels = textRewardLevels
			set AAbstractQuest.textRewardSkillPoints = textRewardSkillPoints
			set AAbstractQuest.textRewardExperience = textRewardExperience
			set AAbstractQuest.textRewardStrength = textRewardStrength
			set AAbstractQuest.textRewardAgility = textRewardAgility
			set AAbstractQuest.textRewardIntelligence = textRewardIntelligence
			set AAbstractQuest.textRewardGold = textRewardGold
			set AAbstractQuest.textRewardLumber = textRewardLumber

			if (stateNewSoundPath != null) then
				call PreloadSoundPath(stateNewSoundPath)
			endif
			if (stateCompletedSoundPath != null) then
				call PreloadSoundPath(stateCompletedSoundPath)
			endif
			if (stateFailedSoundPath != null) then
				call PreloadSoundPath(stateFailedSoundPath)
			endif
			
			call AAbstractQuest.createPingTimer()
		endmethod
		
		private static method destroyPingTimer takes nothing returns nothing
			if (AAbstractQuest.pingRate > 0.0) then
				call DestroyTimer(AAbstractQuest.pingTimer)
				set AAbstractQuest.pingTimer = null
			endif
		endmethod
		
		public static method cleanUp takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == AAbstractQuest.abstractQuestCount)
				call AAbstractQuest.abstractQuests[i].destroy()
				set i = i + 1
			endloop
			call AAbstractQuest.destroyPingTimer()
		endmethod
		
		public static method enablePing takes nothing returns nothing
			if (AAbstractQuest.pingRate != 0.0) then
				call PauseTimerBJ(false, AAbstractQuest.pingTimer)
			debug else
				debug call AAbstractQuest.staticPrint("There is no ping timer.")
			endif
		endmethod
		
		public static method disablePing takes nothing returns nothing
			if (AAbstractQuest.pingRate != 0.0) then
				call PauseTimerBJ(true, AAbstractQuest.pingTimer)
			debug else
				debug call AAbstractQuest.staticPrint("There is no ping timer.")
			endif
		endmethod
	endstruct

endlibrary