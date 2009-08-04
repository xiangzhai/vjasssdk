//! import "Systems/Character/Import.j"
//! import "Systems/Debug/Import.j" //debug
//! import "Systems/Gui/Import.j"
//! import "Systems/World/Import.j"

/**
* Requests all sub systems of the ASL.
* A system covers a specific range like character treatment or graphical user interfaces.
*/
library ASystems requires ASystemsCharacter, ASystemsDebug, ASystemsGui, ASystemsWorld
endlibrary