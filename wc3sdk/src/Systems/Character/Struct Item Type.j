library AStructSystemsCharacterItemType requires optional ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, AStructCoreGeneralVector, ALibraryCoreMathsConversion, AStructSystemsCharacterClass, AStructSystemsCharacterCharacter

	/// Represents an item type, not an item object!
	struct AItemType
		//static constant members
		public static constant integer equipmentTypeHeaddress = 0
		public static constant integer equipmentTypeArmour = 1
		public static constant integer equipmentTypePrimaryWeapon = 2
		public static constant integer equipmentTypeSecondaryWeapon = 3
		public static constant integer equipmentTypeAmulet = 4
		public static constant integer maxEquipmentTypes = 5
		// static construction members
		private static string textLevel
		private static string textStrength
		private static string textAgility
		private static string textIntelligence
		private static string textClass
		// construction members
		private integer m_itemType
		private integer m_equipmentType
		private integer m_requiredLevel
		private integer m_requiredStrength
		private integer m_requiredAgility
		private integer m_requiredIntelligence
		private AClass m_requiredClass
		// members
		private AIntegerVector m_abilities
		private ABooleanVector m_permanent

		//! runtextmacro optional A_STRUCT_DEBUG("\"AItemType\"")

		// construction members

		public method itemType takes nothing returns integer
			return this.m_itemType
		endmethod

		public method equipmentType takes nothing returns integer
			return this.m_equipmentType
		endmethod

		public method requiredLevel takes nothing returns integer
			return this.m_requiredLevel
		endmethod

		public method requiredStrength takes nothing returns integer
			return this.m_requiredStrength
		endmethod

		public method requiredAgility takes nothing returns integer
			return this.m_requiredAgility
		endmethod

		public method requiredIntelligence takes nothing returns integer
			return this.m_requiredIntelligence
		endmethod

		public method requiredClass takes nothing returns AClass
			return this.m_requiredClass
		endmethod

		// methods

		public method addAbility takes integer abilityId, boolean permanent returns integer
			call this.m_abilities.pushBack(abilityId)
			call this.m_permanent.pushBack(permanent)
			return this.m_abilities.backIndex()
		endmethod

		public method checkRequirement takes ACharacter character returns boolean
			if (GetHeroLevel(character.unit()) < this.m_requiredLevel) then
				call character.displayMessage(ACharacter.messageTypeError, thistype.textLevel)
				return false
			elseif (GetHeroStr(character.unit(), true) < this.m_requiredStrength) then
				call character.displayMessage(ACharacter.messageTypeError, thistype.textStrength)
				return false
			elseif (GetHeroAgi(character.unit(), true) < this.m_requiredAgility) then
				call character.displayMessage(ACharacter.messageTypeError, thistype.textAgility)
				return false
			elseif (GetHeroInt(character.unit(), true) < this.m_requiredIntelligence) then
				call character.displayMessage(ACharacter.messageTypeError, thistype.textIntelligence)
				return false
			elseif (this.m_requiredClass != 0 and character.class() != this.m_requiredClass) then
				call character.displayMessage(ACharacter.messageTypeError, thistype.textClass)
				return false
			endif
			return true
		endmethod

		public method addAllAbilities takes unit who returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_abilities.size())
				debug call this.print("Test add")
				call UnitAddAbility(who, this.m_abilities[i])
				//call UnitMakeAbilityPermanent(who, this.m_permanent[i], this.m_abilities[i]) //stay when unit morphs
				set i = i + 1
			endloop
		endmethod

		public method removeAllAbilities takes unit who returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_abilities.size())
				debug call this.print("Test remove")
				call UnitRemoveAbility(who, this.m_abilities[i])
				set i = i + 1
			endloop
		endmethod

		public method addPermanentAbilities takes unit who returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_abilities.size())
				if (this.m_permanent[i]) then
					//debug call this.print("Adding permanent ability " + GetObjectName(this.m_abilities[i]) + " to unit " + GetUnitName(who))
					debug call this.print("Test add")
					call UnitAddAbility(who, this.m_abilities[i])
				endif
				set i = i + 1
			endloop
		endmethod

		public method removePermanentAbilities takes unit who returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_abilities.size())
				if (this.m_permanent[i]) then
					//debug call this.print("Removing permanent ability " + GetObjectName(this.m_abilities[i]) + " from unit " + GetUnitName(who))
					debug call this.print("Test remove")
					call UnitRemoveAbility(who, this.m_abilities[i])
				endif
				set i = i + 1
			endloop
		endmethod

		public method addUsableAbilities takes unit who returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_abilities.size())
				if (not this.m_permanent[i]) then
					call UnitAddAbility(who, this.m_abilities[i])
				endif
				set i = i + 1
			endloop
		endmethod

		public method removeUsableAbilities takes unit who returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_abilities.size())
				if (not this.m_permanent[i]) then
					call UnitRemoveAbility(who, this.m_abilities[i])
				endif
				set i = i + 1
			endloop
		endmethod

		/// @param equipmentType If this value is -1 it will always be added to rucksack.
		public static method create takes integer itemType, integer equipmentType, integer requiredLevel, integer requiredStrength, integer requiredAgility, integer requiredIntelligence, AClass requiredClass returns thistype
			local thistype this = thistype.allocate()
			// construction members
			set this.m_itemType = itemType
			set this.m_equipmentType = equipmentType
			set this.m_requiredLevel = requiredLevel
			set this.m_requiredStrength = requiredStrength
			set this.m_requiredAgility = requiredAgility
			set this.m_requiredIntelligence = requiredIntelligence
			set this.m_requiredClass = requiredClass
			// members
			set this.m_abilities = AIntegerVector.create()
			set this.m_permanent = ABooleanVector.create()

			debug if (AHashTable.global().integer("AItemTypes", I2S(itemType)) != 0) then
				debug call this.print("Item type " + I2S(itemType) + " already has an item type.")
			debug endif
			call AHashTable.global().setInteger("AItemTypes", I2S(itemType), this)
			//debug call this.print("Storing item type " + I2S(itemType))
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			// members
			call this.m_abilities.destroy()
			call this.m_permanent.destroy()

			call AHashTable.global().removeInteger("AItemTypes", I2Hexadecimal(this.m_itemType))
		endmethod

		public static method init takes string textLevel, string textStrength, string textAgility, string textIntelligence, string textClass returns nothing
			// static construction members
			set thistype.textLevel = textLevel
			set thistype.textStrength = textStrength
			set thistype.textAgility = textAgility
			set thistype.textIntelligence = textIntelligence
			set thistype.textClass = textClass
		endmethod

		public static method getItemTypeOfItemTypeId takes integer itemTypeId returns thistype
			//debug call thistype.staticPrint("Getting item type of item type id " + I2S(itemTypeId))
			return AHashTable.global().integer("AItemTypes", I2S(itemTypeId))
		endmethod

		public static method getItemTypeOfItem takes item usedItem returns thistype
			return thistype.getItemTypeOfItemTypeId(GetItemTypeId(usedItem))
		endmethod
	endstruct

endlibrary