/**
* This library provides some cheats only usable in debug mode to test your map.
* Cheat list:
* version - Displays ASL information.
* clear - Clears screen messages for cheating player.
* gold - Gives player gold or shows his current amount.
* lumber - Gives player lumber or shows his current amount.
* foodcap - Gives player foot capacity or shows his current amount.
* info - Shows some information about selected unit of cheating player.
* setlevel - Sets level for selected unit of cheating player.
* kill - Kills selected unit of cheating player.
* copy - Copies selected unit of cheating player.
* giveall - Resets hit points, mana and all ability cooldowns of selected unit of cheating player.
* damage - Damages selected unit.
* xp - Adds experience to selected hero or shows experience information about selected unit.
* pathing - Enables unit's pathing.
* nopathing - Disables unit's pathing.
* order - Displays unit order information or issues unit order.
* timeofday - Suspends or continue time of day or shows current.
* benchmarks - Shows all benchmarks.
* clearbenchmarks - Clears all benchmarks.
* enable - Enables debug identifier(s).
* disable - Disables debug identifier(s) or shows all disabled.
* units - Shows all units.
* items - Shows all items.
* destructables - Shows all destructables.
* string - Runs string debug.
* interface - Runs interface debug.
* signal - Runs signal debug.
* list - Runs list debug.
* map - Runs map debug.
* @todo Causes crash in debug mode!!!
*/
library ALibrarySystemsDebugUtilities requires AStructCoreDebugBenchmark, AStructCoreDebugCheat, ALibraryCoreDebugInterface, ALibraryCoreDebugList, ALibraryCoreDebugMap, ALibraryCoreDebugMisc, ALibraryCoreDebugSignal, ALibraryCoreDebugString, ALibraryCoreEnvironmentUnit, ALibraryCoreGeneralUnit, ALibraryCoreStringConversion, ALibraryCoreInterfaceSelection

	private function help takes ACheat cheat returns nothing
		local player triggerPlayer = GetTriggerPlayer()
static if (DEBUG_MODE) then
		call Print("version")
		call Print("clear")
		call Print("gold <gold>")
		call Print("lumber <lumber>")
		call Print("foodcap <food capacity>")
		call Print("info")
		call Print("setlevel")
		call Print("kill")
		call Print("copy")
		call Print("giveall")
		call Print("damage <damage>")
		call Print("xp <experience>")
		call Print("pathing")
		call Print("nopathing")
		call Print("order <order>")
		call Print("timeofday <stop/continue>")
		call Print("benchmarks")
		call Print("clearbenchmarks")
		call Print("enable <identifier>")
		call Print("disable <identifier>")
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
		call Print("map")
endif
		set triggerPlayer = null
	endfunction

	private function showVersion takes ACheat cheat returns nothing
static if (DEBUG_MODE) then
		call Asl.showInformation()
endif
	endfunction

	private function clear takes ACheat cheat returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		call ClearScreenMessagesForPlayer(triggerPlayer)
		set triggerPlayer = null
	endfunction

	private function playerState takes string playerStateName, player whichPlayer, integer amount, playerstate whichPlayerState returns nothing
		if (amount == -1) then
			debug call Print(IntegerArg(StringArg(tr("You have %i %s."), playerStateName), GetPlayerState(whichPlayer, whichPlayerState)))
		else
			call AdjustPlayerStateBJ(amount, whichPlayer, whichPlayerState)
			debug call Print(StringArg(IntegerArg(tr("Added %i %s."), amount), playerStateName))
		endif
	endfunction

	private function gold takes ACheat cheat returns nothing
		local string amountString = cheat.argument()
		local integer amount
		if (StringLength(amountString) == 0) then
			set amount = -1
		else
			set amount = S2I(amountString)
		endif
		call playerState(tr("gold"), GetTriggerPlayer(), amount, PLAYER_STATE_RESOURCE_GOLD)
	endfunction

	private function lumber takes ACheat cheat returns nothing
		local string amountString = cheat.argument()
		local integer amount
		if (StringLength(amountString) == 0) then
			set amount = -1
		else
			set amount = S2I(amountString)
		endif
		call playerState(tr("lumber"), GetTriggerPlayer(), amount, PLAYER_STATE_RESOURCE_LUMBER)
	endfunction

	private function foodcap takes ACheat cheat returns nothing
		local string amountString = cheat.argument()
		local integer amount
		if (StringLength(amountString) == 0) then
			set amount = -1
		else
			set amount = S2I(amountString)
		endif
		call playerState(tr("food capacity"), GetTriggerPlayer(), amount, PLAYER_STATE_RESOURCE_FOOD_CAP)
	endfunction

	/// @todo Add some information.
	private function info takes ACheat cheat returns nothing
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

	private function setlevel takes ACheat cheat returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local string message = GetEventPlayerChatString()
		local unit hero = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		local integer level
		local boolean suspend
		if (hero != null) then
			if (IsUnitType(hero, UNIT_TYPE_HERO)) then
				set level = S2I(cheat.argument())
				set suspend = IsSuspendedXP(hero)
				if (suspend) then
					call SuspendHeroXP(hero, false)
				endif
				call SetHeroLevelBJ(hero, level, true)
				if (suspend) then
					call SuspendHeroXP(hero, true)
				endif
				debug call Print(IntegerArg(tr("Set level to %i."), level))
			debug else
				debug call Print(tr("Selected unit is not a hero."))
			endif
			set hero = null
		endif
		set triggerPlayer = null
	endfunction

	/// @todo If no unit is selected, kill an item or a destructable
	private function kill takes ACheat cheat returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		if (selectedUnit != null) then
			call KillUnit(selectedUnit)
			set selectedUnit = null
		endif
		set triggerPlayer = null
	endfunction

	private function copy takes ACheat cheat returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		local unit copiedUnit
		if (selectedUnit != null) then
			set copiedUnit = CopyUnit(selectedUnit, GetUnitX(selectedUnit), GetUnitY(selectedUnit), GetUnitFacing(selectedUnit), bj_UNIT_STATE_METHOD_ABSOLUTE)
			debug call Print(tr("Copying unit."))
			set copiedUnit = null
		endif
		set triggerPlayer = null
	endfunction

	private function giveall takes ACheat cheat returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		if (selectedUnit != null) then
			call SetUnitLifePercentBJ(selectedUnit, 100.0)
			call SetUnitManaPercentBJ(selectedUnit, 100.0)
			call UnitResetCooldown(selectedUnit)
			debug call Print(tr("Gave all."))
			set selectedUnit = null
		endif
		set triggerPlayer = null
	endfunction

	private function damage takes ACheat cheat returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		local real damageAmount
		if (selectedUnit != null) then
			set damageAmount = S2R(cheat.argument())
			debug call Print("Damage amount is " + R2S(damageAmount) + ".")
			if (damageAmount > 0.0) then
				call UnitDamageTargetBJ(selectedUnit, selectedUnit, damageAmount, ATTACK_TYPE_NORMAL, DAMAGE_TYPE_NORMAL)
			endif
			set selectedUnit = null
		endif
		set triggerPlayer = null
	endfunction

	private function xp takes ACheat cheat returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		local integer experience
		local boolean suspend
		if (selectedUnit != null) then
			set experience = S2I(cheat.argument())
			if (experience != 0) then
				if (IsUnitType(selectedUnit, UNIT_TYPE_HERO)) then
					set suspend = IsSuspendedXP(selectedUnit)
					if (suspend) then
						call SuspendHeroXP(selectedUnit, false)
					endif
					call AddHeroXP(selectedUnit, experience, true)
					if (suspend) then
						call SuspendHeroXP(selectedUnit, true)
					endif
					debug call Print(IntegerArg(tr("Added %i experience."), experience))
				else
					debug call Print(tr("Selected unit is not a hero."))
				endif
			elseif (IsUnitType(selectedUnit, UNIT_TYPE_HERO)) then
				debug call Print(IntegerArg(IntegerArg(tr("%i/%i experience."), GetHeroXP(selectedUnit)), GetHeroMaxXP(selectedUnit)))
				debug call Print(IntegerArg(tr("%i unit experience."), GetUnitXP(selectedUnit)))
			else
				debug call Print(IntegerArg(tr("%i unit experience."), GetUnitXP(selectedUnit)))
			endif
			set selectedUnit = null
		endif
		set triggerPlayer = null
	endfunction

	private function pathing takes ACheat cheat returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		if (selectedUnit != null) then
			call SetUnitPathing(selectedUnit, true)
			debug call Print(tr("Enabled unit pathing."))
			set selectedUnit = null
		endif
		set triggerPlayer = null
	endfunction

	private function nopathing takes ACheat cheat returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(triggerPlayer)
		if (selectedUnit != null) then
			call SetUnitPathing(selectedUnit, false)
			debug call Print(tr("Disabled unit pathing."))
			set selectedUnit = null
		endif
		set triggerPlayer = null
	endfunction

	private function order takes ACheat cheat returns nothing
		local unit selectedUnit = GetFirstSelectedUnitOfPlayer(GetTriggerPlayer())
		local string orderString
		if (selectedUnit != null) then
			set orderString = cheat.argument()
			if (orderString == null) then
				debug call Print(StringArg(StringArg(tr("%s's order: %s"), GetUnitName(selectedUnit)), OrderId2String(GetUnitCurrentOrder(selectedUnit))))
			elseif (IsStringAlphabetical(orderString)) then
				debug call Print(StringArg(StringArg(tr("Order %s: %s"), GetUnitName(selectedUnit)), orderString))
				call IssueImmediateOrder(selectedUnit, orderString)
			else
				debug call Print(StringArg(StringArg(tr("Order %s: %s"), GetUnitName(selectedUnit)), OrderId2String(S2I(orderString))))
				call IssueImmediateOrderById(selectedUnit, S2I(orderString))
			endif
		else
			debug call Print(tr("No unit is selected."))
		endif
	endfunction

	private function timeofday takes ACheat cheat returns nothing
		local string argument = cheat.argument()
		if (argument == "stop") then
			call SuspendTimeOfDay(true)
		elseif (argument == "continue") then
			call SuspendTimeOfDay(false)
		else
			debug call Print(StringArg(tr("Current time of day: %s"), GetTimeOfDayString()))
			debug call Print("Test, elapsed hours: " + I2S(GetTimeOfDayElapsedHours()))
			debug call Print("Test, elapsed minutes in hour: " + I2S(GetTimeOfDayElapsedMinutesInHour()))
			debug call Print("Test, remaining hours: " + I2S(GetTimeOfDayRemainingHours()))
			debug call Print("Test, elapsed minutes: " + I2S(GetTimeOfDayElapsedMinutes()))
		endif
	endfunction

	private function benchmarks takes ACheat cheat returns nothing
		debug call Print(tr("Showing all benchmarks:"))
		call ABenchmark.showBenchmarks()
	endfunction

	private function clearbenchmarks takes ACheat cheat returns nothing
		debug call Print(tr("Clearing all benchmarks."))
		call ABenchmark.clearAll()
	endfunction

	private function enable takes ACheat cheat returns nothing
static if (DEBUG_MODE) then
		local string identifier = cheat.argument()
		if (StringLength(identifier) == 0) then
			call Print(tr("Enabling all identifiers."))
			call EnableAllPrintIdentifiers()
		elseif (IsPrintIdentifierDisabled(identifier)) then
			call Print(StringArg(tr("Enabling identifier \"%s\"."), identifier))
			call EnablePrintIdentifier(identifier)
		else
			call Print(StringArg(tr("Identifier \"%s\" is not disabled."), identifier))
		endif
endif
	endfunction

	private function disable takes ACheat cheat returns nothing
static if (DEBUG_MODE) then
		local string identifier = cheat.argument()
		if (StringLength(identifier) == 0) then
			call PrintDisabledIdentifiers()
		elseif (IsPrintIdentifierEnabled(identifier)) then
			call Print(StringArg(tr("Disabling identifier \"%s\"."), identifier))
		else
			call Print(StringArg(tr("Identifier \"%s\" is not enabled."), identifier))
		endif
endif
	endfunction

static if (A_DEBUG_HANDLES) then
	private function units takes ACheat cheat returns nothing
		call ABenchmark.showUnits()
	endfunction

	private function items takes ACheat cheat returns nothing
		call ABenchmark.showItems()
	endfunction

	private function destructables takes ACheat cheat returns nothing
		call ABenchmark.showDestructables()
	endfunction
endif

static if (DEBUG_MODE) then
	private function stringDebug takes ACheat cheat returns nothing
		call AStringDebug()
	endfunction

	private function interfaceDebug takes ACheat cheat returns nothing
		call AInterfaceDebug()
	endfunction

	private function signalDebug takes ACheat cheat returns nothing
		call ASignalDebug()
	endfunction

	private function listDebug takes ACheat cheat returns nothing
		call AListDebug()
	endfunction

	private function mapDebug takes ACheat cheat returns nothing
		call AMapDebug()
	endfunction
endif

	function AInitUtilityCheats takes nothing returns nothing
static if (DEBUG_MODE) then
		call ACheat.create("help", true, help)
		call ACheat.create("version", true, showVersion)
		call ACheat.create("clear", true, clear)
		call ACheat.create("gold", false, gold)
		call ACheat.create("lumber", false, lumber)
		call ACheat.create("foodcap", false, foodcap)
		call ACheat.create("info", true, info)
		call ACheat.create("setlevel", false, setlevel)
		call ACheat.create("kill", true, kill)
		call ACheat.create("copy", true, copy)
		call ACheat.create("giveall", true, giveall)
		call ACheat.create("damage", false, damage)
		call ACheat.create("xp", false, xp)
		call ACheat.create("pathing", true, pathing)
		call ACheat.create("nopathing", true, nopathing)
		call ACheat.create("order", false, order)
		call ACheat.create("timeofday", true, timeofday)
		call ACheat.create("benchmarks", true, benchmarks)
		call ACheat.create("clearbenchmarks", true, clearbenchmarks)
		call ACheat.create("enable", false, enable)
		call ACheat.create("disable", false, disable)
endif

static if (A_DEBUG_HANDLES) then
		call ACheat.create("units", true, units)
		call ACheat.create("items", true, items)
		call ACheat.create("destructables", true, destructables)
endif

static if (DEBUG_MODE) then
		call ACheat.create("string", true, stringDebug)
		call ACheat.create("interface", true, interfaceDebug)
		call ACheat.create("signal", true, signalDebug)
		call ACheat.create("list", true, listDebug)
		call ACheat.create("map", true, mapDebug)
endif
	endfunction

endlibrary