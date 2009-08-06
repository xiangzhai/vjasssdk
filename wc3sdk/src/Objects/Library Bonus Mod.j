/**
* The Bonus Mod (originally created by @author weaaddar) is a famous system
* for modifing specific unit properties where Jass natives are missed and can not be used.
* It uses a special method by adding and removing property-modifing abilities to a unit.
* For this reason it allows the user to modify damage, armour, hit points and mana of a unit.
* Note that you've to call @function AInitBonusMod before using this library.
* It won't be called by default (using it as initializer) since this would be decrease the performance for users who maybe won't use it at all.
* @author weaaddar
*/
library ALibraryObjectsBonusMod requires AStructCoreGeneralHashTable, ALibraryCoreDebugMisc //misc for debuggings

/*
	Uncomment this to use Bonus Mod.
	//A1
	//! external ObjectMerger w3a AItg AZZ1 Iatt 1 1 anam "Damage +1" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AItg AZZ2 Iatt 1 2 anam "Damage +2" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AItg AZZ3 Iatt 1 4 anam "Damage +4" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AItg AZZ4 Iatt 1 8 anam "Damage +8" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AItg AZZ5 Iatt 1 16 anam "Damage +16" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AItg AZZ6 Iatt 1 32 anam "Damage +32" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AItg AZZ7 Iatt 1 64 anam "Damage +64" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AItg AZZ8 Iatt 1 128 anam "Damage +128" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AItg AZZ9 Iatt 1 256 anam "Damage +256" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AItg AZZA Iatt 1 -512 anam "Damage -512" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AId1 AZY1 Idef 1 1 anam "Armour +1" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AId1 AZY2 Idef 1 2 anam "Armour +2" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AId1 AZY3 Idef 1 4 anam "Armour +4" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AId1 AZY4 Idef 1 8 anam "Armour +8" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AId1 AZY5 Idef 1 16 anam "Armour +16" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AId1 AZY6 Idef 1 32 anam "Armour +32" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AId1 AZY7 Idef 1 64 anam "Armour +64" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AId1 AZY8 Idef 1 128 anam "Armour +128" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AId1 AZY9 Idef 1 256 anam "Armour +256" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AId1 AZYA Idef 1 -512 anam "Armour -512" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AIl2 AZX1 Ilif 1 10 anam "HP +10" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AIl2 AZX2 Ilif 1 20 anam "HP +20" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AIl2 AZX3 Ilif 1 40 anam "HP +40" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AIl2 AZX4 Ilif 1 80 anam "HP +80" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AIl2 AZX5 Ilif 1 160 anam "HP +160" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AIl2 AZX6 Ilif 1 320 anam "HP +320" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AIl2 AZX7 Ilif 1 640 anam "HP +640" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AIl2 AZX8 Ilif 1 1280 anam "HP +1280" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AIl2 AZX9 Ilif 1 2560 anam "HP +2560" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AIl2 AZXA Ilif 1 -5120 anam "HP -5120" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AImb AZW1 Iman 1 10 anam "Mana +10" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AImb AZW2 Iman 1 20 anam "Mana +20" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AImb AZW3 Iman 1 40 anam "Mana +40" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AImb AZW4 Iman 1 80 anam "Mana +80" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AImb AZW5 Iman 1 160 anam "Mana +160" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AImb AZW6 Iman 1 320 anam "Mana +320" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AImb AZW7 Iman 1 640 anam "Mana +640" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AImb AZW8 Iman 1 1280 anam "Mana +1280" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AImb AZW9 Iman 1 2560 anam "Mana +2560" EditorSuffix "ASL" Art ""
	//! external ObjectMerger w3a AImb AZWA Iman 1 -5120 anam "Mana -5120" EditorSuffix "ASL" Art ""
*/

	globals
		constant integer A_BONUS_TYPE_DAMAGE = 0
		constant integer A_BONUS_TYPE_ARMOR = 1
		constant integer A_BONUS_TYPE_HP = 2
		constant integer A_BONUS_TYPE_MANA = 3
		/**
		* Bonus Mod's Number of abilities per bonus, if last power of 2 ability is -512,
		* this shall be 10, and the maximum bonus would be 511 while the minimum -512.
		*/
		private constant integer IxLimit = 10
		/**
		* If it is 10, the  hp, mana abilities will be: 10, 20,40,80,160,...
		* If it is 1, they would be : 1,2,4,8,16,...
		* If it is 5, they would be: 5,10,20,40,80,..
		*/
		private constant integer BonusHPManaFactor = 10
		private integer array abilityId
		private AHashTable hashTable
	endglobals
		
	private function Ix takes integer x, integer y returns integer
		return (x * IxLimit + y)
	endfunction

	private function BonusMax takes nothing returns integer
		return (R2I(Pow(2, Ix(1, 0) - 1)) - 1)
	endfunction

	private function BonusMin takes nothing returns integer
	   return -R2I(Pow(2, Ix(1, 0) - 1))
	endfunction
	
	/**
	* Returns units bonus of a specific property.
	* @param who The unit which is checked for the bonus.
	* @param t The property type which is checked for.
	* Possible values are:
	* @global A_BONUS_TYPE_DAMAGE
	* @global A_BONUS_TYPE_ARMOR
	* @global A_BONUS_TYPE_HP
	* @global A_BONUS_TYPE_MANA
	* @return Returns the bonus value.
	*/
	function AUnitGetBonus takes unit who, integer t returns integer
		return hashTable.handleInteger(who, I2S(t))
	endfunction

	/**
	* Sets units bonus of a specific property.
	* @param who The unit which should get the bonus.
	* @param t The property type which is used for adding the bonus.
	* Possible values are:
	* @global A_BONUS_TYPE_DAMAGE
	* @global A_BONUS_TYPE_ARMOR
	* @global A_BONUS_TYPE_HP
	* @global A_BONUS_TYPE_MANA
	* @return Returns if the bonus was added successfully.
	*/
	function AUnitSetBonus takes unit who, integer t, integer amount returns boolean
		local integer x
		local integer i = Ix(1, -1)
		local integer bit = R2I(Pow(2, i))
		local boolean bol = false
		local boolean man = false
		if (who == null or t < 0 or t > 3) then
			return false
		elseif (t > 1) then
			set amount = amount / BonusHPManaFactor
			call hashTable.setHandleInteger(who, I2S(t), amount * BonusHPManaFactor)
		else
			call hashTable.setHandleInteger(who, I2S(t), amount)
		endif
		if (t == 3) and (GetUnitState(who,UNIT_STATE_MAX_MANA) <= 0) then
			set man = true
		endif
		call UnitRemoveAbility(who, abilityId[Ix(t, Ix(1, -1))])
		if (amount < 0) then
			set bol = true
			set amount= bit + amount
		endif
		loop
			set bit = bit / 2
			set i = i - 1
			exitwhen (i < 0)
			if (amount >= bit) then
				set x = abilityId[Ix(t, i)]
				call UnitAddAbility(who, x)
				debug call Print("Adding ability " + GetObjectName(x))
				call UnitMakeAbilityPermanent(who, true, x)
				set amount = amount - bit
			else
				call UnitRemoveAbility(who, abilityId[Ix(t, i)])
			endif
		endloop
		if (bol) then
			set x = abilityId[Ix(t,Ix(1, -1))]
			call UnitAddAbility(who, x)
			call UnitMakeAbilityPermanent(who, true, x)
		endif
		if (man and (GetUnitState(who,UNIT_STATE_MAX_MANA) > 0)) then
			call SetUnitState(who,UNIT_STATE_MANA, 0)
		endif
		return amount == 0
	endfunction

	function AUnitClearBonus takes unit who, integer t returns nothing
	   call AUnitSetBonus(who, t, 0)
	endfunction

	function AUnitAddBonus takes unit who, integer t, integer amount returns boolean
		local integer x
		local boolean b
		local real l
		local string s
		local real min
		local real mod
		if (amount == 0) then
			return true
		endif
		if (t > 1) then
			set amount = (amount / BonusHPManaFactor) * BonusHPManaFactor
		endif
		if (t >= 2) then

			if (t == 2) then
				set l = GetWidgetLife(who)
				set mod = ModuloReal(l, BonusHPManaFactor)
				set s = "BonusModLeftHP"
				if mod >= 1 then
					set min = 0
				else
					set min = BonusHPManaFactor
				endif
			else
				set l = GetUnitState(who, UNIT_STATE_MAX_MANA)
				set mod = ModuloReal(l, BonusHPManaFactor)
				set s= "BonusModLeftMana"
				set min = 0
			endif
			set amount= amount + hashTable.handleInteger(who, s)
			if (amount < 0) and (l + amount < 1) then
				set x = R2I(min + mod - l)
				set b = AUnitSetBonus(who, t, AUnitGetBonus(who, t) + x)
				call hashTable.setHandleInteger(who, s, amount - x)
			else
				call hashTable.setHandleInteger(who, s, 0)
				set b = AUnitSetBonus(who, t, AUnitGetBonus(who, t) + amount)
			endif
			return b
		endif
		return AUnitSetBonus(who, t, AUnitGetBonus(who, t) + amount)
	endfunction
	
	/// @author Tamino Dauth
	function AFlushBonusUnit takes unit who returns nothing
		call hashTable.flushHandle(who)
	endfunction
	
	/// So here you set each of the bonus abilities, 0 is damage, 1 armor, 2 health, and 3 mana
	/// The second numbers are the powers (2^1 is 1, 2^2=4, 2^3=8, ...
	/// Remember: Last power ability should be negative.
	function AInitBonusMod takes nothing returns nothing
		//Damage
		set abilityId[Ix(0,0)] = 'AZZ1' //+001
		set abilityId[Ix(0,1)] = 'AZZ2' //+002
		set abilityId[Ix(0,2)] = 'AZZ3' //+004
		set abilityId[Ix(0,3)] = 'AZZ4' //+008
		set abilityId[Ix(0,4)] = 'AZZ5' //+016
		set abilityId[Ix(0,5)] = 'AZZ6' //+032
		set abilityId[Ix(0,6)] = 'AZZ7' //+064
		set abilityId[Ix(0,7)] = 'AZZ8' //+128
		set abilityId[Ix(0,8)] = 'AZZ9' //+256
		set abilityId[Ix(0,9)] = 'AZZA' //-512
		//Armor
		set abilityId[Ix(1,0)] = 'AZY1' //+001
		set abilityId[Ix(1,1)] = 'AZY2' //+002
		set abilityId[Ix(1,2)] = 'AZY3' //+004
		set abilityId[Ix(1,3)] = 'AZY4' //+008
		set abilityId[Ix(1,4)] = 'AZY5' //+016
		set abilityId[Ix(1,5)] = 'AZY6' //+032
		set abilityId[Ix(1,6)] = 'AZY7' //+064
		set abilityId[Ix(1,7)] = 'AZY8' //+128
		set abilityId[Ix(1,8)] = 'AZY9' //+256
		set abilityId[Ix(1,9)] = 'AZYA' //-512
		//HP
		set abilityId[Ix(2,0)] = 'AZX1' //+0010
		set abilityId[Ix(2,1)] = 'AZX2' //+0020
		set abilityId[Ix(2,2)] = 'AZX3' //+0040
		set abilityId[Ix(2,3)] = 'AZX4' //+0080
		set abilityId[Ix(2,4)] = 'AZX5' //+0160
		set abilityId[Ix(2,5)] = 'AZX6' //+0320
		set abilityId[Ix(2,6)] = 'AZX7' //+0640
		set abilityId[Ix(2,7)] = 'AZX8' //+1280
		set abilityId[Ix(2,8)] = 'AZX9' //+2560
		set abilityId[Ix(2,9)] = 'AZXA' //-5120
		//Mana
		set abilityId[Ix(3,0)] = 'AZW1' //+0010
		set abilityId[Ix(3,1)] = 'AZW2' //+0020
		set abilityId[Ix(3,2)] = 'AZW3' //+0040
		set abilityId[Ix(3,3)] = 'AZW4' //+0080
		set abilityId[Ix(3,4)] = 'AZW5' //+0160
		set abilityId[Ix(3,5)] = 'AZW6' //+0320
		set abilityId[Ix(3,6)] = 'AZW7' //+0640
		set abilityId[Ix(3,7)] = 'AZW8' //+1280
		set abilityId[Ix(3,8)] = 'AZW9' //+2560
		set abilityId[Ix(3,9)] = 'AZWA' //-5120
		
		set hashTable = AHashTable.create()
	endfunction

endlibrary