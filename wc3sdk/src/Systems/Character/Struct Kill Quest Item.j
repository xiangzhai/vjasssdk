library AStructSystemsCharacterKillQuestItem requires ALibraryCoreStringConversion, AStructSystemsCharacterQuest, AStructSystemsCharacterQuestItem

	/// @state untested
	struct AKillQuestItem extends AQuestItem
		// static construction members
		private static string message
		// construction members
		private integer m_unitTypeId
		private integer m_targetUnitTypeId
		private integer m_count
		// members
		private integer m_killed

		// construction members

		public method unitTypeId takes nothing returns integer
			return this.m_unitTypeId
		endmethod

		public method targetUnitTypeId takes nothing returns integer
			return this.m_targetUnitTypeId
		endmethod

		public method count takes nothing returns integer
			return this.m_count
		endmethod

		// members

		public method killed takes nothing returns integer
			return this.m_killed
		endmethod

		private method stateEventCompleted takes thistype questItem, trigger whichTrigger returns nothing
			call TriggerRegisterAnyUnitEventBJ(whichTrigger, EVENT_PLAYER_UNIT_DEATH)
		endmethod

		private method stateConditionCompleted takes thistype questItem returns boolean
			local unit killer
			local unit triggerUnit
			local boolean result = true
			if (questItem.m_unitTypeId != 0) then
				set killer = GetKillingUnit()
				set result = GetUnitTypeId(killer) == questItem.m_unitTypeId
				set killer = null
			endif
			if (result and questItem.m_targetUnitTypeId != 0) then
				set triggerUnit = GetTriggerUnit()
				set result = GetUnitTypeId(triggerUnit) == questItem.m_targetUnitTypeId
				set triggerUnit = null
			endif
			if (result) then
				set questItem.m_killed = questItem.m_killed + 1
				call questItem.quest().displayUpdateMessage(IntegerArg(IntegerArg(thistype.message, questItem.m_killed), questItem.m_count))
			endif
			return questItem.m_killed == questItem.m_count
		endmethod

		/**
		* @param unitTypeId If this value is 0 all unit types can kill the target unit type.
		* @param targetUnitTypeId If this value is 0 all unit types can be killed by an unit-type unit.
		*/
		public static method create takes AQuest usedQuest, string description, integer unitTypeId, integer targetUnitTypeId, integer count returns thistype
			local thistype this = thistype.allocate(usedQuest, description)
			// construction members
			set this.m_unitTypeId = unitTypeId
			set this.m_targetUnitTypeId = targetUnitTypeId
			set this.m_count = count
			// members
			set this.m_killed = 0

			call this.setStateEvent(AAbstractQuest.stateCompleted, thistype.stateEventCompleted)
			call this.setStateCondition(AAbstractQuest.stateCompleted, thistype.stateConditionCompleted)
			return this
		endmethod

		public static method init0 takes string message returns nothing
			// static construction members
			set thistype.message = message
		endmethod
	endstruct

endlibrary