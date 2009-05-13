library AStructSystemsCharacterAbstractCharacterSystem

	private interface AAbstractCharacterSystemInterface
		public method enable takes nothing returns nothing
		public method disable takes nothing returns nothing
		//static method create takes ACharacter character returns ACharacterSystemInterface
	endinterface

	struct AAbstractCharacterSystem extends AAbstractCharacterSystemInterface
		//start members
		private ACharacter character

		//start members

		public method getCharacter takes nothing returns ACharacter
			return this.character
		endmethod

		//convenience methods

		public method getUser takes nothing returns player
			return this.character.getUser()
		endmethod

		public method getUnit takes nothing returns unit
			return this.character.getUsedUnit()
		endmethod

		//methods

		public method enable takes nothing returns nothing
		endmethod

		public method disable takes nothing returns nothing
		endmethod

		public static method create takes ACharacter character returns AAbstractCharacterSystem
			local AAbstractCharacterSystem this = AAbstractCharacterSystem.allocate()
			//start members
			set this.character = character

			return this
		endmethod
	endstruct

endlibrary