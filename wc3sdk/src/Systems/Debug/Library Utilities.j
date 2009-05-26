/// This library provides some cheats only usable in debug mode to test your map.
/// @param clear Clears the screen messages for the cheating player.
/// @param info Shows some information about the selected unit of the cheating player.
/// @param setlevel Sets the level of the selected unit of the cheating player.
/// @param kill Kills the selected unit of the cheating player.
/// @param giveall Resets hit points, mana and all ability cooldowns of the selected unit of the cheating player.
/// @param container Runs the container debug.
/// @param string Runs the string debug.
/// @param interface Runs the interface debug.
library ALibrarySystemsDebugUtilities initializer init requires AStructCoreDebugCheat, ALibraryCoreDebugMisc, ALibraryCoreStringConversion, ALibraryCoreInterfaceSelection

	private function help takes nothing returns nothing
		debug local player triggerPlayer = GetTriggerPlayer()
		debug call Print("clear")
		debug call Print("info")
		debug call Print("setlevel")
		debug call Print("kill")
		debug call Print("giveall")
		debug call Print("container")
		debug call Print("string")
		debug call Print("interface")
		debug set triggerPlayer = null
	endfunction

	private function clear takes nothing returns nothing
		debug local player triggerPlayer = GetTriggerPlayer()
		debug call ClearScreenMessagesForPlayer(triggerPlayer)
		debug set triggerPlayer = null
	endfunction

	/// @todo Add some information.
	private function info takes nothing returns nothing
		debug local player triggerPlayer = GetTriggerPlayer()
		debug local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer) //ALibraryInterfaceSelection
		debug if (selectedUnit != null) then
			debug call Print(StringArg(tr("Name: %s"), GetUnitName(selectedUnit)))
			debug call Print(RealArg(RealArg(tr("Position: (%r|%r)"), GetUnitX(selectedUnit)), GetUnitY(selectedUnit)))
			debug call Print(IntegerArg(tr("Level: %i"), GetUnitLevel(selectedUnit)))
			debug if (IsUnitType(selectedUnit, UNIT_TYPE_HERO)) then
				debug call Print(IntegerArg(tr("Hero Level: %i"), GetHeroLevel(selectedUnit)))
			debug endif
			debug call Print(RealArg(tr("Acquire Range: %r"), GetUnitAcquireRange(selectedUnit)))
			debug call Print(IntegerArg(tr("Current Order: %i"), GetUnitCurrentOrder(selectedUnit)))
			debug set selectedUnit = null
		debug endif
		debug set triggerPlayer = null
	endfunction

	private function setlevel takes nothing returns nothing
		debug local player triggerPlayer = GetTriggerPlayer()
		debug local string message = GetEventPlayerChatString()
		debug local unit hero = GetFirstSelectedUnitOfPlayer(triggerPlayer) //ALibraryInterfaceSelection
		debug if (hero != null) then
			debug if (IsUnitType(hero, UNIT_TYPE_HERO)) then
				debug call SetHeroLevelBJ(hero, S2I(SubString(message, StringLength("setlevel") + 1, StringLength(message))), true)
			debug else
				debug call Print("Unit is not a hero.")
			debug endif
			debug set hero = null
		debug endif
		debug set triggerPlayer = null
	endfunction

	/// @todo If no unit is selected, kill an item or a destructable
	private function kill takes nothing returns nothing
		debug local player triggerPlayer = GetTriggerPlayer()
		debug local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer) //ALibraryInterfaceSelection
		debug if (selectedUnit != null) then
			debug call KillUnit(selectedUnit)
			debug set selectedUnit = null
		debug endif
		debug set triggerPlayer = null
	endfunction
	
	private function giveall takes nothing returns nothing
		debug local player triggerPlayer = GetTriggerPlayer()
		debug local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer) //ALibraryInterfaceSelection
		debug if (selectedUnit != null) then
			debug call SetUnitLifePercentBJ(selectedUnit, 100.0)
			debug call SetUnitManaPercentBJ(selectedUnit, 100.0)
			debug call UnitResetCooldown(selectedUnit)
			debug set selectedUnit = null
		debug endif
		debug set triggerPlayer = null
	endfunction
	
	private function init takes nothing returns nothing
		debug call ACheat.create("help", true, help)
		debug call ACheat.create("clear", true, clear)
		debug call ACheat.create("info", true, info)
		debug call ACheat.create("setlevel", false, setlevel)
		debug call ACheat.create("kill", true, kill)
		debug call ACheat.create("giveall", true, giveall)
		debug call ACheat.create("container", true, GetContainerDebug)
		debug call ACheat.create("string", true, GetStringDebug)
		debug call ACheat.create("interface", true, GetInterfaceDebug)
	endfunction

endlibrary