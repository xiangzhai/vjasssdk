library ALibraryCoreDebugMisc requires ALibraryCoreGeneralPlayer

	/**
	* Displays text on the screen if the debug mode is enabled.
	* DebugMsg: Your Message - Echoes Your Message to logs//war3erruser.txt
	* Ursprünglich war es eine Platzhalter-Funktion.
	* @author Tamino Dauth
	* @param message The message which will be shown on the screen and if Warcraft is started with Grimore it prints the messag into logs//war3err.txt.
	*/
	debug function Print takes string message returns nothing
		debug local integer i
		debug local player user
		debug set i = 0
		debug loop
			debug exitwhen(i == bj_MAX_PLAYERS)
			debug set user = Player(i)
			debug if (IsPlayerPlayingUser(user)) then
				debug call DisplayTimedTextToPlayer(user, 0.0, 0.0, 999999.0, message)
			debug endif
			debug set user = null
			debug set i = i + 1
		debug endloop
		debug call Cheat("DebugMsg: " + message) //JNGP
	debug endfunction
	
	debug function PrintFunctionError takes string functionName, string message returns nothing
		debug call Print("Function error in function \"" + functionName + "\": " + message)
	debug endfunction

	/**
	* Useful for getting the causing struct and instance of the debug message.
	* Displays the string @param message in the following schema: "<structname> - <instanceid>: <message>"
	* @author Tamino Dauth
	*/
	//! textmacro A_STRUCT_DEBUG takes STRUCTNAME
		debug public method print takes string message returns nothing //stub
			debug call Print($STRUCTNAME$ + " - " + I2S(this) + ": " + message)
		debug endmethod

		debug private static method staticPrint takes string message returns nothing
			debug call Print($STRUCTNAME$ + ": " + message)
		debug endmethod
	//! endtextmacro

	/**
	* war3err DumpGlobalHT - prints the names and values of all global variables to logs//war3err.txt
	* @author Tamino Dauth
	*/
	debug function PrintGlobals takes nothing returns nothing
		debug call Cheat("war3err_DumpGlobalHT") //JNGP
	debug endfunction

	/**
	* war3err DumpLocalHT - As above, but for the local variables of the current scope
	* @author Tamino Dauth
	*/
	debug function PrintLocals takes nothing returns nothing
		debug call Cheat("war3err_DumpLocalHT") //JNGP
	debug endfunction

	/**
	* PauseTracer: If bytecode logging is enabled, this pauses recording
	* @author Tamino Dauth
	*/
	debug function PauseBytecodeLogging takes nothing returns nothing
		debug call Cheat("PauseTracer") //JNGP
	debug endfunction

	/**
	* ContinueTracer: If bytecode logging is enabled, this resumes recording
	* @author Tamino Dauth
	*/
	debug function ContinueBytecodeLogging takes nothing returns nothing
		debug call Cheat("ContinueTracer") //JNGP
	debug endfunction
	
	/**
	* When the breakpoint is reached Warcraft 3 is frozen.
	* @author Tamino Dauth
	*/
	debug function Breakpoint takes nothing returns nothing
		debug call Cheat("war3err_Break") //JNGP
	debug endfunction

	/// @todo Should be private, vJass bug.
	debug function DebugPlayer takes integer number returns nothing
		debug if (number < 0 or number >= bj_MAX_PLAYER_SLOTS) then
			debug call PrintFunctionError("Player", "Invalid parameter number, value " + I2S(number) + ". Setting number to 0 to avoid game crash.")
			debug set number = 0
		debug endif
		//debug return Player(number)
	debug endfunction

	globals
		/// @todo Should be private, vJass bug.
		debug integer gameCacheCounter = 0
	endglobals

	/// @todo Should be private, vJass bug.
	debug function DebugInitGameCache takes string campaignFile returns nothing
		debug if (gameCacheCounter == 256) then
			debug call PrintFunctionError("InitGameCache", "Reached game cache maximum. Canceling function call.")
			//debug return null
		debug endif
		debug set gameCacheCounter = gameCacheCounter + 1
		//debug return InitGameCache(campaignFile)
	debug endfunction

	/// @todo Should be private, vJass bug.
	debug function DebugDestroyTimer takes timer whichTimer returns nothing
		//IsTimerPaused
		//Timer-Ablauf
		//- Zerstörte Timer laufen trotz ihrer Zerstörung ab. Man muss einen Timer anhalten bevor man ihn zerstört.
	debug endfunction

	/// @todo Should be private, vJass bug.
	debug function DebugSetUnitX takes unit whichUnit, real newX returns nothing
		debug if (newX < GetRectMinX(bj_mapInitialPlayableArea) or newX > GetRectMaxX(bj_mapInitialPlayableArea)) then
			debug call PrintFunctionError("SetUnitX", "Invalid parameter newX, value " + R2S(newX) + ". Canceling function call to avoid game crash.")
			//debug return
		debug endif
	debug endfunction

	/// @todo Should be private, vJass bug.
	debug function DebugSetUnitY takes unit whichUnit, real newY returns nothing
		debug if (newY < GetRectMinY(bj_mapInitialPlayableArea) or newY > GetRectMaxY(bj_mapInitialPlayableArea)) then
			debug call PrintFunctionError("SetUnitY", "Invalid parameter newY, value " + R2S(newY) + ". Canceling function call to avoid game crash.")
			//debug return
		debug endif
	debug endfunction

	/// @todo Should be private, vJass bug.
	debug function DebugCreateUnit takes player id, integer unitid, real x, real y, real face returns nothing
		debug if (x < GetRectMinX(bj_mapInitialPlayableArea) or x > GetRectMaxX(bj_mapInitialPlayableArea) or y < GetRectMinY(bj_mapInitialPlayableArea) or y > GetRectMaxY(bj_mapInitialPlayableArea)) then
			debug call PrintFunctionError("CreateUnit", "Invalid parameter x or y, values " + R2S(x) + " and " + R2S(y) + ". Canceling function call to avoid game crash.")
			//debug return null
		debug endif
		//debug return CreateUnit(id, unitid, x, y, face)
	debug endfunction

	/// @todo Should be private, vJass bug.
	debug function DebugRestoreUnit takes gamecache cache, string missionKey, string key, player forWhichPlayer, real x, real y, real facing returns nothing
		debug if (not HaveStoredUnit(cache, missionKey, key)) then
			debug call PrintFunctionError("RestoreUnit", "Stored unit does not exist in game cache. Canceling function call to avoid game crash.")
			//debug return null
		debug endif
		//debug return RestoreUnit(cache, missionKey, key, forWhichPlayer, x, y, facing)
	debug endfunction

	/// @todo Should be private, vJass bug.
	debug function DebugSetImageRender takes image whichImage, boolean flag returns nothing
		debug call PrintFunctionError("SetImageRender", "Does not work, use SetImageRenderAlways instead.")
	debug endfunction

	debug hook Player DebugPlayer
	debug hook InitGameCache DebugInitGameCache
	//debug hook DestroyTimer SCOPE_PRIVATEDestroyTimer
	debug hook SetUnitX DebugSetUnitX
	debug hook SetUnitY DebugSetUnitY
	debug hook CreateUnit DebugCreateUnit
	debug hook RestoreUnit DebugRestoreUnit
	debug hook SetImageRender DebugSetImageRender

	//GroupEnumUnitsInRectCounted und GroupEnumUnitsInRangeCounted
	//- Die Funktionen GroupEnumUnitsInRectCounted und GroupEnumUnitsInRangeCounted können sich unterschiedlich verhalten, falls sie eine große Anzahl von Einheiten haben.
	//GetExpiredTimer
	//- GetExpiredTimer() kann einen Absturtz des Spiels verursachen, wenn es keinen Timer gibt.
	//Einheitengruppen – vom InWarcraft-Nutzer gexxo
	//- Werden Einheiten per RemoveUnit() aus dem Spiel entfernt und befinden sich bereits in einer Einheitengruppe, so kann es zu Fehlern kommen, da die erste Einheit der Gruppe nun immer null ist.

endlibrary
