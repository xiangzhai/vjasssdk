static if (A_RTC) then
//! import "rtc.j"
endif
static if (A_JAPI) then
//! import "japi.j"
endif
//! import "Core/Import.j"
static if (A_SYSTEMS) then
//! import "Systems/Import.j"
endif
static if (A_OBJECTS) then
//! import "Objects/Import.j"
endif

/**
* The Advanced Script Libray library.
* Request this library if you want to use any system of the ASL.
*/
library Asl requires ACore, optional AObjects, ASystems
endlibrary