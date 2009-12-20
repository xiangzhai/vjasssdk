library AStructSystemsCharacterAbstractCharacterSystem

	private interface AAbstractCharacterSystemInterface
		public method enable takes nothing returns nothing
		public method disable takes nothing returns nothing
		public method changedUnit takes nothing returns nothing
		//static method create takes ACharacter character returns ACharacterSystemInterface
	endinterface

	struct AAbstractCharacterSystem extends AAbstractCharacterSystemInterface
		//dynamic members
		private boolean m_enableAgain
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

		public method setEnableAgain takes boolean enableAgain returns nothing
			set this.m_enableAgain = enableAgain
		endmethod

		public method enableAgain takes nothing returns boolean
			return this.m_enableAgain
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

		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate()
			//dynamic members
			set this.m_enableAgain = true
			//start members
			set this.m_character = character
			//members
			set this.m_isEnabled = true

			return this
		endmethod
	endstruct

endlibrary