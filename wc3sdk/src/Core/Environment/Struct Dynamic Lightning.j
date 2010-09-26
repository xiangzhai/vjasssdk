library AStructCoreEnvironmentDynamicLightning requires optional ALibraryCoreDebugMisc, ALibraryCoreEnvironmentLightning, ALibraryCoreMathsHandle

	/**
	* Dynamic lightnings are lightnings which are being moved automatically between positions of to specific units.
	*/
	struct ADynamicLightning
		// construction members
		private player m_player
		private string m_code
		private real m_interval
		// dynamic members
		private unit m_firstUnit
		private unit m_secondUnit
		private boolean m_destroyOnDeath
		// members
		private lightning m_lightning
		private timer m_timer

		//! runtextmacro optional A_STRUCT_DEBUG("\"ADynamicLightning\"")

		// construction members

		public method player takes nothing returns player
			return this.m_player
		endmethod

		public method code takes nothing returns string
			return this.m_code
		endmethod

		public method interval takes nothing returns real
			return this.m_interval
		endmethod

		// dynamic members

		public method setFirstUnit takes unit firstUnit returns nothing
			set this.m_firstUnit = firstUnit
		endmethod

		public method firstUnit takes nothing returns unit
			return this.m_firstUnit
		endmethod

		public method setSecondUnit takes unit secondUnit returns nothing
			set this.m_secondUnit = secondUnit
		endmethod

		public method secondUnit takes nothing returns unit
			return this.m_secondUnit
		endmethod

		public method setDestroyOnDeath takes boolean destroyOnDeath returns nothing
			set this.m_destroyOnDeath = destroyOnDeath
		endmethod

		public method destroyOnDeath takes nothing returns boolean
			return this.m_destroyOnDeath
		endmethod

		// methods

		public method enable takes nothing returns nothing
			call ResumeTimer(this.m_timer)
		endmethod

		public method disable takes nothing returns nothing
			call PauseTimer(this.m_timer)
		endmethod

		public method setLightningColor takes real red, real green, real blue, real alpha returns boolean
			return SetLightningColor(this.m_lightning, red, green, blue, alpha)
		endmethod

		public method lightningColorRed takes nothing returns real
			return GetLightningColorR(this.m_lightning)
		endmethod

		public method lightningColorGreen takes nothing returns real
			return GetLightningColorG(this.m_lightning)
		endmethod

		public method lightningColorBlue takes nothing returns real
			return GetLightningColorB(this.m_lightning)
		endmethod

		public method lightningColorAlpha takes nothing returns real
			return GetLightningColorA(this.m_lightning)
		endmethod

		private static method timerFunction takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetExpiredTimer(), "this")
			if (this.m_destroyOnDeath and (IsUnitDeadBJ(this.m_firstUnit) or IsUnitDeadBJ(this.m_secondUnit))) then
				call this.destroy()
			else
				call MoveLightningEx(this.m_lightning, false, GetUnitX(this.m_firstUnit), GetUnitY(this.m_firstUnit), GetUnitZ(this.m_firstUnit), GetUnitX(this.m_secondUnit), GetUnitY(this.m_secondUnit), GetUnitZ(this.m_secondUnit))
			endif
		endmethod

		/**
		* @param user If user is null, dynamic lightning will be visible for all players.
		*/
		public static method create takes player user, string usedCode, real interval, unit firstUnit, unit secondUnit returns thistype
			local thistype this = thistype.allocate()
			// construction members
			set this.m_player = user
			set this.m_code = usedCode
			set this.m_interval = interval
			// dynamic members
			set this.m_firstUnit = firstUnit
			set this.m_secondUnit = secondUnit
			set this.m_destroyOnDeath = true
			// members
			if (this.m_player == null) then
				set this.m_lightning = AddLightningEx(this.m_code, false, GetUnitX(this.m_firstUnit), GetUnitY(this.m_firstUnit), GetUnitZ(this.m_firstUnit), GetUnitX(this.m_secondUnit), GetUnitY(this.m_secondUnit), GetUnitZ(this.m_secondUnit))
			else
				set this.m_lightning = CreateLightningForPlayer(this.m_player, this.m_code, GetUnitX(this.m_firstUnit), GetUnitY(this.m_firstUnit), GetUnitZ(this.m_firstUnit), GetUnitX(this.m_secondUnit), GetUnitY(this.m_secondUnit), GetUnitZ(this.m_secondUnit))
			endif
			set this.m_timer = CreateTimer()
			call TimerStart(this.m_timer, interval, true, function thistype.timerFunction)
			call AHashTable.global().setHandleInteger(this.m_timer, "this", this)

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			// construction members
			set this.m_player = null
			// dynamic members
			set this.m_firstUnit = null
			set this.m_secondUnit = null
			// members
			call DestroyLightning(this.m_lightning)
			set this.m_lightning = null
			call PauseTimer(this.m_timer)
			call AHashTable.global().destroyTimer(this.m_timer)
			set this.m_timer = null
		endmethod
	endstruct

endlibrary