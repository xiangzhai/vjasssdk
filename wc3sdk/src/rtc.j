globals
	constant integer EVENT_LMOUSEUP = 0
	constant integer EVENT_LMOUSEDOWN = 1
	constant integer EVENT_RMOUSEUP = 2
	constant integer EVENT_RMOUSEDOWN = 3
	constant integer EVENT_MMOUSEUP = 4
	constant integer EVENT_MMOUSEDOWN = 5
	constant integer EVENT_MOUSEWHEEL = 6
endglobals

//============================================================================
// StopWatch API
//
native StopWatchCreate  takes nothing returns integer
native StopWatchMark    takes integer stopwatch returns real
native StopWatchDestroy takes integer stopwatch returns nothing

//============================================================================
// Time API
//
native TimeGetTimestamp    takes nothing returns integer
native TimeGetSeconds      takes nothing returns integer
native TimeGetMinutes      takes nothing returns integer
native TimeGetHours        takes nothing returns integer
native TimeGetDayMonth     takes nothing returns integer
native TimeGetDayWeek      takes nothing returns integer
native TimeGetMonth        takes nothing returns integer
native TimeGetYear         takes nothing returns integer
native TimeGetMilliseconds takes nothing returns integer
native GetTickCount        takes nothing returns integer

//============================================================================
// Vector API
//
native VectorCreate     takes integer size returns integer
native VectorDestroy    takes integer vector returns nothing
native VectorClear      takes integer vector returns nothing
native VectorGetSize    takes integer vector returns integer
native VectorSetSize    takes integer vector, integer size returns nothing
native VectorGet        takes integer vector, integer index returns integer
native VectorSet        takes integer vector, integer index, integer value returns nothing
native VectorPushBack   takes integer vector, integer value returns nothing
native VectorPopBack    takes integer vector returns nothing
native VectorBack       takes integer vector returns integer
native VectorEmpty      takes integer vector returns boolean

//============================================================================
// UserInput API
//
native GetMouseTerrainX          takes nothing returns real
native GetMouseTerrainY          takes nothing returns real
native GetMouseTerrainZ          takes nothing returns real
native GetMouseScaleX            takes nothing returns real
native GetMouseScaleY            takes nothing returns real
native GetMouseScreenX           takes nothing returns integer
native GetMouseScreenY           takes nothing returns integer
native SetMouseScreenX           takes integer x returns nothing
native SetMouseScreenY           takes integer y returns nothing
native SetMouseScreenPos         takes integer x, integer y returns nothing
native IsMouseOverUI             takes nothing returns boolean
native SetMouseBlock             takes integer key, boolean doBlock returns nothing
native GetTriggerWheelDelta      takes nothing returns integer
native TriggerRegisterMouseEvent takes trigger trig, integer etype returns nothing
native TriggerRegisterKeyEvent   takes trigger trig, integer etype returns nothing
native GetTriggerKey             takes nothing returns integer
native SetKeyBlock               takes integer key, boolean doBlock returns nothing
native ForceKeyDown              takes integer key returns nothing
native ForceKeyUp                takes integer key returns nothing
native ForceKeyType              takes integer key returns nothing

//============================================================================
// Misc API
//
native StringReplace takes string input, string oldstr, string newstr returns string
native StringPos     takes string input, string substr returns integer
native StringRPos    takes string input, string substr returns integer
native BitAnd        takes integer o1, integer o2 returns integer
native BitOr         takes integer o1, integer o2 returns integer
native BitXor        takes integer o1, integer o2 returns integer
native BitBSL        takes integer o1, integer o2 returns integer
native BitBSR        takes integer o1, integer o2 returns integer
native BitFlip       takes integer o returns integer
native Log           takes real input returns real
native Ln            takes real input returns real