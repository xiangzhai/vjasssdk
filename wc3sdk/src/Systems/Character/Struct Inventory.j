library AStructSystemsCharacterInventory requires ALibraryCoreGeneralPlayer, AStructCoreGeneralHashTable, ALibraryCoreGeneralUnit, ALibraryCoreStringConversion, AStructSystemsCharacterAbstractCharacterSystem, AStructSystemsCharacterCharacter, AStructSystemsCharacterItemType

	struct AInventoryItemData
		//dynamic members
		private integer m_itemTypeId
		private integer m_charges
		private integer m_dropId
		private boolean m_invulnerable
		private real m_life
		private boolean m_pawnable
		private player m_player
		private integer m_userData
		private boolean m_visible
		//members
		private itemtype m_itemType
		
		public method setItemTypeId takes integer itemTypeId returns nothing
			set this.m_itemTypeId = itemTypeId
		endmethod
		
		public method itemTypeId takes nothing returns integer
			return this.m_itemTypeId
		endmethod
		
		public method setCharges takes integer charges returns nothing
			set this.m_charges = charges
		endmethod
		
		public method charges takes nothing returns integer
			return this.m_charges
		endmethod
		
		public method setDropId takes integer dropId returns nothing
			set this.m_dropId = dropId
		endmethod
		
		public method dropId takes nothing returns integer
			return this.m_dropId
		endmethod
		
		public method setInvulnerable takes boolean invulnerable returns nothing
			set this.m_invulnerable = invulnerable
		endmethod
		
		public method invulnerable takes nothing returns boolean
			return this.m_invulnerable
		endmethod
		
		public method setLife takes real life returns nothing
			set this.m_life = life
		endmethod
		
		public method life takes nothing returns real
			return this.m_life
		endmethod
		
		public method setPawnable takes boolean pawnable returns nothing
			set this.m_pawnable = pawnable
		endmethod
		
		public method pawnable takes nothing returns boolean
			return this.m_pawnable
		endmethod
		
		public method setPlayer takes player user returns nothing
			set this.m_player = user
		endmethod
		
		public method player takes nothing returns player
			return this.m_player
		endmethod
		
		public method setUserData takes integer userData returns nothing
			set this.m_userData = userData
		endmethod
		
		public method userData takes nothing returns integer
			return this.m_userData
		endmethod
		
		public method setVisible takes boolean visible returns nothing
			set this.m_visible = visible
		endmethod
		
		public method visible takes nothing returns boolean
			return this.m_visible
		endmethod
		
		public method itemType takes nothing returns itemtype
			return this.m_itemType
		endmethod
		
		public method createItem takes real x, real y returns item
			local item result = CreateItem(this.m_itemTypeId, x, y)
			call SetItemCharges(result, this.m_charges)
			call SetItemDropID(result, this.m_dropId)
			call SetItemInvulnerable(result, this.m_invulnerable)
			call SetWidgetLife(result, this.m_life)
			call SetItemPawnable(result, this.m_pawnable)
			call SetItemPlayer(result, this.m_player, true)
			call SetItemUserData(result, this.m_userData)
			call SetItemVisible(result, this.m_visible)
			return result
		endmethod
		
		public method assignToItem takes item usedItem returns nothing
			call SetItemCharges(usedItem, this.m_charges)
			call SetItemDropID(usedItem, this.m_dropId)
			call SetItemInvulnerable(usedItem, this.m_invulnerable)
			call SetWidgetLife(usedItem, this.m_life)
			call SetItemPawnable(usedItem, this.m_pawnable)
			call SetItemPlayer(usedItem, this.m_player, true)
			call SetItemUserData(usedItem, this.m_userData)
			call SetItemVisible(usedItem, this.m_visible)
		endmethod
		
		public method isCharged takes nothing returns boolean
			return (this.m_itemType == ITEM_TYPE_CHARGED and this.m_charges > 1) or (this.m_itemType != ITEM_TYPE_CHARGED and this.m_charges > 0)
		endmethod
		
		public static method create takes item usedItem, unit usedUnit returns thistype
			local thistype this = thistype.allocate()
			//dynamic members
			set this.m_itemTypeId = GetItemTypeId(usedItem)
			set this.m_charges = GetItemCharges(usedItem)
			set this.m_dropId = GetUnitTypeId(usedUnit)
			set this.m_invulnerable = IsItemInvulnerable(usedItem)
			set this.m_life = GetWidgetLife(usedItem)
			set this.m_pawnable = IsItemPawnable(usedItem)
			set this.m_player = GetItemPlayer(usedItem)
			set this.m_userData = GetItemUserData(usedItem)
			set this.m_visible = IsItemVisible(usedItem)
			//members
			set this.m_itemType = GetItemType(usedItem)
			return this
		endmethod
		
		public method onDestroy takes nothing returns nothing
			//members
			set this.m_itemType = null
		endmethod
	endstruct

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
		private static string textDropPageItem
		//members
		private AInventoryItemData array m_equipmentItemData[thistype.maxEquipmentTypes]
		private AInventoryItemData array m_rucksackItemData[thistype.maxRucksackItems]
		private trigger m_openTrigger
		private trigger m_orderTrigger
		private trigger m_useTrigger //show next page, show previous page, disable in equipment
		private trigger m_pickupTrigger //pickup
		private trigger m_dropTrigger //drop
		private integer m_rucksackPage
		private boolean m_rucksackIsEnabled
		
		//! runtextmacro optional A_STRUCT_DEBUG("\"AInventory\"")

		//members

		public method equipmentItemData takes integer equipmentType returns AInventoryItemData
			debug if (equipmentType >= thistype.maxEquipmentTypes or equipmentType < 0) then
				debug call this.print("Wrong equipment type: " + I2S(equipmentType) + ".")
				debug return 0
			debug endif
			return this.m_equipmentItemData[equipmentType]
		endmethod

		public method rucksackItemData takes integer index returns AInventoryItemData
			debug if (index >= thistype.maxRucksackItems or index < 0) then
				debug call this.print("Wrong rucksack index: " + I2S(index) + ".")
				debug return 0
			debug endif
			return this.m_rucksackItemData[index]
		endmethod
		
		//convenience methods
		
		public method allEquipmentItems takes nothing returns integer
			local integer result = 0
			local integer i = 0
			loop
				exitwhen (i == thistype.maxEquipmentTypes)
				if (this.m_equipmentItemData[i] != 0) then
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
				if (this.m_rucksackItemData[i] != 0) then
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
				if (this.m_rucksackItemData[i] != 0) then
					set result = result + this.m_rucksackItemData[i].charges()
				endif
				set i = i + 1
			endloop
			return result
		endmethod
		
		/// Removes the first found item with item type id @param itemTypeId.
		public method removeFromRucksackByTypeId takes integer itemTypeId, boolean drop returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.maxRucksackItems)
				if (this.m_rucksackItemData[i].itemTypeId() != itemTypeId) then
					call this.clearRucksackItem(i, drop)
					exitwhen (true)
				endif
				set i = i + 1
			endloop
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
			call super.enable()
			if (this.m_rucksackIsEnabled) then
				call this.enableRucksack()
			else
				call this.enableEquipment()
			endif

			/// @todo wait for calling methods above?
			call EnableTrigger(this.m_openTrigger)
			call EnableTrigger(this.m_orderTrigger)
			call EnableTrigger(this.m_pickupTrigger)
			call EnableTrigger(this.m_dropTrigger)
		endmethod

		/// Usually you do not have to call this method. The system handles itself.
		public method disable takes nothing returns nothing
			call super.disable()
			if (this.m_rucksackIsEnabled) then
				call this.disableRucksack()
			else
				call this.disableEquipment()
			endif

			/// @todo wait for calling methods above?
			call DisableTrigger(this.m_openTrigger)
			call DisableTrigger(this.m_orderTrigger)
			call DisableTrigger(this.m_pickupTrigger)
			call DisableTrigger(this.m_dropTrigger)
		endmethod

		/// @return Returns the slot of the equipped item. If no item was found it returns -1.
		public method hasItemEquipped takes integer itemTypeId returns integer
			local integer i = 0
			loop
				exitwhen (i == thistype.maxEquipmentTypes)
				if (this.m_equipmentItemData[i].itemTypeId() == itemTypeId) then
					return i
				endif
				set i = i + 1
			endloop
			return -1
		endmethod

		/// @return Returns the slot of the rucksack item. If not item was found it returns -1.
		public method hasItemTypeInRucksack takes integer itemTypeId returns integer
			local integer i = 0
			loop
				exitwhen (i == thistype.maxRucksackItems)
				if (this.m_rucksackItemData[i].itemTypeId() == itemTypeId) then
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
			debug call this.print("Cleaning up rucksack.")
			loop
				exitwhen (i < 0)
				set j = 0
				loop
					exitwhen (j == thistype.maxRucksackItems)
					if (i != j and this.m_rucksackItemData[i] != 0 and this.m_rucksackItemData[i].itemTypeId() == this.m_rucksackItemData[j].itemTypeId()) then
						call this.m_rucksackItemData[i].setCharges(this.m_rucksackItemData[i].charges() + this.m_rucksackItemData[j].charges())
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
				if (this.m_equipmentItemData[i] != 0) then
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
				if (this.m_equipmentItemData[i] != 0) then
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
			local string itemName
			if (not this.m_rucksackIsEnabled and UnitHasItem(characterUnit, usedItem)) then //already picked up
				call DisableTrigger(this.m_dropTrigger)
				call UnitDropItemPoint(characterUnit, usedItem, GetUnitX(characterUnit), GetUnitY(characterUnit))
				call EnableTrigger(this.m_dropTrigger)
			endif

			if (itemPlayer != this.user() and IsPlayerPlayingUser(itemPlayer)) then
				call this.character().displayMessage(ACharacter.messageTypeError, tr("Gegenstand gehört einem anderen Benutzer.")) /// @todo use static string member
				set itemPlayer = null
				return
			endif
			set itemPlayer = null
			
			if (itemType != 0 and equipmentType != -1) then
				if (swapWithAlreadyEquipped or this.m_equipmentItemData[equipmentType] == 0) then
					if (itemType.checkRequirement(this.character())) then
						if (swapWithAlreadyEquipped and this.m_equipmentItemData[equipmentType] != 0) then
							set equippedItem = this.m_equipmentItemData[equipmentType].createItem(GetUnitX(characterUnit), GetUnitY(characterUnit))
							call this.addItemToRucksack(equippedItem, true, false)
							call this.clearEquipmentItem(equipmentType, false)
							set equippedItem = null
						endif
						set itemName = GetItemName(usedItem)
						call this.setEquipmentItemByItem(equipmentType, usedItem, not this.m_rucksackIsEnabled)
						if (showEquipMessage) then
							call this.character().displayMessage(ACharacter.messageTypeInfo, StringArg(thistype.textEquipItem, itemName))
						endif
						set characterUnit = null
						return
					endif
				endif
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
			local string itemName
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
				if (this.m_rucksackItemData[i] == 0 or  this.m_rucksackItemData[i].itemTypeId() == GetItemTypeId(usedItem)) then
					set itemName = GetItemName(usedItem)
					call this.setRucksackItemByItem(i, usedItem, this.m_rucksackIsEnabled and this.itemRucksackPage(i) == this.m_rucksackPage)
					if (showAddMessage) then
						call this.character().displayMessage(ACharacter.messageTypeInfo, StringArg(thistype.textAddItemToRucksack, itemName))
					endif
					return
				endif
				set i = i + 1
			endloop

			//equip
			if (not dontMoveToEquipment) then
				call this.equipItem(usedItem, true, false, true)
			else
				call this.character().displayMessage(ACharacter.messageTypeError, thistype.textUnableToAddRucksackItem)
			endif
		endmethod
		
		private method setEquipmentItem takes integer equipmentType, AInventoryItemData inventoryItemData, boolean add returns nothing
			local AItemType itemType
			set this.m_equipmentItemData[equipmentType] = inventoryItemData
			if (add) then
				call this.showEquipmentItem(equipmentType)
			elseif (this.m_rucksackIsEnabled) then
				set itemType = AItemType.getItemTypeOfItemTypeId(inventoryItemData.itemTypeId())
				call itemType.addPermanentAbilities(this.unit())
			endif
		endmethod

		private method setEquipmentItemByItem takes integer equipmentType, item usedItem, boolean add returns nothing
			local AInventoryItemData inventoryItemData = AInventoryItemData.create(usedItem, this.unit())
			call DisableTrigger(this.m_dropTrigger)
			call RemoveItem(usedItem) 
			call EnableTrigger(this.m_dropTrigger)
			set usedItem = null
			call this.setEquipmentItem(equipmentType, inventoryItemData, add)
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
				set itemType = AItemType.getItemTypeOfItemTypeId(this.m_equipmentItemData[equipmentType].itemTypeId())
				call itemType.removePermanentAbilities(characterUnit)
			endif
			call this.m_equipmentItemData[equipmentType].destroy()
			set this.m_equipmentItemData[equipmentType] = 0
			call this.checkEquipment() //added
			set characterUnit = null
		endmethod

		private method showEquipmentItem takes integer equipmentType returns nothing
			local unit characterUnit = this.unit()
			local item slotItem
			local AItemType itemType = AItemType.getItemTypeOfItemTypeId(this.m_equipmentItemData[equipmentType].itemTypeId())
			call itemType.removePermanentAbilities(characterUnit)
			call DisableTrigger(this.m_pickupTrigger)
			call UnitAddItemToSlotById(characterUnit, this.m_equipmentItemData[equipmentType].itemTypeId(), equipmentType)
			call EnableTrigger(this.m_pickupTrigger)
			set slotItem = UnitItemInSlot(characterUnit, equipmentType)
			call this.m_equipmentItemData[equipmentType].assignToItem(slotItem)
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
			call itemType.addPermanentAbilities(characterUnit)
			set characterUnit = null
			set slotItem = null
		endmethod

		/**
		* Checks requirements of all equipped items. If some requirements aren't met the checked item is dropped.
		* This should be called whenever character units attributes which are used for item type requirement change.
		* Note: Now it should work while rucksack is opened, too.
		*/
		private method checkEquipment takes nothing returns nothing
			local AItemType itemType
			local integer i = 0
			loop
				exitwhen (i == thistype.maxEquipmentTypes)
				if (this.m_equipmentItemData[i] != 0) then
					set itemType = AItemType.getItemTypeOfItemTypeId(this.m_equipmentItemData[i].itemTypeId())
					if (not itemType.checkRequirement(this.character())) then
						call this.clearEquipmentItem(i, true)
					endif
				endif
				set i = i + 1
			endloop
		endmethod
		
		private method setRucksackItem takes integer index, AInventoryItemData inventoryItemData, boolean add returns nothing
			local boolean refreshOnly = false
			if (this.m_rucksackItemData[index] == 0) then
				set this.m_rucksackItemData[index] = inventoryItemData
			else //same type
				call this.m_rucksackItemData[index].setCharges(this.m_rucksackItemData[index].charges() + IMaxBJ(inventoryItemData.charges(), 1))
				call inventoryItemData.destroy()
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
			local AInventoryItemData inventoryItemData = AInventoryItemData.create(usedItem, this.unit())
			call DisableTrigger(this.m_dropTrigger)
			call RemoveItem(usedItem)
			call EnableTrigger(this.m_dropTrigger)
			set usedItem = null
			call this.setRucksackItem(index, inventoryItemData, add)
		endmethod

		public method clearRucksackItem takes integer index, boolean drop returns nothing
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
			call this.m_rucksackItemData[index].destroy()
			set this.m_rucksackItemData[index] = 0
		endmethod

		private method showRucksackItem takes integer index returns nothing
			local unit characterUnit = this.unit()
			local integer slot = this.rucksackItemSlot(index)
			local item slotItem
			local AItemType itemType = AItemType.getItemTypeOfItemTypeId(this.m_rucksackItemData[index].itemTypeId())
			call DisableTrigger(this.m_pickupTrigger)
			call UnitAddItemToSlotById(characterUnit, this.m_rucksackItemData[index].itemTypeId(), slot)
			call EnableTrigger(this.m_pickupTrigger)
			set slotItem = UnitItemInSlot(characterUnit, slot)
			call this.m_rucksackItemData[index].assignToItem(slotItem)
			call SetItemDropOnDeath(slotItem, false)
			call itemType.removePermanentAbilities(characterUnit)
			call thistype.setItemIndex(slotItem, index)
			set characterUnit = null
			set slotItem = null
		endmethod
		
		public method refreshRucksackItemCharges takes integer index returns nothing
			local unit characterUnit
			local integer slot
			local item slotItem
			if (this.m_rucksackPage != this.itemRucksackPage(index)) then
				return
			endif
			set characterUnit = this.unit()
			set slot = this.rucksackItemSlot(index)
			set slotItem = UnitItemInSlot(characterUnit, slot)
			call SetItemCharges(slotItem, this.m_rucksackItemData[index].charges())
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
				if (this.m_rucksackItemData[i] != 0) then
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
				if (this.m_rucksackItemData[i] != 0) then
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
			local AInventoryItemData currentItemData = 0
			local integer currentItemIndex
			local item otherItem = UnitItemInSlot(characterUnit, oldSlot)
			local AInventoryItemData otherItemData = 0
			local integer otherItemIndex
			call DisableTrigger(this.m_dropTrigger)
			if (currentItem != null) then
				set currentItemData = AInventoryItemData.create(currentItem, this.unit())
				if (currentItemData.itemTypeId() != thistype.leftArrowItemType and currentItemData.itemTypeId() != thistype.rightArrowItemType) then
					set currentItemIndex = thistype.itemIndex(currentItem)
				endif
				call RemoveItem(currentItem)
				set currentItem = null
			endif
			if (otherItem != null) then
				set otherItemData = AInventoryItemData.create(otherItem, this.unit())
				if (otherItemData.itemTypeId() != thistype.leftArrowItemType and otherItemData.itemTypeId() != thistype.rightArrowItemType) then
					set otherItemIndex = thistype.itemIndex(otherItem)
				endif
				call RemoveItem(otherItem)
				set otherItem = null
			endif
			call EnableTrigger(this.m_dropTrigger)
			call DisableTrigger(this.m_pickupTrigger)
			if (currentItemData != 0) then
				call UnitAddItemToSlotById(characterUnit, currentItemData.itemTypeId(), oldSlot)
				set currentItem = UnitItemInSlot(characterUnit, oldSlot)
				call currentItemData.assignToItem(currentItem)
				if (currentItemData.itemTypeId() != thistype.leftArrowItemType and currentItemData.itemTypeId() != thistype.rightArrowItemType) then
					call thistype.setItemIndex(currentItem, currentItemIndex)
				endif
				call currentItemData.destroy()
				set currentItem = null
			endif
			if (otherItemData != 0) then
				call UnitAddItemToSlotById(characterUnit, otherItemData.itemTypeId(), currentSlot)
				set otherItem = UnitItemInSlot(characterUnit, currentSlot)
				call otherItemData.assignToItem(otherItem)
				if (otherItemData.itemTypeId() != thistype.leftArrowItemType and otherItemData.itemTypeId() != thistype.rightArrowItemType) then
					call thistype.setItemIndex(otherItem, otherItemIndex)
				endif
				call otherItemData.destroy()
				set otherItem = null
			endif
			call EnableTrigger(this.m_pickupTrigger)
		endmethod

		private method moveRucksackItemToPage takes item slotItem, boolean next returns nothing
			local unit characterUnit = this.unit()
			local integer oldIndex = this.itemIndex(slotItem)
			local integer oldSlot = this.rucksackItemSlot(oldIndex)
			local integer i
			local integer exitValue
			
			debug call this.print("Moving item " + GetItemName(slotItem))
			
			//reset page item (items were swapped) and drop moved item
			call thistype.clearItemIndex(slotItem)
			call DisableTrigger(this.m_dropTrigger)
			call RemoveItem(slotItem)
			call EnableTrigger(this.m_dropTrigger)
			set slotItem = null
			
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
				endif
			endif
			
			loop
				//reached old index, remove dropped slot item and show it again. show error message.
				if (i == oldIndex) then
					//call RemoveItem(slotItem) //do not disable drop triggers, item is dropped
					call this.showRucksackItem(oldIndex)
					call this.character().displayMessage(ACharacter.messageTypeError, thistype.textUnableToMoveRucksackItem)
					exitwhen (true)
				//found stack place
				elseif (this.m_rucksackItemData[i].itemTypeId() == this.m_rucksackItemData[oldIndex].itemTypeId()) then
					call this.m_rucksackItemData[i].setCharges(this.m_rucksackItemData[i].charges() + 1)
					call this.clearRucksackItem(oldIndex, false)
					exitwhen (true)
				//found a free place
				elseif (this.m_rucksackItemData[i] == 0) then
					call this.setRucksackItem(i, this.m_rucksackItemData[oldIndex], this.m_rucksackIsEnabled and this.itemRucksackPage(i) == this.m_rucksackPage)
					set this.m_rucksackItemData[oldIndex] = 0
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
			local AInventoryItemData itemData = this.m_rucksackItemData[firstIndex]
			set this.m_rucksackItemData[firstIndex] = this.m_rucksackItemData[secondIndex]
			call thistype.setItemIndex(firstItem, secondIndex)
			set this.m_rucksackItemData[secondIndex] = itemData
			call thistype.setItemIndex(secondItem, firstIndex)
		endmethod

		private method moveRucksackItem takes item movedItem, integer slot returns nothing
			local unit characterUnit = this.unit()
			local item targetItem
			local integer oldIndex = this.itemIndex(movedItem)
			local integer newIndex = this.slotRucksackIndex(slot)
			//equip
			if (oldIndex == newIndex) then
				debug call this.print("Same index: Equip.")
				set movedItem = null
				debug call this.print("Creating item at characters position and trying to equip.")
				set movedItem = this.m_rucksackItemData[oldIndex].createItem(GetUnitX(characterUnit), GetUnitY(characterUnit))
				call SetItemCharges(movedItem, 0)
				if (this.m_rucksackItemData[oldIndex].charges() > 0) then
					call this.m_rucksackItemData[oldIndex].setCharges(this.m_rucksackItemData[oldIndex].charges() - 1)
					call this.refreshRucksackItemCharges(oldIndex)
				else
					call this.clearRucksackItem(oldIndex, false)
				endif
				call this.equipItem(movedItem, false, true, true) //test
				set characterUnit = null
				return
			endif
			set targetItem = UnitItemInSlot(this.unit(), this.rucksackItemSlot(oldIndex))
			//move
			if (targetItem == null) then
				set this.m_rucksackItemData[newIndex] = this.m_rucksackItemData[oldIndex]
				call thistype.setItemIndex(movedItem, newIndex)
				//destack
				if (this.m_rucksackItemData[oldIndex].isCharged()) then
					call this.m_rucksackItemData[oldIndex].setCharges(this.m_rucksackItemData[oldIndex].charges() - 1)
					if (this.m_rucksackItemData[oldIndex].itemType() == ITEM_TYPE_CHARGED) then
						call this.m_rucksackItemData[newIndex].setCharges(1)
					else
						call this.m_rucksackItemData[newIndex].setCharges(0)
					endif
					call this.showRucksackItem(oldIndex)
					call this.refreshRucksackItemCharges(newIndex)
				//normal movement
				else
					call this.m_rucksackItemData[newIndex].setCharges(this.m_rucksackItemData[oldIndex].charges())
					//clear old, do not destroy since data was moved to new index!
					set this.m_rucksackItemData[oldIndex] = 0
				endif
				call RemoveItem(targetItem)
				set targetItem = null
			//stack
			elseif (GetItemTypeId(movedItem) == GetItemTypeId(targetItem)) then
				call thistype.setItemIndex(movedItem, newIndex)
				call this.m_rucksackItemData[newIndex].setCharges(this.m_rucksackItemData[newIndex].charges() + IMaxBJ(1, this.m_rucksackItemData[oldIndex].charges()))
				call this.refreshRucksackItemCharges(newIndex)
				call this.clearRucksackItem(oldIndex, false)
			//swap
			else
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
			return GetIssuedOrderId() >= 852002 and GetIssuedOrderId() <= 852007
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
			if (this.m_rucksackIsEnabled) then
				debug call this.print("Rucksack is enabled.")
				if (GetItemTypeId(usedItem) == thistype.leftArrowItemType and newSlot != thistype.maxRucksackItemsPerPage) then
					call this.resetItemSlots(newSlot, AInventory.maxRucksackItemsPerPage)
				elseif (GetItemTypeId(usedItem) == thistype.rightArrowItemType and newSlot != thistype.maxRucksackItemsPerPage + 1) then
					call this.resetItemSlots(newSlot, thistype.maxRucksackItemsPerPage + 1)
				//move item previous - player drops an item on the next page item
				elseif (GetItemTypeId(usedItem) != thistype.leftArrowItemType and newSlot == thistype.maxRucksackItemsPerPage) then
					call this.moveRucksackItemToPage(usedItem, false)
				//move item next - player drops an item on the previous page item
				elseif (GetItemTypeId(usedItem) != thistype.rightArrowItemType and newSlot == thistype.maxRucksackItemsPerPage + 1) then
					call this.moveRucksackItemToPage(usedItem, true)
				//equip item/stack items/swap items
				elseif (newSlot >= 0 and newSlot < thistype.maxRucksackItemsPerPage) then
					call this.moveRucksackItem(usedItem, newSlot)
				endif
			//equipment is enabled
			else
				set oldSlot = thistype.itemIndex(usedItem)
				//reset moved equipped items to their positions
				if (newSlot != oldSlot) then
					call this.resetItemSlots(newSlot, oldSlot)
				//old slot, add to rucksack
				else
					call this.clearItemIndex(usedItem)
					call this.clearEquipmentItem(oldSlot, true)
					call this.addItemToRucksack(usedItem, true, true)
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
				//page items
				if (GetItemTypeId(usedItem) == thistype.leftArrowItemType) then
					call TriggerSleepAction(0.0)
					call RemoveItem(usedItem)
					set usedItem = null
					call DisableTrigger(this.m_pickupTrigger)
					call UnitAddItemToSlotById(this.unit(), thistype.leftArrowItemType, thistype.maxRucksackItemsPerPage)
					call EnableTrigger(this.m_pickupTrigger)
					call this.character().displayMessage(ACharacter.messageTypeError, thistype.textDropPageItem)
				elseif (GetItemTypeId(usedItem) == thistype.rightArrowItemType) then
					call RemoveItem(usedItem)
					set usedItem = null
					call DisableTrigger(this.m_pickupTrigger)
					call UnitAddItemToSlotById(this.unit(), thistype.rightArrowItemType, thistype.maxRucksackItemsPerPage + 1)
					call EnableTrigger(this.m_pickupTrigger)
					set usedItem = UnitItemInSlot(this.unit(), thistype.maxRucksackItemsPerPage + 1)
					call SetItemCharges(usedItem, this.m_rucksackPage)
					set usedItem = null
					call this.character().displayMessage(ACharacter.messageTypeError, thistype.textDropPageItem)
				//destack and drop
				elseif (this.m_rucksackItemData[index].isCharged()) then
					call TriggerSleepAction(0.0) //wait until it has been dropped
					call this.m_rucksackItemData[index].setCharges(this.m_rucksackItemData[index].charges() - 1)
					call this.showRucksackItem(index)
					
					if (GetItemType(usedItem) == ITEM_TYPE_CHARGED) then
						call SetItemCharges(usedItem, 1)
					else
						call SetItemCharges(usedItem, 0)
					endif
				//drop
				else
					call this.clearRucksackItem(index, true) //before drop
					/// @todo Stop unit?
				endif
			//unequip and drop
			else
				call this.clearEquipmentItem(index, true)
				/// @todo Stop unit?
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
				else
					set index = thistype.itemIndex(usedItem)
					//if an item is used by decreasing its number of charges (not to 0!) we have to decrease our number, too
					if (this.m_rucksackItemData[index].isCharged()) then
						call this.m_rucksackItemData[index].setCharges(this.m_rucksackItemData[index].charges() - 1)
						call this.refreshRucksackItemCharges(index)
					//use == drop
					/// Drop action is called when last charge is used!!!
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

			if (GetUnitAbilityLevel(this.unit(), thistype.openRucksackAbilityId) == 0) then
				call UnitAddAbility(this.unit(), thistype.openRucksackAbilityId)
			endif

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
			local integer i = 0
			loop
				exitwhen (i == thistype.maxEquipmentTypes)
				if (this.m_equipmentItemData[i] != 0) then
					call this.m_equipmentItemData[i].destroy()
				endif
				set i = i + 1
			endloop
			set i = 0
			loop
				exitwhen (i == thistype.maxRucksackItems)
				if (this.m_rucksackItemData[i] != 0) then
					call this.m_rucksackItemData[i].destroy()
				endif
				set i = i + 1
			endloop
			call UnitRemoveAbility(this.unit(), thistype.openRucksackAbilityId)

			call this.destroyOpenTrigger()
			call this.destroyOrderTrigger()
			call this.destroyPickupTrigger()
			call this.destroyDropTrigger()
			call this.destroyUseTrigger()
		endmethod

		public static method init takes integer leftArrowItemType, integer rightArrowItemType, integer openRucksackAbilityId, string textUnableToEquipItem, string textEquipItem, string textUnableToAddRucksackItem, string textAddItemToRucksack, string textUnableToMoveRucksackItem, string textDropPageItem returns nothing
			//static start members
			set thistype.leftArrowItemType = leftArrowItemType
			set thistype.rightArrowItemType = rightArrowItemType
			set thistype.openRucksackAbilityId = openRucksackAbilityId
			set thistype.textUnableToEquipItem = textUnableToEquipItem
			set thistype.textEquipItem = textEquipItem
			set thistype.textUnableToAddRucksackItem = textUnableToAddRucksackItem
			set thistype.textAddItemToRucksack = textAddItemToRucksack
			set thistype.textUnableToMoveRucksackItem = textUnableToMoveRucksackItem
			set thistype.textDropPageItem = textDropPageItem
		endmethod
		
		/// @return Returns the page of a rucksack item by index.
		public static method itemRucksackPage takes integer index returns integer
			debug if (index >= thistype.maxRucksackItems or index < 0) then
				debug call thistype.staticPrint("Wrong rucksack index: " + I2S(index) + ".")
				debug return 0
			debug endif
			return index / thistype.maxRucksackItemsPerPage
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