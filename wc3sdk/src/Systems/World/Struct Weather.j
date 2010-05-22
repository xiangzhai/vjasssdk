library AStructSystemsWorldWeather requires optional ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, AStructCoreGeneralVector

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
		// static construction members
		private static integer array m_weatherTypeEffectId[thistype.maxWeatherTypes]
		private static string array m_skyModelFile[thistype.maxWeatherTypes]
		// dynamic members
		private real m_minimumChangeTime
		private real m_maximumChangeTime
		private boolean m_changeSky
		private boolean array m_isWeatherTypeAllowed[thistype.maxWeatherTypes]
		// members
		private region m_region
		private timer m_changeTimer
		private ARectVector m_rects
		private AWeatherEffectVector m_weatherEffects

		//! runtextmacro optional A_STRUCT_DEBUG("\"AWeather\"")

		// dynamic mebers

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

		public method setChangeSky takes boolean changeSky returns nothing
			set this.m_changeSky = changeSky
		endmethod

		public method changeSky takes nothing returns boolean
			return this.m_changeSky
		endmethod

		public method setWeatherTypeAllowed takes integer weatherType, boolean allowed returns nothing
			set this.m_isWeatherTypeAllowed[weatherType] = allowed
		endmethod

		public method isWeatherTypeAllowed takes integer weatherType returns boolean
			return this.m_isWeatherTypeAllowed[weatherType]
		endmethod

		// methods

		/// Rects are not removed by this struct!
		public method addRect takes rect usedRect returns integer
			call this.m_rects.pushBack(usedRect)
			call this.m_weatherEffects.pushBack(null)
			return this.m_rects.backIndex()
		endmethod

		public method removeRectByIndex takes integer index returns nothing
			call this.m_rects.erase(index)
			if (this.m_weatherEffects[index] != null) then
				call RemoveWeatherEffect(this.m_weatherEffects[index])
				set this.m_weatherEffects[index] = null
			endif
			call this.m_weatherEffects.erase(index)
		endmethod

		public method removeRect takes rect usedRect returns nothing
			local integer index = this.m_rects.find(usedRect)
			if (index == -1) then
				return
			endif
			call this.removeRectByIndex(index)
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

		public method enable takes nothing returns nothing
			call this.setEnabled(true)
		endmethod

		public method disable takes nothing returns nothing
			call this.setEnabled(false)
		endmethod

		public method changeWeather takes integer weatherType returns nothing
			local integer i = 0
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
			if (this.m_changeSky) then
				call SetSkyModel(thistype.m_skyModelFile[weatherType])
			endif
		endmethod

		private static method timerFunctionChangeWeather takes nothing returns nothing
			local timer expiredTimer = GetExpiredTimer()
			local thistype this = AHashTable.global().handleInteger(expiredTimer, "this")
			local AIntegerVector possibleWeatherTypes = AIntegerVector.create()
			local integer i = 0
			loop
				exitwhen (i == thistype.maxWeatherTypes)
				if (this.m_isWeatherTypeAllowed[i]) then
					call possibleWeatherTypes.pushBack(i)
				endif
				set i = i + 1
			endloop
			call this.changeWeather(possibleWeatherTypes.random())
			call possibleWeatherTypes.destroy()
			call this.start.evaluate() // start again with new time
			set expiredTimer = null
		endmethod

		/// Make sure that you've set minimum and maximum change time before!
		public method start takes nothing returns nothing
			call TimerStart(this.m_changeTimer, GetRandomReal(this.m_minimumChangeTime, this.m_maximumChangeTime), false, function thistype.timerFunctionChangeWeather)
		endmethod

		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate()
			// dynamic members
			set this.m_minimumChangeTime = 0.0
			set this.m_maximumChangeTime = 24.0
			set this.m_changeSky = false
			call this.setAllWeatherTypesAllowed(false)
			// members
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
			// members
			call RemoveRegion(this.m_region)
			set this.m_region = null
			call AHashTable.global().destroyTimer(this.m_changeTimer)
			set this.m_changeTimer = null
			call this.m_rects.destroy() // DO NOT REMOVE RECTS, there were set by user
			call this.removeWeatherEffects()
		endmethod

		/// @todo Set all sky types (maybe add custom skies)
		public static method init takes nothing returns nothing
			// static construction members
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeAshenvaleRainHeavy] = 'RAhr'
			set thistype.m_skyModelFile[thistype.weatherTypeAshenvaleRainHeavy] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeAshenvaleRainLight] = 'RAlr'
			set thistype.m_skyModelFile[thistype.weatherTypeAshenvaleRainLight] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeDalaranShield] = 'MEds'
			set thistype.m_skyModelFile[thistype.weatherTypeDalaranShield] = "Environment\\Sky\\DalaranSky\\DalaranSky.mdl"
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeBlueDungeonFogHeavy] = 'FDbh'
			set thistype.m_skyModelFile[thistype.weatherTypeBlueDungeonFogHeavy] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeBlueDungeonFogLight] = 'FDbl'
			set thistype.m_skyModelFile[thistype.weatherTypeBlueDungeonFogLight] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeGreenDungeonFogHeavy] = 'FDgh'
			set thistype.m_skyModelFile[thistype.weatherTypeGreenDungeonFogHeavy] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeGreenDungeonFogLight] = 'FDgl'
			set thistype.m_skyModelFile[thistype.weatherTypeGreenDungeonFogLight] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeRedDungeonFogHeavy] = 'FDrh'
			set thistype.m_skyModelFile[thistype.weatherTypeRedDungeonFogHeavy] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeRedDungeonFogLight] = 'FDrl'
			set thistype.m_skyModelFile[thistype.weatherTypeRedDungeonFogLight] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeWhiteDungeonFogHeavy] = 'FDwh'
			set thistype.m_skyModelFile[thistype.weatherTypeWhiteDungeonFogHeavy] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeWhiteDungeonFogLight] = 'FDwl'
			set thistype.m_skyModelFile[thistype.weatherTypeWhiteDungeonFogLight] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeLordaeronRainHeavy] = 'RLhr'
			set thistype.m_skyModelFile[thistype.weatherTypeLordaeronRainHeavy] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeLordaeronRainLight] = 'RLlr'
			set thistype.m_skyModelFile[thistype.weatherTypeLordaeronRainLight] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeNorhtrendBlizzard] = 'SNbs'
			set thistype.m_skyModelFile[thistype.weatherTypeNorhtrendBlizzard] = "Environment\\Sky\\BlizzardSky\\BlizzardSky.mdl"
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeNorhtrendSnowfallHeavy] = 'SNhs'
			set thistype.m_skyModelFile[thistype.weatherTypeNorhtrendSnowfallHeavy] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeNorhtrendSnowfallLight] = 'SNls'
			set thistype.m_skyModelFile[thistype.weatherTypeNorhtrendSnowfallLight] = ""
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeForeignCountryWindHeavy] = 'WOcw'
			set thistype.m_skyModelFile[thistype.weatherTypeForeignCountryWindHeavy] = "Environment\\Sky\\Outland_Sky\\Outland_Sky.mdl"
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeForeignCountryWindLight] = 'WOlw'
			set thistype.m_skyModelFile[thistype.weatherTypeForeignCountryWindLight] = "Environment\\Sky\\Outland_Sky\\Outland_Sky.mdl"
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeRays] = 'LRaa'
			set thistype.m_skyModelFile[thistype.weatherTypeRays] = "Environment\\Sky\\LordaeronSummerSky\\LordaeronSummerSky.mdl"
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeRaysOfMoonlight] = 'LRma'
			set thistype.m_skyModelFile[thistype.weatherTypeRaysOfMoonlight] = "Environment\\Sky\\LordaeronSummerSky\\LordaeronSummerSky.mdl"
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeWindHeavy] = 'WNcw'
			set thistype.m_skyModelFile[thistype.weatherTypeWindHeavy] = "Environment\\Sky\\LordaeronSummerSky\\LordaeronSummerSky.mdl"
			set thistype.m_weatherTypeEffectId[thistype.weatherTypeNoWeather] = 0
			set thistype.m_skyModelFile[thistype.weatherTypeNoWeather] = "Environment\\Sky\\Sky\\SkyLight.mdl"
		endmethod
	endstruct

endlibrary