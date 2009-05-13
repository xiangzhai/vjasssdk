library ALibraryCoreInterfaceMultiboard

	function ShowMultiboardForPlayer takes player user, multiboard usedMultiboard, boolean show returns nothing
		local player localPlayer = GetLocalPlayer()
		if (user == localPlayer) then 
			call MultiboardDisplay(usedMultiboard, show)
		endif
		set localPlayer = null
	endfunction

endlibrary