// JASS and vJass
globals
	hashtable table
endglobals

// In vJass you could initialize the value in globals block.
function InitTable takes nothing returns nothing
	set table = InitHashtable()
endfunction

function ShowDelayedResult takes nothing returns nothing
	local integer result = LoadInteger(table, GetHandleId(GetExpiredTimer()), 0)
	call DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "Your result is: " + I2S(result))
	call DestroyTimer(GetExpiredTimer())
endfunction

// Won't wait for calling ShowResult.
function Test1 takes nothing returns nothing
	local integer result = GetRandomInt(0, 100)
	local timer whichTimer = CreateTimer()
	call DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "We will show you the result in 20 seconds.")
	call SaveInteger(table, GetHandleId(whichTimer), 0, result)
	call TimerStart(whichTimer, 20.0, false, function ShowDelayedResult)
	call DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "Finished function call!")
endfunction

function ShowDelayedResult takes integer result returns nothing
	call DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "Your result is: " + I2S(result))
endfunction

function Test2 takes nothing returns nothing
	local integer result = GetRandomInt(0, 100)
	call DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "We will show you the result in 20 seconds (synchronized time).")
	call PolledWait(20.0) // Note that this is not always exact (waits 0.10 seconds when 2 seconds or less are left).
	call ShowResult(result)
	call DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "Finished function call!")
endfunction

function Test3 takes nothing returns nothing
	local integer result = GetRandomInt(0, 100)
	call DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "We will show you the result in 20 seconds (local time).")
	call TriggerSleepAction(20.0)
	call ShowResult(result)
	call DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "Finished function call!")
endfunction

// JASS++

void ShowResult(integer result)
	DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "Your result is: " + string(result))

// Won't wait for calling ShowResult.
threaded void Test1()
	integer result = GetRandomInt(0, 100)
	DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "We will show you the result in 20 seconds.")
	executewait 20.0 ShowResult(result)
	DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "Finished function call!")

threaded void Test2()
	integer result = GetRandomInt(0, 100)
	DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "We will show you the result in 20 seconds (synchronized time).")
	wait 20.0
	ShowResult(result)
	DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "Finished function call!")

threaded void Test3()
	integer result = GetRandomInt(0, 100)
	DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "We will show you the result in 20 seconds (local time).")
	sleep 20.0
	ShowResult(result)
	DisplayTimedTextToPlayer(Player(0), 0.0, 0.0, 20.0, "Finished function call!")