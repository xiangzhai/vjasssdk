library ALibraryCoreInterfaceCinematic

	//smaller function than the real
	//you don't have to use player forces
	function SetCinematicSceneForPlayer takes player user, integer unitType, string title, string text, real sceneTime, real voiceTime returns nothing
		local player localPlayer = GetLocalPlayer()
		local playercolor playerColour = GetPlayerColor(user)
		if (user == localPlayer) then
			call SetCinematicScene(unitType, playerColour, title, text, sceneTime, voiceTime)
		endif
		set localPlayer = null
		set playerColour = null
	endfunction
	
	/// @author Tamino Dauth
	/// @state untested
	function TransmissionFromUnit takes unit usedUnit, string text, sound playedSound returns nothing
		local player owner = GetOwningPlayer(usedUnit)
		local playercolor playerColor = GetPlayerColor(owner)
		local real time
		if (playedSound != null) then
			set time = GetSoundDurationBJ(playedSound)
			call StartSound(playedSound)
		else
			set time = bj_NOTHING_SOUND_DURATION
		endif
		call SetCinematicScene(GetUnitTypeId(usedUnit), playerColor, GetUnitName(usedUnit) + ":", text, time, time)
		set owner = null
		set playerColor = null
	endfunction
	
	/// @author Tamino Dauth
	/// @state untested
	function TransmissionFromUnitForPlayer takes player usedPlayer, unit usedUnit, string text, sound playedSound returns nothing
		local player localPlayer = GetLocalPlayer()
		local player owner = GetOwningPlayer(usedUnit)
		local playercolor playerColor = GetPlayerColor(owner)
		local real time
		if (playedSound != null) then
			set time = GetSoundDurationBJ(playedSound)
		else
			set time = bj_NOTHING_SOUND_DURATION
		endif
		if (usedPlayer == localPlayer) then
			if (playedSound != null) then
				call StartSound(playedSound)
			endif
			call SetCinematicScene(GetUnitTypeId(usedUnit), playerColor, GetUnitName(usedUnit) + ":", text, time, time)
		endif
		set localPlayer = null
		set owner = null
		set playerColor = null
	endfunction
	
	function GetSimpleTransmissionDuration takes sound playedSound returns real
		if (playedSound != null) then
			return GetSoundDurationBJ(playedSound)
		endif
		return bj_NOTHING_SOUND_DURATION
	endfunction
	
	//here it's the same
	//you don't have to use fucking player forces
	//we want to use players only!
	function ClearScreenMessagesForPlayer takes player user returns nothing
		local player localPlayer = GetLocalPlayer()
		if (user == localPlayer) then
			call ClearTextMessages()
		endif
		set localPlayer = null
	endfunction

	//and the same
	//we combinated the interface settings and the user control settings
	function SetUserInterfaceForPlayer takes player user, boolean show, boolean control returns nothing
		local player localPlayer = GetLocalPlayer()
		if (user == localPlayer) then
			call ShowInterface(show, 0.0)
		endif
		if (user == localPlayer) then
			call EnableUserControl(control) //test
		endif
		set localPlayer = null		
	endfunction

endlibrary