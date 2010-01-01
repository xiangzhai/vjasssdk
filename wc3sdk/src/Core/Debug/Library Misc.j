library ALibraryCoreDebugMisc requires ALibraryCoreGeneralPlayer

	/**
	* Displays text on the screen if the debug mode is enabled.
	* DebugMsg: Your Message - Echoes Your Message to logs//war3erruser.txt
	* Ursprünglich war es eine Platzhalter-Funktion.
	* @author Tamino Dauth
	* @param message The message which will be shown on the screen and if Warcraft is started with Grimore it prints the messag into logs//war3err.txt.
	*/
static if (DEBUG_MODE) then
	function Print takes string message returns nothing
		local integer i
		local player user
		set i = 0
		loop
			exitwhen(i == bj_MAX_PLAYERS)
			set user = Player(i)
			if (IsPlayerPlayingUser(user)) then
				call DisplayTimedTextToPlayer(user, 0.0, 0.0, 999999.0, message)
			endif
			set user = null
			set i = i + 1
		endloop
		call Cheat("DebugMsg: " + message) //JNGP
	endfunction

	function PrintFunctionError takes string functionName, string message returns nothing
		call Print("Function error in function \"" + functionName + "\": " + message)
	endfunction
endif

	/**
	* Useful for getting the causing struct and instance of the message.
	* Displays the string @param message in the following schema: "<structname> - <instanceid>: <message>"
	* @author Tamino Dauth
	*/
	//! textmacro A_STRUCT_DEBUG takes STRUCTNAME
static if (DEBUG_MODE) then
		public method print takes string message returns nothing //stub
			call Print($STRUCTNAME$ + " - " + I2S(this) + ": " + message)
		endmethod

		public method printMethodError takes string methodName, string message returns nothing
			call Print($STRUCTNAME$ + " - " + I2S(this) + ": Method error in method \"" + methodName + "\": " + message)
		endmethod

		private static method staticPrint takes string message returns nothing
			call Print($STRUCTNAME$ + ": " + message)
		endmethod

		private static method staticMethodErrorPrint takes string methodName, string message returns nothing
			call Print($STRUCTNAME$ + ": Method error in method \"" + methodName + "\": " + message)
		endmethod
endif
	//! endtextmacro

	/**
	* Useful for getting the causing struct and instance of the debug message.
	* Displays the string @param message in the following schema: "<structname> - <instanceid>: <message>"
	* @author Tamino Dauth
	*/
	//! textmacro A_ZINC_STRUCT_DEBUG takes STRUCTNAME
			public method print(string message)
			{
				Print($STRUCTNAME$ + " - " + I2S(this) + ": " + message);
			}

			private static method staticPrint(string message)
			{
				Print($STRUCTNAME$ + ": " + message);
			}
	//! endtextmacro

	/**
	* war3err DumpGlobalHT - prints the names and values of all global variables to logs//war3err.txt
	* @author Tamino Dauth
	*/
	function PrintGlobals takes nothing returns nothing
		call Cheat("war3err_DumpGlobalHT") //JNGP
	endfunction

	/**
	* war3err DumpLocalHT - As above, but for the local variables of the current scope
	* @author Tamino Dauth
	*/
	function PrintLocals takes nothing returns nothing
		call Cheat("war3err_DumpLocalHT") //JNGP
	endfunction

	/**
	* PauseTracer: If bytecode logging is enabled, this pauses recording
	* @author Tamino Dauth
	*/
	function PauseBytecodeLogging takes nothing returns nothing
		call Cheat("PauseTracer") //JNGP
	endfunction

	/**
	* ContinueTracer: If bytecode logging is enabled, this resumes recording
	* @author Tamino Dauth
	*/
	function ContinueBytecodeLogging takes nothing returns nothing
		call Cheat("ContinueTracer") //JNGP
	endfunction

	/**
	* When the breakpoint is reached Warcraft 3 is frozen.
	* @author Tamino Dauth
	*/
	function Breakpoint takes nothing returns nothing
		call Cheat("war3err_Break") //JNGP
	endfunction

	/// @todo Should be private, vJass bug.
	function DebugPlayer takes integer number returns nothing
		if (number < 0 or number >= bj_MAX_PLAYER_SLOTS) then
			debug call PrintFunctionError("Player", "Invalid parameter number, value " + I2S(number) + ". Setting number to 0 to avoid game crash.")
			//set number = 0
		endif
		//return Player(number)
	endfunction

	globals
		/// @todo Should be private, vJass bug.
		integer gameCacheCounter = 0
	endglobals

	/// @todo Should be private, vJass bug.
	function DebugInitGameCache takes string campaignFile returns nothing
		if (gameCacheCounter == 256) then
			debug call PrintFunctionError("InitGameCache", "Reached game cache maximum. Canceling function call.")
			//return null
		endif
		set gameCacheCounter = gameCacheCounter + 1
		//return InitGameCache(campaignFile)
	endfunction

	/// @todo Should be private, vJass bug.
	function DebugDestroyTimer takes timer whichTimer returns nothing
		//IsTimerPaused
		//Timer-Ablauf
		//- Zerstörte Timer laufen trotz ihrer Zerstörung ab. Man muss einen Timer anhalten bevor man ihn zerstört.
	endfunction

	/// @todo Should be private, vJass bug.
	function DebugSetUnitX takes unit whichUnit, real newX returns nothing
		if (newX < GetRectMinX(bj_mapInitialPlayableArea) or newX > GetRectMaxX(bj_mapInitialPlayableArea)) then
			debug call PrintFunctionError("SetUnitX", "Invalid parameter newX, value " + R2S(newX) + ". Canceling function call to avoid game crash.")
			//return
		endif
	endfunction

	/// @todo Should be private, vJass bug.
	function DebugSetUnitY takes unit whichUnit, real newY returns nothing
		if (newY < GetRectMinY(bj_mapInitialPlayableArea) or newY > GetRectMaxY(bj_mapInitialPlayableArea)) then
			debug call PrintFunctionError("SetUnitY", "Invalid parameter newY, value " + R2S(newY) + ". Canceling function call to avoid game crash.")
			//return
		endif
	endfunction

	/// @todo Should be private, vJass bug.
	function DebugCreateUnit takes player id, integer unitid, real x, real y, real face returns nothing
		if (x < GetRectMinX(bj_mapInitialPlayableArea) or x > GetRectMaxX(bj_mapInitialPlayableArea) or y < GetRectMinY(bj_mapInitialPlayableArea) or y > GetRectMaxY(bj_mapInitialPlayableArea)) then
			debug call PrintFunctionError("CreateUnit", "Invalid parameter x or y, values " + R2S(x) + " and " + R2S(y) + ". Canceling function call to avoid game crash.")
			//return null
		endif
		//return CreateUnit(id, unitid, x, y, face)
	endfunction

	/// @todo Should be private, vJass bug.
	function DebugRestoreUnit takes gamecache cache, string missionKey, string key, player forWhichPlayer, real x, real y, real facing returns nothing
		if (not HaveStoredUnit(cache, missionKey, key)) then
			debug call PrintFunctionError("RestoreUnit", "Stored unit does not exist in game cache. Canceling function call to avoid game crash.")
			//return null
		endif
		//return RestoreUnit(cache, missionKey, key, forWhichPlayer, x, y, facing)
	endfunction

	/// @todo Should be private, vJass bug.
	function DebugSetImageRender takes image whichImage, boolean flag returns nothing
		debug call PrintFunctionError("SetImageRender", "Does not work, use SetImageRenderAlways instead.")
	endfunction

static if (DEBUG_MODE and A_DEBUG_NATIVES) then
	hook Player DebugPlayer
	hook InitGameCache DebugInitGameCache
	//debug hook DestroyTimer SCOPE_PRIVATEDestroyTimer
	hook SetUnitX DebugSetUnitX
	hook SetUnitY DebugSetUnitY
	hook CreateUnit DebugCreateUnit
	hook RestoreUnit DebugRestoreUnit
	hook SetImageRender DebugSetImageRender
endif

	//GroupEnumUnitsInRectCounted und GroupEnumUnitsInRangeCounted
	//- Die Funktionen GroupEnumUnitsInRectCounted und GroupEnumUnitsInRangeCounted können sich unterschiedlich verhalten, falls sie eine große Anzahl von Einheiten haben.
	//GetExpiredTimer
	//- GetExpiredTimer() kann einen Absturtz des Spiels verursachen, wenn es keinen Timer gibt.
	//Einheitengruppen – vom InWarcraft-Nutzer gexxo
	//- Werden Einheiten per RemoveUnit() aus dem Spiel entfernt und befinden sich bereits in einer Einheitengruppe, so kann es zu Fehlern kommen, da die erste Einheit der Gruppe nun immer null ist.

endlibrary
