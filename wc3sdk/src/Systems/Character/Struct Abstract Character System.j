library AStructSystemsCharacterAbstractCharacterSystem

	private interface AAbstractCharacterSystemInterface
		public method enable takes nothing returns nothing
		public method disable takes nothing returns nothing
		public method changedUnit takes nothing returns nothing
		//static method create takes ACharacter character returns ACharacterSystemInterface
	endinterface

	struct AAbstractCharacterSystem extends AAbstractCharacterSystemInterface
		//start members
		private ACharacter m_character
		//members
		private boolean m_isEnabled

		//start members

		public method character takes nothing returns ACharacter
			return this.m_character
		endmethod
		
		//members
		
		public method isEnabled takes nothing returns boolean
			return this.m_isEnabled
		endmethod

		//convenience methods

		public method user takes nothing returns player
			return this.m_character.user()
		endmethod

		public method unit takes nothing returns unit
			return this.m_character.unit()
		endmethod

		//methods

		public stub method enable takes nothing returns nothing
			set this.m_isEnabled = true
		endmethod

		public stub method disable takes nothing returns nothing
			set this.m_isEnabled = false
		endmethod
		
		/// @todo Friend relation to @struct ACharacter, do not use!
		/// Is called when character unit is changed. Change all registered specific unit events.
		public stub method changedUnit takes nothing returns nothing
		endmethod

		public static method create takes ACharacter character returns AAbstractCharacterSystem
			local AAbstractCharacterSystem this = AAbstractCharacterSystem.allocate()
			//start members
			set this.m_character = character
			//members
			set this.m_isEnabled = true

			return this
		endmethod
	endstruct

endlibrary