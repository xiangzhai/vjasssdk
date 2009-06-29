/// Why should it be contained by ACharacter?
library AStructSystemsCharacterClass

	struct AClass
		//static constant members
		public static constant integer maxAbilities = 12 //For class selection
		public static constant integer maxDescriptionLines = 8 //For class selection
		//static start members
		private static integer m_startLevel
		private static integer m_startSkillPoints
		//dynamic members
		private string array m_abilityIconPath[AClass.maxAbilities] //for character class selection
		private integer array m_ability[AClass.maxAbilities] //for character class selection
		private string array m_descriptionLine[AClass.maxDescriptionLines] //for character class selection
		//start members
		private integer m_unitType
		private string m_animation
		private string m_soundPath

		//dynamic members

		public method setAbilityIconPath takes integer number, string abilityIconPath returns nothing
			set this.m_abilityIconPath[number] = abilityIconPath
		endmethod

		public method abilityIconPath takes integer number returns string
			return this.m_abilityIconPath[number]
		endmethod

		public method setAbility takes integer number, integer usedAbility returns nothing
			set this.m_ability[number] = usedAbility
		endmethod

		public method ability takes integer number returns integer
			return this.m_ability[number]
		endmethod

		public method setDescriptionLine takes integer number, string descriptionLine returns nothing
			set this.m_descriptionLine[number] = descriptionLine
		endmethod

		public method descriptionLine takes integer number returns string
			return this.m_descriptionLine[number]
		endmethod

		//start members

		/// Friend relation to AClassSelection, don't use.
		public method unitType takes nothing returns integer
			return this.m_unitType
		endmethod

		/// Friend relation to AClassSelection, don't use.
		public method animation takes nothing returns string
			return this.m_animation
		endmethod

		/// Friend relation to AClassSelection, don't use.
		public method soundPath takes nothing returns string
			return this.m_soundPath
		endmethod

		//methods

		public method containsAbility takes integer usedAbility returns boolean
			local integer i = 0
			loop
				exitwhen (i == AClass.maxAbilities)
				if (this.m_ability[i] == usedAbility) then
					return true
				endif
				set i = i + 1
			endloop
			return false
		endmethod

		public method generateUnit takes player user, real x, real y, real facing returns unit
			local unit usedUnit = CreateUnit(user, this.m_unitType, x, y, facing)
			call SetHeroLevelBJ(usedUnit, AClass.m_startLevel, false)
			call UnitModifySkillPoints(usedUnit, (AClass.m_startSkillPoints - GetHeroSkillPoints(usedUnit))) //Setzt die Fähigkeiten-Punkte
			return usedUnit
		endmethod

		/// @param unitType Should be the type of a hero
		public static method create takes integer unitType, string animation, string soundPath returns AClass
			local AClass this = AClass.allocate()
			//start members
			set this.m_unitType = unitType
			set this.m_animation = animation
			set this.m_soundPath = soundPath

			return this
		endmethod

		public static method init takes integer startLevel, integer startSkillPoints returns nothing
			//static start members
			set AClass.m_startLevel = startLevel
			set AClass.m_startSkillPoints = startSkillPoints
		endmethod
	endstruct

endlibrary