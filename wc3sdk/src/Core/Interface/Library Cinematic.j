library ALibraryCoreInterfaceCinematic

	/**
	* @author Tamino Dauth
	* @todo Test it!
	* @see EndCinematicScene, CancelCineSceneBJ
	*/
	function EndCinematicSceneForPlayer takes player whichPlayer returns nothing
		if (whichPlayer == GetLocalPlayer()) then
			call EndCinematicScene()
		endif
	endfunction

	/**
	* Smaller function than the real function (SetCinematicSceneBJ
	* You don't have to use player forces.
	* @see SetCinematicSceneBJ, TransmissionFromUnitWithNameBJ
	*/
	function SetCinematicSceneForPlayer takes player user, integer unitType, player owner, string title, string text, real sceneTime, real voiceTime returns nothing
		local player localPlayer = GetLocalPlayer()
		local playercolor playerColour = GetPlayerColor(owner)
		if (user == localPlayer) then
			call SetCinematicScene(unitType, playerColour, title, text, sceneTime, voiceTime)
		endif
		set localPlayer = null
		set playerColour = null
	endfunction

	/**
	* Does not init cinematic behaviour, wait for sound and ping the minimap.
	* @author Tamino Dauth
	* @see TransmissionFromUnitTypeWithNameBJ, TransmissionFromUnit
	*/
	function TransmissionFromUnitType takes integer unitType, player owner, string name, string text, sound playedSound returns nothing
		local playercolor playerColor = GetPlayerColor(owner)
		local real time
		if (playedSound != null) then
			set time = GetSoundDurationBJ(playedSound)
			call StartSound(playedSound)
		else
			set time = bj_NOTHING_SOUND_DURATION
		endif
		call SetCinematicScene(unitType, playerColor, name, text, time, time)
		set playerColor = null
	endfunction

	/**
	* Same differences like @function TransmissionFromUnitType and additionally does not add unit's indicator.
	* @author Tamino Dauth
	* @see TransmissionFromUnitWithNameBJ, TransmissionFromUnitType
	*/
	function TransmissionFromUnit takes unit usedUnit, string text, sound playedSound returns nothing
		local player owner = GetOwningPlayer(usedUnit)
		call TransmissionFromUnitType(GetUnitTypeId(usedUnit), owner, GetUnitName(usedUnit), text, playedSound)
		set owner = null
	endfunction

	/**
	* Shows a transmission with text @param text and sound @param playedSound from unit @param usedUnit for player @param usedPlayer.
	* @param playedSound If this value is null sound duration will be @global bj_NOTHING_SOUND_DURATION.
	* @author Tamino Dauth
	* @state untested
	*/
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
			call SetCinematicScene(GetUnitTypeId(usedUnit), playerColor, GetUnitName(usedUnit), text, time, time)
		endif
		set localPlayer = null
		set owner = null
		set playerColor = null
	endfunction

	/**
	* @see GetTransmissionDuration
	*/
	function GetSimpleTransmissionDuration takes sound playedSound returns real
		if (playedSound != null) then
			return GetSoundDurationBJ(playedSound)
		endif
		return bj_NOTHING_SOUND_DURATION
	endfunction

	/**
	* Alternate function without using forces.
	* @see ClearTextMessagesBJ, ClearTextMessages
	*/
	function ClearScreenMessagesForPlayer takes player user returns nothing
		local player localPlayer = GetLocalPlayer()
		if (user == localPlayer) then
			call ClearTextMessages()
		endif
		set localPlayer = null
	endfunction

	/**
	* @see ShowInterface, ShowInterfaceForceOn, ShowInterfaceForceOff
	*/
	function SetInterfaceForPlayer takes player whichPlayer, boolean show, real fadeDuration returns nothing
		if (whichPlayer == GetLocalPlayer()) then
			call ShowInterface(show, fadeDuration)
		endif
	endfunction

	/**
	* @see EnableUserControl, SetUserControlForceOn, SetUserControlForceOff
	*/
	function SetUserControlForPlayer takes player whichPlayer, boolean enable returns nothing
		if (whichPlayer == GetLocalPlayer()) then
			call EnableUserControl(enable)
		endif
	endfunction

	/**
	* Alternate function without using forces. Combines functions @function SetUserUIForPlayer and @function SetUserControlForPlayer.
	* @see SetInterfaceForPlayer, SetUserControlForPlayer, SetUserControlForceOn, SetUserControlForceOff, ShowInterfaceForceOn, ShowInterfaceForceOff, ShowInterface, EnableUserControl
	*/
	function SetUserInterfaceForPlayer takes player user, boolean show, boolean enableControl returns nothing
		local player localPlayer = GetLocalPlayer()
		if (user == localPlayer) then
			call ShowInterface(show, 0.0)
			call EnableUserControl(enableControl)
		endif
		set localPlayer = null
	endfunction

endlibrary