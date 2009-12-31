/**
* This library provides some cheats only usable in debug mode to test your map.
* Cheat list:
* version - Displays ASL information.
* clear - Clears screen messages for cheating player.
* info - Shows some information about selected unit of cheating player.
* setlevel - Sets level for selected unit of cheating player.
* kill - Kills selected unit of cheating player.
* copy - Copies selected unit of cheating player.
* giveall - Resets hit points, mana and all ability cooldowns of selected unit of cheating player.
* damage - Damages selected unit.
* benchmarks - Shows all benchmarks.
* clearbenchmarks - Clears all benchmarks.
* units - Shows all units.
* items - Shows all items.
* destructables - Shows all destructables.
* string - Runs string debug.
* interface - Runs interface debug.
* signal - Runs signal debug.
* list - Runs list debug.
* @todo Causes crash in debug mode!!!
*/
library ALibrarySystemsDebugUtilities initializer initFunction requires AStructCoreDebugBenchmark, AStructCoreDebugCheat, ALibraryCoreDebugInterface, ALibraryCoreDebugList, ALibraryCoreDebugMisc, ALibraryCoreDebugSignal, ALibraryCoreDebugString, ALibraryCoreGeneralUnit, ALibraryCoreStringConversion, ALibraryCoreInterfaceSelection

	private function help takes nothing returns nothing
		local player triggerPlayer = GetTriggerPlayer()
static if (DEBUG_MODE) then
		call Print("version")
		call Print("clear")
		call Print("info")
		call Print("setlevel")
		call Print("kill")
		call Print("copy")
		call Print("giveall")
		call Print("damage <damage>")
		call Print("benchmarks")
		call Print("clearbenchmarks")
endif
static if (DEBUG_MODE and A_DEBUG_HANDLES) then
		call Print("units")
		call Print("items")
		call Print("destructables")
endif
static if (DEBUG_MODE) then
		call Print("string")
		call Print("interface")
		call Print("signal")
		call Print("list")
endif
		set triggerPlayer = null
	endfunction

	private function showVersion takes nothing returns nothing
static if (DEBUG_MODE) then
		call Asl.showInformation()
endif
	endfunction

	private function clear takes nothing returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		call ClearScreenMessagesForPlayer(triggerPlayer)
		set triggerPlayer = null
	endfunction

	/// @todo Add some information.
	private function info takes nothing returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		if (selectedUnit != null) then
static if (DEBUG_MODE) then
			call Print(StringArg(tr("Name: %s"), GetUnitName(selectedUnit)))
			call Print(RealArg(RealArg(tr("Position: (%r | %r)"), GetUnitX(selectedUnit)), GetUnitY(selectedUnit)))
			call Print(IntegerArg(tr("Level: %i"), GetUnitLevel(selectedUnit)))
			call Print(RealArg(tr("Acquire Range: %r"), GetUnitAcquireRange(selectedUnit)))
			call Print(IntegerArg(tr("Current Order: %i"), GetUnitCurrentOrder(selectedUnit)))
endif
			if (IsUnitType(selectedUnit, UNIT_TYPE_HERO)) then
static if (DEBUG_MODE) then
				call Print(tr("--------- Hero Data ---------"))
				call Print(IntegerArg(tr("Level: %i"), GetHeroLevel(selectedUnit)))
				call Print(IntegerArg(tr("Experience: %i"), GetHeroXP(selectedUnit)))
				call Print(IntegerArg(tr("Skill points: %i"), GetHeroSkillPoints(selectedUnit)))
				call Print(IntegerArg(IntegerArg(tr("Strength: %i + %i"), GetHeroStr(selectedUnit, false)), GetHeroStrBonus(selectedUnit)))
				call Print(IntegerArg(IntegerArg(tr("Agility: %i + %i"), GetHeroAgi(selectedUnit, false)), GetHeroAgiBonus(selectedUnit)))
				call Print(IntegerArg(IntegerArg(tr("Intelligence: %i + %i"), GetHeroInt(selectedUnit, false)), GetHeroIntBonus(selectedUnit)))
endif
			endif
			set selectedUnit = null
		endif
		set triggerPlayer = null
	endfunction

	private function setlevel takes nothing returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local string message = GetEventPlayerChatString()
		local unit hero = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		if (hero != null) then
			if (IsUnitType(hero, UNIT_TYPE_HERO)) then
				call SetHeroLevelBJ(hero, S2I(SubString(message, StringLength("setlevel") + 1, StringLength(message))), true)
			else
				debug call Print("Unit is not a hero.")
			endif
			set hero = null
		endif
		set triggerPlayer = null
	endfunction

	/// @todo If no unit is selected, kill an item or a destructable
	private function kill takes nothing returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		if (selectedUnit != null) then
			call KillUnit(selectedUnit)
			set selectedUnit = null
		endif
		set triggerPlayer = null
	endfunction

	private function copy takes nothing returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		local unit copiedUnit
		if (selectedUnit != null) then
			set copiedUnit = CopyUnit(selectedUnit, GetUnitX(selectedUnit), GetUnitY(selectedUnit), GetUnitFacing(selectedUnit), bj_UNIT_STATE_METHOD_ABSOLUTE)
			set copiedUnit = null
		endif
		set triggerPlayer = null
	endfunction

	private function giveall takes nothing returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		if (selectedUnit != null) then
			call SetUnitLifePercentBJ(selectedUnit, 100.0)
			call SetUnitManaPercentBJ(selectedUnit, 100.0)
			call UnitResetCooldown(selectedUnit)
			set selectedUnit = null
		endif
		set triggerPlayer = null
	endfunction

	private function damage takes nothing returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		local real damageAmount
		if (selectedUnit != null) then
			set damageAmount = S2R(SubString(GetEventPlayerChatString(), StringLength("damage"), StringLength(GetEventPlayerChatString())))
			debug call Print("Damage amount is " + R2S(damageAmount) + ".")
			if (damageAmount > 0.0) then
				call UnitDamageTargetBJ(selectedUnit, selectedUnit, damageAmount, ATTACK_TYPE_NORMAL, DAMAGE_TYPE_NORMAL)
			endif
			set selectedUnit = null
		endif
		set triggerPlayer = null
	endfunction

	private function benchmarks takes nothing returns nothing
		call ABenchmark.showBenchmarks()
	endfunction

	private function clearbenchmarks takes nothing returns nothing
		//call ABenchmark.clearAll()
	endfunction

static if (A_DEBUG_HANDLES) then
	private function units takes nothing returns nothing
		call ABenchmark.showUnits()
	endfunction

	private function items takes nothing returns nothing
		call ABenchmark.showItems()
	endfunction

	private function destructables takes nothing returns nothing
		call ABenchmark.showDestructables()
	endfunction
endif

	private function initFunction takes nothing returns nothing
static if (DEBUG_MODE) then
		call ACheat.create("help", true, help)
		call ACheat.create("version", true, showVersion)
		call ACheat.create("clear", true, clear)
		call ACheat.create("info", true, info)
		call ACheat.create("setlevel", false, setlevel)
		call ACheat.create("kill", true, kill)
		call ACheat.create("copy", true, copy)
		call ACheat.create("giveall", true, giveall)
		call ACheat.create("damage", false, damage)
		call ACheat.create("benchmarks", true, benchmarks)
		call ACheat.create("clearbenchmarks", true, clearbenchmarks)
endif

static if (A_DEBUG_HANDLES) then
		call ACheat.create("units", true, units)
		call ACheat.create("items", true, items)
		call ACheat.create("destructables", true, destructables)
endif

static if (DEBUG_MODE) then
		call ACheat.create("string", true, AStringDebug)
		call ACheat.create("interface", true, AInterfaceDebug)
		call ACheat.create("signal", true, ASignalDebug)
		call ACheat.create("list", true, AListDebug)
endif
	endfunction

endlibrary