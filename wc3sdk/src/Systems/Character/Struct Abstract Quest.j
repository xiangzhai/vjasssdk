library AStructSystemsCharacterAbstractQuest requires optional ALibraryCoreDebugMisc, ALibraryCoreEnvironmentSound, AStructCoreGeneralHashTable, ALibraryCoreStringConversion, AStructSystemsCharacterCharacter

	/// @todo Should be a part of @struct AAbstractQuest, vJass bug.
	function interface AAbstractQuestStateEvent takes AAbstractQuest abstractQuest, trigger usedTrigger returns nothing

	/// @todo Should be a part of @struct AAbstractQuest, vJass bug.
	function interface AAbstractQuestStateCondition takes AAbstractQuest abstractQuest returns boolean

	/// @todo Should be a part of @struct AAbstractQuest, vJass bug.
	function interface AAbstractQuestStateAction takes AAbstractQuest abstractQuest returns nothing

	struct AAbstractQuest
		// static constant members
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
		// static construction members
		private static real m_pingRate
		private static string m_stateNewSoundPath
		private static string m_stateCompletedSoundPath
		private static string m_stateFailedSoundPath
		private static string m_textStateNew
		private static string m_textStateCompleted
		private static string m_textStateFailed
		private static string m_textRewardLevels
		private static string m_textRewardSkillPoints
		private static string m_textRewardExperience
		private static string m_textRewardStrength
		private static string m_textRewardAgility
		private static string m_textRewardIntelligence
		private static string m_textRewardGold
		private static string m_textRewardLumber
		// static members
		private static AIntegerVector m_abstractQuests
		private static timer m_pingTimer
		// dynamic members
		private integer m_state /// Should be setted by state method.
		private AAbstractQuestStateCondition array m_stateCondition[thistype.maxStates]
		private AAbstractQuestStateAction array m_stateAction[thistype.maxStates]
		private integer array m_reward[thistype.maxRewards]
		private boolean m_ping
		private real m_pingX
		private real m_pingY
		private real m_pingDuration
		private real m_pingRed
		private real m_pingGreen
		private real m_pingBlue
		private widget m_pingWidget
		private boolean m_distributeRewardsOnCompletion
		// construction members
		private ACharacter m_character
		private string m_title
		// members
		private integer m_index
		private trigger array m_stateTrigger[thistype.maxStates]

		//! runtextmacro optional A_STRUCT_DEBUG("\"AAbstractQuest\"")

		debug private method checkState takes integer state returns boolean
			debug if ((state <= thistype.stateNotUsed) or (state >= thistype.maxStates)) then
				debug call this.print("Wrong state: " + I2S(state) + ".")
				debug return false
			debug endif
			debug return true
		debug endmethod

		// dynamic members

		public method modifiedTitle takes nothing returns string
			if (this.m_state == thistype.stateNew) then
				return StringArg(thistype.m_textStateNew, this.m_title)
			elseif (this.m_state == thistype.stateCompleted) then
				return StringArg(thistype.m_textStateCompleted, this.m_title)
			elseif (this.m_state == thistype.stateFailed) then
				return StringArg(thistype.m_textStateFailed, this.m_title)
			debug else
				debug call this.print("Unknown state (in getModifiedTitle()): " + I2S(this.m_state))
			endif
			return this.m_title
		endmethod

		public method soundPath takes nothing returns string
			if (this.m_state == thistype.stateNew) then
				return thistype.m_stateNewSoundPath
			elseif (this.m_state == thistype.stateCompleted) then
				return thistype.m_stateCompletedSoundPath
			else
				return thistype.m_stateFailedSoundPath
			endif
			return ""
		endmethod

		private method displayRewardMessage takes integer reward returns nothing
			local string message
			if (reward == thistype.rewardLevel) then
				set message = thistype.m_textRewardLevels
			elseif (reward == thistype.rewardSkillPoints) then
				set message = thistype.m_textRewardSkillPoints
			elseif (reward == thistype.rewardExperience) then
				set message = thistype.m_textRewardExperience
			elseif (reward == thistype.rewardStrength) then
				set message = thistype.m_textRewardStrength
			elseif (reward == thistype.rewardAgility) then
				set message = thistype.m_textRewardAgility
			elseif (reward == thistype.rewardIntelligence) then
				set message = thistype.m_textRewardIntelligence
			elseif (reward == thistype.rewardGold) then
				set message = thistype.m_textRewardGold
			elseif (reward == thistype.rewardLumber) then
				set message = thistype.m_textRewardLumber
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

		/// @todo Should be protected
		public method distributeRewards takes nothing returns nothing
			local integer i
			if (this.m_character != 0) then
				if (this.m_reward[thistype.rewardLevel] != 0) then
					call this.m_character.addLevels(this.m_reward[thistype.rewardLevel], true)
				endif
				if (this.m_reward[thistype.rewardSkillPoints] != 0) then
					call this.m_character.addSkillPoints(this.m_reward[thistype.rewardSkillPoints])
				endif
				if (this.m_reward[thistype.rewardExperience] != 0) then
					call this.m_character.addExperience(this.m_reward[thistype.rewardExperience], true)
				endif
				if (this.m_reward[thistype.rewardStrength] != 0) then
					call this.m_character.addStrength(this.m_reward[thistype.rewardStrength])
				endif
				if (this.m_reward[thistype.rewardAgility] != 0) then
					call this.m_character.addAgility(this.m_reward[thistype.rewardAgility])
				endif
				if (this.m_reward[thistype.rewardIntelligence] != 0) then
					call this.m_character.addIntelligence(this.m_reward[thistype.rewardIntelligence])
				endif
				if (this.m_reward[thistype.rewardGold] != 0) then
					call this.m_character.addGold(this.m_reward[thistype.rewardGold])
				endif
				if (this.m_reward[thistype.rewardLumber] != 0) then
					call this.m_character.addLumber(this.m_reward[thistype.rewardLumber])
				endif
			else
				if (this.m_reward[thistype.rewardLevel] != 0) then
					call ACharacter.addLevelsToAll(this.m_reward[thistype.rewardLevel], true)
				endif
				if (this.m_reward[thistype.rewardSkillPoints] != 0) then
					call ACharacter.addSkillPointsToAll(this.m_reward[thistype.rewardSkillPoints])
				endif
				if (this.m_reward[thistype.rewardExperience] != 0) then
					call ACharacter.addExperienceToAll(this.m_reward[thistype.rewardExperience], true)
				endif
				if (this.m_reward[thistype.rewardStrength] != 0) then
					call ACharacter.addStrengthToAll(this.m_reward[thistype.rewardStrength])
				endif
				if (this.m_reward[thistype.rewardAgility] != 0) then
					call ACharacter.addAgilityToAll(this.m_reward[thistype.rewardAgility])
				endif
				if (this.m_reward[thistype.rewardIntelligence] != 0) then
					call ACharacter.addIntelligenceToAll(this.m_reward[thistype.rewardIntelligence])
				endif
				if (this.m_reward[thistype.rewardGold] != 0) then
					call ACharacter.addGoldToAll(this.m_reward[thistype.rewardGold])
				endif
				if (this.m_reward[thistype.rewardLumber] != 0) then
					call ACharacter.addLumberToAll(this.m_reward[thistype.rewardLumber])
				endif
			endif
			set i = 0
			loop
				exitwhen(i == thistype.maxRewards)
				if (this.m_reward[i] != 0) then
					call this.displayRewardMessage(i)
				endif
				set i = i + 1
			endloop
		endmethod

		/**
		* Reimplement this method to use your custom condition or
		* define a custom function by using method @method AAbstractQuest.setStateCondition.
		*/
		public stub method onStateCondition takes integer state returns boolean
			if (this.m_stateCondition[state] != 0 and not this.m_stateCondition[state].evaluate(this)) then
				return false
			endif
			return true
		endmethod

		/**
		* Reimplement this method to use your custom action or
		* define a custom function by using method @method AAbstractQuest.setStateAction.
		*/
		public stub method onStateAction takes integer state returns nothing
			if (this.m_stateAction[state] != 0) then
				call this.m_stateAction[state].execute(this) //call custom function
			endif
		endmethod

		// used by state trigger
		/// @todo Should be protected
		public stub method setStateWithoutCondition takes integer state returns nothing
			debug if (this.checkState(state)) then
				set this.m_state = state
				if (state == thistype.stateNotUsed) then
					if (this.m_stateTrigger[thistype.stateNew] != null) then
						call EnableTrigger(this.m_stateTrigger[thistype.stateNew])
					endif
					if (this.m_stateTrigger[thistype.stateCompleted] != null) then
						call DisableTrigger(this.m_stateTrigger[thistype.stateCompleted])
					endif
					if (this.m_stateTrigger[thistype.stateFailed] != null) then
						call DisableTrigger(this.m_stateTrigger[thistype.stateFailed])
					endif
				elseif (state == thistype.stateNew) then
					if (this.m_stateTrigger[thistype.stateNew] != null) then
						call DisableTrigger(this.m_stateTrigger[thistype.stateNew])
					endif
					if (this.m_stateTrigger[thistype.stateCompleted] != null) then
						call EnableTrigger(this.m_stateTrigger[thistype.stateCompleted])
					endif
					if (this.m_stateTrigger[thistype.stateFailed] != null) then
						call EnableTrigger(this.m_stateTrigger[thistype.stateFailed])
					endif
				elseif (state == thistype.stateCompleted) then
					if (this.m_stateTrigger[thistype.stateNew] != null) then
						call DisableTrigger(this.m_stateTrigger[thistype.stateNew])
					endif
					if (this.m_stateTrigger[thistype.stateCompleted] != null) then
						call DisableTrigger(this.m_stateTrigger[thistype.stateCompleted])
					endif
					if (this.m_stateTrigger[thistype.stateFailed] != null) then
						call DisableTrigger(this.m_stateTrigger[thistype.stateFailed])
					endif
					if (this.m_distributeRewardsOnCompletion) then
						call this.distributeRewards()
					endif
				elseif (state == thistype.stateFailed) then
					if (this.m_stateTrigger[thistype.stateNew] != null) then
						call DisableTrigger(this.m_stateTrigger[thistype.stateNew])
					endif
					if (this.m_stateTrigger[thistype.stateCompleted] != null) then
						call DisableTrigger(this.m_stateTrigger[thistype.stateCompleted])
					endif
					if (this.m_stateTrigger[thistype.stateFailed] != null) then
						call DisableTrigger(this.m_stateTrigger[thistype.stateFailed])
					endif
				endif
				call this.onStateAction(state)
			debug endif
		endmethod

		/// Call this method if you want to set the state manually.
		public stub method setState takes integer state returns boolean
			debug if (this.checkState(state)) then
				if (not this.onStateCondition(state)) then
					return false
				endif
				call this.setStateWithoutCondition(state)
				return true
			debug endif
			debug return false
		endmethod

		public method state takes nothing returns integer
			return this.m_state
		endmethod

		// call first setStateEvent then setStateCondition and at least setStateAction
		public method setStateEvent takes integer state, AAbstractQuestStateEvent stateEvent returns nothing
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			debug if (not this.checkState(state)) then
				debug return
			debug endif
			if (this.m_stateTrigger[state] == null) then
				call this.createStateTrigger.evaluate(state)
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

		/**
		* If you set any ping widget to abstract quest instance, it will be pinged dynamically at its current position.
		* If you want to use its initial position use @function GetWidgetX and @function GetWidgetY in methods @method AAbstractQuest.setPingX and @method AAbstractQuest.setPingY.
		*/
		public method setPingWidget takes widget pingWidget returns nothing
			set this.m_pingWidget = pingWidget
		endmethod

		public method pingWidget takes nothing returns widget
			return this.m_pingWidget
		endmethod

		public method setDistributeRewardsOnCompletion takes boolean distribute returns nothing
			set this.m_distributeRewardsOnCompletion = distribute
		endmethod

		public method distributeRewardsOnCompletion takes nothing returns boolean
			return this.m_distributeRewardsOnCompletion
		endmethod

		// construction members

		public method character takes nothing returns ACharacter
			return this.m_character
		endmethod

		public method title takes nothing returns string
			return this.m_title
		endmethod

		// convenience methods

		public method setPingLocation takes location usedLocation returns nothing
			set this.m_pingX = GetLocationX(usedLocation)
			set this.m_pingY = GetLocationY(usedLocation)
		endmethod

		public method setPingCoordinatesFromRect takes rect usedRect returns nothing
			set this.m_pingX = GetRectCenterX(usedRect)
			set this.m_pingY = GetRectCenterY(usedRect)
		endmethod

		public method setPingCoordinatesFromWidget takes widget usedWidget returns nothing
			set this.m_pingX = GetWidgetX(usedWidget)
			set this.m_pingY = GetWidgetY(usedWidget)
		endmethod

		public method setPingCoordinatesFromUnit takes unit usedUnit returns nothing
			set this.m_pingX = GetUnitX(usedUnit)
			set this.m_pingY = GetUnitY(usedUnit)
		endmethod

		public method setPingCoordinatesFromDestructable takes destructable usedDestructable returns nothing
			set this.m_pingX = GetDestructableX(usedDestructable)
			set this.m_pingY = GetDestructableY(usedDestructable)
		endmethod

		public method setPingCoordinatesFromItem takes item usedItem returns nothing
			set this.m_pingX = GetItemX(usedItem)
			set this.m_pingY = GetItemY(usedItem)
		endmethod

		public method setPingUnit takes unit pingUnit returns nothing
			call this.setPingWidget(pingUnit)
		endmethod

		public method setPingDestructable takes destructable whichDestructable returns nothing
			call this.setPingWidget(whichDestructable)
		endmethod

		public method setPingItem takes item whichItem returns nothing
			call this.setPingWidget(whichItem)
		endmethod

		public method setPingColour takes real red, real green, real blue returns nothing
			set this.m_pingRed = red
			set this.m_pingGreen = green
			set this.m_pingBlue = blue
		endmethod

		public method isNotUsed takes nothing returns boolean
			return this.m_state == thistype.stateNotUsed
		endmethod

		public method isNew takes nothing returns boolean
			return this.m_state == thistype.stateNew
		endmethod

		public method isCompleted takes nothing returns boolean
			return this.m_state == thistype.stateCompleted
		endmethod

		public method isFailed takes nothing returns boolean
			return this.m_state == thistype.stateFailed
		endmethod

		// methods

		private static method triggerConditionRunQuestState takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local integer state = AHashTable.global().handleInteger(triggeringTrigger, "state")
			local boolean result = this.onStateCondition(state)
			set triggeringTrigger = null
			return result
		endmethod

		private static method triggerActionRunQuestState takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local integer state = AHashTable.global().handleInteger(triggeringTrigger, "state")
			/*
			if (this.getType() == AQuest.typeid) then
				call AQuest(this).setState(state)
				debug call this.print("Is AQuest!")
			elseif (this.getType() == AQuestItem.typeid) then
				call AQuestItem(this).setState(state)
				debug call this.print("Is AQuestItem!")
			else
				call this.setState(state) //custom function will be called in this method
				debug call this.print("Is not AQuest and AQuestItem!")
			endif
			*/
			call this.setStateWithoutCondition(state) // condition has already been checked
			set triggeringTrigger = null
		endmethod

		private method createStateTrigger takes integer state returns nothing
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.m_stateTrigger[state] = CreateTrigger()
			set conditionFunction = Condition(function thistype.triggerConditionRunQuestState)
			set triggerCondition = TriggerAddCondition(this.m_stateTrigger[state], conditionFunction)
			set triggerAction = TriggerAddAction(this.m_stateTrigger[state], function thistype.triggerActionRunQuestState)
			call AHashTable.global().setHandleInteger(this.m_stateTrigger[state], "this", this)
			call AHashTable.global().setHandleInteger(this.m_stateTrigger[state], "state", state)
			if ((this.m_state != thistype.stateNew and state != thistype.stateNew) or (this.m_state == thistype.stateFailed or this.m_state == thistype.stateCompleted)) then /// new should be enable by default
				call DisableTrigger(this.m_stateTrigger[state])
			endif
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		public static method create takes ACharacter character, string title returns thistype
			local thistype this = thistype.allocate()
			// dynamic members
			set this.m_state = thistype.stateNotUsed
			set this.m_pingWidget = null
			set this.m_distributeRewardsOnCompletion = true
			// construction members
			set this.m_character = character
			set this.m_title = title
			// static members
			call thistype.m_abstractQuests.pushBack(this)
			// members
			set this.m_index = thistype.m_abstractQuests.backIndex()

			return this
		endmethod

		private method destroyStateTriggers takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.maxStates)
				if (this.m_stateTrigger[i] != null) then
					call AHashTable.global().destroyTrigger(this.m_stateTrigger[i])
					set this.m_stateTrigger[i] = null
				endif
				set i = i + 1
			endloop
		endmethod

		public method onDestroy takes nothing returns nothing
			// static members
			call thistype.m_abstractQuests.erase(this.m_index)

			call this.destroyStateTriggers()
		endmethod

		private static method timerFunctionPing takes nothing returns nothing
			local thistype abstractQuest
			local player user
			local real x
			local real y
			local integer i = thistype.m_abstractQuests.backIndex()
			loop
				exitwhen (i < 0)
				set abstractQuest = thistype.m_abstractQuests[i]
				if (abstractQuest.m_ping and abstractQuest.m_state == thistype.stateNew) then
					if (abstractQuest.m_pingWidget != null) then
						set x = GetWidgetX(abstractQuest.m_pingWidget)
						set y = GetWidgetY(abstractQuest.m_pingWidget)
					else
						set x = abstractQuest.m_pingX
						set y = abstractQuest.m_pingY
					endif
					if (abstractQuest.m_character != 0) then
						set user = abstractQuest.m_character.player()
						call PingMinimapExForPlayer(user, x, y, abstractQuest.m_pingDuration, abstractQuest.m_pingRed, abstractQuest.m_pingGreen, abstractQuest.m_pingBlue, true)
						set user = null
					else
						call PingMinimapEx(x, y, abstractQuest.m_pingDuration, PercentTo255(abstractQuest.m_pingRed), PercentTo255(abstractQuest.m_pingGreen), PercentTo255(abstractQuest.m_pingBlue), true)
					endif
				endif
				set i = i - 1
			endloop
		endmethod

		/// @param pingRate If this value is 0.0 or smaller there won't be any pings.
		public static method init takes real pingRate, string stateNewSoundPath, string stateCompletedSoundPath, string stateFailedSoundPath, string textStateNew, string textStateCompleted, string textStateFailed, string textRewardLevels, string textRewardSkillPoints, string textRewardExperience, string textRewardStrength, string textRewardAgility, string textRewardIntelligence, string textRewardGold, string textRewardLumber returns nothing
			// static construction members
			set thistype.m_pingRate = pingRate
			set thistype.m_stateNewSoundPath = stateNewSoundPath
			set thistype.m_stateCompletedSoundPath = stateCompletedSoundPath
			set thistype.m_stateFailedSoundPath = stateFailedSoundPath
			set thistype.m_textStateNew = textStateNew
			set thistype.m_textStateCompleted = textStateCompleted
			set thistype.m_textStateFailed = textStateFailed
			set thistype.m_textRewardLevels = textRewardLevels
			set thistype.m_textRewardSkillPoints = textRewardSkillPoints
			set thistype.m_textRewardExperience = textRewardExperience
			set thistype.m_textRewardStrength = textRewardStrength
			set thistype.m_textRewardAgility = textRewardAgility
			set thistype.m_textRewardIntelligence = textRewardIntelligence
			set thistype.m_textRewardGold = textRewardGold
			set thistype.m_textRewardLumber = textRewardLumber
			// static members
			set thistype.m_abstractQuests = AIntegerVector.create()
			if (thistype.m_pingRate > 0.0) then
				set thistype.m_pingTimer = CreateTimer()
				call TimerStart(thistype.m_pingTimer, thistype.m_pingRate, true, function thistype.timerFunctionPing)
			endif

			if (stateNewSoundPath != null) then
				call PreloadSoundFile(stateNewSoundPath)
			endif
			if (stateCompletedSoundPath != null) then
				call PreloadSoundFile(stateCompletedSoundPath)
			endif
			if (stateFailedSoundPath != null) then
				call PreloadSoundFile(stateFailedSoundPath)
			endif
		endmethod

		public static method cleanUp takes nothing returns nothing
			loop
				exitwhen (thistype.m_abstractQuests.empty())
				call thistype(thistype.m_abstractQuests.back()).destroy()
			endloop
			// static members
			call thistype.m_abstractQuests.destroy()
			if (thistype.m_pingRate > 0.0) then
				call DestroyTimer(thistype.m_pingTimer)
				set thistype.m_pingTimer = null
			endif
		endmethod

		public static method enablePing takes nothing returns nothing
			if (thistype.m_pingRate != 0.0) then
				call ResumeTimer(thistype.m_pingTimer)
			debug else
				debug call thistype.staticPrint("There is no ping timer.")
			endif
		endmethod

		public static method disablePing takes nothing returns nothing
			if (thistype.m_pingRate != 0.0) then
				call PauseTimer(thistype.m_pingTimer)
			debug else
				debug call thistype.staticPrint("There is no ping timer.")
			endif
		endmethod
	endstruct

endlibrary