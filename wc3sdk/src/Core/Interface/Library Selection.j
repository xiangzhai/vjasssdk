library ALibraryCoreInterfaceSelection

	function IsPlayerSelectionEmpty takes player user returns boolean
		local group selectedUnits = CreateGroup()
		local boolean result = true
		call GroupEnumUnitsSelected(selectedUnits, user, null)
		if (selectedUnits != null) then
			set result = false
		endif
		call DestroyGroup(selectedUnits)
		set selectedUnits = null
		return result
	endfunction

	function GetFirstSelectedUnitOfPlayer takes player user returns unit
		local group selectedUnits = null
		local unit selectedUnit = null //Startwert, falls die Gruppe leer ist
		set selectedUnits = GetUnitsSelectedAll(user)
		set selectedUnit = FirstOfGroup(selectedUnits)
		call DestroyGroup(selectedUnits)
		set selectedUnits = null
		return selectedUnit
	endfunction

	/// Makes a unit select- or unselectable by removing or adding the gasshopper ability.
	function MakeUnitSelectable takes unit usedUnit, boolean selectable returns nothing
		if (selectable) then
			call UnitRemoveAbility(usedUnit, 'Aloc')
		else
			call UnitAddAbility(usedUnit, 'Aloc')
		endif
	endfunction

endlibrary