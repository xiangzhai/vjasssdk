library AStructSystemsCharacterTalkLog requires AStructCoreGeneralVector, AStructSystemsCharacterAbstractCharacterSystem

	struct ATalkLog extends AAbstractCharacterSystem
		private AIntegerVector m_talks
		private AIntegerVector m_speeches

		public method addMessage takes ATalk talk, string message returns nothing
			local integer index = this.m_talks.find(talk)
			if (index == -1) then
				call this.m_talks.pushBack(talk)
				call this.m_speeches.pushBack(AStringVector.create())
				set index = this.m_talks.backIndex()
			endif
			call AStringVector(this.m_speeches[index]).pushBack(message)
		endmethod

		public method message takes ATalk talk, integer index returns string
			local integer talkIndex = this.m_talks.find(talk)
			if (talkIndex == -1) then
				return null
			endif
			if (index >= AStringVector(this.m_speeches[talkIndex]).size() or index < 0) then
				return null
			endif
			return AStringVector(this.m_speeches[talkIndex])[index]
		endmethod

		public method talk takes ATalk talk returns AStringVector
			local integer talkIndex = this.m_talks.find(talk)
			if (talkIndex == -1) then
				return 0
			endif
			return AStringVector.createByOther(AStringVector(this.m_speeches[talkIndex]))
		endmethod

		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate(character)
			//members
			set this.m_talks = AIntegerVector.create()
			set this.m_speeches = AIntegerVector.create()
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			//members
			call this.m_talks.destroy()
			loop
				exitwhen (this.m_speeches.empty())
				call AStringVector(this.m_speeches.back()).destroy()
				call this.m_speeches.popBack()
			endloop
			call this.m_speeches.destroy()
		endmethod
	endstruct

endlibrary