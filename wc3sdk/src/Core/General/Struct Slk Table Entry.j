/// Do not use this library, it is unfinished!
/// @todo Finish and test this library.
library AStructCoreGeneralSlkTableEntry

	/// This struct is a basic inheritance struct for SLK tables.
	/// SLK tables are used by Warcraft 3 for saving object data of various kinds of objects like doodads, units etc..
	/// @todo There exists a problem with strings because their quotes will be ignored.
	struct ASlkTableEntry
		private static ASlkTableEntry array entry

		/// This method is based on the JassHelper manual's example.
		public static method getEntry takes integer index returns ASlkTableEntry
			if (ASlkTableEntry.entry[index] == 0) then
				set ASlkTableEntry.entry[index] = ASlkTableEntry.create()
			endif
			return ASlkTableEntry.entry[index]
		endmethod
	endstruct

endlibrary