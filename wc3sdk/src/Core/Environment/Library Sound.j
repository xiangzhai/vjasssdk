library ALibraryCoreEnvironmentSound

	/**
	* Plays a sound for the player @param user.
	* @author Tamino Dauth
	* @param user Player who the sound is played for.
	* @param usedSound Played sound.
	*/
	function PlaySoundForPlayer takes player user, sound usedSound returns nothing
		local player localPlayer = GetLocalPlayer()
		if (user == localPlayer) then
			call StartSound(usedSound)
		endif
		set localPlayer = null	
	endfunction

	/// Plays a sound for the player user by its file path.
	/// Note that sound paths has to be preloaded before they can be played.
	/// Otherwise the sound will just be played for the second time.
	/// @author Tamino Dauth
	/// @param user Player who can hear the sound.
	/// @param soundPath Played sound file.
	function PlaySoundPathForPlayer takes player user, string soundPath returns nothing
		local sound usedSound = CreateSound(soundPath, false, false, true, 12700, 12700, "")
		call PlaySoundForPlayer(user, usedSound)
		call KillSoundWhenDone(usedSound)
		set usedSound = null 
	endfunction

	/// Preloads a sound file by its path.
	/// Note that sound paths has to be preloaded before they can be played.
	/// Otherwise the sound will just be played for the second time.
	/// @author Tamino Dauth
	/// @param soundPath Preloaded sound file.
	function PreloadSoundPath takes string soundPath returns nothing
		local sound usedSound = CreateSound(soundPath, false, false, false, 10, 10, "")
		call SetSoundVolume(usedSound, 0)
		call StartSound(usedSound)
		call KillSoundWhenDone(usedSound)
		set usedSound = null 
	endfunction

endlibrary