library AStructSystemsWorldWeather requires ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, AStructCoreGeneralVector

	struct AWeather
		public static constant integer weatherTypeAshenvaleRainHeavy = 0
		public static constant integer weatherTypeAshenvaleRainLight = 1
		public static constant integer weatherTypeDalaranShield = 2
		public static constant integer weatherTypeBlueDungeonFogHeavy = 3
		public static constant integer weatherTypeBlueDungeonFogLight = 4
		public static constant integer weatherTypeGreenDungeonFogHeavy = 5
		public static constant integer weatherTypeGreenDungeonFogLight = 6
		public static constant integer weatherTypeRedDungeonFogHeavy = 7
		public static constant integer weatherTypeRedDungeonFogLight = 8
		public static constant integer weatherTypeWhiteDungeonFogHeavy = 9
		public static constant integer weatherTypeWhiteDungeonFogLight = 10
		public static constant integer weatherTypeLordaeronRainHeavy = 11
		public static constant integer weatherTypeLordaeronRainLight = 12
		public static constant integer weatherTypeNorhtrendBlizzard = 13
		public static constant integer weatherTypeNorhtrendSnowfallHeavy = 14
		public static constant integer weatherTypeNorhtrendSnowfallLight = 15
		public static constant integer weatherTypeForeignCountryWindHeavy = 16
		public static constant integer weatherTypeForeignCountryWindLight = 17
		public static constant integer weatherTypeRays = 18
		public static constant integer weatherTypeRaysOfMoonlight = 19
		public static constant integer weatherTypeWindHeavy = 20
		public static constant integer weatherTypeNoWeather = 21
		private static constant integer maxWeatherTypes = 22
		//static start members
		private static integer array m_weatherTypeEffectId
		//dynamic members
		private real m_minimumChangeTime
		private real m_maximumChangeTime
		private boolean array m_isWeatherTypeAllowed[thistype.maxWeatherTypes]
		//members
		private region m_region
		private timer m_changeTimer
		private ARectVector m_rects
		private AWeatherEffectVector m_weatherEffects
		
		//! runtextmacro A_STRUCT_DEBUG("\"AWeather\"")
		
		//dynamic mebers
		
		public method setMinimumChangeTime takes real minimumChangeTime returns nothing
			set this.m_minimumChangeTime = minimumChangeTime
		endmethod
		
		public method minimumChangeTime takes nothing returns real
			return this.m_minimumChangeTime
		endmethod
		
		public method setMaximumChangeTime takes real minimumChangeTime returns nothing
			set this.m_maximumChangeTime = minimumChangeTime
		endmethod
		
		public method maximumChangeTime takes nothing returns real
			return this.m_maximumChangeTime
		endmethod
		
		public method setWeatherTypeAllowed takes integer weatherType, boolean allowed returns nothing
			set this.m_isWeatherTypeAllowed[weatherType] = allowed
		endmethod
		
		public method isWeatherTypeAllowed takes integer weatherType returns boolean
			return this.m_isWeatherTypeAllowed[weatherType]
		endmethod
		
		//methods
		
		/// Rects are not removed by this struct!
		public method addRect takes rect usedRect returns integer
			call this.m_rects.pushBack(usedRect)
			call this.m_weatherEffects.pushBack(null)
			return this.m_rects.backIndex()
		endmethod
		
		public method removeRect takes rect usedRect returns nothing
			local integer index = this.m_rects.find(usedRect)
			if (index == -1) then
				return
			endif
			call this.removeRectByIndex(index)
		endmethod
		
		public method removeRectByIndex takes integer index returns nothing
			call this.m_rects.erase(index)
			if (this.m_weatherEffects[index] != null) then
				call RemoveWeatherEffect(this.m_weatherEffects[index])
				set this.m_weatherEffects[index] = null
			endif
			call this.m_weatherEffects.erase(index)
		endmethod
		
		public method setAllWeatherTypesAllowed takes boolean allowed returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.maxWeatherTypes)
				set this.m_isWeatherTypeAllowed[i] = allowed
				set i = i + 1
			endloop
		endmethod
		
		public method setEnabled takes boolean enabled returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_rects.size())
				if (this.m_weatherEffects[i] != null) then
					call EnableWeatherEffect(this.m_weatherEffects[i], enabled)
				endif
				set i = i + 1
			endloop
			if (enabled) then
				call ResumeTimer(this.m_changeTimer)
			else
				call PauseTimer(this.m_changeTimer)
			endif
		endmethod
		
		public method changeWeather takes integer weatherType returns nothing
			local integer i = 0
			debug call this.print("Change weather to " + I2S(weatherType) + ".")
			loop
				exitwhen (i == this.m_rects.size())
				if (this.m_weatherEffects[i] != null) then
					call RemoveWeatherEffect(this.m_weatherEffects[i])
					set this.m_weatherEffects[i] = null
				endif
				if (thistype.m_weatherTypeEffectId[weatherType] != 0) then
					set this.m_weatherEffects[i] = AddWeatherEffect(this.m_rects[i], thistype.m_weatherTypeEffectId[weatherType])
					call EnableWeatherEffect(this.m_weatherEffects[i], true)
				endif
				set i = i + 1
			endloop
		endmethod
		
		private static method timerFunctionChangeWeather takes nothing returns nothing
			local timer expiredTimer = GetExpiredTimer()
			local thistype this = AHashTable.global().handleInteger(expiredTimer, "this")
			local integer array possibleWeatherTypes
			local integer maxPossibleWeatherTypes = 0
			local integer i = 0
			debug call thistype.staticPrint("Weather changes!")
			loop
				exitwhen (i == thistype.maxWeatherTypes)
				if (this.m_isWeatherTypeAllowed[i]) then
					set possibleWeatherTypes[maxPossibleWeatherTypes] = i
					set maxPossibleWeatherTypes = maxPossibleWeatherTypes + 1
				endif
				set i = i + 1
			endloop
			call this.changeWeather(possibleWeatherTypes[GetRandomInt(0, maxPossibleWeatherTypes - 1)])
			call this.start() //start again with new time
			set expiredTimer = null
		endmethod
		
		/// Make sure that you've set minimum and maximum change time before!
		public method start takes nothing returns nothing
			call TimerStart(this.m_changeTimer, GetRandomReal(this.m_minimumChangeTime, this.m_maximumChangeTime), false, function thistype.timerFunctionChangeWeather)
		endmethod
		
		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate()
			//members
			set this.m_region = CreateRegion()
			set this.m_changeTimer = CreateTimer()
			call AHashTable.global().setHandleInteger(this.m_changeTimer, "this", this)
			set this.m_rects = ARectVector.create()
			set this.m_weatherEffects = AWeatherEffectVector.create()

			return this
		endmethod
		
		private method removeWeatherEffects takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_weatherEffects.size())
				if (this.m_weatherEffects[i] != null) then
					call RemoveWeatherEffect(this.m_weatherEffects[i])
					set this.m_weatherEffects[i] = null
				endif
				set i = i + 1
			endloop
			call this.m_weatherEffects.destroy()
		endmethod
		
		public method onDestroy takes nothing returns nothing
			//members
			call RemoveRegion(this.m_region)
			set this.m_region = null
			call AHashTable.global().destroyTimer(this.m_changeTimer)
			set this.m_changeTimer = null
			call this.m_rects.destroy() // DO NOT REMOVE RECTS, there were set by user
			call this.removeWeatherEffects()
		endmethod
		
		public static method init takes nothing returns nothing
			//static start members
			set thistype.m_weatherTypeEffectId[0] = 'RAhr'
			set thistype.m_weatherTypeEffectId[1] = 'RAlr'
			set thistype.m_weatherTypeEffectId[2] = 'MEds'
			set thistype.m_weatherTypeEffectId[3] = 'FDbh'
			set thistype.m_weatherTypeEffectId[4] = 'FDbl'
			set thistype.m_weatherTypeEffectId[5] = 'FDgh'
			set thistype.m_weatherTypeEffectId[6] = 'FDgl'
			set thistype.m_weatherTypeEffectId[7] = 'FDrh'
			set thistype.m_weatherTypeEffectId[8] = 'FDrl'
			set thistype.m_weatherTypeEffectId[9] = 'FDwh'
			set thistype.m_weatherTypeEffectId[10] = 'FDwl'
			set thistype.m_weatherTypeEffectId[11] = 'RLhr'
			set thistype.m_weatherTypeEffectId[12] = 'RLlr'
			set thistype.m_weatherTypeEffectId[13] = 'SNbs'
			set thistype.m_weatherTypeEffectId[14] = 'SNhs'
			set thistype.m_weatherTypeEffectId[15] = 'SNls'
			set thistype.m_weatherTypeEffectId[16] = 'WOcw'
			set thistype.m_weatherTypeEffectId[17] = 'WOlw'
			set thistype.m_weatherTypeEffectId[19] = 'LRaa'
			set thistype.m_weatherTypeEffectId[19] = 'LRma'
			set thistype.m_weatherTypeEffectId[20] = 'WNcw'
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeNoWeather] = 0
		endmethod
	endstruct

endlibrary