library ALibraryCoreEnvironmentDestructable

	/// Creates a dummy unit which tries to harvest the filtered destructable.
	/// It should only used by filters.
	/// @author PitzerMike
	/// @return Returns true if the filtered destructable is a tree.
	function TreeFilter takes nothing returns boolean
		local player neutralPassivePlayer = Player(PLAYER_NEUTRAL_PASSIVE)
		local destructable filterDestructable = GetFilterDestructable()
		local boolean isInvulnerable = IsDestructableInvulnerable(filterDestructable)
		local unit dummy = CreateUnit(neutralPassivePlayer, 'h000',GetWidgetX(filterDestructable), GetWidgetY(filterDestructable), 0)
		local boolean result = false
		call UnitAddAbility(dummy, 'Ahrl')
		if isInvulnerable then
			call SetDestructableInvulnerable(filterDestructable, false)
		endif
		set result = IssueTargetOrder(dummy, "harvest", filterDestructable)
		call RemoveUnit(dummy)
		if isInvulnerable then
			call SetDestructableInvulnerable(filterDestructable, true)
		endif
		set neutralPassivePlayer = null
		set filterDestructable = null
		set dummy = null
		return result
	endfunction

endlibrary