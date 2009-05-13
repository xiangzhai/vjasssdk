/// @author weaaddar
library ALibraryObjectsBonusMod requires AStructCoreGeneralHashTable

	//! external ObjectMerger w3a AItg AZZ1 DataA1 1 1 anam "Damage +1"
	//! external ObjectMerger w3a AItg AZZ2 DataA1 1 2 anam "Damage +2"
	//! external ObjectMerger w3a AItg AZZ3 DataA1 1 4 anam "Damage +4"
	//! external ObjectMerger w3a AItg AZZ4 DataA1 1 8 anam "Damage +8"
	//! external ObjectMerger w3a AItg AZZ5 DataA1 1 16 anam "Damage +16"
	//! external ObjectMerger w3a AItg AZZ6 DataA1 1 32 anam "Damage +32"
	//! external ObjectMerger w3a AItg AZZ7 DataA1 1 64 anam "Damage +64"
	//! external ObjectMerger w3a AItg AZZ8 DataA1 1 128 anam "Damage +128"
	//! external ObjectMerger w3a AItg AZZ9 DataA1 1 256 anam "Damage +256"
	//! external ObjectMerger w3a AItg AZZA DataA1 1 -512 anam "Damage -512"
	//! external ObjectMerger w3a Ald1 AZY1 DataA1 1 1 anam "Armour +1"
	//! external ObjectMerger w3a Ald1 AZY2 DataA1 1 2 anam "Armour +2"
	//! external ObjectMerger w3a Ald1 AZY3 DataA1 1 4 anam "Armour +4"
	//! external ObjectMerger w3a Ald1 AZY4 DataA1 1 8 anam "Armour +8"
	//! external ObjectMerger w3a Ald1 AZY5 DataA1 1 16 anam "Armour +16"
	//! external ObjectMerger w3a Ald1 AZY6 DataA1 1 32 anam "Armour +32"
	//! external ObjectMerger w3a Ald1 AZY7 DataA1 1 64 anam "Armour +64"
	//! external ObjectMerger w3a Ald1 AZY8 DataA1 1 128 anam "Armour +128"
	//! external ObjectMerger w3a Ald1 AZY9 DataA1 1 256 anam "Armour +256"
	//! external ObjectMerger w3a Ald1 AZYA DataA1 1 -512 anam "Armour -512"
	//! external ObjectMerger w3a All2 AZX1 DataA1 1 10 anam "HP +10"
	//! external ObjectMerger w3a All2 AZX2 DataA1 1 20 anam "HP +20"
	//! external ObjectMerger w3a All2 AZX3 DataA1 1 40 anam "HP +40"
	//! external ObjectMerger w3a All2 AZX4 DataA1 1 80 anam "HP +80"
	//! external ObjectMerger w3a All2 AZX5 DataA1 1 160 anam "HP +160"
	//! external ObjectMerger w3a All2 AZX6 DataA1 1 320 anam "HP +320"
	//! external ObjectMerger w3a All2 AZX7 DataA1 1 640 anam "HP +640"
	//! external ObjectMerger w3a All2 AZX8 DataA1 1 1280 anam "HP +1280"
	//! external ObjectMerger w3a All2 AZX9 DataA1 1 2560 anam "HP +2560"
	//! external ObjectMerger w3a All2 AZXA DataA1 1 -5120 anam "HP -5120"
	//! external ObjectMerger w3a Almb AZW1 DataA1 1 10 anam "Mana +10"
	//! external ObjectMerger w3a Almb AZW2 DataA1 1 20 anam "Mana +20"
	//! external ObjectMerger w3a Almb AZW3 DataA1 1 40 anam "Mana +40"
	//! external ObjectMerger w3a Almb AZW4 DataA1 1 80 anam "Mana +80"
	//! external ObjectMerger w3a Almb AZW5 DataA1 1 160 anam "Mana +160"
	//! external ObjectMerger w3a Almb AZW6 DataA1 1 320 anam "Mana +320"
	//! external ObjectMerger w3a Almb AZW7 DataA1 1 640 anam "Mana +640"
	//! external ObjectMerger w3a Almb AZW8 DataA1 1 1280 anam "Mana +1280"
	//! external ObjectMerger w3a Almb AZW9 DataA1 1 2560 anam "Mana +2560"
	//! external ObjectMerger w3a Almb AZWA DataA1 1 -5120 anam "Mana -5120"
	
	globals
		constant integer A_BONUS_TYPE_DAMAGE = 0
		constant integer A_BONUS_TYPE_ARMOR = 1
		constant integer A_BONUS_TYPE_HP = 2
		constant integer A_BONUS_TYPE_MANA = 3
		/// Bonus Mod's Number of abilities per bonus, if last power of 2 ability is -512,
		/// this shall be 10, and the maximum bonus would be 511 while the minimum -512.
		private constant integer IxLimit = 10
		/// If it is 10, the  hp, mana abilities will be: 10, 20,40,80,160,...
		/// If it is 1, they would be : 1,2,4,8,16,...
		/// If it is 5, they would be: 5,10,20,40,80,..
		private constant integer BonusHPManaFactor = 10
		private integer array abilityId
		private AHashTable hashTable
	endglobals
		
	private function Ix takes integer x, integer y returns integer
		return(x * IxLimit +y)
	endfunction

	private function BonusMax takes nothing returns integer
		return (R2I(Pow(2, Ix(1, 0) - 1)) - 1)
	endfunction

	private function BonusMin takes nothing returns integer
	   return -R2I(Pow(2, Ix(1, 0) - 1))
	endfunction
	
	function AUnitGetBonus takes unit who, integer t returns integer
		return hashTable.getHandleInteger(who, I2S(t))
	endfunction

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
			call hashTable.storeHandleInteger(who, I2S(t), amount * BonusHPManaFactor)
		else
			call hashTable.storeHandleInteger(who, I2S(t), amount)
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
				call UnitMakeAbilityPermanent(who, true, x)
				set amount = amount - bit
			else
				call UnitRemoveAbility(who, abilityId[Ix(t, i)])
			endif
		endloop
		if (bol) then
			set x= abilityId[Ix(t,Ix(1, -1))]
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
			set amount= (amount / BonusHPManaFactor) * BonusHPManaFactor
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
			set amount= amount + hashTable.getHandleInteger(who, s)
			if (amount < 0) and (l + amount < 1) then
				set x = R2I(min + mod - l)
				set b = AUnitSetBonus(who, t, AUnitGetBonus(who, t) + x)
				call hashTable.storeHandleInteger(who, s, amount - x)
			else
				call hashTable.storeHandleInteger(who, s, 0)
				set b = AUnitSetBonus(who, t, AUnitGetBonus(who, t) + amount)
			endif
			return b
		endif
		return AUnitSetBonus(who, t, AUnitGetBonus(who, t) + amount)
	endfunction
	
	/// @author Tamino Dauth
	function AFlushBonusUnit takes unit who returns nothing
		call hashTable.flushHandleValues(who)
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
		
		set hashTable = AHashTable.create("BonusMod")
	endfunction

endlibrary