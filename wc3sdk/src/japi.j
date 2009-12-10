//custom natives
//
native xGetUnitDefenseType	takes unit u returns integer
native xGetDefenseType		takes integer i returns integer
native xGetUnitAttackType1	takes unit u returns integer
native xGetAttackType1		takes integer i returns integer
native xGetUnitAttackType2	takes unit u returns integer
native xGetAttackType2		takes integer i returns integer
native xGetUnitWeaponType1	takes unit u returns integer
native xGetWeaponType1		takes integer i returns integer
native xGetUnitWeaponType2	takes unit u returns integer
native xGetWeaponType2		takes integer i returns integer
native DumpUnitInfo		takes unit u returns nothing

native ArrayAlloc	takes integer size returns integer
native ArraySetI	takes integer obj, integer i, integer v returns integer
native ArrayGetI	takes integer obj, integer i returns integer
native ArrayFree	takes integer obj returns nothing

//STL deque
native DequeNew		takes nothing returns integer
native DequePushFrontI	takes integer obj, integer v returns integer
native DequePushBackI	takes integer obj, integer v returns integer
native DequeFrontI	takes integer obj returns integer
native DequeBackI	takes integer obj returns integer
native DequePopFrontI	takes integer obj returns integer
native DequePopBackI	takes integer obj returns integer
native DequeFree	takes integer obj returns nothing

////STL map
native MapNew		takes nothing returns integer
native MapSetII		takes integer obj, integer key, integer value returns integer
native MapGetII		takes integer obj, integer key returns integer
native MapSetSI		takes integer obj, string key, integer value returns integer
native MapGetSI		takes integer obj, string key returns integer
native MapFree		takes integer obj returns nothing

//lisp style lists
native consII		takes integer car, integer cdr returns integer
native carI		takes integer pair returns integer
native cdrI		takes integer pair returns integer
native destroypair	takes integer pair returns nothing

//Measure wall clock elapsed time
native StopWatchCreate	takes nothing returns integer
native StopWatchMark	takes integer obj returns real
native StopWatchDestroy	takes integer obj returns nothing

//fast "return bug" functions that aren't bugged
native ItoR		takes integer i returns real
native RtoI		takes real r returns integer

//Look at source to see what these compute
native Encrypt		takes integer x, integer y, integer z returns nothing
native Decrypt		takes integer x, integer y, integer z returns nothing

native DebugPrint	takes string msg returns nothing