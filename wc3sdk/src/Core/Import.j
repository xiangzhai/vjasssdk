//! import "Core/Ai/Import.j"
static if (DEBUG_MODE) then
//! import "Core/Debug/Import.j"
endif
//! import "Core/Environment/Import.j"
//! import "Core/General/Import.j"
//! import "Core/Interface/Import.j"
//! import "Core/Maths/Import.j"
static if (A_NET) then
///! import "Core/Net/Import.j"
endif
//! import "Core/String/Import.j"

/**
* ASL core covers all core functions, structures and interfaces such as containers, player-only functions, maths stuff, string treatment functions and debugging functions and structures for instance.
* @see Asl
* @if A_OBJECTS
* @see AObjects
* @endif
* @if A_SYSTEMS
* @see ASystems
* @endif
*/
library ACore requires ACoreAi, optional ACoreDebug, ACoreEnvironment, ACoreGeneral, ACoreInterface, ACoreMaths, optional ACoreNet, ACoreString
endlibrary