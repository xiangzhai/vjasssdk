library AStructSystemsCharacterItemType requires ALibraryCoreDebugMisc, AStructSystemsCharacterCharacterHashTable, AStructSystemsCharacterClass, AStructSystemsCharacterCharacter

	/// Represents an item type, not an item object!
	struct AItemType
		//static constant members
		public static constant integer equipmentTypeHeaddress = 0
		public static constant integer equipmentTypeArmour = 1
		public static constant integer equipmentTypePrimaryWeapon = 2
		public static constant integer equipmentTypeSecondaryWeapon = 3
		public static constant integer maxEquipmentTypes = 4
		private static constant integer maxAbilities = 10
		//static start members
		private static string textLevel
		private static string textStrength
		private static string textAgility
		private static string textIntelligence
		private static string textClass
		//start members
		private integer itemType
		private integer equipmentType
		private integer requiredLevel
		private integer requiredStrength
		private integer requiredAgility
		private integer requiredIntelligence
		private AClass requiredClass
		private integer m_damage
		private integer m_armor
		private integer m_hp
		private integer m_mana
		//members
		private integer m_abilityCount
		private integer array m_abilities[10] /// @todo vJass bug, thistype.maxAbilities
		
		//! runtextmacro A_STRUCT_DEBUG("\"AItemType\"")

		//start members

		/// Friend relation to AInventory, don't use
		public method getEquipmentType takes nothing returns integer
			return this.equipmentType
		endmethod

		//methods
		
		public method addAbility takes integer abilityId returns integer
			local integer id = this.m_abilityCount
			if (id < AItemType.maxAbilities) then
				set this.m_abilities[id] = abilityId
				set this.m_abilityCount = id + 1
				return id
			endif
			return -1
		endmethod

		public method checkRequirement takes ACharacter character returns boolean
			if (GetHeroLevel(character.getUsedUnit()) < this.requiredLevel) then
				call character.displayMessage(ACharacter.messageTypeError, AItemType.textLevel)
				return false
			elseif (GetHeroStr(character.getUsedUnit(), true) < this.requiredStrength) then
				call character.displayMessage(ACharacter.messageTypeError, AItemType.textStrength)
				return false
			elseif (GetHeroAgi(character.getUsedUnit(), true) < this.requiredAgility) then
				call character.displayMessage(ACharacter.messageTypeError, AItemType.textAgility) 
				return false
			elseif (GetHeroInt(character.getUsedUnit(), true) < this.requiredIntelligence) then
				call character.displayMessage(ACharacter.messageTypeError, AItemType.textIntelligence)
				return false
			elseif (this.requiredClass != 0 and character.getClass() != this.requiredClass) then
				call character.displayMessage(ACharacter.messageTypeError, AItemType.textClass)
				return false
			endif
			return true
		endmethod
		
		public method addAbilities takes unit who returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_abilityCount)
				call UnitAddAbility(who, this.m_abilities[i])
				set i = i + 1
			endloop
		endmethod
		
		public method removeAbilities takes unit who returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_abilityCount)
				call UnitRemoveAbility(who, this.m_abilities[i])
				set i = i + 1
			endloop
		endmethod

		public static method create takes integer itemType, integer equipmentType, integer requiredLevel, integer requiredStrength, integer requiredAgility, integer requiredIntelligence, AClass requiredClass returns AItemType
			local AItemType this = AItemType.allocate()
			//start members
			set this.itemType = itemType
			set this.equipmentType = equipmentType
			set this.requiredLevel = requiredLevel
			set this.requiredStrength = requiredStrength
			set this.requiredAgility = requiredAgility
			set this.requiredIntelligence = requiredIntelligence
			set this.requiredClass = requiredClass
			//members
			set this.m_abilityCount = 0

			debug if (AGetCharacterHashTable().getStoredInteger("AItemTypes", I2S(this.itemType)) != 0) then
				debug call this.print("Item type " + I2S(this.itemType) + " already has an item type.")
			debug endif
			call AGetCharacterHashTable().storeInteger("AItemTypes", I2S(this.itemType), this)
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			call AGetCharacterHashTable().flushStoredInteger("AItemTypes", I2S(this.itemType))
		endmethod

		public static method init takes string textLevel, string textStrength, string textAgility, string textIntelligence, string textClass returns nothing
			//static start members
			set AItemType.textLevel = textLevel
			set AItemType.textStrength = textStrength
			set AItemType.textAgility = textAgility
			set AItemType.textIntelligence = textIntelligence
			set AItemType.textClass = textClass
		endmethod

		public static method getItemTypeOfItemTypeId takes integer itemTypeId returns AItemType
			return AGetCharacterHashTable().getStoredInteger("AItemTypes", I2S(itemTypeId))
		endmethod

		/// @author Tamino Dauth
		public static method getItemTypeOfItem takes item usedItem returns AItemType
			return AItemType.getItemTypeOfItemTypeId(GetItemTypeId(usedItem))
		endmethod
	endstruct

endlibrary