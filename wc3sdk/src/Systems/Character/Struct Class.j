/// Why should it be contained by ACharacter?
library AStructSystemsCharacterClass

	struct AClass
		//static constant members
		public static constant integer maxAbilities = 12 //For class selection
		public static constant integer maxDescriptionLines = 5 //For class selection
		//static start members
		private static integer startLevel
		private static integer startSkillPoints
		//dynamic members
		private string array abilityIconPath[AClass.maxAbilities] //for character class selection
		private integer array usedAbility[AClass.maxAbilities] //for character class selection
		private string array descriptionLine[AClass.maxDescriptionLines] //for character class selection
		//start members
		private integer unitType
		private string animation
		private string soundPath

		//dynamic members

		public method setAbilityIconPath takes integer number, string abilityIconPath returns nothing
			set this.abilityIconPath[number] = abilityIconPath
		endmethod

		public method getAbilityIconPath takes integer number returns string
			return this.abilityIconPath[number]
		endmethod

		public method setUsedAbility takes integer number, integer usedAbility returns nothing
			set this.usedAbility[number] = usedAbility
		endmethod

		public method getUsedAbility takes integer number returns integer
			return this.usedAbility[number]
		endmethod

		public method setDescriptionLine takes integer number, string descriptionLine returns nothing
			set this.descriptionLine[number] = descriptionLine
		endmethod

		public method getDescriptionLine takes integer number returns string
			return this.descriptionLine[number]
		endmethod

		//start members

		/// Friend relation to AClassSelection, don't use.
		public method getUnitType takes nothing returns integer
			return this.unitType
		endmethod

		/// Friend relation to AClassSelection, don't use.
		public method getAnimation takes nothing returns string
			return this.animation
		endmethod

		/// Friend relation to AClassSelection, don't use.
		public method getSoundPath takes nothing returns string
			return this.soundPath
		endmethod

		//methods

		public method containsAbility takes integer usedAbility returns boolean
			local integer i = 0
			loop
				exitwhen (i == AClass.maxAbilities)
				if (this.usedAbility[i] == usedAbility) then
					return true
				endif
				set i = i + 1
			endloop
			return false
		endmethod

		public method generateUnit takes player user, real x, real y, real facing returns unit
			local unit usedUnit = CreateUnit(user, this.unitType, x, y, facing)
			call SetHeroLevelBJ(usedUnit, AClass.startLevel, false)
			call UnitModifySkillPoints(usedUnit, (AClass.startSkillPoints - GetHeroSkillPoints(usedUnit))) //Setzt die Fähigkeiten-Punkte
			return usedUnit
		endmethod

		/// @param unitType Should be the type of a hero
		public static method create takes integer unitType, string animation, string soundPath returns AClass
			local AClass this = AClass.allocate()
			//start members
			set this.unitType = unitType
			set this.animation = animation
			set this.soundPath = soundPath

			return this
		endmethod

		public static method init takes integer startLevel, integer startSkillPoints returns nothing
			//static start members
			set AClass.startLevel = startLevel
			set AClass.startSkillPoints = startSkillPoints
		endmethod
	endstruct

endlibrary