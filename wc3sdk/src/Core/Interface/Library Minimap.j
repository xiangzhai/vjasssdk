library ALibraryCoreInterfaceMinimap

	//WE DON'T WANT TO USE PLAYER FORCES!!!!!!!!!!!!!!!!!!!!!!!!!
	function PingMinimapExForPlayer takes player user, real x, real y, real duration, real red, real green, real blue, boolean extraEffect returns nothing
		local player localPlayer = GetLocalPlayer()
		if (user == localPlayer) then 
			call PingMinimapEx(x, y, duration, PercentTo255(red), PercentTo255(green), PercentTo255(blue), extraEffect)
		endif
		set localPlayer = null
	endfunction

endlibrary