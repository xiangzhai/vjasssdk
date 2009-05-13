//============================================================================
// WarSocAPI - v2.00
native SocStart			takes nothing returns boolean //initializer
native SocExit			takes nothinng returns boolean //deinitializer
native SocTCPConnect		takes string ip, integer port, integer mode returns integer //Creates a csocket and connects to the ip and port
native SocTCPListen		takes integer port, integer maxqueue, integer mode returns integer //Create a lsocket to monitor a certain port for incomming connections
native SocTCPAccept		takes integer socket, integer mode returns integer //Accepts incoming connections on a listen socket.
native SocTCPIP			takes integer socket returns string  //Returns the ip that the socket is connected to
native SocSetNagle		takes integer socket, boolean on returns integer //Toggles the naggle (TCP_NODELAY) algorithm.
native SocTCPConnected		takes integer socket returns boolean //Returns true if the tcp socket is connected.
native SocUDPConnect		takes integer port, integer mode returns integer //Creates a udp socket.
native SocSendUDPMessage	takes integer udpsocket, string ip, integer port, integer buffer returns integer
native SocSendTCPMessage	takes integer tcpsocket, integer buffer returns integer
native SocReceiveMessage	takes integer socket, integer length, integer buffer returns integer
native SocPeekMessage		takes integer socket, integer length, integer buffer returns integer
native SocGetLastInIP		takes nothing returns string
native SocGetLastInPort		takes nothing returns integer
native SocSetSync		takes integer socket, integer mode returns integer
native SocCloseSocket		takes integer socket returns boolean
native SocGetSocketLastError	takes integer socket returns integer
native SocGetMyHost		takes nothing returns string
native SocCompareIp		takes string ip, string mask returns boolean
native SocBufferWriteByte	takes integer val, integer buffer returns integer
native SocBufferWriteShort	takes integer val, integer buffer returns integer
native SocBufferWriteUShort	takes integer val, integer buffer returns integer
native SocBufferWriteInt	takes integer val, integer buffer returns integer
native SocBufferWriteUInt	takes integer val, integer buffer returns integer
//native SocBufferWriteFloat	takes real val, integer buffer returns integer
native SocBufferReadByte	takes integer buffer returns integer
native SocBufferReadShort	takes integer buffer returns integer
native SocBufferReadUShort	takes integer buffer returns integer
native SocBufferReadInt		takes integer buffer returns integer
native SocBufferReadUInt	takes integer buffer returns integer
//native SocBufferReadFloat takes integer buffer returns real
native SocBufferWriteString	takes string str, integer buffer returns integer
native SocBufferReadString	takes integer buffer returns string
native SocBufferClear		takes integer buffer returns boolean
native SocBufferSize		takes integer buffer returns integer
native SocBufferCreate		takes nothing returns integer
native SocBufferDestroy		takes integer buffer returns integer
native SocBufferCopy		takes integer bufferdestination, integer buffersource returns integer
native SocMacAddress		takes nothing returns string
native MD5			takes string s returns string
native SocBufferMD5		takes integer buffer returns string
native SocBufferExists		takes integer buffer returns boolean
native SocNetConnected		takes nothing returns boolean
native SocSetFormat		takes integer socket, integer mode, string delimiter returns integer
native SocBufferWriteOrderID	takes integer val, integer buffer returns integer
native SocBufferReadOrderID	takes integer buffer returns integer

//============================================================================
//I64API
native CreateI64		takes integer baseHigh, integer baseLow returns integer
native DestroyI64		takes integer id returns nothing
native AddIntToI64		takes integer id, integer addme returns nothing
native SubIntFromI64		takes integer id, integer subme returns nothing
native AddI64ToI64 		takes integer id, integer addid returns nothing
native SubI64FromI64		takes integer id, integer subid returns nothing
native MulI64 			takes integer id, integer mul returns nothing
native DivI64 			takes integer id, integer div returns nothing
native I64AsString	 	takes integer id returns string
native I64AsHex		 	takes integer id returns string
native I64AsIntHigh		takes integer id returns integer
native I64AsIntLow 		takes integer id returns integer
native I64Compare 		takes integer id1, integer id2 returns integer

//============================================================================
// MouseAPI
native GetMouseX		takes nothing returns integer
native GetMouseY		takes nothing returns integer
native GetMouseXRelative	takes nothing returns integer
native GetMouseYRelative	takes nothing returns integer
native SetMouseX		takes integer x returns nothing
native SetMouseY		takes integer y returns nothing
native SetMousePos		takes integer x, integer y returns nothing
native TriggerRegisterMouseEvent takes trigger whichTrigger, integer eventtype returns nothing
native GetTriggerWheelDelta	takes nothing returns integer

//============================================================================
// WindowAPI
native GetWindowWidth		takes nothing returns integer
native GetWindowHeight		takes nothing returns integer
native GetWindowX		takes nothing returns integer
native GetWindowY		takes nothing returns integer

//============================================================================
// TimeAPI
native GetTimeSeconds		takes nothing returns integer // seconds (0 - 60)
native GetTimeMinutes		takes nothing returns integer // minutes (0 - 59)
native GetTimeHours		takes nothing returns integer // hour (0 - 23)
native GetTimeMonth		takes nothing returns integer // month (0 - 11, 0 = January)
native GetTimeDayWeek		takes nothing returns integer // day of week (0 - 6, 0 = Sunday)
native GetTimeDayMonth		takes nothing returns integer // day of month (1 - 31)
native GetTimeDayYear		takes nothing returns integer // day of year (0 - 365)
native GetTimeYear		takes nothing returns integer

//============================================================================
// BitAPI
native BitAnd			takes integer a, integer b returns integer
native BitOr			takes integer a, integer b returns integer
native BitXor			takes integer a, integer b returns integer
native BitBSL			takes integer val, integer shift returns integer
native BitBSR			takes integer val, integer shift returns integer
native BitFlip			takes integer val returns integer

//============================================================================
// SQLiteAPI
//native SQLite_Open 			takes string name 												returns integer
//native SQLite_Close 		takes integer id 												returns nothing
//native SQLite_WriteInt 		takes integer id, string name, integer value 					returns nothing
//native SQLite_WriteReal 	takes integer id, string name, integer value 					returns nothing
//native SQLite_WriteString 	takes integer id, string name, integer value 					returns nothing
//native SQLite_ReadInt 		takes integer id, string name 									returns integer
//native SQLite_ReadReal 		takes integer id, string name 									returns integer
//native SQLite_ReadString 	takes integer id, string name 									returns integer
//native SQLite_DelInt 		takes integer id, string name 									returns nothing
//native SQLite_DelReal 		takes integer id, string name 									returns nothing
//native SQLite_DelString 	takes integer id, string name 									returns nothing
//native SQLite_IntExists 	takes integer id, string name 									returns boolean
//native SQLite_RealExists 	takes integer id, string name 									returns boolean
//native SQLite_StringExists 	takes integer id, string name 									returns boolean
//native GetCustomDBValue 	takes string name 												returns string

//============================================================================
// MathExtAPI
native log 			takes real num returns real
native ln 			takes real num returns real

//============================================================================
// MiscAPI
native StrNatCompare		takes string first, string second, boolean matchcase returns integer
native String2RawCode		takes string s	returns integer
native RawCode2String		takes integer i returns string
native GetParamCount		takes nothing returns integer
native GetParam			takes integer i returns string
native GetMapPath		takes nothing returns string
native GetMapPathAbsolute	takes nothing returns string
native StringReplace		takes string input, string oldstr, string newstr returns string
native StringPos		takes string input, string lookup returns integer

//============================================================================
// KeyboardAPI
native TriggerRegisterKeyEvent 	takes trigger whichTrigger, integer eventtype returns nothing
native GetTriggerKey 		takes nothing returns integer
native SetKeyBlock 		takes integer i, boolean doBlock returns nothing
native ForceKeyDown		takes integer key returns nothing
native ForceKeyUp		takes integer key returns nothing
native ForceKeyType		takes integer key returns nothing

//============================================================================
// TriggerGroupAPI
native CreateTriggerGroup	takes nothing returns integer
native DestroyTriggerGroup	takes integer tg returns nothing
native TriggerGroupAdd		takes integer tg, trigger trig returns boolean
native TriggerGroupRun		takes integer tg returns nothing