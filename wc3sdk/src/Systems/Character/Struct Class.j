library AStructSystemsCharacterClass requires AStructCoreGeneralVector

	struct AClass
		// static construction members
		private static integer m_startLevel
		private static integer m_startSkillPoints
		// dynamic members
		private AStringVector m_abilityIconPaths // for character class selection
		private AIntegerVector m_abilities // for character class selection
		private AStringVector m_descriptionLines // for character class selection
		// construction members
		private integer m_unitType
		private string m_animation
		private string m_soundPath

		// dynamic members

		public method addAbilityIconPath takes string abilityIconPath returns integer
			call this.m_abilityIconPaths.pushBack(abilityIconPath)
			return this.m_abilityIconPaths.backIndex()
		endmethod

		public method abilityIconPath takes integer index returns string
			return this.m_abilityIconPaths[index]
		endmethod

		public method abilityIconPaths takes nothing returns integer
			return this.m_abilityIconPaths.size()
		endmethod

		public method addAbility takes integer whichAbility returns integer
			call this.m_abilities.pushBack(whichAbility)
			return this.m_abilities.backIndex()
		endmethod

		public method ability takes integer index returns integer
			return this.m_abilities[index]
		endmethod

		public method abilities takes nothing returns integer
			return this.m_abilities.size()
		endmethod

		public method addDescriptionLine takes string descriptionLine returns integer
			call this.m_descriptionLines.pushBack(descriptionLine)
			return this.m_descriptionLines.backIndex()
		endmethod

		public method descriptionLine takes integer index returns string
			return this.m_descriptionLines[index]
		endmethod

		public method descriptionLines takes nothing returns integer
			return this.m_descriptionLines.size()
		endmethod

		// construction members

		/// Friend relation to @struct AClassSelection, don't use.
		public method unitType takes nothing returns integer
			return this.m_unitType
		endmethod

		/// Friend relation to @struct AClassSelection, don't use.
		public method animation takes nothing returns string
			return this.m_animation
		endmethod

		/// Friend relation to @struct AClassSelection, don't use.
		public method soundPath takes nothing returns string
			return this.m_soundPath
		endmethod

		// methods

		public method containsAbility takes integer whichAbility returns boolean
			local integer i = 0
			loop
				exitwhen (i == this.m_abilities.size())
				if (this.m_abilities[i] == whichAbility) then
					return true
				endif
				set i = i + 1
			endloop
			return false
		endmethod

		/**
		* Creates a unit for the given player at the given position with unit type of class and its start level and start skill point if unit type is hero.
		* Since this method is stub you can overwrite it and create another unit.
		* This can be useful if you want to provide various unit types for the same class for example.
		*/
		public stub method generateUnit takes player whichPlayer, real x, real y, real facing returns unit
			local unit whichUnit = CreateUnit(whichPlayer, this.m_unitType, x, y, facing)
			if (IsUnitType(whichUnit, UNIT_TYPE_HERO)) then
				call SetHeroLevelBJ(whichUnit, thistype.m_startLevel, false)
				call UnitModifySkillPoints(whichUnit, (thistype.m_startSkillPoints - GetHeroSkillPoints(whichUnit))) // sets the skill points
			endif
			return whichUnit
		endmethod

		/// @param unitType Should be the type of a hero
		public static method create takes integer unitType, string animation, string soundPath returns thistype
			local thistype this = thistype.allocate()
			// dynamic members
			set this.m_abilityIconPaths = AStringVector.create()
			set this.m_abilities = AIntegerVector.create()
			set this.m_descriptionLines = AStringVector.create()
			// construction members
			set this.m_unitType = unitType
			set this.m_animation = animation
			set this.m_soundPath = soundPath

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			// dynamic members
			call this.m_abilityIconPaths.destroy()
			call this.m_abilities.destroy()
			call this.m_descriptionLines.destroy()
		endmethod

		public static method init takes integer startLevel, integer startSkillPoints returns nothing
			// static construction members
			set thistype.m_startLevel = startLevel
			set thistype.m_startSkillPoints = startSkillPoints
		endmethod
	endstruct

endlibrary