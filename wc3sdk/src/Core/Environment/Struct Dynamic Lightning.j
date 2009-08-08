library AStructCoreEnvironmentDynamicLightning requires ALibraryCoreDebugMisc, ALibraryCoreEnvironmentLightning, AStructCoreGeneralVector, ALibraryCoreMathsHandle

	struct ADynamicLightning
		//static start members
		private static boolean removeOnDeath
		//static members
		private static timer moveTimer
		private static AIntegerVector dynamicLightnings
		//start members
		private player m_player
		private string m_code
		//dynamic members
		private unit m_firstUnit
		private unit m_secondUnit
		//members
		private integer m_index
		private lightning m_lightning
		
		//! runtextmacro A_STRUCT_DEBUG("\"ADynamicLightning\"")
		
		//start members
		
		public method player takes nothing returns player
			return this.m_player
		endmethod
		
		public method code takes nothing returns string
			return this.m_code
		endmethod
		
		//dynamic members
		
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
		
		//methods
		
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
		
		private method refreshPosition takes nothing returns nothing
			call MoveLightningEx(this.m_lightning, false, GetUnitX(this.m_firstUnit), GetUnitY(this.m_firstUnit), GetUnitZ(this.m_firstUnit), GetUnitX(this.m_secondUnit), GetUnitY(this.m_secondUnit), GetUnitZ(this.m_secondUnit))
		endmethod
		
		private method createLightning takes nothing returns nothing
			if (this.m_player == null) then
				set this.m_lightning = AddLightningEx(this.m_code, false, GetUnitX(this.m_firstUnit), GetUnitY(this.m_firstUnit), GetUnitZ(this.m_firstUnit), GetUnitX(this.m_secondUnit), GetUnitY(this.m_secondUnit), GetUnitZ(this.m_secondUnit))
			else
				set this.m_lightning = CreateLightningForPlayer(this.m_player, this.m_code, GetUnitX(this.m_firstUnit), GetUnitY(this.m_firstUnit), GetUnitZ(this.m_firstUnit), GetUnitX(this.m_secondUnit), GetUnitY(this.m_secondUnit), GetUnitZ(this.m_secondUnit))
			endif
		endmethod
		
		public static method create takes player user, string usedCode, unit firstUnit, unit secondUnit returns thistype
			local thistype this = thistype.allocate()
			//start members
			set this.m_player = user
			set this.m_code = usedCode
			//dynamic members
			set this.m_firstUnit = firstUnit
			set this.m_secondUnit = secondUnit
			//members
			call thistype.dynamicLightnings.pushBack(this)
			set this.m_index = thistype.dynamicLightnings.backIndex()
			call this.createLightning()
			
			return this
		endmethod
		
		private method destroyLightning takes nothing returns nothing
			call DestroyLightning(this.m_lightning)
			set this.m_lightning = null
		endmethod
		
		public method onDestroy takes nothing returns nothing
			//static members
			call thistype.dynamicLightnings.erase(this.m_index)
			//start members
			set this.m_player = null
			//dynamic members
			set this.m_firstUnit = null
			set this.m_secondUnit = null
			//members
			call this.destroyLightning()
		endmethod
		
		private static method timerFunction takes nothing returns nothing
			local thistype dynamicLightning
			local integer i = 0
			loop
				exitwhen (i == thistype.dynamicLightnings.size())
				set dynamicLightning = thistype.dynamicLightnings[i]
				if (thistype.removeOnDeath and (IsUnitDeadBJ(dynamicLightning.m_firstUnit) or IsUnitDeadBJ(dynamicLightning.m_secondUnit))) then
					call dynamicLightning.destroy()
				else
					call dynamicLightning.refreshPosition()
					set i = i + 1
				endif
			endloop
		endmethod
		
		public static method init takes real refreshRate, boolean removeOnDeath returns nothing
			//static start members
			set thistype.removeOnDeath = removeOnDeath
			//static members
			set thistype.moveTimer = CreateTimer()
			call TimerStart(thistype.moveTimer, refreshRate, true, function thistype.timerFunction)
			set thistype.dynamicLightnings = AIntegerVector.create()
		endmethod
		
		public static method cleanUp takes nothing returns nothing
			//static members
			call PauseTimer(thistype.moveTimer)
			call DestroyTimer(thistype.moveTimer)
			set thistype.moveTimer = null
			loop
				exitwhen (thistype.dynamicLightnings.empty())
				call thistype(thistype.dynamicLightnings.back()).destroy()
			endloop
			call thistype.dynamicLightnings.destroy()
		endmethod
	endstruct

endlibrary