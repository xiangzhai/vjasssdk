/// Provides several functions for unit handling.
/// @author Tamino Dauth
library ALibraryCoreEnvironmentUnit requires ALibraryCoreMathsReal, AStructCoreGeneralHashTable

	/**
	* Adds or removes the move ability to unit \p whichUnit.
	* \author Tamino Dauth
	* \param whichUnit Unit which the ability is added to.
	* \param movable If this value is true the ability will be added to unit \p whichUnit, otherwise it will be removed.
	*/
	function MakeUnitMovable takes unit whichUnit, boolean movable returns nothing
		if (movable) then
			call UnitAddAbility(whichUnit, 'Amov')
		else
			call UnitRemoveAbility(whichUnit, 'Amov')
		endif
	endfunction

	/**
	* Adds or removes the attack ability to unit \p whichUnit.
	* @author Tamino Dauth
	* \param whichUnit Unit which the ability is added to.
	* \param attackable If this value is true the ability will be added to unit \p whichUnit, otherwise it will be removed.
	*/
	function MakeUnitAttackable takes unit whichUnit, boolean attackable returns nothing
		if (attackable) then
			call UnitAddAbility(whichUnit, 'Aatk')
		else
			call UnitRemoveAbility(whichUnit, 'Aatk')
		endif
	endfunction

	/// @todo Doesn't work.
	/// @author Tamino Dauth
	/// @return Returns true if unit @param usedUnit is invulnerable.
	function IsUnitInvulnerable takes unit usedUnit returns boolean
		return (GetUnitAbilityLevel(usedUnit, 'Bvul') > 0)
	endfunction

	/**
	* \todo Function does not support all alliance states (only \ref bj_ALLIANCE_NEUTRAL, \ref bj_ALLIANCE_ALLIED and \ref bj_ALLIANCE_UNALLIED).
	* \author Tamino Dauth
	* \return Returns the alliance state of the two unit's owners.
	*/
	function GetUnitAllianceStateToUnit takes unit usedUnit, unit otherUnit returns integer
		local player usedUnitOwner = GetOwningPlayer(usedUnit)
		local player otherUnitOwner = GetOwningPlayer(otherUnit)
		local integer allianceState = -1
		if (IsPlayerAlly(usedUnitOwner, otherUnitOwner)) then
			if (GetPlayerAlliance(usedUnitOwner, otherUnitOwner, ALLIANCE_PASSIVE)) then
				set allianceState = bj_ALLIANCE_ALLIED
			else
				set allianceState = bj_ALLIANCE_NEUTRAL
			endif
		else
			set allianceState = bj_ALLIANCE_UNALLIED
		endif
		set usedUnitOwner = null
		set otherUnitOwner = null
		return allianceState
	endfunction

	/**
	* \author Anitarf
	*/
	function IsUnitSpellResistant takes unit u returns boolean
		local player owner = GetOwningPlayer(u)
		local boolean result = IsUnitType(u, UNIT_TYPE_HERO) or IsUnitType(u, UNIT_TYPE_RESISTANT) or (GetPlayerId(owner) >= PLAYER_NEUTRAL_AGGRESSIVE and GetUnitLevel(u) >= 6) //the level at which creeps gain spell resistance
		set owner = null
		return result
	endfunction

	/// \author Anitarf
	function IsUnitSpellImmune takes unit u returns boolean
		return IsUnitType(u, UNIT_TYPE_MAGIC_IMMUNE)
	endfunction

	/**
	* Other functions available for use are GetFullDamage and GetReducedDamage.
	* GetFullDamage, when passed the actual damage a unit takes (In most cases,
	* GetEventDamage from EVENT_UNIT_DAMAGED event callbacks) and a unit's armor,
	* it will return how much damage was dealt before armor reduction. Similarly,
	* GetReducedDamage, when given the base damage and armor, will return how much
	* damage will be dealt after armor is considered. These functions DO NOT
	* consider armor types in their calculations, so any further reductions or
	* bonuses due to that will need to be considered BEFORE using these functions.
	* I recommend using your damage detection system to modify and build your own
	* armor types anyways.
	*
	* You can use the ObjectMerger call below in order to generate the ability for
	* keeping units with maximum life lower than DAMAGE_TEST from dying when
	* using GetUnitArmor on them. If you do not plan on editing the 'AIlz' ability
	* in your map, you can keep the ObjectMerger call commented out and replace
	* 'lif&' in the configuration constants with 'AIlz'. The 'AIlz' ability adds
	* 50 max life, which is plenty for the script.
	*
	* Function Listing --
	* function GetUnitArmor takes unit u returns real
	* function GetReducedDamage takes real baseDamage, real armor returns real
	* function GetFullDamage takes real damage, real armor returns real
	*/
	globals
		// Values that should be changed for your map
		/// @author Rising_Dusk
		private constant real ARMOR_REDUCTION_MULTIPLIER = 0.06
		/// @author Rising_Dusk
		private constant integer LIFE_BONUS_SPELL_ID = 'lif&'
		// Values that do not need to be changed
		/// @author Rising_Dusk
		private constant real ARMOR_INVULNERABLE = 917451.519
		/// @author Rising_Dusk
		private constant real DAMAGE_TEST = 16.
		/// @author Rising_Dusk
		private constant real DAMAGE_LIFE = 30.
		/// @author Rising_Dusk
		private constant real NATLOG_094 = -0.061875
	endglobals

	////! external ObjectMerger w3a AIlz lif& anam "GetUnitArmorLifeBonus" ansf "" Ilif 1 30 aite 0

	/// @author Rising_Dusk
	function GetUnitArmor takes unit u returns real
		local real life = GetWidgetLife(u)
		local real test = life
		local real redc = 0.
		local boolean enab = false
		local trigger trig = GetTriggeringTrigger()
		if (u != null and life >= 0.405) then
			if (GetUnitState(u, UNIT_STATE_MAX_LIFE) <= DAMAGE_TEST) then
				//Add max life to keep it alive
				call UnitAddAbility(u, LIFE_BONUS_SPELL_ID)
			endif
			if (life <= DAMAGE_LIFE) then
				//If under the threshold, heal it for the moment
				call SetWidgetLife(u, DAMAGE_LIFE)
				set test = DAMAGE_LIFE
			endif
			if (trig != null and IsTriggerEnabled(trig)) then
				//Disable the trigger to prevent it registering with damage detection systems
				call DisableTrigger(trig)
				set enab = true
			endif
			call UnitDamageTarget(u, u, DAMAGE_TEST, true, false, ATTACK_TYPE_CHAOS, DAMAGE_TYPE_NORMAL, null)
			set redc = (DAMAGE_TEST - test + GetWidgetLife(u)) / DAMAGE_TEST
			if (enab) then
				//Re-enable the trigger
				call EnableTrigger(trig)
			endif
			//Remove the max life ability
			call UnitRemoveAbility(u, LIFE_BONUS_SPELL_ID)
			call SetWidgetLife(u, life)
			set trig = null
			if (redc >= 1.0) then
				//Invulnerable
				return ARMOR_INVULNERABLE
			elseif (redc < 0.0) then
				//Negative Armor
				return -ALog(redc + 1.0, 20) / NATLOG_094
			else
				//Positive Armor
				return redc / (ARMOR_REDUCTION_MULTIPLIER * (1.0 - redc))
			endif
		endif
		set trig = null
		return 0.0
	endfunction

	/// @author Rising_Dusk
	function GetReducedDamage takes real baseDamage, real armor returns real
		if (armor >= 0.0) then
			return baseDamage * (1.0 - ((armor * ARMOR_REDUCTION_MULTIPLIER) / (1.0 + ARMOR_REDUCTION_MULTIPLIER * armor)))
		endif
		return baseDamage * (2.0 -Pow(0.94, -armor))
	endfunction

	/// @author Rising_Dusk
	function GetFullDamage takes real damage, real armor returns real
		if (armor >= 0.0) then
			return damage / (1.0 -((armor * ARMOR_REDUCTION_MULTIPLIER) / (1.0 + ARMOR_REDUCTION_MULTIPLIER * armor)))
		endif
		return damage / (2.0 -Pow(0.94, -armor))
	endfunction

	/**
	* @author HaiZhung, Tamino Dauth
	*/
	function GetUnitLevelXP takes integer unitLevel returns integer
		local integer result = 25 // default XP
		local integer i = 2
		loop
			exitwhen (i > unitLevel)
			set result = result + 1 * ((i * 5) + 5)
			set i = i + 1
		endloop
		return result
	endfunction

	/**
	* @author HaiZhung, Tamino Dauth
	*/
	function GetUnitXP takes unit whichUnit returns integer
		return GetUnitLevelXP(GetUnitLevel(whichUnit))
	endfunction

	/**
	* Considers default creep experience reduction table.
	* @author HaiZhung, Tamino Dauth
	*/
	function GetUnitHeroXP takes unit whichUnit, unit hero returns integer
		if (GetOwningPlayer(whichUnit) == Player(PLAYER_NEUTRAL_AGGRESSIVE)) then
			return R2I(I2R(GetUnitXP(whichUnit)) * (0.80 - I2R(GetHeroLevel(hero) - 1) * 0.10))
		endif
		return GetUnitXP(whichUnit)
	endfunction

	function GetHeroLevelMaxXP takes integer heroLevel returns integer
		local integer result = 0 // level 1 XP
		local integer i = 2
		loop
			exitwhen (i > heroLevel + 1)
			set result = result + i * 100
			set i = i + 1
		endloop
		return result
	endfunction

	function GetHeroMaxXP takes unit hero returns integer
		return GetHeroLevelMaxXP(GetHeroLevel(hero))
	endfunction

	/// @see FlushUnitTypeCollisionSize, GetUnitCollisionSizeEx, GetUnitCollisionSize
	function FlushUnitCollisionSizes takes nothing returns nothing
		call AHashTable.global().flushKey("UnitCollisionSizes")
	endfunction

	/// @see FlushUnitCollisionSizes, GetUnitCollisionSizeEx, GetUnitCollisionSize
	function FlushUnitTypeCollisionSize takes integer unitTypeId returns nothing
		call AHashTable.global().removeReal("UnitCollisionSizes", I2S(unitTypeId))
	endfunction

	/**
	* Caches collision sizes of unit types.
	* Use FlushUnitCollisionSizes or FlushUnitTypeCollisionSize to flush cached data.
	* Use GetUnitCollisionSize to use predefined maximum collision size and iterations of map.
	* @author Vexoiran, Tamino Dauth
	* @see FlushUnitCollisionSizes, FlushUnitTypeCollisionSize, GetUnitCollisionSize
	* @link http://www.wc3c.net/showthread.php?t=101309
	*/
	function GetUnitCollisionSizeEx takes unit u, real maxCollisionSize, integer iterations returns real
		local integer i = 0
		local real x = GetUnitX(u)
		local real y = GetUnitY(u)
		local real hi
		local real lo
		local real mid
		local string unitType = I2S(GetUnitTypeId(u))
		if (AHashTable.global().hasReal("UnitCollisionSizes", unitType)) then
			return AHashTable.global().real("UnitCollisionSizes", unitType)
		endif
		set hi = maxCollisionSize
		set lo = 0.0
		loop
			set mid = (lo+hi) / 2.0
			exitwhen (i== iterations)
			if (IsUnitInRangeXY(u, x + mid,y,0)) then
				set lo=mid
			else
				set hi=mid
			endif
			set i=i+1
		endloop
		call AHashTable.global().setReal("UnitCollisionSizes", unitType, mid)
		return mid
	endfunction

	/**
	* Same as GetUnitCollisionSizeEx but uses predefined constants.
	* @see FlushUnitCollisionSizes, FlushUnitTypeCollisionSize, GetUnitCollisionSizeEx
	*/
	function GetUnitCollisionSize takes unit u returns real
		return GetUnitCollisionSizeEx(u, A_MAX_COLLISION_SIZE, A_MAX_COLLISION_SIZE_ITERATIONS)
	endfunction

endlibrary