/// @todo Wenn der Charakter einen Gegenstand ablegt, muss überprüft werden, ob die anderen noch getragen werden können (Attribute).
library AStructSystemsCharacterInventory requires AStructSystemsCharacterCharacterHashTable, AStructSystemsCharacterAbstractCharacterSystem, AStructSystemsCharacterCharacter, AStructSystemsCharacterItemType

	struct AInventory extends AAbstractCharacterSystem
		//static constant members, useful for GUIs
		public static constant integer maxEquipmentTypes = 4//AItemType.maxEuqipmentTypes /// @todo vJass bug //AClassCharacterItemType
		public static constant integer maxRucksackItems = 16
		public static constant integer maxRucksackPages = 4 //16 / 4
		public static constant integer maxRucksackItemsPerPage = 4
		//static start members
		private static integer leftArrowItemType
		private static integer rightArrowItemType
		private static integer openRucksackAbilityId
		private static string textUnableToEquipItem
		private static string textUnableToAddRucksackItem
		private static string textUnableToMoveRucksackItem
		//members
		private integer array equipmentItemType[4] /// @todo vJass bug AInventory.maxEquipmentTypes
		private integer array rucksackItemType[AInventory.maxRucksackItems]
		private integer array rucksackItemCharges[AInventory.maxRucksackItems]
		private trigger openTrigger
		private trigger orderTrigger
		private trigger useTrigger //show next page, show previous page, disable in equipment
		private trigger pickupTrigger //pickup
		private trigger dropTrigger //drop
		private integer rucksackPage
		private boolean rucksackIsEnabled
		
		//! runtextmacro A_STRUCT_DEBUG("\"AInventory\"")

		//members

		public method getEquipmentItemType takes integer equipmentType returns integer
			return this.equipmentItemType[equipmentType]
		endmethod

		public method getRucksackItemType takes integer slot returns integer
			return this.rucksackItemType[slot]
		endmethod

		public method getRucksackItemCharges takes integer slot returns integer
			return this.rucksackItemCharges[slot]
		endmethod

		//methods
		
		public method getRucksackItemSlot takes integer index returns integer
			return index - this.rucksackPage * AInventory.maxRucksackItemsPerPage
		endmethod

		public method getSlotRucksackIndex takes integer slot returns integer
			return this.rucksackPage * AInventory.maxRucksackItemsPerPage + slot
		endmethod

		/// Shows the current page in the inventory of the character's unit
		/// In general you do not have to call this method. The system handles itself.
		public method enable takes nothing returns nothing
			if (this.rucksackIsEnabled) then
				call this.enableRucksack()
			else
				call this.enableEquipment()
			endif

			call EnableTrigger(this.openTrigger)
			call EnableTrigger(this.orderTrigger)
			call EnableTrigger(this.pickupTrigger)
			call EnableTrigger(this.dropTrigger)
		endmethod

		/// Usually you do not have to call this method. The system handles itself.
		public method disable takes nothing returns nothing
			if (this.rucksackIsEnabled) then
				call this.disableRucksack()
			else
				call this.disableEquipment()
			endif

			call DisableTrigger(this.openTrigger)
			call DisableTrigger(this.orderTrigger)
			call DisableTrigger(this.pickupTrigger)
			call DisableTrigger(this.dropTrigger)
		endmethod

		/// @return Returns the slot of the equipped item. If no item was found it returns -1.
		public method hasItemEquipped takes integer itemType returns integer
			local integer i = 0
			loop
				exitwhen (i == AInventory.maxEquipmentTypes)
				if (this.equipmentItemType[i] == itemType) then
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
				exitwhen (i == AInventory.maxRucksackItems)
				if (this.rucksackItemType[i] == itemType) then
					return i
				endif
				set i = i + 1
			endloop
			return -1
		endmethod

		private method enableEquipment takes nothing returns nothing
			local integer i
			set i = 0
			loop
				exitwhen (i == AInventory.maxEquipmentTypes)
				if (this.equipmentItemType[i] != 0) then
					call this.showEquipmentItem(i)
				endif
				set i = i + 1
			endloop
		endmethod

		private method disableEquipment takes nothing returns nothing
			local integer i
			set i = 0
			loop
				exitwhen (i == AInventory.maxEquipmentTypes)
				if (this.equipmentItemType[i] != 0) then
					call this.hideEquipmentItem(i)
				endif
				set i = i + 1
			endloop
		endmethod

		private method enableRucksack takes nothing returns nothing
			local item leftArrowItem
			local item rightArrowItem
			set this.rucksackIsEnabled = true
			call DisableTrigger(this.pickupTrigger)
			call UnitAddItemToSlotById(this.getUnit(), AInventory.leftArrowItemType, AInventory.maxRucksackItemsPerPage)
			call UnitAddItemToSlotById(this.getUnit(), AInventory.rightArrowItemType, AInventory.maxRucksackItemsPerPage + 1)
			call EnableTrigger(this.pickupTrigger)
			set leftArrowItem = UnitItemInSlot(this.getUnit(), AInventory.maxRucksackItemsPerPage)
			set rightArrowItem = UnitItemInSlot(this.getUnit(), AInventory.maxRucksackItemsPerPage + 1)
			call SetItemDroppable(leftArrowItem, false)
			call SetItemDroppable(rightArrowItem, false)
			call SetItemCharges(rightArrowItem, this.rucksackPage)
			set leftArrowItem = null
			set rightArrowItem = null
			call this.showRucksackPage(this.rucksackPage, true)
			call EnableTrigger(this.useTrigger) //just required in rucksack
		endmethod

		private method disableRucksack takes nothing returns nothing
			local item leftArrowItem
			local item rightArrowItem
			set this.rucksackIsEnabled = false
			set leftArrowItem = UnitItemInSlot(this.getUnit(), AInventory.maxRucksackItemsPerPage)
			set rightArrowItem = UnitItemInSlot(this.getUnit(), AInventory.maxRucksackItemsPerPage + 1)
			call DisableTrigger(this.dropTrigger)
			call RemoveItem(leftArrowItem)
			call RemoveItem(rightArrowItem)
			call EnableTrigger(this.dropTrigger)
			set leftArrowItem = null
			set rightArrowItem = null
			call this.hideCurrentRucksackPage()
			call DisableTrigger(this.useTrigger) //just required in rucksack
		endmethod

		private method equipItem takes item usedItem, boolean fromRucksack returns nothing
			local AItemType itemType = AItemType.getItemTypeOfItem(usedItem)
			local integer equipmentType = itemType.getEquipmentType()
			local unit usedUnit = this.getUnit()
			if (not this.rucksackIsEnabled and UnitHasItem(usedUnit, usedItem)) then //already picked up
				call DisableTrigger(this.dropTrigger)
				debug call this.print("Drop item because it was already picked up.")
				call UnitDropItemPoint(usedUnit, usedItem, GetUnitX(usedUnit), GetUnitY(usedUnit))
				call EnableTrigger(this.dropTrigger)
			endif
			set usedUnit = null
			debug call this.print("Equipment type is " + I2S(equipmentType) + ".")
			if (itemType != 0 and this.equipmentItemType[equipmentType] == 0) then
				debug call this.print("Item type is not 0 and equipment slot is free.")
				if (itemType.checkRequirement(this.getCharacter())) then
					debug call this.print("Requirement is okay.")
					call this.setEquipmentItem(usedItem, itemType, not this.rucksackIsEnabled)
					return
				endif
			endif
			//move to rucksack
			if (not fromRucksack) then
				debug call this.print("Item is not from rucksack and will be added to rucksack.")
				call this.addItemToRucksack(usedItem, true) //if item type is 0 it will be placed in rucksack, too
			else
				call this.getCharacter().displayMessage(ACharacter.messageTypeError, AInventory.textUnableToEquipItem)
			endif
		endmethod

		private method addItemToRucksack takes item usedItem, boolean fromEquipment returns nothing
			local integer i
			local unit usedUnit = this.getUnit()
			if (this.rucksackIsEnabled and UnitHasItem(usedUnit, usedItem)) then //already picked up
				call DisableTrigger(this.dropTrigger)
				debug call this.print("Drop item from rucksack because it was already placed in")
				call UnitDropItemPoint(usedUnit, usedItem, GetUnitX(usedUnit), GetUnitY(usedUnit))
				call EnableTrigger(this.dropTrigger)
			endif
			set usedUnit = null
			set i = 0
			loop
				exitwhen (i == AInventory.maxRucksackItems)
				if (this.rucksackItemType[i] == 0 or  this.rucksackItemType[i] == GetItemTypeId(usedItem)) then
					debug call this.print("Rucksack slot " + I2S(i) + " is free or has the same item type and will be used.")
					call this.setRucksackItem(usedItem, i, (this.rucksackIsEnabled and this.getItemRucksackPage(i) == this.rucksackPage))
					return
				endif
				set i = i + 1
			endloop

			//equip
			if (not fromEquipment) then
				debug call this.print("Item is not from equipment and will be equipped.")
				call this.equipItem(usedItem, true)
			else
				call this.getCharacter().displayMessage(ACharacter.messageTypeError, AInventory.textUnableToAddRucksackItem) //AClassCharacterCharacter
			endif
		endmethod

		private method getItemRucksackPage takes integer index returns integer
			local integer i = 0
			loop
				exitwhen (i == AInventory.maxRucksackPages)
				if (index < i + AInventory.maxRucksackItemsPerPage) then
					return i
				endif
				set i = i + 1
			endloop
			return -1
		endmethod

		/// Just required for the move order and for item dropping.
		private method getItemIndex takes item usedItem returns integer
			return AGetCharacterHashTable().getHandleInteger(usedItem, "AInventory_index") //AClassCharacterCharacterHashTable
		endmethod

		private method setEquipmentItem takes item usedItem, integer equipmentType, boolean add returns nothing
			local item slotItem
			set this.equipmentItemType[equipmentType] = GetItemTypeId(usedItem)
			debug call this.print("Has set item type id " + I2S(GetItemTypeId(usedItem)))
			call DisableTrigger(this.dropTrigger)
			call RemoveItem(usedItem) 
			call EnableTrigger(this.dropTrigger)
			set usedItem = null
			debug call this.print("After item removal")
			if (add) then
				debug call Print("Add equipment item, rucksack is not enabled.")
				call this.showEquipmentItem(equipmentType)
				debug call this.print("Equipment item was added")
			endif
		endmethod

		private method clearEquipmentItem takes integer equipmentType, boolean drop returns nothing
			local item slotItem = UnitItemInSlot(this.getUnit(), equipmentType)
			call AGetCharacterHashTable().flushHandleInteger(slotItem, "AInventory_index") //AClassCharacterCharacterHashTable
			set this.equipmentItemType[equipmentType] = 0
			call DisableTrigger(this.dropTrigger)
			if (drop) then
				call UnitDropItemPoint(this.getUnit(), slotItem, GetUnitX(this.getUnit()), GetUnitY(this.getUnit()))
			else
				call RemoveItem(slotItem)
			endif
			call EnableTrigger(this.dropTrigger)
			set slotItem = null
			call this.checkEquipment() //added
		endmethod

		private method showEquipmentItem takes integer equipmentType returns nothing
			local item slotItem
			local AItemType itemType = AItemType.getItemTypeOfItemTypeId(this.equipmentItemType[equipmentType])
			call itemType.removeAbilities(this.getUnit())
			call DisableTrigger(this.pickupTrigger)
			call UnitAddItemToSlotById(this.getUnit(), this.equipmentItemType[equipmentType], equipmentType)
			call EnableTrigger(this.pickupTrigger)
			set slotItem = UnitItemInSlot(this.getUnit(), equipmentType)
			call SetItemDropOnDeath(slotItem, false)
			call AGetCharacterHashTable().storeHandleInteger(slotItem, "AInventory_index", equipmentType) //AClassCharacterCharacterHashTable
			set slotItem = null
		endmethod

		private method hideEquipmentItem takes integer equipmentType returns nothing
			local item slotItem = UnitItemInSlot(this.getUnit(), equipmentType)
			local AItemType itemType = AItemType.getItemTypeOfItem(slotItem)
			call DisableTrigger(this.dropTrigger)
			call RemoveItem(slotItem)
			call EnableTrigger(this.dropTrigger)
			call itemType.addAbilities(this.getUnit())
			set slotItem = null
		endmethod

		private method checkEquipment takes nothing returns nothing
			local item slotItem
			local AItemType itemType
			local integer i = 0
			loop
				exitwhen (i == AInventory.maxEquipmentTypes)
				if (this.equipmentItemType[i] != 0) then
					set slotItem = UnitItemInSlot(this.getUnit(), i)
					set itemType = AItemType.getItemTypeOfItem(slotItem) //AClassCharacterItemType
					debug if (itemType == 0) then
						debug call Print("AItemType of item " + GetItemName(slotItem) + " in instance " + I2S(this) + " and slot " + I2S(i) + " is 0.")
					debug endif
					if (not itemType.checkRequirement(this.getCharacter())) then
						call this.clearEquipmentItem(i, true)
					endif
					set slotItem = null
				endif
				set i = i + 1
			endloop
		endmethod

		private method setRucksackItem takes item usedItem, integer index, boolean add returns nothing
			local integer slot
			if (this.rucksackItemType[index] == 0) then
				set this.rucksackItemType[index] = GetItemTypeId(usedItem)
				set this.rucksackItemCharges[index] = GetItemCharges(usedItem)
			else //same type
				set this.rucksackItemCharges[index] = this.rucksackItemCharges[index] + IMaxBJ(GetItemCharges(usedItem), 1)
			endif
			call DisableTrigger(this.dropTrigger)
			call RemoveItem(usedItem)
			call EnableTrigger(this.dropTrigger)
			set usedItem = null
			if (add) then
				debug call this.print("Add - must be in rucksack.")
				if (this.rucksackItemType[index] == 0) then
					call this.showRucksackItem(index)
				else
					call this.refreshRucksackItemCharges(index)
				endif
			endif
		endmethod

		private method clearRucksackItem takes integer index, boolean drop returns nothing
			local item slotItem = UnitItemInSlot(this.getUnit(), this.getRucksackItemSlot(index))
			call AGetCharacterHashTable().flushHandleInteger(slotItem, "AInventory_index") //AClassCharacterCharacterHashTable
			set this.rucksackItemType[index] = 0
			set this.rucksackItemCharges[index] = 0
			call DisableTrigger(this.dropTrigger)
			if (drop) then
				call UnitDropItemPoint(this.getUnit(), slotItem, GetUnitX(this.getUnit()), GetUnitY(this.getUnit()))
			else
				call RemoveItem(slotItem)
			endif
			call EnableTrigger(this.dropTrigger)
			set slotItem = null
		endmethod

		private method showRucksackItem takes integer index returns nothing
			local integer slot = this.getRucksackItemSlot(index)
			local item slotItem
			call DisableTrigger(this.pickupTrigger)
			call UnitAddItemToSlotById(this.getUnit(), this.rucksackItemType[index], slot)
			call EnableTrigger(this.pickupTrigger)
			set slotItem = UnitItemInSlot(this.getUnit(), slot)
			call SetItemDropOnDeath(slotItem, false)
			call SetItemCharges(slotItem, this.rucksackItemCharges[index])
			call AGetCharacterHashTable().storeHandleInteger(slotItem, "AInventory_index", index) //AClassCharacterCharacterHashTable
			set slotItem = null
		endmethod
		
		private method refreshRucksackItemCharges takes integer index returns nothing
			local item slotItem = UnitItemInSlot(this.getUnit(), index)
			call SetItemCharges(slotItem, this.rucksackItemCharges[index])
			set slotItem = null
		endmethod

		private method hideRucksackItem takes integer index returns nothing
			local integer slot = this.getRucksackItemSlot(index)
			local item slotItem = UnitItemInSlot(this.getUnit(), slot)
			call DisableTrigger(this.dropTrigger)
			call RemoveItem(slotItem)
			call EnableTrigger(this.dropTrigger)
			set slotItem = null
		endmethod

		private method showRucksackPage takes integer page, boolean firstCall returns nothing
			local integer i
			local integer exitValue
			debug if (page > AInventory.maxRucksackPages) then
				debug call Print("AInventory: Page value is too high.")
				debug return
			debug endif

			set this.rucksackPage = page

			if (not firstCall) then
				call this.hideCurrentRucksackPage()
			endif

			//add inventory items
			set i = page * AInventory.maxRucksackItemsPerPage
			set exitValue = i + AInventory.maxRucksackItemsPerPage
			loop
				exitwhen (i == exitValue)
				if (this.rucksackItemType[i] != 0) then
					call this.showRucksackItem(i)
				endif
				set i = i + 1
			endloop
		endmethod

		private method hideCurrentRucksackPage takes nothing returns nothing
			local integer i = this.rucksackPage * AInventory.maxRucksackItemsPerPage
			local integer exitValue = i + AInventory.maxRucksackItemsPerPage
			loop
				exitwhen (i == exitValue)
				if (this.rucksackItemType[i] != 0) then
					call this.hideRucksackItem(i)
				endif
				set i = i + 1
			endloop
		endmethod

		private method showNextRucksackPage takes nothing returns nothing
			if (this.rucksackPage == AInventory.maxRucksackPages) then
				call this.showRucksackPage(this.rucksackPage + 1, false)
			else
				call this.showRucksackPage(0, false)
			endif
		endmethod

		private method showPreviousRucksackPage takes nothing returns nothing
			if (this.rucksackPage == 0) then
				call this.showRucksackPage(AInventory.maxRucksackPages, false)
			else
				call this.showRucksackPage(this.rucksackPage - 1, false)
			endif
		endmethod

		private method moveRucksackItemToPage takes integer slot, boolean next returns nothing
			local integer i
			local integer exitValue
			local integer j
			local integer index
			local item slotItem
			local boolean result = false
			set i = this.rucksackPage

			if (next) then
				set exitValue = AInventory.maxRucksackPages
			else
				set exitValue = 0
			endif
			loop
				exitwhen (i == exitValue or result)
					set j = 0
					loop
						exitwhen (j == AInventory.maxRucksackItemsPerPage)
						set index = i * j
						if (this.rucksackItemType[index] == 0) then
							set slotItem = UnitItemInSlot(this.getUnit(), slot)
							call this.addItemToRucksack(slotItem, false)
							set slotItem = null
							set result = true
							exitwhen (true)
						endif
						set j = j + 1
					endloop
				if (next) then
					set i = i + 1
				else
					set i = i - 1
				endif
			endloop

			if (not result) then
				call this.getCharacter().displayMessage(ACharacter.messageTypeError, AInventory.textUnableToMoveRucksackItem) //AClassCharacterCharacter
			endif
		endmethod

		private method swapRucksackItemData takes integer firstIndex, integer secondIndex returns nothing
			local integer itemType = this.rucksackItemType[firstIndex]
			local integer itemCharges = this.rucksackItemCharges[firstIndex]
			set this.rucksackItemType[firstIndex] = this.rucksackItemType[secondIndex]
			set this.rucksackItemCharges[firstIndex] = this.rucksackItemCharges[secondIndex]
			set this.rucksackItemType[secondIndex] = itemType
			set this.rucksackItemCharges[secondIndex] = itemCharges
		endmethod

		private method moveRucksackItem takes item movedItem, integer slot returns nothing
			local item targetItem
			local integer index0 = this.getItemIndex(movedItem)
			local integer index1 = this.getSlotRucksackIndex(slot)

			//equip
			if (index0 == index1) then
				call this.equipItem(movedItem, true) //test
				call this.clearRucksackItem(index0, false)
				return
			endif

			//stack
			set targetItem = UnitItemInSlot(this.getUnit(), slot)
			if (GetItemTypeId(movedItem) == GetItemTypeId(targetItem)) then
				set this.rucksackItemCharges[index1] = this.rucksackItemCharges[index1] + this.rucksackItemCharges[index0]
				call SetItemCharges(targetItem, this.rucksackItemCharges[index1])
				call this.clearRucksackItem(index0, false)
				set targetItem = null
				return
			endif
			set targetItem = null

			//swap
			call this.swapRucksackItemData(index0, index1)
		endmethod

		private static method triggerConditionOpen takes nothing returns boolean
			return GetSpellAbilityId() == AInventory.openRucksackAbilityId
		endmethod

		private static method triggerActionOpen takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AInventory inventory = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "inventory") //AClassCharacterCharacterHashTable
			debug call inventory.print("Open.")
			if (inventory.rucksackIsEnabled) then
				call inventory.disableRucksack()
				call inventory.enableEquipment()
			else
				call inventory.disableEquipment()
				call inventory.enableRucksack()
			endif
			set triggeringTrigger = null
		endmethod

		private method createOpenTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.openTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.openTrigger, this.getUnit(), EVENT_UNIT_SPELL_CAST)
			set conditionFunction = Condition(function AInventory.triggerConditionOpen)
			set triggerCondition = TriggerAddCondition(this.openTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.openTrigger, function AInventory.triggerActionOpen)
			call AGetCharacterHashTable().storeHandleInteger(this.openTrigger, "inventory", this) //AClassCharacterCharacterHashTable
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
			local AInventory this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this") //AClassCharacterCharacterHashTable
			local integer order = GetIssuedOrderId() - 852002
			local item usedItem = GetManipulatedItem()
			local integer oldSlot
			local item swappedItem
			local integer itemTypeId
			debug call this.print("Order " + I2S(order) + ".")
			if (this.rucksackIsEnabled) then
				// move item next - player drops an item on the next page item
				if (order == AInventory.maxRucksackItemsPerPage) then
					call this.moveRucksackItemToPage(order, true)
					debug call this.print("Move item to next page.")
				// move item previous - player drops an item on the previous page item
				elseif (order == AInventory.maxRucksackItemsPerPage + 1) then
					call this.moveRucksackItemToPage(order, false)
					debug call this.print("Move item to previous page.")
				// equip item/stack items/swap items
				elseif (order >= 0 and order < AInventory.maxRucksackItemsPerPage) then
					call this.moveRucksackItem(usedItem, order)
					debug call this.print("Move item to slot " + I2S(order) + ".")
				endif
			else
				//unequip?
				//drop the item on its own slot
				//if you want to swap equipment items it is not possible!
				set oldSlot = this.getItemIndex(usedItem)
				set itemTypeId = GetItemTypeId(usedItem)
				call RemoveItem(usedItem)
				set swappedItem = UnitItemInSlot(this.getUnit(), oldSlot)
				if (swappedItem != null) then
					call UnitAddItemToSlotById(this.getUnit(), GetItemTypeId(swappedItem), order)
					call RemoveItem(swappedItem)
					set swappedItem = null
				endif
				call UnitAddItemToSlotById(this.getUnit(), itemTypeId, oldSlot)
			endif
			set triggeringTrigger = null
			set usedItem = null
		endmethod

		private method createOrderTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.orderTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.orderTrigger, this.getUnit(), EVENT_UNIT_ISSUED_ORDER)
			set conditionFunction = Condition(function AInventory.triggerConditionOrder)
			set triggerCondition = TriggerAddCondition(this.orderTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.orderTrigger, function AInventory.triggerActionOrder)
			call AGetCharacterHashTable().storeHandleInteger(this.orderTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
			//add, drop, move item next, move item previous, stack items
		endmethod
		
		private static method triggerConditionIsNoPowerup takes nothing returns boolean
			local item usedItem = GetManipulatedItem()
			local boolean result = not IsItemIdPowerup(GetItemTypeId(usedItem))
			set usedItem = null
			return result
		endmethod

		private static method triggerActionPickup takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AInventory this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this")
			local item usedItem = GetManipulatedItem()
			debug call Print("Pickup item " + GetItemName(usedItem))
			if (this.rucksackIsEnabled) then
				call this.addItemToRucksack(usedItem, false)
			else
				call this.equipItem(usedItem, false)
			endif
			set triggeringTrigger = null
			set usedItem = null
		endmethod

		private method createPickupTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.pickupTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.pickupTrigger, this.getUnit(), EVENT_UNIT_PICKUP_ITEM) //pawn?
			set conditionFunction = Condition(function AInventory.triggerConditionIsNoPowerup)
			set triggerCondition = TriggerAddCondition(this.pickupTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.pickupTrigger, function AInventory.triggerActionPickup)
			call AGetCharacterHashTable().storeHandleInteger(this.pickupTrigger, "this", this) //AClassCharacterCharacterHashTable
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		private static method triggerActionDrop takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AInventory this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this")
			local item usedItem = GetManipulatedItem()
			debug call this.print("Drop")
			if (this.rucksackIsEnabled) then
				call this.clearRucksackItem(this.getItemIndex(usedItem), true)
			else
				call this.clearEquipmentItem(this.getItemIndex(usedItem), true)
			endif
			set triggeringTrigger = null
			set usedItem = null
		endmethod

		private method createDropTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.dropTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.dropTrigger, this.getUnit(), EVENT_UNIT_DROP_ITEM)
			set conditionFunction = Condition(function AInventory.triggerConditionIsNoPowerup)
			set triggerCondition = TriggerAddCondition(this.dropTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.dropTrigger, function AInventory.triggerActionDrop)
			call AGetCharacterHashTable().storeHandleInteger(this.dropTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		private static method triggerActionUse takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AInventory this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this") //AClassCharacterCharacterHashTable
			local item usedItem = GetManipulatedItem()
			local integer itemTypeId = GetItemTypeId(usedItem)
			debug call this.print("Use page item")
			if (this.rucksackIsEnabled) then
				//show next page
				if (itemTypeId == AInventory.rightArrowItemType) then
					call this.showNextRucksackPage()
				//show previous page
				elseif (itemTypeId == AInventory.leftArrowItemType) then
					call this.showPreviousRucksackPage()
				endif
			endif
			set triggeringTrigger = null
			set usedItem = null
		endmethod

		private method createUseTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.useTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.useTrigger, this.getUnit(), EVENT_UNIT_USE_ITEM)
			set triggerAction = TriggerAddAction(this.useTrigger, function AInventory.triggerActionUse)
			call AGetCharacterHashTable().storeHandleInteger(this.useTrigger, "this", this) //AClassCharacterCharacterHashTable
			set triggerEvent = null
			set triggerAction = null
		endmethod

		public static method create takes ACharacter character returns AInventory
			local AInventory this = AInventory.allocate(character)
			//members
			set this.rucksackPage = 0
			set this.rucksackIsEnabled = false

			call UnitAddAbility(this.getUnit(), AInventory.openRucksackAbilityId)

			call this.createOpenTrigger()
			call this.createOrderTrigger()
			call this.createPickupTrigger()
			call this.createDropTrigger()
			call this.createUseTrigger()
			return this
		endmethod

		private method destroyOpenTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.openTrigger)
			set this.openTrigger = null
		endmethod

		private method destroyOrderTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.orderTrigger)
			set this.orderTrigger = null
		endmethod

		private method destroyPickupTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.pickupTrigger)
			set this.pickupTrigger = null
		endmethod

		private method destroyDropTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.dropTrigger)
			set this.dropTrigger = null
		endmethod

		private method destroyUseTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.useTrigger)
			set this.useTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing
			call UnitRemoveAbility(this.getUnit(), AInventory.openRucksackAbilityId)

			call this.destroyOpenTrigger()
			call this.destroyOrderTrigger()
			call this.destroyPickupTrigger()
			call this.destroyDropTrigger()
			call this.destroyUseTrigger()
		endmethod

		public static method init takes integer leftArrowItemType, integer rightArrowItemType, integer openRucksackAbilityId, string textUnableToEquipItem, string textUnableToAddRucksackItem, string textUnableToMoveRucksackItem returns nothing
			//static start members
			set AInventory.leftArrowItemType = leftArrowItemType
			set AInventory.rightArrowItemType = rightArrowItemType
			set AInventory.openRucksackAbilityId = openRucksackAbilityId
			set AInventory.textUnableToEquipItem = textUnableToEquipItem
			set AInventory.textUnableToAddRucksackItem = textUnableToAddRucksackItem
			set AInventory.textUnableToMoveRucksackItem = textUnableToMoveRucksackItem
		endmethod
	endstruct

endlibrary