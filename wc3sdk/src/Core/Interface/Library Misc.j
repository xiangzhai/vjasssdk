library ALibraryCoreInterfaceMisc requires ALibraryCoreStringConversion

	globals
		//Keys
		constant integer KEY_ESCAPE = 0
		constant integer KEY_UP = 1
		constant integer KEY_DOWN = 2
		constant integer KEY_RIGHT = 3
		constant integer KEY_LEFT = 4
		//Hotkeys für Dialoge nach dem ASCII
		constant integer SHORTCUT_BACKSPACE = 8
		constant integer SHORTCUT_TAB = 9
		constant integer SHORTCUT_ESCAPE = 27
		constant integer SHORTCUT_SPACE = 32
		constant integer SHORTCUT_0 = 48
		constant integer SHORTCUT_1 = 49
		constant integer SHORTCUT_2 = 50
		constant integer SHORTCUT_3 = 51
		constant integer SHORTCUT_4 = 52
		constant integer SHORTCUT_5 = 53
		constant integer SHORTCUT_6 = 54
		constant integer SHORTCUT_7 = 55
		constant integer SHORTCUT_8 = 56
		constant integer SHORTCUT_9 = 57
		//Kürzel der Kleinbuchstaben
		constant integer SHORTCUT_A = 65 
		constant integer SHORTCUT_B = 66 
		constant integer SHORTCUT_C = 67
		constant integer SHORTCUT_D = 68
		constant integer SHORTCUT_E = 69
		constant integer SHORTCUT_F = 70
		constant integer SHORTCUT_G = 71
		constant integer SHORTCUT_H = 72
		constant integer SHORTCUT_I = 73
		constant integer SHORTCUT_J = 74
		constant integer SHORTCUT_K = 75
		constant integer SHORTCUT_L = 76
		constant integer SHORTCUT_M = 77
		constant integer SHORTCUT_N = 78
		constant integer SHORTCUT_O = 79
		constant integer SHORTCUT_P = 80
		constant integer SHORTCUT_Q = 81
		constant integer SHORTCUT_R = 82
		constant integer SHORTCUT_S = 83
		constant integer SHORTCUT_T = 84
		constant integer SHORTCUT_U = 85
		constant integer SHORTCUT_V = 86
		constant integer SHORTCUT_W = 87
		constant integer SHORTCUT_X = 88
		constant integer SHORTCUT_Y = 89
		constant integer SHORTCUT_Z = 90
		//dialogs
		constant integer AMaxDialogButtons = 12 /// @todo 16?
	endglobals
	
	debug function KeyIsValid takes integer key returns boolean
		debug return key >= KEY_ESCAPE and key <= KEY_LEFT
	debug endfunction

	function TriggerRegisterKeyEventForPlayer takes player user, trigger usedTrigger, integer key, boolean press returns event
		if (key == KEY_ESCAPE) then
			return TriggerRegisterPlayerEvent(usedTrigger, user, EVENT_PLAYER_END_CINEMATIC)
		elseif (key == KEY_UP) then
			if (press) then
				return TriggerRegisterPlayerEvent(usedTrigger, user, EVENT_PLAYER_ARROW_UP_DOWN)
			else
				return TriggerRegisterPlayerEvent(usedTrigger, user, EVENT_PLAYER_ARROW_UP_UP)
			endif
		elseif (key == KEY_DOWN) then
			if (press) then
				return TriggerRegisterPlayerEvent(usedTrigger, user, EVENT_PLAYER_ARROW_DOWN_DOWN)
			else
				return TriggerRegisterPlayerEvent(usedTrigger, user, EVENT_PLAYER_ARROW_DOWN_UP)
			endif
		elseif (key == KEY_RIGHT) then
			if (press) then
				return TriggerRegisterPlayerEvent(usedTrigger, user, EVENT_PLAYER_ARROW_RIGHT_DOWN)
			else
				return TriggerRegisterPlayerEvent(usedTrigger, user, EVENT_PLAYER_ARROW_RIGHT_UP)
			endif
		elseif (key == KEY_LEFT) then
			if (press) then
				return TriggerRegisterPlayerEvent(usedTrigger, user, EVENT_PLAYER_ARROW_LEFT_DOWN)
			else
				return TriggerRegisterPlayerEvent(usedTrigger, user, EVENT_PLAYER_ARROW_LEFT_UP)
			endif
		endif
		return null
	endfunction

	function GetModifiedPlayerName takes player user returns string
		local playercolor playerColor = GetPlayerColor(user)
		local string modifiedPlayerName = ("|c00" + PlayerColorToString(playerColor) + GetPlayerName(user) + "|r |c00ffffff[" + I2S(GetPlayerId(user) + 1) + "]|r") //ALibraryStringConversion
		set playerColor = null
		return modifiedPlayerName
	endfunction

	function GetBar takes real value, real maxValue, integer length, string colour returns string
		local integer i
		local integer colouredPart = 0
		local string result = ""
		if (maxValue != 0.0) then
			set colouredPart = R2I((value / maxValue) * I2R(length)) 
			//coloured part exists
			if (colouredPart > 0.0) then
				set result = ("|c00" + colour)
			endif
			//set bar
			set i = 0
			loop
			exitwhen(i == length)
				set result = (result + "|")
				//set end of coloured part
				if ((i == colouredPart) and (colouredPart > 0.0) and (colouredPart != length)) then
					set result = (result +  "I|c00ffffff") //I ist ein notwendiges Trennzeichen   
				endif
			set i = (i + 1)
			endloop
		endif
		return result
	endfunction

	/// @state untested
	function SetUnitVertexColourForPlayer takes player user, unit usedUnit, real red, real green, real blue, real transparency returns nothing
		local player localPlayer = GetLocalPlayer()
		if (user == localPlayer) then 
			call SetUnitVertexColorBJ(usedUnit, red, green, blue, transparency)
		endif
		set localPlayer = null
	endfunction

endlibrary