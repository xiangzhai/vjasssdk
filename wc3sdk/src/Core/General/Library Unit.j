library ALibraryCoreGeneralUnit requires ALibraryCoreGeneralItem

	/**
	* Sicheres Iterieren per FirstOfGroup
	*
	* Heyhey,
	*
	* dieser Bug (oder Feature, je nachdem wie mans nimmt) hat mich etwa 4 Stunden debuggen 
	* gekostet.
	*
	* Hintergrund: Wie ihr wisst kann man ja über eine Unitgroup iterieren (und diese dabei leeren) 
	* (Die Gruppe nenne ich einfach mal g):
	*
	* @code
	* local unit u
	* loop
	* 	set u = FirstOfGroup(g)
	* 	exitwhen u == null
	*
	* 	//HIER das mit der unit machen was man machen will
	*
	* 	call GroupRemoveUnit(g,u)
	* endloop
	* @endcode
	*
	* Tja, nur blöd dass das buggy ist, sobald einmal eine unit in der Gruppe aus dem Spiel 
	* entfernt wurde ohne sie vorher ordnungsgemäß aus der Gruppe zu removen. Das wusste ich bisher 
	* auch nicht.
	*
	* Beispiel:
	* Wir adden in eine Gruppe 4 units. Dann entfernen wir die erste geaddete unit per 
	* RemoveUnit(...) aus dem Spiel. Nun ist die Gruppe kaputt (zumindest für das Iterieren per 
	* FirstOfGroup()). Denn FirstOfGroup(g) wird nun null liefern, die schleife endet also sofort 
	* obwohl drei units in der Group sind. Denn die "Lücke" die die unit hinterlassen hat bleibt in 
	* der Group. Und das blödeste: Diese lücke können wir auch nicht mehr per call 
	* GroupRemoveUnit(FirstOfGroup()) entfernen, da ja FoG nun null zurückliefert und 
	* GroupRemoveUnit(null) nichts macht.
	*
	*
	* ALSO VORSICHT! Das FirstOfGroup(...) null zurückliefert heisst nicht unbedingt, dass die 
	* Gruppe leer ist!
	*
	* Deshalb hab ich mir eine kleine Funktion geschrieben die äquivalent zu einem Aufruf von 
	* FirstOfGroup ist, aber solche Lücken erkennt und die Gruppe automatisch repariert sobald sie 
	* auf so eine Lücke trifft, hier ist sie:
	*
	* Die Idee:
	* Erstmal führt die Funktion ein normales FirstOfGroup auf die gewünschte Gruppe aus. Liefert 
	* dieser aufruf NICHT null, klappt ja alles perfekt, dann gibt sie einfach die unit zurück. 
	* Liefert der Aufruf aber null können zwei Fälle eingetreten sein:
	*
	* 1.) Die Group ist tatsächlich empty
	* 2.) Wir sind auf eine Lücke gestoßen die repariert werden muss.
	*
	* Das überprüfen wir einfach indem wir ein GroupIsEmpty-derivat aufrufen. Liefert es true
	* zurück war die gruppe wirklich leer und wir können null returnen. sonst reparieren wir sie, 
	* das geht so:
	* Wir kopieren per ForGroup alle units in eine swap gruppe. Dabei werden lücken nicht 
	* mitkopiert, da ForGroup so schlau ist und solche lücken beim iterieren nicht beachtet. Dann 
	* leeren wir die gruppe und kopieren einfach die units aus der swapgruppe zurück und fertig ist 
	* die reparierte Gruppe auf die wir nun ein erneutes FirstOfGroup aufrufen um die wirkliche 
	* first unit zu ermitteln die wir zurück geben.
	*
	* Ich empfehle euch dringend diese Funktion zu benutzen wenn ihr über eine Group per 
	* FirstOfGroup iterieren wollt, aber nicht sicher sein könnt, dass keine unit jemals aus dem 
	* Spiel entfernt wurde (was auch nach dem normalen tod und dekay automatisch passiert) die in 
	* der Gruppe war.
	*
	* Hoffe ich kann euch damit die Stunden des Bugfixen die ich hatte ersparen...
	* @author gexxo
	* @param g Used unit group.
	* @return Returns the first unit of group g. If the group is empty it will return null.
	*/
	function FirstOfGroupSave takes group g returns unit
		local unit u = FirstOfGroup(g) //Try a normal first of group
		local group swap
		
		// If the result is null there may be gaps in the group
		if u == null then
			//Check if the group is empty. If it is not, then there must be gaps -> repair
			set bj_isUnitGroupEmptyResult = true
			call ForGroup(g, function IsUnitGroupEmptyBJEnum)
			if not bj_isUnitGroupEmptyResult then
				//** Repair the group **
				set swap = CreateGroup()
				call GroupAddGroup(g,swap) //Add all units to a swapping group
				call GroupClear(g) //Clear the buggy group hence removing the gaps
				call GroupAddGroup(swap,g) //Put the units back in from the swapping group
				
				//Collect garbage
				call DestroyGroup(swap)
				set swap = null
				
				//Do another FirstOfGroup to gain the real first unit
				set u = FirstOfGroup(g)
			endif
		endif
		
		return u //Return the unit we wanted   
	endfunction
	
	/// @author Tamino Dauth
	/// @todo Probably bugged, do not use.
	function MoveItemToSlot takes unit usedUnit, item usedItem, integer slot returns boolean
		return IssueTargetOrderById(usedUnit, 852002 + slot, usedItem)
	endfunction
	
	/// @author Tamino Dauth
	/// @todo Probably bugged, do not use.
	function UseItemOfSlot takes unit usedUnit, integer slot returns boolean
		return IssueImmediateOrderById(usedUnit, 852008 + slot)
	endfunction
	
	/// @author Tamino Dauth
	/// @todo Probably bugged, do not use.
	function UseItemOfSlotOnTarget takes unit usedUnit, integer slot, widget target returns boolean
		return IssueTargetOrderById(usedUnit, 852008 + slot, target)
	endfunction
	
	/// @author Tamino Dauth
	/// @todo Probably bugged, do not use.
	function UseItemOfSlotOnPoint takes unit usedUnit, integer slot, real x, real y returns boolean
		return IssuePointOrderById(usedUnit, 852008 + slot, x, y)
	endfunction
	
	/// @author Tamino Dauth
	function UnitDropSlot takes unit usedUnit, integer slot0, integer slot1 returns boolean
		local item slotItem = UnitItemInSlot(usedUnit, slot0)
		local boolean result = UnitDropItemSlot(usedUnit, slotItem, slot1)
		set slotItem = null
		return result
	endfunction
	
	/// @author Tamino Dauth
	/// @return Missing life of unit @param usedUnit.
	function GetUnitMissingLife takes unit usedUnit returns real
		return GetUnitState(usedUnit, UNIT_STATE_MAX_LIFE) - GetUnitState(usedUnit, UNIT_STATE_LIFE)
	endfunction
	
	/// @author Tamino Dauth
	/// @return Missing life of unit @param usedUnit in percent.
	function GetUnitMissingLifePercent takes unit usedUnit returns real
		return 100.0 - GetUnitStatePercent(usedUnit, UNIT_STATE_LIFE, UNIT_STATE_MAX_LIFE)
	endfunction

	/// @author Tamino Dauth
	/// @return Missing mana of unit @param usedUnit.
	function GetUnitMissingMana takes unit usedUnit returns real
		return GetUnitState(usedUnit, UNIT_STATE_MAX_MANA) - GetUnitState(usedUnit, UNIT_STATE_MANA)
	endfunction
	
	/// @author Tamino Dauth
	/// @return Missing mana of unit @param usedUnit in percent.
	function GetUnitMissingManaPercent takes unit usedUnit returns real
		return 100.0 - GetUnitStatePercent(usedUnit, UNIT_STATE_MANA, UNIT_STATE_MAX_MANA)
	endfunction
	
	/// @author Tamino Dauth
	/// @todo Finish
	function CopyUnit takes unit usedUnit, real x, real y, real facing, integer stateMethod returns unit
		local player owner = GetOwningPlayer(usedUnit)
		local unit result = CreateUnit(owner, GetUnitTypeId(usedUnit), x, y, facing)
		local real oldRatio
		local player user
		local integer i
		local item oldSlotItem
		local item newSlotItem
		call ShowUnit(result, IsUnitHidden(usedUnit))
		call PauseUnit(result, IsUnitPaused(usedUnit))
		call SetResourceAmount(result, GetResourceAmount(usedUnit))
		call SetUnitUserData(result, GetUnitUserData(usedUnit))
		// Set the unit's life and mana according to the requested method.
		if (stateMethod == bj_UNIT_STATE_METHOD_RELATIVE) then
			// Set the replacement's current/max life ratio to that of the old unit.
			// If both units have mana, do the same for mana.
			if (GetUnitState(usedUnit, UNIT_STATE_MAX_LIFE) > 0) then
				set oldRatio = GetUnitState(usedUnit, UNIT_STATE_LIFE) / GetUnitState(usedUnit, UNIT_STATE_MAX_LIFE)
				call SetUnitState(result, UNIT_STATE_LIFE, oldRatio * GetUnitState(result, UNIT_STATE_MAX_LIFE))
			endif
			if (GetUnitState(usedUnit, UNIT_STATE_MAX_MANA) > 0) and (GetUnitState(result, UNIT_STATE_MAX_MANA) > 0) then
				set oldRatio = GetUnitState(usedUnit, UNIT_STATE_MANA) / GetUnitState(usedUnit, UNIT_STATE_MAX_MANA)
				call SetUnitState(result, UNIT_STATE_MANA, oldRatio * GetUnitState(result, UNIT_STATE_MAX_MANA))
			endif
		elseif (stateMethod == bj_UNIT_STATE_METHOD_ABSOLUTE) then
			// Set the replacement's current life to that of the old unit.
			// If the new unit has mana, do the same for mana.
			call SetUnitState(result, UNIT_STATE_LIFE, GetUnitState(usedUnit, UNIT_STATE_LIFE))
			if (GetUnitState(result, UNIT_STATE_MAX_MANA) > 0) then
				call SetUnitState(result, UNIT_STATE_MANA, GetUnitState(usedUnit, UNIT_STATE_MANA))
			endif
		elseif (stateMethod == bj_UNIT_STATE_METHOD_DEFAULTS) then
			// The newly created unit should already have default life and mana.
		elseif (stateMethod == bj_UNIT_STATE_METHOD_MAXIMUM) then
			// Use max life and mana.
			call SetUnitState(result, UNIT_STATE_LIFE, GetUnitState(result, UNIT_STATE_MAX_LIFE))
			call SetUnitState(result, UNIT_STATE_MANA, GetUnitState(result, UNIT_STATE_MAX_MANA))
		endif
		
		set i = 0
		loop
			exitwhen (i == bj_MAX_PLAYERS)
			set user = Player(i)
			if (IsUnitSelected(usedUnit, user)) then
				call SelectUnitAddForPlayer(result, user)
			endif
			set user = null
			set i = i + 1
		endloop
		
		set i = 0
		loop
			exitwhen (i == bj_MAX_INVENTORY)
			set oldSlotItem = UnitItemInSlot(usedUnit, i)
			if (oldSlotItem != null) then
				set newSlotItem = CopyItem(oldSlotItem, GetUnitX(result), GetUnitY(result))
				call UnitAddItem(result, newSlotItem) /// @todo add to old slot
				set oldSlotItem = null
			endif
			set i = i + 1
		endloop
		
		if (IsUnitType(usedUnit, UNIT_TYPE_HERO) and IsUnitType(result, UNIT_TYPE_HERO)) then
			call SetHeroXP(result, GetHeroXP(usedUnit), false)
		endif
		
		return result
	endfunction
	
	/// @author Tamino Dauth
	function CreateUnitsAtPoint takes integer count, integer unitTypeId, player whichPlayer, real x, real y, real face returns group
		local group unitGroup = CreateGroup()
		local unit groupMember
		loop
			set count = count - 1
			exitwhen (count < 0)
			set groupMember = CreateUnit(whichPlayer, unitTypeId, x, y, face)
			call GroupAddUnit(unitGroup, groupMember)
		endloop
		return unitGroup
	endfunction
	
	/// @author Tamino Dauth
	function CreateUnitAtRect takes player whichPlayer, integer unitTypeId, rect whichRect, real facing returns unit
		return CreateUnit(whichPlayer, unitTypeId, GetRectCenterX(whichRect), GetRectCenterY(whichRect), facing)
	endfunction

endlibrary