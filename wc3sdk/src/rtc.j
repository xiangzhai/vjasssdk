globals
	constant integer EVENT_LMOUSEUP = 0
	constant integer EVENT_LMOUSEDOWN = 1
	constant integer EVENT_RMOUSEUP = 2
	constant integer EVENT_RMOUSEDOWN = 3
	constant integer EVENT_MMOUSEUP = 4
	constant integer EVENT_MMOUSEDOWN = 5
	constant integer EVENT_MOUSEWHEEL = 6
endglobals

native StringPos takes string input, string lookup returns integer
native StringReplace takes string input, string oldstr, string newstr returns string

native IsWindowInFocus takes nothing returns integer
native Char takes integer i returns string
native Log takes real r returns real
native Ln takes real r returns real

native BitAnd takes integer a, integer b returns integer
native BitOr takes integer a, integer b returns integer
native BitXor takes integer a, integer b returns integer
native BitBSL takes integer val, integer shift returns integer
native BitBSR takes integer val, integer shift returns integer
native BitFlip takes integer val returns integer

native GetTimestamp takes nothing returns integer
native GetTimeSeconds takes nothing returns integer
native GetTimeMinutes takes nothing returns integer
native GetTimeHours takes nothing returns integer
native GetTimeMonth takes nothing returns integer
native GetTimeDayWeek takes nothing returns integer
native GetTimeDayMonth takes nothing returns integer
native GetTimeDayYear takes nothing returns integer
native GetTimeYear takes nothing returns integer
native GetTickCount takes nothing returns integer
native GetTimeMilliseconds takes nothing returns integer

native GetMouseX takes nothing returns integer
native GetMouseY takes nothing returns integer
native GetMouseXRelative takes nothing returns integer
native GetMouseYRelative takes nothing returns integer
native SetMouseX takes integer x returns nothing
native SetMouseY takes integer y returns nothing
native SetMousePos takes integer x, integer y returns nothing
native TriggerRegisterMouseEvent takes trigger whichTrigger, integer eventtype returns nothing
native GetMouseTargetX takes nothing returns real
native GetMouseTargetY takes nothing returns real
native SetMouseBlock takes integer whichMouseButton, boolean blocked returns nothing
native IsMouseOverUI takes nothing returns boolean

native TriggerRegisterKeyEvent takes trigger whichTrigger, integer eventtype returns nothing
native IsKeyDown takes integer key returns boolean
native SetKeyBlock takes integer key, boolean blocked returns nothing

native GetScreenWidth takes nothing returns integer
native GetScreenHeight takes nothing returns integer
native GetWindowWidth takes nothing returns integer
native GetWindowHeight takes nothing returns integer
native GetWindowX takes nothing returns integer
native GetWindowY takes nothing returns integer

native GetTriggerParam_Integer takes integer id returns integer
native GetTriggerParam_Real takes integer id returns real
native GetTriggerParam_String takes integer id returns string

native SocTCPConnect takes string ip, integer port, integer mode returns integer
native SocTCPListen takes integer port, integer maxqueue, integer mode returns integer
native SocTCPAccept takes integer socket, integer mode returns integer
native SocTCPIP takes integer socket returns string
native SocSetNagle takes integer socket, boolean on returns integer
native SocTCPConnected takes integer socket returns boolean
native SocUDPConnect takes integer port, integer mode returns integer
native SocSendUDPMessage takes integer udpsocket, string ip, integer port, integer buffer returns integer
native SocSendTCPMessage takes integer tcpsocket, integer buffer returns integer
native SocReceiveMessage takes integer socket, integer length, integer buffer returns integer
native SocPeekMessage takes integer socket, integer length, integer buffer returns integer
native SocSetFormat takes integer socket, integer mode, string delimiter returns integer
native SocGetLastInIP takes nothing returns string
native SocGetLastInPort takes nothing returns integer
native SocSetSync takes integer socket, integer mode returns integer
native SocCloseSocket takes integer socket returns boolean
native SocGetSocketLastError takes integer socket returns integer
native SocGetMyHost takes nothing returns string
native SocCompareIp takes string ip, string mask returns boolean
native SocExit takes nothing returns boolean
native SocStart takes nothing returns boolean
native SocNetConnected takes nothing returns boolean
native BufferWriteByte takes integer val, integer buffer returns integer
native BufferWriteShort takes integer val, integer buffer returns integer
native BufferWriteUShort takes integer val, integer buffer returns integer
native BufferWriteInt takes integer val, integer buffer returns integer
native BufferWriteUInt takes integer val, integer buffer returns integer
native BufferWriteFloat takes real val, integer buffer returns integer
native BufferWriteString takes string str, integer buffer returns integer
native BufferWriteChars takes string str, integer buffer returns integer
native BufferWriteOrderID takes integer val, integer buffer returns integer
native BufferReadByte takes integer buffer returns integer
native BufferReadShort takes integer buffer returns integer
native BufferReadUShort takes integer buffer returns integer
native BufferReadInt takes integer buffer returns integer
native BufferReadUInt takes integer buffer returns integer
native BufferReadFloat takes integer buffer returns real
native BufferReadString takes integer buffer returns string
native BufferReadChars takes integer buffer, integer length returns string
native BufferReadOrderID takes integer buffer returns integer
native CreateBuffer takes nothing returns integer
native DestroyBuffer takes integer buffer returns nothing
native BufferCopy takes integer bufferdestination, integer buffersource returns integer
native BufferClear takes integer buffer	returns boolean
native BufferSize takes integer buffer returns integer
native BufferBytesLeft takes integer buffer returns integer

native CreateVector takes integer size returns integer
native DestroyVector takes integer id returns nothing
native VectorClear takes integer id returns nothing
native GetVectorSize takes integer id returns integer
native SetVectorSize takes integer id, integer newsize returns nothing
native VectorGet takes integer id, integer index returns integer
native VectorSet takes integer id, integer index, integer value returns nothing
native VectorPushBack takes integer id, integer value returns integer
native VectorPopBack takes integer id returns nothing
native VectorBack takes integer id returns integer
native VectorEmpty takes integer id returns boolean

native CreateI64 takes integer baseHigh, integer baseLow returns integer
native DestroyI64 takes integer id returns nothing
native AddIntToI64 takes integer id, integer addme returns nothing
native SubIntFromI64 takes integer id, integer subme returns nothing
native AddI64ToI64 takes integer id, integer addid returns nothing
native SubI64FromI64 takes integer id, integer subid returns nothing
native MulI64 takes integer id, integer mul returns nothing
native DivI64 takes integer id, integer div returns nothing
native I64AsString takes integer id returns string
native I64AsIntHigh takes integer id returns integer
native I64AsIntLow takes integer id returns integer
native I64Compare takes integer id1, integer id2 returns integer

native TriggerRegisterTooltipEvent takes trigger t returns nothing
native TriggerReturnString takes string s returns nothing
native GetTriggeringRawcode takes nothing returns integer
native GetTriggeringLevel takes nothing returns integer

native CreateTriggerGroup takes nothing returns integer
native DestroyTriggerGroup takes integer triggergroup returns nothing
native TriggerGroupAdd takes integer triggergroup, trigger t returns boolean
native TriggerGroupRun takes integer triggergroup returns nothing