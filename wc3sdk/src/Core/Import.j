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

library ACore requires ACoreAi, optional ACoreDebug, ACoreEnvironment, ACoreGeneral, ACoreInterface, ACoreMaths, optional ACoreNet, ACoreString
endlibrary