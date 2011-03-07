library ALibraryCoreDebugMisc initializer init requires ALibraryCoreGeneralPlayer

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

	function PrintIf takes boolean condition, string message returns nothing
		if (condition) then
			call Print(message)
		endif
	endfunction

	globals
		private AStringList disabledPrintIdentifiers
	endglobals

	function EnablePrintIdentifier takes string identifier returns nothing
		call disabledPrintIdentifiers.remove.evaluate(identifier)
	endfunction

	function DisablePrintIdentifier takes string identifier returns nothing
		if (disabledPrintIdentifiers.contains.evaluate(identifier)) then
			return
		endif
		call disabledPrintIdentifiers.pushBack.evaluate(identifier)
	endfunction

	function IsPrintIdentifierEnabled takes string identifier returns boolean
		return not disabledPrintIdentifiers.contains.evaluate(identifier)
	endfunction

	function IsPrintIdentifierDisabled takes string identifier returns boolean
		return disabledPrintIdentifiers.contains.evaluate(identifier)
	endfunction

	function SetPrintIdentifierEnabled takes string identifier, boolean enabled returns nothing
		if (enabled) then
			call EnablePrintIdentifier(identifier)
		else
			call DisablePrintIdentifier(identifier)
		endif
	endfunction

	function EnableAllPrintIdentifiers takes nothing returns nothing
		call disabledPrintIdentifiers.clear()
	endfunction

	function PrintDisabledIdentifiers takes nothing returns integer
		local AStringListIterator iterator = disabledPrintIdentifiers.begin()
		call Print("Disabled identifiers:")
		loop
			exitwhen (not iterator.isValid())
			call Print("- " + iterator.data())
			call iterator.next()
		endloop
		call iterator.destroy()
		call Print("Total: " + I2S(disabledPrintIdentifiers.size()))
		return disabledPrintIdentifiers.size()
	endfunction

	function PrintFunctionError takes string functionName, string message returns nothing
		if (IsPrintIdentifierDisabled(functionName)) then
			return
		endif
		call Print("Function error in function \"" + functionName + "\": " + message)
	endfunction

	function PrintFunctionErrorIf takes boolean condition, string functionName, string message returns boolean
		if (condition) then
			call PrintFunctionError(functionName, message)
		endif
		return condition
	endfunction

	function PrintStructInstanceMessage takes string structName, integer instance, string message returns nothing
		if (IsPrintIdentifierDisabled(structName)) then
			return
		endif
		call Print(structName + " - " + I2S(instance) + ": " + message)
	endfunction

	function PrintStructInstanceMessageIf takes string structName, integer instance, boolean condition, string message returns boolean
		if (condition) then
			call PrintStructInstanceMessage(structName, instance, message)
		endif
		return condition
	endfunction

	function PrintMethodError takes string structName, integer instance, string methodName, string message returns nothing
		if (IsPrintIdentifierDisabled(structName) or IsPrintIdentifierDisabled(structName + "." + methodName)) then
			return
		endif
		call Print(structName + " - " + I2S(instance) + ": Method error in method \"" + methodName + "\": " + message)
	endfunction

	function PrintMethodErrorIf takes string structName, integer instance, boolean condition, string methodName, string message returns boolean
		if (condition) then
			call PrintMethodError(structName, instance, methodName, message)
		endif
		return condition
	endfunction

	function StaticPrint takes string structName, string message returns nothing
		if (IsPrintIdentifierDisabled(structName)) then
			return
		endif
		call Print(structName + ": " + message)
	endfunction

	function StaticPrintIf takes string structName, boolean condition, string message returns boolean
		if (condition) then
			call StaticPrint(structName, message)
		endif
		return condition
	endfunction

	function StaticPrintMethodError takes string structName, string methodName, string message returns nothing
		if (IsPrintIdentifierDisabled(structName) or IsPrintIdentifierDisabled(structName + "." + methodName)) then
			return
		endif
		call Print(structName + ": Method error in method \"" + methodName + "\": " + message)
	endfunction

	function StaticPrintMethodErrorIf takes string structName, boolean condition, string methodName, string message returns boolean
		if (condition) then
			call StaticPrintMethodError(structName, methodName, message)
		endif
		return condition
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
			call PrintStructInstanceMessage($STRUCTNAME$, this, message)
		endmethod

		public method printIf takes boolean condition, string message returns boolean
			return PrintStructInstanceMessageIf($STRUCTNAME$, this, condition, message)
		endmethod

		public method printMethodError takes string methodName, string message returns nothing
			call PrintMethodError($STRUCTNAME$, this, methodName, message)
		endmethod

		public method printMethodErrorIf takes boolean condition, string methodName, string message returns boolean
			return PrintMethodErrorIf($STRUCTNAME$, this, condition, methodName, message)
		endmethod

		private static method staticPrint takes string message returns nothing
			call StaticPrint($STRUCTNAME$, message)
		endmethod

		private static method staticPrintIf takes boolean condition, string message returns boolean
			return StaticPrintIf($STRUCTNAME$, condition, message)
		endmethod

		private static method staticPrintMethodError takes string methodName, string message returns nothing
			call StaticPrintMethodError($STRUCTNAME$, methodName, message)
		endmethod

		private static method staticPrintMethodErrorIf takes boolean condition, string methodName, string message returns boolean
			return StaticPrintMethodErrorIf($STRUCTNAME$, condition, methodName, message)
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
				if (IsPrintIdentifierDisabled($STRUCTNAME$))
				{
					return;
				}

				Print($STRUCTNAME$ + " - " + I2S(this) + ": " + message);
			}

			private static method staticPrint(string message)
			{
				if (IsPrintIdentifierDisabled($STRUCTNAME$))
				{
					return;
				}

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

	/// @todo Should be private, vJass bug.
	function DebugDestroyTrigger takes trigger whichTrigger returns nothing
		/// @todo CHECK IF TRIGGER RUNS
	endfunction

	private function FlushTimer takes timer whichTimer returns nothing
		call AHashTable.global().removeHandleBoolean(whichTimer, "DebugTimerIsPeriodic")
		call AHashTable.global().removeHandleBoolean(whichTimer, "DebugTimerRuns")
		call AHashTable.global().removeHandleTrigger(whichTimer, "DebugTimerTrigger")
	endfunction

	private function TriggerActionTimerExpires takes nothing returns nothing
		if (not AHashTable.global().handleBoolean(GetExpiredTimer(), "DebugTimerIsPeriodic")) then
			call FlushTimer(GetExpiredTimer()) // do not flush everything!
			call DestroyTrigger(GetTriggeringTrigger())
		endif
	endfunction

	function DebugTimerStart takes timer whichTimer, real timeout, boolean periodic, code handlerFunc returns nothing
		local trigger whichTrigger = CreateTrigger()
		call TriggerRegisterTimerExpireEvent(whichTrigger, whichTimer)
		call TriggerAddAction(whichTrigger, function TriggerActionTimerExpires)
		call AHashTable.global().setHandleBoolean(whichTimer, "DebugTimerIsPeriodic", periodic)
		call AHashTable.global().setHandleBoolean(whichTimer, "DebugTimerRuns", true)
		call AHashTable.global().setHandleTrigger(whichTimer, "DebugTimerTrigger", whichTrigger)
		set whichTrigger = null
	endfunction

	function DebugPauseTimer takes timer whichTimer returns nothing
		call AHashTable.global().setHandleBoolean(whichTimer, "DebugTimerRuns", false)
	endfunction

	function DebugResumeTimer takes timer whichTimer returns nothing
		call AHashTable.global().setHandleBoolean(whichTimer, "DebugTimerRuns", true)
	endfunction

	/// @todo Should be private, vJass bug.
	/// - Zerstörte Timer laufen trotz ihrer Zerstörung ab. Man muss einen Timer anhalten bevor man ihn zerstört.
	function DebugDestroyTimer takes timer whichTimer returns nothing
		if (AHashTable.global().handleBoolean(whichTimer, "DebugTimerRuns")) then
			debug call Print("Timer with id " + I2S(GetHandleId(whichTimer)) + " has not been stopped before destroying it.")
			call PauseTimer(whichTimer)
			call DestroyTrigger(AHashTable.global().handleTrigger(whichTimer, "DebugTimerTrigger"))
		endif

		call AHashTable.global().flushHandle(whichTimer)
	endfunction
	
	function DebugShowInterface takes boolean flag, real fadeDuration returns nothing
		if (fadeDuration == 0.0 and flag) then
			debug call PrintFunctionError("ShowInterface", "Don't use 0.0 for fade duration when enabling flag since it will prevent unit portraits from working correctly.")
		endif
	endfunction

/// @todo Seems to prevent map from being able to be started.
static if (DEBUG_MODE and A_DEBUG_NATIVES) then
	hook Player DebugPlayer
	hook InitGameCache DebugInitGameCache
	hook SetUnitX DebugSetUnitX
	hook SetUnitY DebugSetUnitY
	hook CreateUnit DebugCreateUnit
	hook RestoreUnit DebugRestoreUnit
	hook SetImageRender DebugSetImageRender
	hook DestroyTrigger DebugDestroyTrigger
	hook TimerStart DebugTimerStart
	hook PauseTimer DebugPauseTimer
	hook ResumeTimer DebugResumeTimer
	hook DestroyTimer DebugDestroyTimer
	hook ShowInterface DebugShowInterface
endif

	//GroupEnumUnitsInRectCounted und GroupEnumUnitsInRangeCounted
	//- Die Funktionen GroupEnumUnitsInRectCounted und GroupEnumUnitsInRangeCounted können sich unterschiedlich verhalten, falls sie eine große Anzahl von Einheiten haben.
	//GetExpiredTimer
	//- GetExpiredTimer() kann einen Absturtz des Spiels verursachen, wenn es keinen Timer gibt.
	//Einheitengruppen – vom InWarcraft-Nutzer gexxo
	//- Werden Einheiten per RemoveUnit() aus dem Spiel entfernt und befinden sich bereits in einer Einheitengruppe, so kann es zu Fehlern kommen, da die erste Einheit der Gruppe nun immer null ist.

	private function init takes nothing returns nothing
static if (DEBUG_MODE) then
		set disabledPrintIdentifiers = AStringList.create.evaluate()
endif
	endfunction

endlibrary
