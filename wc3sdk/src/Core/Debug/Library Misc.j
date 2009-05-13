library ALibraryCoreDebugMisc requires ALibraryCoreGeneralPlayer

	/// Displays text on the screen if the debug mode is enabled.
	/// DebugMsg: Your Message - Echoes Your Message to logs//war3erruser.txt
	/// Ursprünglich war es eine Platzhalter-Funktion.
	/// @author Tamino Dauth
	/// @param message The message which will be shown on the screen and if Warcraft is started with Grimore it prints the messag into logs//war3err.txt.
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

	/// Useful for getting the causing struct and instance of the debug message.
	/// Displays the string @param message in the following schema: "<structname> - <instanceid>: <message>"
	/// @author Tamino Dauth
	//! textmacro A_STRUCT_DEBUG takes STRUCTNAME
		debug private method print takes string message returns nothing
			debug call Print($STRUCTNAME$ + " - " + I2S(this) + ": " + message)
		debug endmethod

		debug private static method staticPrint takes string message returns nothing
			debug call Print($STRUCTNAME$ + ": " + message)
		debug endmethod
	//! endtextmacro

	/// war3err DumpGlobalHT - prints the names and values of all global variables to logs//war3err.txt
	/// @author Tamino Dauth
	debug function PrintGlobals takes nothing returns nothing
		debug call Cheat("war3err_DumpGlobalHT") //JNGP
	debug endfunction

	/// war3err DumpLocalHT - As above, but for the local variables of the current scope
	/// @author Tamino Dauth
	debug function PrintLocals takes nothing returns nothing
		debug call Cheat("war3err_DumpLocalHT") //JNGP
	debug endfunction

	/// PauseTracer: If bytecode logging is enabled, this pauses recording
	/// @author Tamino Dauth
	debug function PauseBytecodeLogging takes nothing returns nothing
		debug call Cheat("PauseTracer") //JNGP
	debug endfunction

	/// ContinueTracer: If bytecode logging is enabled, this resumes recording
	/// @author Tamino Dauth
	debug function ContinueBytecodeLogging takes nothing returns nothing
		debug call Cheat("ContinueTracer") //JNGP
	debug endfunction
	
	/// When the breakpoint is reached Warcraft 3 is frozen.
	/// @author Tamino Dauth
	debug function Breakpoint takes nothing returns nothing
		debug call Cheat("war3err_Break") //JNGP
	debug endfunction

endlibrary
