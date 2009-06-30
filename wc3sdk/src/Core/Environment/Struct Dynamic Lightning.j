library AStructCoreEnvironmentDynamicLightning requires ALibraryCoreDebugMisc, ALibraryCoreEnvironmentLightning, AStructCoreGeneralVector, ALibraryCoreMathsHandle

	//! runtextmacro A_VECTOR("private", "ADynamicLightningVector", "ADynamicLightning", "0", "100") /// @todo @member ADynamicLightning.maxDynamicLightnings
	
	private function unaryFunctionDestroy takes ADynamicLightning element returns nothing
		call element.destroy()
	endfunction

	struct ADynamicLightning
		//static constant members
		public static constant integer maxDynamicLightnings = 100
		//static start members
		private static boolean removeOnDeath
		//static members
		private static timer moveTimer
		private static ADynamicLightningVector dynamicLightnings
		//start members
		private player m_player
		private string m_code
		//dynamic members
		private unit m_firstUnit
		private unit m_secondUnit
		//members
		private integer index
		private lightning m_lightning
		
		//! runtextmacro A_STRUCT_DEBUG("\"ALightningChain\"")
		
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
		
		public static method create takes player user, string usedCode, unit firstUnit, unit secondUnit returns ADynamicLightning
			local ADynamicLightning this = ADynamicLightning.allocate()
			//start members
			set this.m_player = user
			set this.m_code = usedCode
			//dynamic members
			set this.m_firstUnit = firstUnit
			set this.m_secondUnit = secondUnit
			//members
			call this.createLightning()
			set this.index = ADynamicLightning.addDynamicLightning(this)
			
			return this
		endmethod
		
		private method destroyLightning takes nothing returns nothing
			call DestroyLightning(this.m_lightning)
			set this.m_lightning = null
		endmethod
		
		public method onDestroy takes nothing returns nothing
			//move list, drop this
			call ADynamicLightning.removeDynamicLightning(this.index)
			//start members
			set this.m_player = null
			//dynamic members
			set this.m_firstUnit = null
			set this.m_secondUnit = null
			//members
			call this.destroyLightning()
		endmethod
		
		private static method timerFunction takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == ADynamicLightning.dynamicLightnings.size())
				if (ADynamicLightning.removeOnDeath and (IsUnitDeadBJ(ADynamicLightning.dynamicLightnings[i].m_firstUnit) or IsUnitDeadBJ(ADynamicLightning.dynamicLightnings[i].m_secondUnit))) then
					call ADynamicLightning.dynamicLightnings[i].destroy()
					exitwhen (i == ADynamicLightning.dynamicLightnings.size())
				endif
				call ADynamicLightning.dynamicLightnings[i].refreshPosition()
				set i = i + 1
			endloop
		endmethod
		
		public static method init takes real refreshRate, boolean removeOnDeath returns nothing
			//static start members
			set ADynamicLightning.removeOnDeath = removeOnDeath
			//static members
			set ADynamicLightning.moveTimer = CreateTimer()
			call TimerStart(ADynamicLightning.moveTimer, refreshRate, true, function ADynamicLightning.timerFunction)
			set ADynamicLightning.dynamicLightnings = ADynamicLightningVector.create()
		endmethod
		
		public static method cleanUp takes nothing returns nothing
			//static members
			call PauseTimer(ADynamicLightning.moveTimer)
			call DestroyTimer(ADynamicLightning.moveTimer)
			set ADynamicLightning.moveTimer = null
			call ADynamicLightning.dynamicLightnings.forEach(unaryFunctionDestroy)
			call ADynamicLightning.dynamicLightnings.destroy()
		endmethod
		
		private static method addDynamicLightning takes ADynamicLightning dynamicLightning returns integer
			call ADynamicLightning.dynamicLightnings.pushBack(dynamicLightning)
			return ADynamicLightning.dynamicLightnings.backIndex()
		endmethod
		
		private static method removeDynamicLightning takes integer index returns nothing
			call ADynamicLightning.dynamicLightnings.erase(index)
		endmethod
	endstruct

endlibrary