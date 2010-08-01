library AStructCoreEnvironmentUnitCopy requires optional ALibraryCoreDebugMisc, ALibraryCoreEnvironmentUnit, ALibraryCoreGeneralUnit, AStructCoreGeneralHashTable

	/**
	* Unit copies can be used to create copies of units which will be refreshed.
	* User can specify which properties should be copied.
	* @see AHeroIcon
	*/
	struct AUnitCopy
		// dynamic members
		private boolean m_copyVisibility
		private boolean m_copyPause
		private boolean m_copyVulnerbility
		private boolean m_copyDeath
		// construction members
		private unit m_realUnit
		private real m_refreshTime
		// members
		private unit m_unit
		private timer m_refreshTimer

		//! runtextmacro A_STRUCT_DEBUG("\"AUnitCopy\"")

		// dynamic members

		public method setCopyVisibility takes boolean copyVisibility returns nothing
			set this.m_copyVisibility = copyVisibility
		endmethod

		public method copyVisibility takes nothing returns boolean
			return this.m_copyVisibility
		endmethod

		public method setCopyPause takes boolean copyPause returns nothing
			set this.m_copyPause = copyPause
		endmethod

		public method copyPause takes nothing returns boolean
			return this.m_copyPause
		endmethod

		public method setCopyVulnerbility takes boolean copyVulnerbility returns nothing
			set this.m_copyVulnerbility = copyVulnerbility
		endmethod

		public method copyVulnerbility takes nothing returns boolean
			return this.m_copyVulnerbility
		endmethod

		public method setCopyDeath takes boolean copyDeath returns nothing
			set this.m_copyDeath = copyDeath
		endmethod

		public method copyDeath takes nothing returns boolean
			return this.m_copyDeath
		endmethod

		// construction members

		public method realUnit takes nothing returns unit
			return this.m_realUnit
		endmethod

		// members

		public method unit takes nothing returns unit
			return this.m_unit
		endmethod

		public stub method onCopy takes nothing returns nothing
			/// @todo Copy revival?
			if (this.m_copyDeath) then
				call SetUnitState(this.m_unit, UNIT_STATE_MAX_LIFE, GetUnitState(this.m_realUnit, UNIT_STATE_MAX_LIFE))
				call SetUnitState(this.m_unit, UNIT_STATE_LIFE, GetUnitState(this.m_realUnit, UNIT_STATE_LIFE))
			else
				call SetUnitState(this.m_unit, UNIT_STATE_MAX_LIFE, RMaxBJ(GetUnitState(this.m_realUnit, UNIT_STATE_MAX_LIFE), 1.0))
				call SetUnitState(this.m_unit, UNIT_STATE_LIFE, RMaxBJ(GetUnitState(this.m_realUnit, UNIT_STATE_LIFE), 1.0))
			endif
			call SetUnitState(this.m_unit, UNIT_STATE_MAX_MANA, GetUnitState(this.m_realUnit, UNIT_STATE_MAX_MANA))
			call SetUnitState(this.m_unit, UNIT_STATE_MANA, GetUnitState(this.m_realUnit, UNIT_STATE_MANA))
			if (IsUnitType(this.m_unit, UNIT_TYPE_HERO)) then
				call SetHeroLevel(this.m_unit, GetHeroLevel(this.m_realUnit), false)
				call SuspendHeroXP(this.m_unit, false)
				call SetHeroXP(this.m_unit, GetHeroXP(this.m_realUnit), false)
				call SuspendHeroXP(this.m_unit, true)
				call UnitModifySkillPoints(this.m_unit, GetHeroSkillPoints(this.m_realUnit) - GetHeroSkillPoints(this.m_unit))
				call SetHeroStr(this.m_unit, GetHeroStr(this.m_realUnit, false), true)
				call SetHeroStr(this.m_unit, GetHeroStrBonus(this.m_realUnit), false)
				call SetHeroAgi(this.m_unit, GetHeroAgi(this.m_realUnit, false), true)
				call SetHeroAgi(this.m_unit, GetHeroAgiBonus(this.m_realUnit), false)
				call SetHeroInt(this.m_unit, GetHeroInt(this.m_realUnit, false), true)
				call SetHeroInt(this.m_unit, GetHeroIntBonus(this.m_realUnit), false)
			debug else
				debug call this.print("Is no hero!")
			endif
			if (this.m_copyVisibility) then
				call ShowUnit(this.m_unit, not IsUnitHidden(this.m_realUnit))
			endif
			if (this.m_copyPause) then
				call PauseUnit(this.m_unit, IsUnitPaused(this.m_realUnit))
			endif
			if (this.m_copyVulnerbility) then
				call SetUnitInvulnerable(this.m_unit, IsUnitInvulnerable(this.m_realUnit))
			endif
		endmethod

		private static method timerFunctionRefresh takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetExpiredTimer(), "this")
			call this.onCopy.evaluate()
		endmethod

		public method start takes nothing returns nothing
			call TimerStart(this.m_refreshTimer, this.m_refreshTime, true, function thistype.timerFunctionRefresh)
		endmethod

		public method resume takes nothing returns nothing
			call ResumeTimer(this.m_refreshTimer)
		endmethod

		public method enable takes nothing returns nothing
			call this.resume()
		endmethod

		public method pause takes nothing returns nothing
			call PauseTimer(this.m_refreshTimer)
		endmethod

		public method disable takes nothing returns nothing
			call this.pause()
		endmethod

		public static method create takes unit realUnit, real refreshTime, real x, real y, real facing returns thistype
			local thistype this = thistype.allocate()
			// dynamic members
			set this.m_copyVisibility = true
			set this.m_copyPause = true
			set this.m_copyVulnerbility = true
			set this.m_copyDeath = true
			// construction members
			set this.m_realUnit = realUnit
			set this.m_refreshTime = refreshTime
			// members
			set this.m_unit = CopyUnit(realUnit, x, y, facing, bj_UNIT_STATE_METHOD_MAXIMUM)
			if (IsUnitType(this.m_unit, UNIT_TYPE_HERO)) then
				call SuspendHeroXP(this.m_unit, true)
			endif
			set this.m_refreshTimer = CreateTimer()
			call AHashTable.global().setHandleInteger(this.m_refreshTimer, "this", this)

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			// construction members
			set this.m_realUnit = null
			// members
			call RemoveUnit(this.m_unit)
			set this.m_unit = null
			call PauseTimer(this.m_refreshTimer)
			call AHashTable.global().destroyTimer(this.m_refreshTimer)
			set this.m_refreshTimer = null
		endmethod
	endstruct

endlibrary