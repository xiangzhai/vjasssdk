/**
* This library provides some cheats only usable in debug mode to test your map.
* Cheat list:
* clear - Clears screen messages for cheating player.
* info - Shows some information about selected unit of cheating player.
* setlevel - Sets level for selected unit of cheating player.
* kill - Kills selected unit of cheating player.
* copy - Copies selected unit of cheating player.
* giveall - Resets hit points, mana and all ability cooldowns of selected unit of cheating player.
* damage - Damages selected unit.
* benchmarks - Shows all benchmarks.
* units - Shows all units.
* items - Shows all items.
* destructables - Shows all destructables.
* string - Runs string debug.
* interface - Runs interface debug.
* signal - Runs signal debug.
*/
library ALibrarySystemsDebugUtilities initializer init requires AStructCoreDebugBenchmark, AStructCoreDebugCheat, ALibraryCoreDebugInterface, ALibraryCoreDebugMisc, ALibraryCoreDebugSignal, ALibraryCoreDebugString, ALibraryCoreStringConversion, ALibraryCoreInterfaceSelection

	private function help takes nothing returns nothing
		debug local player triggerPlayer = GetTriggerPlayer()
		debug call Print("clear")
		debug call Print("info")
		debug call Print("setlevel")
		debug call Print("kill")
		debug call Print("copy")
		debug call Print("giveall")
		debug call Print("damage <damage>")
		debug call Print("benchmarks")
static if (A_DEBUG_HANDLES) then
		debug call Print("units")
		debug call Print("items")
		debug call Print("destructables")
endif
		debug call Print("string")
		debug call Print("interface")
		debug call Print("signal")
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
		debug local unit hero = GetFirstSelectedUnitOfPlayer(triggerPlayer)
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
		debug local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		debug if (selectedUnit != null) then
			debug call KillUnit(selectedUnit)
			debug set selectedUnit = null
		debug endif
		debug set triggerPlayer = null
	endfunction

	private function copy takes nothing returns nothing
		debug local player triggerPlayer = GetTriggerPlayer()
		debug local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		debug local unit copiedUnit
		debug if (selectedUnit != null) then
			debug set copiedUnit = CopyUnit(selectedUnit, GetUnitX(selectedUnit), GetUnitY(selectedUnit), GetUnitFacing(selectedUnit), bj_UNIT_STATE_METHOD_ABSOLUTE)
			debug set copiedUnit = null
		debug endif
		debug set triggerPlayer = null
	endfunction

	private function giveall takes nothing returns nothing
		debug local player triggerPlayer = GetTriggerPlayer()
		debug local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		debug if (selectedUnit != null) then
			debug call SetUnitLifePercentBJ(selectedUnit, 100.0)
			debug call SetUnitManaPercentBJ(selectedUnit, 100.0)
			debug call UnitResetCooldown(selectedUnit)
			debug set selectedUnit = null
		debug endif
		debug set triggerPlayer = null
	endfunction

	private function damage takes nothing returns nothing
		debug local player triggerPlayer = GetTriggerPlayer()
		debug local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		debug local real damageAmount
		debug if (selectedUnit != null) then
			debug set damageAmount = S2R(SubString(GetEventPlayerChatString(), StringLength("damage"), StringLength(GetEventPlayerChatString())))
			debug call Print("SubString is " + SubString(GetEventPlayerChatString(), StringLength("damage"), StringLength(GetEventPlayerChatString())))
			debug if (damageAmount > 0.0) then
				debug call UnitDamageTargetBJ(selectedUnit, selectedUnit, damageAmount, ATTACK_TYPE_NORMAL, DAMAGE_TYPE_NORMAL)
			debug endif
			debug set selectedUnit = null
		debug endif
		debug set triggerPlayer = null
	endfunction

	private function benchmarks takes nothing returns nothing
		debug call ABenchmark.showBenchmarks()
	endfunction

static if (A_DEBUG_HANDLES) then
	private function units takes nothing returns nothing
		debug call ABenchmark.showUnits()
	endfunction

	private function items takes nothing returns nothing
		debug call ABenchmark.showItems()
	endfunction

	private function destructables takes nothing returns nothing
		debug call ABenchmark.showDestructables()
	endfunction
endif

	private function init takes nothing returns nothing
		debug call ACheat.create("help", true, help)
		debug call ACheat.create("clear", true, clear)
		debug call ACheat.create("info", true, info)
		debug call ACheat.create("setlevel", false, setlevel)
		debug call ACheat.create("kill", true, kill)
		debug call ACheat.create("copy", true, copy)
		debug call ACheat.create("giveall", true, giveall)
		debug call ACheat.create("damage", false, damage)
		debug call ACheat.create("benchmarks", true, benchmarks)

static if (A_DEBUG_HANDLES) then
		debug call ACheat.create("units", true, units)
		debug call ACheat.create("items", true, items)
		debug call ACheat.create("destructables", true, destructables)
endif

		debug call ACheat.create("string", true, AStringDebug)
		debug call ACheat.create("interface", true, AInterfaceDebug)
		debug call ACheat.create("signal", true, ASignalDebug)
	endfunction

endlibrary