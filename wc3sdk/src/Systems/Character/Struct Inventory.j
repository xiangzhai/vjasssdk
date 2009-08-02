/// @todo Wenn der Charakter einen Gegenstand ablegt, muss überprüft werden, ob die anderen noch getragen werden können (Attribute).
library AStructSystemsCharacterInventory requires ALibraryCoreGeneralPlayer, AStructCoreGeneralHashTable, ALibraryCoreGeneralUnit, ALibraryCoreStringConversion, AStructSystemsCharacterAbstractCharacterSystem, AStructSystemsCharacterCharacter, AStructSystemsCharacterItemType

	/// Item types of chargable items should always be ITEM_TYPE_CHARGED!
	/// @todo Use UnitDropItemSlot instead of item removals.
	struct AInventory extends AAbstractCharacterSystem
		//static constant members, useful for GUIs
		public static constant integer maxEquipmentTypes = 5//AItemType.maxEuqipmentTypes /// @todo vJass bug //AClassCharacterItemType
		public static constant integer maxRucksackItems = 128
		public static constant integer maxRucksackPages = 32 //maxRucksackItems / maxRucksackItemsPerPage
		public static constant integer maxRucksackItemsPerPage = 4
		//static start members
		private static integer leftArrowItemType
		private static integer rightArrowItemType
		private static integer openRucksackAbilityId
		private static string textUnableToEquipItem
		private static string textEquipItem
		private static string textUnableToAddRucksackItem
		private static string textAddItemToRucksack
		private static string textUnableToMoveRucksackItem
		//members
		private integer array m_equipmentItemType[5] /// @todo vJass bug thistype.maxEquipmentTypes
		private integer array m_rucksackItemType[thistype.maxRucksackItems]
		private integer array m_rucksackItemCharges[thistype.maxRucksackItems]
		private trigger m_openTrigger
		private trigger m_orderTrigger
		private trigger m_useTrigger //show next page, show previous page, disable in equipment
		private trigger m_pickupTrigger //pickup
		private trigger m_dropTrigger //drop
		private integer m_rucksackPage
		private boolean m_rucksackIsEnabled
		
		//! runtextmacro A_STRUCT_DEBUG("\"AInventory\"")

		//members

		public method equipmentItemType takes integer equipmentType returns integer
			debug if (equipmentType >= thistype.maxEquipmentTypes or equipmentType < 0) then
				debug call this.print("Wrong equipment type: " + I2S(equipmentType) + ".")
				debug return 0
			debug endif
			return this.m_equipmentItemType[equipmentType]
		endmethod

		public method rucksackItemType takes integer index returns integer
			debug if (index >= thistype.maxRucksackItems or index < 0) then
				debug call this.print("Wrong rucksack index: " + I2S(index) + ".")
				debug return 0
			debug endif
			return this.m_rucksackItemType[index]
		endmethod

		public method rucksackItemCharges takes integer index returns integer
			debug if (index >= thistype.maxRucksackItems or index < 0) then
				debug call this.print("Wrong rucksack index: " + I2S(index) + ".")
				debug return 0
			debug endif
			return this.m_rucksackItemCharges[index]
		endmethod
		
		//convenience methods
		
		public method allEquipmentItems takes nothing returns integer
			local integer result = 0
			local integer i = 0
			loop
				exitwhen (i == thistype.maxEquipmentTypes)
				if (this.m_equipmentItemType[i] != 0) then
					set result = result + 1
				endif
				set i = i + 1
			endloop
			return result
		endmethod
		
		public method allRucksackItems takes nothing returns integer
			local integer result = 0
			local integer i = 0
			loop
				exitwhen (i == thistype.maxRucksackItems)
				if (this.m_rucksackItemType[i] != 0) then
					set result = result + 1
				endif
				set i = i + 1
			endloop
			return result
		endmethod
		
		public method allRucksackItemCharges takes nothing returns integer
			local integer result = 0
			local integer i = 0
			loop
				exitwhen (i == thistype.maxRucksackItems)
				if (this.m_rucksackItemType[i] != 0) then
					set result = result + this.m_rucksackItemCharges[i]
				endif
				set i = i + 1
			endloop
			return result
		endmethod

		//methods
		
		/// @return Returns the Warcraft inventory slot number by a rucksack item index.
		public method rucksackItemSlot takes integer index returns integer
			debug if (index >= thistype.maxRucksackItems or index < 0) then
				debug call this.print("Wrong rucksack index: " + I2S(index) + ".")
				debug return 0
			debug endif
			return index - this.m_rucksackPage * AInventory.maxRucksackItemsPerPage
		endmethod

		/// @return Returns the rucksack item index by a Warcraft inventory slot number.
		public method slotRucksackIndex takes integer slot returns integer
			debug if (slot >= thistype.maxRucksackItemsPerPage or slot < 0) then
				debug call this.print("Wrong inventory slot: " + I2S(slot) + ".")
				debug return 0
			debug endif
			return this.m_rucksackPage * thistype.maxRucksackItemsPerPage + slot
		endmethod

		/// Shows the current page in the inventory of the character's unit
		/// In general you do not have to call this method. The system handles itself.
		public method enable takes nothing returns nothing
			if (this.m_rucksackIsEnabled) then
				call this.enableRucksack()
			else
				call this.enableEquipment()
			endif

			call EnableTrigger(this.m_openTrigger)
			call EnableTrigger(this.m_orderTrigger)
			call EnableTrigger(this.m_pickupTrigger)
			call EnableTrigger(this.m_dropTrigger)
		endmethod

		/// Usually you do not have to call this method. The system handles itself.
		public method disable takes nothing returns nothing
			if (this.m_rucksackIsEnabled) then
				call this.disableRucksack()
			else
				call this.disableEquipment()
			endif

			call DisableTrigger(this.m_openTrigger)
			call DisableTrigger(this.m_orderTrigger)
			call DisableTrigger(this.m_pickupTrigger)
			call DisableTrigger(this.m_dropTrigger)
		endmethod

		/// @return Returns the slot of the equipped item. If no item was found it returns -1.
		public method hasItemEquipped takes integer itemType returns integer
			local integer i = 0
			loop
				exitwhen (i == thistype.maxEquipmentTypes)
				if (this.m_equipmentItemType[i] == itemType) then
					return i
				endif
				set i = i + 1
			endloop
			return -1
		endmethod

		/// @return Returns the slot of the rucksack item. If not item was found it returns -1.
		public method hasItemTypeInRucksack takes integer itemType returns integer
			local integer i = 0
			loop
				exitwhen (i == thistype.maxRucksackItems)
				if (this.m_rucksackItemType[i] == itemType) then
					return i
				endif
				set i = i + 1
			endloop
			return -1
		endmethod
		
		/// Stacks everything in rucksack which is stackable.
		public method cleanUpRucksack takes nothing returns nothing
			local integer i = thistype.maxRucksackItems - 1
			local integer j
			loop
				exitwhen (i < 0)
				set j = 0
				loop
					exitwhen (j == thistype.maxRucksackItems)
					if (i != j and this.m_rucksackItemType[i] != 0 and this.m_rucksackItemType[i] == this.m_rucksackItemType[j]) then
						set this.m_rucksackItemCharges[i] = this.m_rucksackItemCharges[i] + this.m_rucksackItemCharges[j]
						call this.clearRucksackItem(j, false)
						call this.refreshRucksackItemCharges(i)
					endif
					set j = j + 1
				endloop
				set i = i - 1
			endloop
		endmethod

		private method enableEquipment takes nothing returns nothing
			local integer i
			set i = 0
			loop
				exitwhen (i == thistype.maxEquipmentTypes)
				if (this.m_equipmentItemType[i] != 0) then
					call this.showEquipmentItem(i)
				endif
				set i = i + 1
			endloop
		endmethod

		private method disableEquipment takes nothing returns nothing
			local integer i
			set i = 0
			loop
				exitwhen (i == thistype.maxEquipmentTypes)
				if (this.m_equipmentItemType[i] != 0) then
					call this.hideEquipmentItem(i)
				endif
				set i = i + 1
			endloop
		endmethod

		private method enableRucksack takes nothing returns nothing
			local unit characterUnit = this.unit()
			local item leftArrowItem
			local item rightArrowItem
			set this.m_rucksackIsEnabled = true
			call this.showRucksackPage(this.m_rucksackPage, true)
			call DisableTrigger(this.m_pickupTrigger)
			call UnitAddItemToSlotById(characterUnit, thistype.leftArrowItemType, AInventory.maxRucksackItemsPerPage)
			call UnitAddItemToSlotById(characterUnit, thistype.rightArrowItemType, thistype.maxRucksackItemsPerPage + 1)
			call EnableTrigger(this.m_pickupTrigger)
			set leftArrowItem = UnitItemInSlot(characterUnit, thistype.maxRucksackItemsPerPage)
			set rightArrowItem = UnitItemInSlot(characterUnit, thistype.maxRucksackItemsPerPage + 1)
			call SetItemDroppable(leftArrowItem, true) //for moving items to next or previous pages
			call SetItemDroppable(rightArrowItem, true)
			call SetItemCharges(rightArrowItem, this.m_rucksackPage)
			set characterUnit = null
			set leftArrowItem = null
			set rightArrowItem = null
		endmethod

		private method disableRucksack takes nothing returns nothing
			local unit characterUnit = this.unit()
			local item leftArrowItem
			local item rightArrowItem
			set this.m_rucksackIsEnabled = false
			set leftArrowItem = UnitItemInSlot(characterUnit, thistype.maxRucksackItemsPerPage)
			set rightArrowItem = UnitItemInSlot(characterUnit, thistype.maxRucksackItemsPerPage + 1)
			call DisableTrigger(this.m_dropTrigger)
			call RemoveItem(leftArrowItem)
			call RemoveItem(rightArrowItem)
			call EnableTrigger(this.m_dropTrigger)
			call this.hideCurrentRucksackPage()
			set characterUnit = null
			set leftArrowItem = null
			set rightArrowItem = null
		endmethod

		/// @todo Added item player check!
		private method equipItem takes item usedItem, boolean dontMoveToRucksack, boolean swapWithAlreadyEquipped, boolean showEquipMessage returns nothing
			local AItemType itemType = AItemType.getItemTypeOfItem(usedItem)
			local integer equipmentType = itemType.equipmentType()
			local player itemPlayer = GetItemPlayer(usedItem)
			local unit characterUnit = this.unit()
			local item equippedItem
			if (not this.m_rucksackIsEnabled and UnitHasItem(characterUnit, usedItem)) then //already picked up
				call DisableTrigger(this.m_dropTrigger)
				debug call this.print("Drop item because it was already picked up.")
				call UnitDropItemPoint(characterUnit, usedItem, GetUnitX(characterUnit), GetUnitY(characterUnit))
				call EnableTrigger(this.m_dropTrigger)
			endif
			
			debug call this.print("Trying to equip item " + GetItemName(usedItem) + " with custom item type " + I2S(itemType) + " and equipment type  "  + I2S(equipmentType))
			
			if (itemPlayer != this.user() and IsPlayerPlayingUser(itemPlayer)) then
				call this.character().displayMessage(ACharacter.messageTypeError, tr("Gegenstand gehört einem anderen Benutzer.")) /// @todo use static string member
				set itemPlayer = null
				return
			endif
			set itemPlayer = null
			
			if (itemType != 0 and equipmentType != -1) then
				if (swapWithAlreadyEquipped or this.m_equipmentItemType[equipmentType] == 0) then
					if (itemType.checkRequirement(this.character())) then
						if (swapWithAlreadyEquipped and this.m_equipmentItemType[equipmentType] != 0) then
							set equippedItem = CreateItem(this.m_equipmentItemType[equipmentType], GetUnitX(characterUnit), GetUnitY(characterUnit))
							call this.addItemToRucksack(equippedItem, true, false)
							call BJDebugMsg("Add old equipment to rucksack")
							call this.clearEquipmentItem(equipmentType, false)
							set equippedItem = null
						endif
						call this.setEquipmentItemByItem(equipmentType, usedItem, not this.m_rucksackIsEnabled)
						if (showEquipMessage) then
							call this.character().displayMessage(ACharacter.messageTypeInfo, StringArg(thistype.textEquipItem, GetItemName(usedItem)))
						endif
						set characterUnit = null
						return
					endif
				debug else
					debug call this.print("Swap with already equipped is false or equipment item type is 0")
				endif
			debug else
				debug call this.print("Item type is " + I2S(itemType) + " and equipmentType is " + I2S(equipmentType))
			endif
			//move to rucksack
			if (not dontMoveToRucksack) then
				call this.addItemToRucksack(usedItem, true, true) //if item type is 0 it will be placed in rucksack, too
			else
				call this.character().displayMessage(ACharacter.messageTypeError, thistype.textUnableToEquipItem)
			endif
			set characterUnit = null
		endmethod

		private method addItemToRucksack takes item usedItem, boolean dontMoveToEquipment, boolean showAddMessage returns nothing
			local integer i
			local player itemPlayer = GetItemPlayer(usedItem)
			local unit characterUnit = this.unit()
			if (this.m_rucksackIsEnabled and UnitHasItem(characterUnit, usedItem)) then //already picked up
				call DisableTrigger(this.m_dropTrigger)
				debug call this.print("Drop item from rucksack because it was already placed in")
				call UnitDropItemPoint(characterUnit, usedItem, GetUnitX(characterUnit), GetUnitY(characterUnit))
				call EnableTrigger(this.m_dropTrigger)
			endif
			set characterUnit = null
			
			if (itemPlayer != this.user() and IsPlayerPlayingUser(itemPlayer)) then
				call this.character().displayMessage(ACharacter.messageTypeError, tr("Gegenstand gehört einem anderen Benutzer.")) /// @todo use static string member
				set itemPlayer = null
				return
			endif
			set itemPlayer = null
			
			set i = 0
			loop
				exitwhen (i == thistype.maxRucksackItems)
				if (this.m_rucksackItemType[i] == 0 or  this.m_rucksackItemType[i] == GetItemTypeId(usedItem)) then
					debug call this.print("Rucksack slot " + I2S(i) + " is free or has the same item type and will be used.")
					call this.setRucksackItemByItem(i, usedItem, this.m_rucksackIsEnabled and this.itemRucksackPage(i) == this.m_rucksackPage)
					if (showAddMessage) then
						call this.character().displayMessage(ACharacter.messageTypeInfo, StringArg(thistype.textAddItemToRucksack, GetItemName(usedItem)))
					endif
					return
				endif
				set i = i + 1
			endloop

			//equip
			if (not dontMoveToEquipment) then
				debug call this.print("Item is not from equipment and will be equipped.")
				call this.equipItem(usedItem, true, false, true)
			else
				call this.character().displayMessage(ACharacter.messageTypeError, thistype.textUnableToAddRucksackItem)
			endif
		endmethod
		
		private method setEquipmentItem takes integer equipmentType, integer itemTypeId, boolean add returns nothing
			set this.m_equipmentItemType[equipmentType] = itemTypeId
			if (add) then
				call this.showEquipmentItem(equipmentType)
			endif
		endmethod

		private method setEquipmentItemByItem takes integer equipmentType, item usedItem, boolean add returns nothing
			local integer itemTypeId = GetItemTypeId(usedItem)
			call DisableTrigger(this.m_dropTrigger)
			call RemoveItem(usedItem) 
			call EnableTrigger(this.m_dropTrigger)
			set usedItem = null
			call this.setEquipmentItem(equipmentType, itemTypeId, add)
		endmethod

		private method clearEquipmentItem takes integer equipmentType, boolean drop returns nothing
			local unit characterUnit = this.unit()
			local item slotItem
			local AItemType itemType
			
			if (not this.m_rucksackIsEnabled) then
				set slotItem = UnitItemInSlot(characterUnit, equipmentType)
				if (slotItem != null) then
					call thistype.clearItemIndex(slotItem)
					call DisableTrigger(this.m_dropTrigger)
					if (drop) then
						call UnitDropItemPoint(characterUnit, slotItem, GetUnitX(characterUnit), GetUnitY(characterUnit))
					else
						call RemoveItem(slotItem)
					endif
					call EnableTrigger(this.m_dropTrigger)
					set slotItem = null
				endif
			else
				set itemType = AItemType.getItemTypeOfItemTypeId(this.m_equipmentItemType[equipmentType])
				call itemType.removeAllAbilities(characterUnit) /// @todo Permanent
			endif
			set this.m_equipmentItemType[equipmentType] = 0
			call this.checkEquipment() //added
			set characterUnit = null
		endmethod

		private method showEquipmentItem takes integer equipmentType returns nothing
			local unit characterUnit = this.unit()
			local item slotItem
			local AItemType itemType = AItemType.getItemTypeOfItemTypeId(this.m_equipmentItemType[equipmentType])
			//call itemType.removeAllAbilities(characterUnit) //required? /// @todo Permanent
			call DisableTrigger(this.m_pickupTrigger)
			call UnitAddItemToSlotById(characterUnit, this.m_equipmentItemType[equipmentType], equipmentType)
			call EnableTrigger(this.m_pickupTrigger)
			set slotItem = UnitItemInSlot(characterUnit, equipmentType)
			call SetItemDropOnDeath(slotItem, false)
			call thistype.setItemIndex(slotItem, equipmentType)
			set characterUnit = null
			set slotItem = null
		endmethod

		private method hideEquipmentItem takes integer equipmentType returns nothing
			local unit characterUnit = this.unit()
			local item slotItem = UnitItemInSlot(characterUnit, equipmentType)
			local AItemType itemType = AItemType.getItemTypeOfItem(slotItem)
			call thistype.clearItemIndex(slotItem)
			call DisableTrigger(this.m_dropTrigger)
			call RemoveItem(slotItem)
			call EnableTrigger(this.m_dropTrigger)
			call itemType.addAllAbilities(characterUnit) /// @todo Permanent
			set characterUnit = null
			set slotItem = null
		endmethod

		/// Checks requirements of all equipped items. If some requirements aren't met the checked item is dropped.
		/// This should be called whenever character units attributes which are used for item type requirement change.
		private method checkEquipment takes nothing returns nothing
			local item slotItem
			local AItemType itemType
			local integer i = 0
			loop
				exitwhen (i == thistype.maxEquipmentTypes)
				if (this.m_equipmentItemType[i] != 0) then
					set slotItem = UnitItemInSlot(this.unit(), i)
					set itemType = AItemType.getItemTypeOfItem(slotItem)
					if (not itemType.checkRequirement(this.character())) then
						call this.clearEquipmentItem(i, true)
					endif
					set slotItem = null
				endif
				set i = i + 1
			endloop
		endmethod
		
		private method setRucksackItem takes integer index, integer itemTypeId, integer charges, boolean add returns nothing
			local boolean refreshOnly = false
			if (this.m_rucksackItemType[index] == 0) then
				set this.m_rucksackItemType[index] = itemTypeId
				set this.m_rucksackItemCharges[index] = charges
			else //same type
				set this.m_rucksackItemCharges[index] = this.m_rucksackItemCharges[index] + IMaxBJ(charges, 1)
				set refreshOnly = true
			endif
			if (add) then
				if (not refreshOnly) then
					call this.showRucksackItem(index)
				else
					call this.refreshRucksackItemCharges(index)
				endif
			endif
		endmethod

		private method setRucksackItemByItem takes integer index, item usedItem, boolean add returns nothing
			local integer itemTypeId = GetItemTypeId(usedItem)
			local integer charges = GetItemCharges(usedItem)
			call DisableTrigger(this.m_dropTrigger)
			call RemoveItem(usedItem)
			call EnableTrigger(this.m_dropTrigger)
			set usedItem = null
			call this.setRucksackItem(index, itemTypeId, charges, add)
		endmethod

		private method clearRucksackItem takes integer index, boolean drop returns nothing
			local unit characterUnit
			local item slotItem
			if (this.m_rucksackIsEnabled and this.m_rucksackPage == this.itemRucksackPage(index)) then
				set characterUnit = this.unit()
				set slotItem = UnitItemInSlot(characterUnit, this.rucksackItemSlot(index))
				if (slotItem != null) then
					call thistype.clearItemIndex(slotItem)
					call DisableTrigger(this.m_dropTrigger)
					if (drop) then
						call UnitDropItemPoint(characterUnit, slotItem, GetUnitX(characterUnit), GetUnitY(characterUnit))
					else
						call RemoveItem(slotItem)
					endif
					call EnableTrigger(this.m_dropTrigger)
					set slotItem = null
				endif
				set characterUnit = null
			endif
			set this.m_rucksackItemType[index] = 0
			set this.m_rucksackItemCharges[index] = 0
		endmethod

		private method showRucksackItem takes integer index returns nothing
			local unit characterUnit = this.unit()
			local integer slot = this.rucksackItemSlot(index)
			local item slotItem
			local AItemType itemType = AItemType.getItemTypeOfItemTypeId(this.m_rucksackItemType[index])
			call DisableTrigger(this.m_pickupTrigger)
			call UnitAddItemToSlotById(characterUnit, this.m_rucksackItemType[index], slot)
			call EnableTrigger(this.m_pickupTrigger)
			set slotItem = UnitItemInSlot(characterUnit, slot)
			call SetItemDropOnDeath(slotItem, false)
			call SetItemCharges(slotItem, this.m_rucksackItemCharges[index])
			call itemType.removeAllAbilities(characterUnit) /// @todo Permanent
			call thistype.setItemIndex(slotItem, index)
			set characterUnit = null
			set slotItem = null
		endmethod
		
		private method refreshRucksackItemCharges takes integer index returns nothing
			local unit characterUnit
			local integer slot
			local item slotItem
			if (this.m_rucksackPage != this.itemRucksackPage(index)) then
				return
			endif
			set characterUnit = this.unit()
			set slot = this.rucksackItemSlot(index)
			set slotItem = UnitItemInSlot(characterUnit, slot)
			call SetItemCharges(slotItem, this.m_rucksackItemCharges[index])
			set characterUnit = null
			set slotItem = null
		endmethod

		private method hideRucksackItem takes integer index returns nothing
			local unit characterUnit = this.unit()
			local integer slot = this.rucksackItemSlot(index)
			local item slotItem = UnitItemInSlot(characterUnit, slot)
			call thistype.clearItemIndex(slotItem)
			call DisableTrigger(this.m_dropTrigger)
			call RemoveItem(slotItem)
			call EnableTrigger(this.m_dropTrigger)
			set characterUnit = null
			set slotItem = null
		endmethod

		private method showRucksackPage takes integer page, boolean firstCall returns nothing
			local integer i
			local integer exitValue
			local item rightArrowItem
			debug if (page > thistype.maxRucksackPages) then
				debug call this.print("Page value is too big.")
				debug return
			debug endif

			if (not firstCall) then
				call this.hideCurrentRucksackPage()
			endif
			
			set this.m_rucksackPage = page
			//add inventory items
			set i = page * thistype.maxRucksackItemsPerPage
			set exitValue = i + thistype.maxRucksackItemsPerPage
			loop
				exitwhen (i == exitValue)
				if (this.m_rucksackItemType[i] != 0) then
					call this.showRucksackItem(i)
				endif
				set i = i + 1
			endloop
			
			set rightArrowItem = UnitItemInSlot(this.unit(), thistype.maxRucksackItemsPerPage + 1)
			call SetItemCharges(rightArrowItem, page)
			set rightArrowItem = null
		endmethod

		private method hideCurrentRucksackPage takes nothing returns nothing
			local integer i = this.m_rucksackPage * thistype.maxRucksackItemsPerPage
			local integer exitValue = i + thistype.maxRucksackItemsPerPage
			loop
				exitwhen (i == exitValue)
				if (this.m_rucksackItemType[i] != 0) then
					call this.hideRucksackItem(i)
				endif
				set i = i + 1
			endloop
		endmethod

		private method showNextRucksackPage takes nothing returns nothing
			if (this.m_rucksackPage == thistype.maxRucksackPages - 1) then
				call this.showRucksackPage(0, false)
			else
				call this.showRucksackPage(this.m_rucksackPage + 1, false)
			endif
		endmethod

		private method showPreviousRucksackPage takes nothing returns nothing
			if (this.m_rucksackPage == 0) then
				call this.showRucksackPage(thistype.maxRucksackPages - 1, false)
			else
				call this.showRucksackPage(this.m_rucksackPage - 1, false)
			endif
		endmethod
		
		private method resetItemSlots takes integer currentSlot, integer oldSlot returns nothing
			local unit characterUnit = this.unit()
			local item currentItem = UnitItemInSlot(characterUnit, currentSlot)
			local integer currentItemTypeId = -1
			local integer currentItemCharges
			local item otherItem = UnitItemInSlot(characterUnit, oldSlot)
			local integer otherItemTypeId = -1
			local integer otherItemCharges
			call DisableTrigger(this.m_dropTrigger)
			if (currentItem != null) then
				set currentItemTypeId = GetItemTypeId(currentItem)
				set currentItemCharges = GetItemCharges(currentItem)
				call RemoveItem(currentItem)
				set currentItem = null
			endif
			if (otherItem != null) then
				set otherItemTypeId = GetItemTypeId(otherItem)
				set otherItemCharges = GetItemCharges(otherItem)
				call RemoveItem(otherItem)
				set otherItem = null
			endif
			call EnableTrigger(this.m_dropTrigger)
			call DisableTrigger(this.m_pickupTrigger)
			if (currentItemTypeId != -1) then
				call UnitAddItemToSlotById(characterUnit, currentItemTypeId, oldSlot)
				set currentItem = UnitItemInSlot(characterUnit, oldSlot)
				call SetItemCharges(currentItem, currentItemCharges)
				if (currentItemTypeId != thistype.leftArrowItemType and currentItemTypeId != thistype.rightArrowItemType) then
					if (this.m_rucksackIsEnabled) then
						call this.setItemIndex(currentItem, this.slotRucksackIndex(oldSlot))
					else
						call this.setItemIndex(currentItem, oldSlot)
					endif
				endif
				set currentItem = null
			endif
			if (otherItemTypeId != -1) then
				call UnitAddItemToSlotById(characterUnit, otherItemTypeId, currentSlot)
				set otherItem = UnitItemInSlot(characterUnit, oldSlot)
				call SetItemCharges(otherItem, otherItemCharges)
				if (otherItemTypeId != thistype.leftArrowItemType and otherItemTypeId != thistype.rightArrowItemType) then
					if (this.m_rucksackIsEnabled) then
						call this.setItemIndex(otherItem, this.slotRucksackIndex(currentSlot))
					else
						call this.setItemIndex(otherItem, currentSlot)
					endif
				endif
				set otherItem = null
			endif
			call EnableTrigger(this.m_pickupTrigger)
		endmethod

		/// @todo If you drop the item instead of removing it drop trigger will be triggered although it was disabled before.
		private method moveRucksackItemToPage takes integer slot, boolean next returns nothing
			local unit characterUnit = this.unit()
			local item slotItem = UnitItemInSlot(characterUnit, slot)
			local integer oldIndex = this.itemIndex(slotItem)
			local integer oldSlot = this.rucksackItemSlot(oldIndex)
			local integer i
			local integer exitValue
			
			if (next) then
				call BJDebugMsg("Move to next page.")
			else
				call BJDebugMsg("Move to previous page.")
			endif
			
			call BJDebugMsg("Move item which is in slot " + I2S(slot) + " now. Its name is " + GetItemName(slotItem))
			
			//reset page item (items were swapped) and drop moved item
			call DisableTrigger(this.m_dropTrigger)
			call RemoveItem(slotItem)
			set slotItem = null
			//call BJDebugMsg("Drop item " + GetItemName(slotItem) + " from unit " + GetUnitName(characterUnit))
			//call UnitDropItemPoint(characterUnit, slotItem, GetUnitX(characterUnit), GetUnitY(characterUnit)) //is not dropped?!
			call EnableTrigger(this.m_dropTrigger)
			
			if (next) then
				call this.resetItemSlots(oldSlot, thistype.maxRucksackItemsPerPage + 1)
				set i = oldIndex + thistype.maxRucksackItemsPerPage - oldSlot
				set exitValue = thistype.maxRucksackItems
				
				if (i == thistype.maxRucksackItems) then
					set i = 0
				endif
			else
				call this.resetItemSlots(oldSlot, thistype.maxRucksackItemsPerPage)
				set i = oldIndex - thistype.maxRucksackItemsPerPage + thistype.maxRucksackItemsPerPage - oldSlot - 1 //test - 1
				set exitValue = 0
				
				if (i < 0) then
					set i = thistype.maxRucksackItems - 1
					debug call Print("i is smaller than 0 " + I2S(i))
				endif
			endif
			
			loop
				//exitwhen (i == exitValue)
				call BJDebugMsg("Checking index " + I2S(i))
				//reached old index, remove dropped slot item and show it again. show error message.
				if (i == oldIndex) then
					call BJDebugMsg("Is on old index again")
					//call RemoveItem(slotItem) //do not disable drop triggers, item is dropped
					call this.showRucksackItem(oldIndex)
					call this.character().displayMessage(ACharacter.messageTypeError, thistype.textUnableToMoveRucksackItem)
					exitwhen (true)
				//found a free place
				elseif (this.m_rucksackItemType[i] == 0) then
					call BJDebugMsg("Found free index " + I2S(i))
					call this.setRucksackItem(i, this.m_rucksackItemType[oldIndex], this.m_rucksackItemCharges[oldIndex], this.m_rucksackIsEnabled and this.itemRucksackPage(i) == this.m_rucksackPage)
					call this.clearRucksackItem(oldIndex, false)
					exitwhen (true)
				endif
				if (next) then
					set i = i + 1
					if (i == exitValue) then
						set i = 0
					endif
				else
					set i = i - 1
					if (i == exitValue) then
						set i = thistype.maxRucksackItems - 1
					endif
				endif
			endloop
			set characterUnit = null
		endmethod

		private method swapRucksackItemData takes item firstItem, item secondItem returns nothing
			local integer firstIndex = this.itemIndex(firstItem)
			local integer secondIndex = this.itemIndex(secondItem)
			local integer itemType = this.m_rucksackItemType[firstIndex]
			local integer itemCharges = this.m_rucksackItemCharges[firstIndex]
			set this.m_rucksackItemType[firstIndex] = this.m_rucksackItemType[secondIndex]
			set this.m_rucksackItemCharges[firstIndex] = this.m_rucksackItemCharges[secondIndex]
			call thistype.setItemIndex(firstItem, secondIndex)
			set this.m_rucksackItemType[secondIndex] = itemType
			set this.m_rucksackItemCharges[secondIndex] = itemCharges
			call thistype.setItemIndex(secondItem, firstIndex)
		endmethod

		private method moveRucksackItem takes item movedItem, integer slot returns nothing
			local unit characterUnit = this.unit()
			local item targetItem
			local integer oldIndex = this.itemIndex(movedItem)
			local integer newIndex = this.slotRucksackIndex(slot)
			local integer itemTypeId

			//equip
			if (oldIndex == newIndex) then
				call BJDebugMsg("Equip")
				set itemTypeId = this.m_rucksackItemType[oldIndex]
				if (this.m_rucksackItemCharges[oldIndex] > 0) then
					debug call this.print("Charges are bigger than 0")
					set this.m_rucksackItemCharges[oldIndex] = this.m_rucksackItemCharges[oldIndex] - 1
					call this.refreshRucksackItemCharges(oldIndex)
				else
					call this.clearRucksackItem(oldIndex, false)
				endif
				set movedItem = null
				set movedItem = CreateItem(itemTypeId, GetUnitX(characterUnit), GetUnitY(characterUnit))
				call this.equipItem(movedItem, false, true, true) //test
				set characterUnit = null
				return
			endif
			set targetItem = UnitItemInSlot(this.unit(), this.rucksackItemSlot(oldIndex))
			//move
			if (targetItem == null) then
				set this.m_rucksackItemType[newIndex] = this.m_rucksackItemType[oldIndex]
				call thistype.setItemIndex(movedItem, newIndex)
				set targetItem = CreateItem(this.m_rucksackItemType[oldIndex], GetUnitX(characterUnit), GetUnitY(characterUnit)) //temporary item for getting item type
				//destack
				if ((this.m_rucksackItemCharges[oldIndex] > 1 and GetItemType(movedItem) == ITEM_TYPE_CHARGED) or (this.m_rucksackItemCharges[oldIndex] > 0 and GetItemType(movedItem) != ITEM_TYPE_CHARGED)) then
					call BJDebugMsg("Destack")
					set this.m_rucksackItemCharges[oldIndex] = this.m_rucksackItemCharges[oldIndex] - 1
					if (GetItemType(targetItem) == ITEM_TYPE_CHARGED) then
						call BJDebugMsg("Is charged set to 1, index " + I2S(newIndex))
						set this.m_rucksackItemCharges[newIndex] = 1
					else
						call BJDebugMsg("Is not charged")
						set this.m_rucksackItemCharges[newIndex] = 0
					endif
					call this.showRucksackItem(oldIndex)
					call this.refreshRucksackItemCharges(newIndex)
				//normal movement
				else
					call BJDebugMsg("Move to new index " + I2S(newIndex))
					set this.m_rucksackItemCharges[newIndex] = this.m_rucksackItemCharges[oldIndex]
					//clear old
					set this.m_rucksackItemType[oldIndex] = 0
					set this.m_rucksackItemCharges[oldIndex] = 0
				endif
				call RemoveItem(targetItem)
				set targetItem = null
			//stack
			elseif (GetItemTypeId(movedItem) == GetItemTypeId(targetItem)) then
				call BJDebugMsg("Stack with target item name " + GetItemName(targetItem))
				set this.m_rucksackItemCharges[newIndex] = this.m_rucksackItemCharges[newIndex] + IMaxBJ(1, this.m_rucksackItemCharges[oldIndex])
				call this.refreshRucksackItemCharges(newIndex)
				call this.clearRucksackItem(oldIndex, false)
			//swap
			else
				call BJDebugMsg("Swap")
				call this.swapRucksackItemData(movedItem, targetItem)
			endif
			set characterUnit = null
			set targetItem = null
		endmethod
		
		private static method triggerConditionOpen takes nothing returns boolean
			return GetSpellAbilityId() == thistype.openRucksackAbilityId
		endmethod

		private static method triggerActionOpen takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call BJDebugMsg("Open")
			if (this.m_rucksackIsEnabled) then
				call this.disableRucksack()
				call this.enableEquipment()
			else
				call this.disableEquipment()
				call this.enableRucksack()
			endif
			set triggeringTrigger = null
		endmethod

		private method createOpenTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.m_openTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.m_openTrigger, this.unit(), EVENT_UNIT_SPELL_CAST)
			set conditionFunction = Condition(function thistype.triggerConditionOpen)
			set triggerCondition = TriggerAddCondition(this.m_openTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.m_openTrigger, function thistype.triggerActionOpen)
			call AHashTable.global().setHandleInteger(this.m_openTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod
		
		private static method triggerConditionOrder takes nothing returns boolean
			return GetIssuedOrderId() >= 852002 and GetIssuedOrderId() <= 852007 //852008) and (i<=852013) then are use item orders
		endmethod

		private static method triggerActionOrder takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local integer newSlot = GetIssuedOrderId() - 852002
			local item usedItem = GetOrderTargetItem()
			local integer oldSlot
			local item targetItem
			local integer itemTypeId
			local integer targetItemTypeId
			call TriggerSleepAction(0.0) //wait until order is done, important!
			call BJDebugMsg("Order with new slot " + I2S(newSlot) + " left item type " + I2S(thistype.leftArrowItemType) + " and right item type " + I2S(thistype.rightArrowItemType) + " and used item type " + I2S(GetItemTypeId(usedItem)))
			if (this.m_rucksackIsEnabled) then
				call BJDebugMsg("Rucksack is enabled.")
				if (GetItemTypeId(usedItem) == thistype.leftArrowItemType and newSlot != thistype.maxRucksackItemsPerPage) then
					call BJDebugMsg("Reset left.")
					call this.resetItemSlots(newSlot, AInventory.maxRucksackItemsPerPage)
				elseif (GetItemTypeId(usedItem) == thistype.rightArrowItemType and newSlot != thistype.maxRucksackItemsPerPage + 1) then
					call BJDebugMsg("Reset right.")
					call this.resetItemSlots(newSlot, thistype.maxRucksackItemsPerPage + 1)
				//move item previous - player drops an item on the next page item
				elseif (GetItemTypeId(usedItem) != thistype.leftArrowItemType and newSlot == thistype.maxRucksackItemsPerPage) then
					call BJDebugMsg("Move to previous page.")
					call this.moveRucksackItemToPage(newSlot, false)
				//move item next - player drops an item on the previous page item
				elseif (GetItemTypeId(usedItem) != thistype.rightArrowItemType and newSlot == thistype.maxRucksackItemsPerPage + 1) then
					call this.moveRucksackItemToPage(newSlot, true)
				//equip item/stack items/swap items
				elseif (newSlot >= 0 and newSlot < thistype.maxRucksackItemsPerPage) then
					call BJDebugMsg("Move item to slot " + I2S(newSlot))
					call this.moveRucksackItem(usedItem, newSlot)
				endif
			else
				set oldSlot = thistype.itemIndex(usedItem)
				//reset moved equipped items to their positions
				if (newSlot != oldSlot) then
					call BJDebugMsg("Moving equipment items is not possible :-) Item index/slot " + I2S(oldSlot))
					call this.resetItemSlots(newSlot, oldSlot)
				//old slot, add to rucksack
				else
					call BJDebugMsg("Add to rucksack")
					call this.clearItemIndex(usedItem)
					call this.clearEquipmentItem(oldSlot, true)
					call this.addItemToRucksack(usedItem, true, true)
					//test
					//call RemoveItem(usedItem)
					//set usedItem = null
				endif
			endif
			set triggeringTrigger = null
			set usedItem = null
		endmethod

		//add, drop, move item next, move item previous, stack items, destack item, swap items
		private method createOrderTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.m_orderTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.m_orderTrigger, this.unit(), EVENT_UNIT_ISSUED_TARGET_ORDER)
			set conditionFunction = Condition(function thistype.triggerConditionOrder)
			set triggerCondition = TriggerAddCondition(this.m_orderTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.m_orderTrigger, function thistype.triggerActionOrder)
			call AHashTable.global().setHandleInteger(this.m_orderTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod
		
		private static method triggerConditionIsNoPowerup takes nothing returns boolean
			local item usedItem = GetManipulatedItem()
			local boolean result = not IsItemIdPowerup(GetItemTypeId(usedItem))
			set usedItem = null
			return result
		endmethod

		private static method triggerActionPickup takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local item usedItem = GetManipulatedItem()
			call this.equipItem(usedItem, false, false, true) //try always equipment first!
			set triggeringTrigger = null
			set usedItem = null
		endmethod

		private method createPickupTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.m_pickupTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.m_pickupTrigger, this.unit(), EVENT_UNIT_PICKUP_ITEM) //pawn?
			set conditionFunction = Condition(function thistype.triggerConditionIsNoPowerup)
			set triggerCondition = TriggerAddCondition(this.m_pickupTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.m_pickupTrigger, function thistype.triggerActionPickup)
			call AHashTable.global().setHandleInteger(this.m_pickupTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		private static method triggerActionDrop takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local item usedItem = GetManipulatedItem()
			local integer index = this.itemIndex(usedItem)
			local unit triggerUnit = GetTriggerUnit()
			if (this.m_rucksackIsEnabled) then
				//destack and drop
				if (GetItemType(usedItem) == ITEM_TYPE_CHARGED and this.m_rucksackItemCharges[index] > 1 or GetItemType(usedItem) != ITEM_TYPE_CHARGED and this.m_rucksackItemCharges[index] > 0) then
					call TriggerSleepAction(0.0) //wait until it has been dropped
					set this.m_rucksackItemCharges[index] = this.m_rucksackItemCharges[index] - 1
					call this.showRucksackItem(index)
					
					if (GetItemType(usedItem) == ITEM_TYPE_CHARGED) then
						call SetItemCharges(usedItem, 1)
					else
						call SetItemCharges(usedItem, 0)
					endif
				//drop
				else
					call this.clearRucksackItem(index, true) //before drop
				endif
			//unequip and drop
			else
				call this.clearEquipmentItem(index, true)
			endif
			set triggeringTrigger = null
			set usedItem = null
			set triggerUnit = null
		endmethod

		//drop, destack and drop, unequip and drop
		private method createDropTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.m_dropTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.m_dropTrigger, this.unit(), EVENT_UNIT_DROP_ITEM)
			set conditionFunction = Condition(function thistype.triggerConditionIsNoPowerup)
			set triggerCondition = TriggerAddCondition(this.m_dropTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.m_dropTrigger, function thistype.triggerActionDrop)
			call AHashTable.global().setHandleInteger(this.m_dropTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		private static method triggerActionUse takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this") 
			local item usedItem = GetManipulatedItem()
			local integer itemTypeId = GetItemTypeId(usedItem)
			local integer index
			/// @todo if we have no usage in equipment we could also add a trigger condition
			if (this.m_rucksackIsEnabled) then
				set usedItem = GetManipulatedItem()
				set itemTypeId = GetItemTypeId(usedItem)
				//show next page
				if (itemTypeId == thistype.rightArrowItemType) then
					call this.showNextRucksackPage()
				//show previous page
				elseif (itemTypeId == thistype.leftArrowItemType) then
					call this.showPreviousRucksackPage()
				//if an item is used by decreasing its number of charges (not to 0!) we have to decrease our number, too
				else
					set index = thistype.itemIndex(usedItem)
					if (this.m_rucksackItemCharges[index] > 1) then
						set this.m_rucksackItemCharges[index] = this.m_rucksackItemCharges[index] - 1
						call this.refreshRucksackItemCharges(index)
					endif
				endif
				set usedItem = null
			endif
			set triggeringTrigger = null
		endmethod

		private method createUseTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_useTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.m_useTrigger, this.unit(), EVENT_UNIT_USE_ITEM)
			set triggerAction = TriggerAddAction(this.m_useTrigger, function thistype.triggerActionUse)
			call AHashTable.global().setHandleInteger(this.m_useTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate(character)
			//members
			set this.m_rucksackPage = 0
			set this.m_rucksackIsEnabled = false

			call UnitAddAbility(this.unit(), thistype.openRucksackAbilityId)

			call this.createOpenTrigger()
			call this.createOrderTrigger()
			call this.createPickupTrigger()
			call this.createDropTrigger()
			call this.createUseTrigger()
			return this
		endmethod

		private method destroyOpenTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_openTrigger)
			set this.m_openTrigger = null
		endmethod

		private method destroyOrderTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_orderTrigger)
			set this.m_orderTrigger = null
		endmethod

		private method destroyPickupTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_pickupTrigger)
			set this.m_pickupTrigger = null
		endmethod

		private method destroyDropTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_dropTrigger)
			set this.m_dropTrigger = null
		endmethod

		private method destroyUseTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_useTrigger)
			set this.m_useTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing
			call UnitRemoveAbility(this.unit(), thistype.openRucksackAbilityId)

			call this.destroyOpenTrigger()
			call this.destroyOrderTrigger()
			call this.destroyPickupTrigger()
			call this.destroyDropTrigger()
			call this.destroyUseTrigger()
		endmethod

		public static method init takes integer leftArrowItemType, integer rightArrowItemType, integer openRucksackAbilityId, string textUnableToEquipItem, string textEquipItem, string textUnableToAddRucksackItem, string textAddItemToRucksack, string textUnableToMoveRucksackItem returns nothing
			//static start members
			set thistype.leftArrowItemType = leftArrowItemType
			set thistype.rightArrowItemType = rightArrowItemType
			set thistype.openRucksackAbilityId = openRucksackAbilityId
			set thistype.textUnableToEquipItem = textUnableToEquipItem
			set thistype.textEquipItem = textEquipItem
			set thistype.textUnableToAddRucksackItem = textUnableToAddRucksackItem
			set thistype.textAddItemToRucksack = textAddItemToRucksack
			set thistype.textUnableToMoveRucksackItem = textUnableToMoveRucksackItem
		endmethod
		
		/// @return Returns the page of a rucksack item by index.
		public static method itemRucksackPage takes integer index returns integer
			debug if (index >= thistype.maxRucksackItems or index < 0) then
				debug call thistype.staticPrint("Wrong rucksack index: " + I2S(index) + ".")
				debug return 0
			debug endif
			return index / thistype.maxRucksackItemsPerPage
			//local integer i = 0
			//loop
				//exitwhen (i == thistype.maxRucksackPages)
				//if (index < i + thistype.maxRucksackItemsPerPage) then
					//return i
				//endif
			//	set i = i + 1
			//endloop
			//return -1
		endmethod
		
		/// Just required for the move order and for item dropping.
		private static method setItemIndex takes item usedItem, integer index returns nothing
			call AHashTable.global().setHandleInteger(usedItem, "AInventory_index", index)
		endmethod
		
		/// Just required for the move order and for item dropping.
		private static method itemIndex takes item usedItem returns integer
			return AHashTable.global().handleInteger(usedItem, "AInventory_index")
		endmethod
		
		/// Just required for the move order and for item dropping.
		private static method clearItemIndex takes item usedItem returns nothing
			call AHashTable.global().removeHandleInteger(usedItem, "AInventory_index")
		endmethod
	endstruct

endlibrary