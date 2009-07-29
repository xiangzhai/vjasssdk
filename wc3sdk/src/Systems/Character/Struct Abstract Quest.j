library AStructSystemsCharacterAbstractQuest requires ALibraryCoreDebugMisc, ALibraryCoreEnvironmentSound, AStructCoreGeneralHashTable, ALibraryCoreStringConversion, AStructSystemsCharacterCharacter

	//! runtextmacro A_VECTOR("private", "AAbstractQuestVector", "AAbstractQuest", "0", "100") /// @todo @member AAbstractQuest.maxAbstractQuests

	/// @todo Should be a part of @struct AAbstractQuest, vJass bug.
	function interface AAbstractQuestStateEvent takes AAbstractQuest abstractQuest, trigger usedTrigger returns nothing

	/// @todo Should be a part of @struct AAbstractQuest, vJass bug.
	function interface AAbstractQuestStateCondition takes AAbstractQuest abstractQuest returns boolean

	/// @todo Should be a part of @struct AAbstractQuest, vJass bug.
	function interface AAbstractQuestStateAction takes AAbstractQuest abstractQuest returns nothing

	struct AAbstractQuest
		//static constant members
		public static constant integer maxAbstractQuests = 100
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
		private static AAbstractQuestVector abstractQuests
		private static timer pingTimer
		//dynamic members
		private integer m_state /// Should be setted by state method.
		private AAbstractQuestStateCondition array m_stateCondition[AAbstractQuest.maxStates]
		private AAbstractQuestStateAction array m_stateAction[AAbstractQuest.maxStates]
		private integer array m_reward[AAbstractQuest.maxRewards]
		private boolean m_ping
		private real m_pingX
		private real m_pingY
		private real m_pingDuration
		private real m_pingRed
		private real m_pingGreen
		private real m_pingBlue
		//start members
		private ACharacter m_character
		private string m_title
		//members
		private integer m_index
		private trigger array m_stateTrigger[AAbstractQuest.maxStates]

		//! runtextmacro A_STRUCT_DEBUG("\"AAbstractQuest\"")
		
		debug private method checkState takes integer state returns boolean
			debug if ((state <= AAbstractQuest.stateNotUsed) or (state >= AAbstractQuest.maxStates)) then
				debug call this.print("Wrong state: " + I2S(state) + ".")
				debug return false
			debug endif
			debug return true
		debug endmethod

		//dynamic members

		public stub method setState takes integer state returns nothing
			///debug call this.print("Set state to " + I2S(state))
			debug if (this.checkState(state)) then
				if (this.m_stateTrigger[state] == null) then
					if (this.m_stateCondition[state] != 0 and not this.m_stateCondition[state].evaluate(this)) then
						return
					endif
				endif
				set this.m_state = state
				///debug call Print("Set state to " + I2S(state))
				if (state == AAbstractQuest.stateNotUsed) then
					if (this.m_stateTrigger[AAbstractQuest.stateNew] != null) then
						call EnableTrigger(this.m_stateTrigger[AAbstractQuest.stateNew])
					endif
					if (this.m_stateTrigger[AAbstractQuest.stateCompleted] != null) then
						call DisableTrigger(this.m_stateTrigger[AAbstractQuest.stateCompleted])
					endif
					if (this.m_stateTrigger[AAbstractQuest.stateFailed] != null) then
						call DisableTrigger(this.m_stateTrigger[AAbstractQuest.stateFailed])
					endif
				elseif (state == AAbstractQuest.stateNew) then
					if (this.m_stateTrigger[AAbstractQuest.stateNew] != null) then
						call DisableTrigger(this.m_stateTrigger[AAbstractQuest.stateNew])
					endif
					if (this.m_stateTrigger[AAbstractQuest.stateCompleted] != null) then
						call EnableTrigger(this.m_stateTrigger[AAbstractQuest.stateCompleted])
					endif
					if (this.m_stateTrigger[AAbstractQuest.stateFailed] != null) then
						call EnableTrigger(this.m_stateTrigger[AAbstractQuest.stateFailed])
					endif
					call this.displayMessage()
				elseif (state == AAbstractQuest.stateCompleted) then
					if (this.m_stateTrigger[AAbstractQuest.stateNew] != null) then
						call DisableTrigger(this.m_stateTrigger[AAbstractQuest.stateNew])
					endif
					if (this.m_stateTrigger[AAbstractQuest.stateCompleted] != null) then
						call DisableTrigger(this.m_stateTrigger[AAbstractQuest.stateCompleted])
					endif
					if (this.m_stateTrigger[AAbstractQuest.stateFailed] != null) then
						call DisableTrigger(this.m_stateTrigger[AAbstractQuest.stateFailed])
					endif
					call this.displayMessage()
					call this.distributeRewards()
				elseif (state == AAbstractQuest.stateFailed) then
					if (this.m_stateTrigger[AAbstractQuest.stateNew] != null) then
						call DisableTrigger(this.m_stateTrigger[AAbstractQuest.stateNew])
					endif
					if (this.m_stateTrigger[AAbstractQuest.stateCompleted] != null) then
						call DisableTrigger(this.m_stateTrigger[AAbstractQuest.stateCompleted])
					endif
					if (this.m_stateTrigger[AAbstractQuest.stateFailed] != null) then
						call DisableTrigger(this.m_stateTrigger[AAbstractQuest.stateFailed])
					endif
					call this.displayMessage()
				endif
				if (this.m_stateAction[state] != 0) then
					call this.m_stateAction[state].execute(this) //call custom function
				endif
			debug endif
		endmethod

		public method state takes nothing returns integer
			return this.m_state
		endmethod

		//call first setStateEvent then setStateCondition and at least setStateAction
		public method setStateEvent takes integer state, AAbstractQuestStateEvent stateEvent returns nothing
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			debug if (not this.checkState(state)) then
				debug return
			debug endif
			if (this.m_stateTrigger[state] == null) then
				call this.createStateTrigger(state)
			//else
			endif
			call stateEvent.execute(this, this.m_stateTrigger[state])
		endmethod

		public method setStateCondition takes integer state, AAbstractQuestStateCondition stateCondition returns nothing
			debug if (not this.checkState(state)) then
				debug return
			debug endif
			set this.m_stateCondition[state] = stateCondition
		endmethod
		
		public method stateCondition takes integer state returns AAbstractQuestStateCondition
			debug if (not this.checkState(state)) then
				debug return 0
			debug endif
			return this.m_stateCondition[state]
		endmethod

		public method setStateAction takes integer state, AAbstractQuestStateAction stateAction returns nothing
			debug if (not this.checkState(state)) then
				debug return
			debug endif
			set this.m_stateAction[state] = stateAction
		endmethod
		
		public method stateAction takes integer state returns AAbstractQuestStateAction
			debug if (not this.checkState(state)) then
				debug return 0
			debug endif
			return this.m_stateAction[state]
		endmethod

		public method setReward takes integer reward, integer value returns nothing
			set this.m_reward[reward] = value
		endmethod

		public method reward takes integer reward returns integer
			return this.m_reward
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

		public method character takes nothing returns ACharacter
			return this.m_character
		endmethod
		
		public method title takes nothing returns string
			return this.m_title
		endmethod

		//convenience methods
		
		public method setPingLocation takes location usedLocation returns nothing
			set this.m_pingX = GetLocationX(usedLocation)
			set this.m_pingY = GetLocationY(usedLocation)
		endmethod
		
		public method setPingRect takes rect usedRect returns nothing
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
		
		public method getModifiedTitle takes nothing returns string
			if (this.m_state == AAbstractQuest.stateNew) then
				return StringArg(AAbstractQuest.textStateNew, this.m_title)
			elseif (this.m_state == AAbstractQuest.stateCompleted) then
				return StringArg(AAbstractQuest.textStateCompleted, this.m_title)
			elseif (this.m_state == AAbstractQuest.stateFailed) then
				return StringArg(AAbstractQuest.textStateFailed, this.m_title)
			debug else
				debug call this.print("Unknown state (in getModifiedTitle()): " + I2S(this.m_state))
			endif
			return this.m_title
		endmethod

		private method getSoundPath takes nothing returns string
			if (this.m_state == AAbstractQuest.stateNew) then
				return AAbstractQuest.stateNewSoundPath
			elseif (this.m_state == AAbstractQuest.stateCompleted) then
				return AAbstractQuest.stateCompletedSoundPath
			else
				return AAbstractQuest.stateFailedSoundPath
			endif
			return ""
		endmethod

		private method displayMessage takes nothing returns nothing
			if (this.m_character != 0) then
				call this.m_character.displayMessage(ACharacter.messageTypeInfo, this.getModifiedTitle())
				call PlaySoundPathForPlayer(this.m_character.user(), this.getSoundPath())
			else
				call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, this.getModifiedTitle())
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

			set message = IntegerArg(message, this.m_reward[reward])

			if (this.m_character != 0) then
				call this.m_character.displayMessage(ACharacter.messageTypeInfo, message)
			else
				call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, message)
			endif
		endmethod

		private method distributeRewards takes nothing returns nothing
			local integer i
			if (this.m_character != 0) then
				if (this.m_reward[AAbstractQuest.rewardLevel] != 0) then
					call this.m_character.addLevels(this.m_reward[AAbstractQuest.rewardLevel], true)
				endif
				if (this.m_reward[AAbstractQuest.rewardSkillPoints] != 0) then
					call this.m_character.addSkillPoints(this.m_reward[AAbstractQuest.rewardSkillPoints])
				endif
				if (this.m_reward[AAbstractQuest.rewardExperience] != 0) then
					call this.m_character.addExperience(this.m_reward[AAbstractQuest.rewardExperience], true)
				endif
				if (this.m_reward[AAbstractQuest.rewardStrength] != 0) then
					call this.m_character.addStrength(this.m_reward[AAbstractQuest.rewardStrength])
				endif
				if (this.m_reward[AAbstractQuest.rewardAgility] != 0) then
					call this.m_character.addAgility(this.m_reward[AAbstractQuest.rewardAgility])
				endif
				if (this.m_reward[AAbstractQuest.rewardIntelligence] != 0) then
					call this.m_character.addIntelligence(this.m_reward[AAbstractQuest.rewardIntelligence])
				endif
				if (this.m_reward[AAbstractQuest.rewardGold] != 0) then
					call this.m_character.addGold(this.m_reward[AAbstractQuest.rewardGold])
				endif
				if (this.m_reward[AAbstractQuest.rewardLumber] != 0) then
					call this.m_character.addLumber(this.m_reward[AAbstractQuest.rewardLumber])
				endif
			else
				if (this.m_reward[AAbstractQuest.rewardLevel] != 0) then
					call ACharacter.addLevelsToAll(this.m_reward[AAbstractQuest.rewardLevel], true)
				endif
				if (this.m_reward[AAbstractQuest.rewardSkillPoints] != 0) then
					call ACharacter.addSkillPointsToAll(this.m_reward[AAbstractQuest.rewardSkillPoints])
				endif
				if (this.m_reward[AAbstractQuest.rewardExperience] != 0) then
					call ACharacter.addExperienceToAll(this.m_reward[AAbstractQuest.rewardExperience], true)
				endif
				if (this.m_reward[AAbstractQuest.rewardStrength] != 0) then
					call ACharacter.addStrengthToAll(this.m_reward[AAbstractQuest.rewardStrength])
				endif
				if (this.m_reward[AAbstractQuest.rewardAgility] != 0) then
					call ACharacter.addAgilityToAll(this.m_reward[AAbstractQuest.rewardAgility])
				endif
				if (this.m_reward[AAbstractQuest.rewardIntelligence] != 0) then
					call ACharacter.addIntelligenceToAll(this.m_reward[AAbstractQuest.rewardIntelligence])
				endif
				if (this.m_reward[AAbstractQuest.rewardGold] != 0) then
					call ACharacter.addGoldToAll(this.m_reward[AAbstractQuest.rewardGold])
				endif
				if (this.m_reward[AAbstractQuest.rewardLumber] != 0) then
					call ACharacter.addLumberToAll(this.m_reward[AAbstractQuest.rewardLumber])
				endif
			endif
			set i = 0
			loop
				exitwhen(i == AAbstractQuest.maxRewards)
				if (this.m_reward[i] != 0) then
					call this.displayRewardMessage(i)
				endif
				set i = i + 1
			endloop
		endmethod

		private static method triggerConditionRunQuestState takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AAbstractQuest this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			local integer state = AHashTable.global().getHandleInteger(triggeringTrigger, "state")
			local boolean result = true
			if (this.m_stateCondition[state] != 0) then
				set result = this.m_stateCondition[state].evaluate(this)
			endif
			set triggeringTrigger = null
			return result
		endmethod
	
		private static method triggerActionRunQuestState takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AAbstractQuest this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			local integer state = AHashTable.global().getHandleInteger(triggeringTrigger, "state")
			//if (this.getType() == AQuest.typeid) then
			//	debug call this.print("Is quest")
			//	call AQuest(this).setState(state)
			//elseif (this.getType() == AQuestItem.typeid) then
			//	debug call this.print("Is quest item")
			//	call AQuestItem(this).setState(state)
			//else
			//	debug call this.print("Is neither quest nor quest item.")
				call this.setState(state) //custom function will be called in this method
			//endif
			set triggeringTrigger = null
		endmethod
		
		/// Condition and action definition must be above!!!!!!!!!!!!!!!!!!!!!!
		private method createStateTrigger takes integer state returns nothing
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.m_stateTrigger[state] = CreateTrigger()
			set conditionFunction = Condition(function AAbstractQuest.triggerConditionRunQuestState)
			set triggerCondition = TriggerAddCondition(this.m_stateTrigger[state], conditionFunction)
			set triggerAction = TriggerAddAction(this.m_stateTrigger[state], function AAbstractQuest.triggerActionRunQuestState)
			call AHashTable.global().storeHandleInteger(this.m_stateTrigger[state], "this", this)
			call AHashTable.global().storeHandleInteger(this.m_stateTrigger[state], "state", state)
			if ((this.m_state != AAbstractQuest.stateNew and state != AAbstractQuest.stateNew) or (this.m_state == AAbstractQuest.stateFailed or this.m_state == AAbstractQuest.stateCompleted)) then /// new should be enable by default
				call DisableTrigger(this.m_stateTrigger[state])
			endif
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		public static method create takes ACharacter character, string title returns AAbstractQuest
			local AAbstractQuest this = AAbstractQuest.allocate()
			//dynamic members
			set this.m_state = AAbstractQuest.stateNotUsed
			//start members
			set this.m_character = character
			set this.m_title = title
			//static members
			call AAbstractQuest.abstractQuests.pushBack(this)
			//members
			set this.m_index = AAbstractQuest.abstractQuests.backIndex()

			return this
		endmethod

		private method destroyStateTriggers takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == AAbstractQuest.maxStates)
				if (this.m_stateTrigger[i] != null) then
					call AHashTable.global().destroyTrigger(this.m_stateTrigger[i])
					set this.m_stateTrigger[i] = null
				endif
				set i = i + 1
			endloop
		endmethod

		public method onDestroy takes nothing returns nothing
			//static members
			call AAbstractQuest.abstractQuests.erase(this.m_index)
			
			call this.destroyStateTriggers()
		endmethod
		
		private static method timerFunctionPing takes nothing returns nothing
			local AAbstractQuest abstractQuest
			local player user
			local integer i = 0
			loop
				exitwhen (i == AAbstractQuest.abstractQuests.size())
				set abstractQuest = AAbstractQuest.abstractQuests[i]
				if (abstractQuest.m_ping and abstractQuest.m_state == AAbstractQuest.stateNew) then
					if (abstractQuest.m_character != 0) then
						set user = abstractQuest.m_character.user()
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
			//static members
			set AAbstractQuest.abstractQuests = AAbstractQuestVector.create()

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
			local integer i = AAbstractQuest.abstractQuests.backIndex()
			loop
				exitwhen (i < 0)
				call AAbstractQuest.abstractQuests[i].destroy()
				set i = i - 1
			endloop
			//static members
			call AAbstractQuest.abstractQuests.destroy()
			
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