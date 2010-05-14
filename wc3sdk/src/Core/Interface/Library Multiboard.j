library ALibraryCoreInterfaceMultiboard

	function ShowMultiboardForPlayer takes player whichPlayer, multiboard whichMultiboard, boolean show returns nothing
		local player localPlayer = GetLocalPlayer()
		if (whichPlayer == localPlayer) then
			call MultiboardDisplay(whichMultiboard, show)
		endif
		set localPlayer = null
	endfunction

	function MultiboardSuppressDisplayForPlayer takes player whichPlayer, boolean flag returns nothing
		if (whichPlayer == GetLocalPlayer()) then
			call MultiboardSuppressDisplay(flag)
		endif
	endfunction

endlibrary