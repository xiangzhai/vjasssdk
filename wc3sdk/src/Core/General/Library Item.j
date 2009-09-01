library ALibraryCoreGeneralItem

	/// Creates a copy of item @param usedItem at position with coordinates @param x and @param y.
	/// @author Tamino Dauth
	function CopyItem takes item usedItem, real x, real y returns item
		local item result = CreateItem(GetItemTypeId(usedItem), x, y)
		local player owner
		call SetItemCharges(result, GetItemCharges(usedItem))
		call SetItemInvulnerable(result, IsItemInvulnerable(usedItem))
		call SetWidgetLife(result, GetWidgetLife(usedItem))
		call SetItemPawnable(result, IsItemPawnable (usedItem))
		if (IsItemOwned(usedItem)) then
			set owner = GetItemPlayer(usedItem)
			call SetItemPlayer(result, owner, true)
			set owner = null
		endif
		call SetItemVisible(result, IsItemVisible(usedItem))
		call SetItemUserData(result, GetItemUserData(usedItem))
		return result
	endfunction

	/**
	* @author Tamino Dauth
	* @return Returns name of item type @param itemTypeId.
	*/
	function GetItemTypeIdName takes integer itemTypeId returns string
		local item whichItem = CreateItem(itemTypeId, 0.0, 0.0)
		local string result = GetItemName(whichItem)
		call RemoveItem(whichItem)
		set whichItem = null
		return result
	endfunction

endlibrary