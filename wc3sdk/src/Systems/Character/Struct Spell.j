/// This library contains the ASpell struct which is used for character spells.
/// @author Tamino Dauth
library AStructSystemsCharacterSpell requires ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, ALibraryCoreEnvironmentUnit, AStructSystemsCharacterAbstractCharacterSystem

	/// @todo vJass bug, should be a part of @struct ASpell.
	function interface ASpellUpgradeAction takes ASpell spell, integer level returns nothing

	/// @todo vJass bug, should be a part of @struct ASpell.
	function interface ASpellCastCondition takes ASpell spell returns boolean
	
	/// @todo vJass bug, should be a part of @struct ASpell.
	function interface ASpellCastAction takes ASpell spell returns nothing

	/// This struct represents exactly one spell which is owned by a character.
	/// @author Tamino Dauth
	/// @param Hello
	struct ASpell extends AAbstractCharacterSystem
		//start members
		private integer m_ability
		private ASpellUpgradeAction m_upgradeAction
		private ASpellCastCondition m_castCondition
		private ASpellCastAction m_castAction
		//members
		private trigger upgradeTrigger
		private trigger castTrigger
		
		//! runtextmacro A_STRUCT_DEBUG("\"ASpell\"")

		//start members
		
		public method ability takes nothing returns integer
			return this.m_ability
		endmethod
		
		public method upgradeAction takes nothing returns ASpellUpgradeAction
			return this.m_upgradeAction
		endmethod
		
		public method castCondition takes nothing returns ASpellCastCondition
			return this.m_castCondition
		endmethod
		
		public method castAction takes nothing returns ASpellCastAction
			return this.m_castAction
		endmethod
		
		//convenience methods
		
		public method name takes nothing returns string
			return GetObjectName(this.m_ability)
		endmethod
		
		public method increaseLevel takes nothing returns nothing
			call IncUnitAbilityLevel(this.unit(), this.m_ability)
		endmethod
		
		public method decreaseLevel takes nothing returns nothing
			call DecUnitAbilityLevel(this.unit(), this.m_ability)
		endmethod
		
		public method setLevel takes integer level returns nothing
			call SetUnitAbilityLevel(this.unit(), this.m_ability, level)
		endmethod
		
		public method level takes nothing returns integer
			return GetUnitAbilityLevel(this.unit(), this.m_ability)
		endmethod
		
		public method add takes nothing returns boolean
			return UnitAddAbility(this.unit(), this.m_ability)
		endmethod
		
		public method remove takes nothing returns boolean
			return UnitRemoveAbility(this.unit(), this.m_ability)
		endmethod
		
		//methods
		
		//Make it available
		public method enable takes nothing returns nothing
		endmethod

		public method disable takes nothing returns nothing
		endmethod
		
		private static method triggerConditionRightAbility takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ASpell this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			local boolean result = (GetLearnedSkill() == this.m_ability)
			set triggeringTrigger = null
			return result
		endmethod

		private static method triggerActionUpgrade takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ASpell this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			call this.m_upgradeAction.execute(this, GetLearnedSkillLevel())
			set triggeringTrigger = null
		endmethod

		/// @todo upgradeAction won't be called correctly
		private method createUpgradeTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.upgradeTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.upgradeTrigger, this.unit(), EVENT_UNIT_HERO_SKILL)
			set conditionFunction = Condition(function ASpell.triggerConditionRightAbility)
			set triggerCondition = TriggerAddCondition(this.upgradeTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.upgradeTrigger, function ASpell.triggerActionUpgrade)
			call AHashTable.global().storeHandleInteger(this.upgradeTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod
		
		private static method triggerConditionCast takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ASpell this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			local boolean result = (GetSpellAbilityId() == this.m_ability)
			if (result) then
				set result = (this.m_castCondition == 0 or this.m_castCondition.evaluate(this))
				if (not result) then
					//taken from wc3jass.com
					call PauseUnit(this.unit(), true)
					call IssueImmediateOrder(this.unit(), "stop")
					call PauseUnit(this.unit(), false)
				endif
			endif
			set triggeringTrigger = null
			return result
		endmethod

		private static method triggerActionCast takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ASpell this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			call this.m_castAction.execute(this)
			set triggeringTrigger = null
		endmethod

		private method createCastTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.castTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.castTrigger, this.unit(), EVENT_UNIT_SPELL_CAST)
			set conditionFunction = Condition(function ASpell.triggerConditionCast)
			set triggerCondition = TriggerAddCondition(this.castTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.castTrigger, function ASpell.triggerActionCast)
			call AHashTable.global().storeHandleInteger(this.castTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		/// @param character Used character.
		/// @param usedAbility The ability which has to be casted by the unit of the character to run the cast action and which has to be skilled for the unit of the character to run the teach action.
		public static method create takes ACharacter character, integer usedAbility, ASpellUpgradeAction upgradeAction, ASpellCastCondition castCondition, ASpellCastAction castAction returns ASpell
			local ASpell this = ASpell.allocate(character)
			//start members
			set this.m_ability = usedAbility
			set this.m_upgradeAction = upgradeAction
			set this.m_castCondition = castCondition
			set this.m_castAction = castAction
			
			call character.addSpell(this)

			if (upgradeAction != 0) then
				call this.createUpgradeTrigger()
			endif
			if (castAction != 0) then
				call this.createCastTrigger()
			endif
			return this
		endmethod
		
		private method destroyUpgradeTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.upgradeTrigger)
			set this.upgradeTrigger = null
		endmethod
		
		private method destroyCastTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.castTrigger)
			set this.castTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing

			if (this.m_upgradeAction != 0) then
				call this.destroyUpgradeTrigger()
			endif
			if (this.m_castAction != 0) then
				call this.destroyCastTrigger()
			endif
		endmethod
		
		public static method enemyTargetLoopCondition takes unit target returns boolean
			return IsUnitDeadBJ(target) or IsUnitSpellImmune(target)
		endmethod
		
		public static method enemyTargetLoopConditionResistant takes unit target returns boolean
			return ASpell.enemyTargetLoopCondition(target) or IsUnitSpellResistant(target)
		endmethod
		
		public static method allyTargetLoopCondition takes unit target returns boolean
			return IsUnitDeadBJ(target)
		endmethod
		
		/// @todo Add IsUnitStunned
		public static method allyChannelLoopCondition takes unit target returns boolean
			return IsUnitDeadBJ(target)// or 
		endmethod
	endstruct

endlibrary