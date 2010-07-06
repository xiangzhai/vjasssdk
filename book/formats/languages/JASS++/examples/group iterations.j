// Group iteration

// JASS

function IterateMyGroup takes group whichGroup returns nothing
	local unit firstOfGroup
	local group tmpGroup = CreateGroup()
	loop
		set firstOfGroup = FirstOfGroup(whichGroup)
		exitwhen (firstOfGroup == null)
		call DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 5.0, GetUnitName(firstOfGroup))
		call GroupRemoveUnit(whichGroup, firstOfGroup)
		call GroupAddUnit(tmpGroup, firstOfGroup)
	endloop
	// adding old members
	loop
		set firstOfGroup = FirstOfGroup(tmpGroup)
		exitwhen (firstOfGroup == null)
		call GroupRemoveUnit(tmpGroup, firstOfGroup)
		call GroupAddUnit(whichGroup, firstOfGroup)
	endloop
	call DestroyGroup(tmpGroup)
endfunction

function IterationFunction takes nothing returns nothing
	call DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 5.0, GetUnitName(GetEnumUnit()))
endfunction

// Note that this version would be hard to be used when you have some more parameters since you would have to attach them on a new copied group to prevent issues.
function IterateMyGroup2 takes group whichGroup returns nothing
	call ForGroup(whichGroup, function IterationFunction)
endfunction

// vJass
// Would be similar but we could use lambda function for the second version.

function IterateMyGroup2 takes group whichGroup returns nothing
	call ForGroup(whichGroup, call DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 5.0, GetUnitName(GetEnumUnit())))
endfunction

// JASS++

void IterateMyGroup(group const whichGroup)
{
	local unit const whichUnit;

	foreach (whichUnit; whichGroup)
	{
		DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 5.0, GetUnitName(whichUnit));
	}
}

// Similar to vJass - using lambda function.
void IterateMyGroup2(group whichGroup)
{
	ForGroup(whichGroup, DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 5.0, GetUnitName(GetEnumUnit())));
}
