//! import "Systems/Character/Import.j"
//! import "Systems/BonusMod/Import.j"
static if (A_DEBUG) then
//! import "Systems/Debug/Import.j"
endif
//! import "Systems/Gui/Import.j"
//! import "Systems/World/Import.j"

/**
* Requests all sub systems of the ASL.
* A system covers a specific range like character treatment or graphical user interfaces.
*/
library ASystems requires ASystemsCharacter, optional ASystemsDebug, ASystemsBonusMod, ASystemsGui, ASystemsWorld
endlibrary