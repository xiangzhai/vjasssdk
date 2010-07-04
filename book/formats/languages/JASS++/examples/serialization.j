// Jass

globals
	string array npcName
	integer array npcAge
	unit array npcUnit
	location array npcLocation
endglobals

function SaveNpc takes hashtable table, integer index returns nothing
	call SaveStr(table, index, 0, npcName[index])
	call SaveInteger(table, index, 1, npcAge[index])
	call SaveUnitHandle(table, index, 2, npcUnit[index])
	call SaveLocationHandle(table, index, 3, npcLocation[index])
endfunction

function LoadNpc takes hashtable table, integer index returns boolean
	if (not HaveSavedStr(table, index, 0)) then
		return false
	endif
	set npcName[index] = LoadStr(table, index, 0)
	set npcAge[index] = LoadInteger(table, index, 1)
	set npcUnit[index] = LoadUnitHandle(table, index, 2)
	set npcLocation[index] = LoadLocationHandle(table, index, 3)
	return true
endfunction

function FlushNpc takes hashtable table, integer index returns nothing
	call FlushChildHashtable(table, index)
endfunction

function Test takes nothing returns nothing
	local hashtable table = InitHashtable()
	call LoadNpc(table, 0)	
	call SaveNpc(table, 0)
	call FlushNpc(table, 0)
endfunction

// vJass

struct Npc
	private string m_name
	private integer m_age
	private unit m_unit
	private location m_location

	public method save takes hashtable table returns nothing
		call SaveStr(table, this, 0, npcName[index])
		call SaveInteger(table, this, 1, this.m_age)
		call SaveUnitHandle(table, this, 2, this.m_unit)
		call SaveLocationHandle(table, this, 3, this.location)
	endmethod

	public method load takes hashtable table returns boolean
		if (not HaveSavedStr(table, this, 0)) then
			return false
		endif
		set this.m_name = LoadStr(table, this, 0)
		set this.m_age = LoadInteger(table, this, 1)
		set this.m_unit = LoadUnitHandle(table, this, 2)
		set this.m_location = LoadLocationHandle(table, this, 3)
		return true
	endmethod

	public method flush takes hashtable table returns nothing
		call FlushChildHashtable(table, this)
	endmethod
endstruct

function Test takes nothing returns nothing
	local hashtable table = InitHashtable()
	local Npc npc = Npc.create()
	call npc.save(table)
	call npc.load(table)
	call npc.flush(table)
endfunction

// Jass++

class Npc
{
	private string m_name
	private integer m_age
	private unit m_unit
	private location m_location

	public void operator save (hashtable table) const
	{
		save this table integer(this);
	}

	public boolean operator load (hashtable table)
	{
		if (!exists table integer(this))
		{
			return false;
		}

		load this table integer(this);

		return true;
	}

	public void operator flush (hashtable table) const
	{
		flush table integer(this);
	}
};
