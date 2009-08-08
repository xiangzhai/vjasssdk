library AStructSystemsCharacterTalkLog requires AStructCoreGeneralVector, AStructSystemsCharacterAbstractCharacterSystem

	//! runtextmacro A_VECTOR("private", "AStringVectorVector", "AStringVector", "0", "100", "8192") /// @todo JASS_MAX_ARRAY_SIZE, vJass bug.

	struct ATalkLog extends AAbstractCharacterSystem
		private AIntegerVector m_talks
		private AStringVectorVector m_speeches
		
		public method addMessage takes ATalk talk, string message returns nothing
			local integer index = this.m_talks.find(talk)
			if (index == -1) then
				call this.m_talks.pushBack(talk)
				call this.m_speeches.pushBack(AStringVector.create())
				set index = this.m_talks.backIndex()
			endif
			call this.m_speeches[index].pushBack(message)
		endmethod
		
		public method message takes ATalk talk, integer index returns string
			local integer talkIndex = this.m_talks.find(talk)
			if (talkIndex == -1) then
				return null
			endif
			if (index >= this.m_speeches[talkIndex].size() or index < 0) then
				return null
			endif
			return this.m_speeches[talkIndex].at(index)
		endmethod
		
		public method talk takes ATalk talk returns AStringVector
			local integer talkIndex = this.m_talks.find(talk)
			if (talkIndex == -1) then
				return 0
			endif
			return AStringVector.createByOther(this.m_speeches[talkIndex])
		endmethod
		
		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate(character)
			//members
			set this.m_talks = AIntegerVector.create()
			set this.m_speeches = AStringVectorVector.create()
			return this
		endmethod
		
		public method onDestroy takes nothing returns nothing
			//members
			call this.m_talks.destroy()
			loop
				exitwhen (this.m_speeches.empty())
				call this.m_speeches[this.m_speeches.backIndex()].destroy()
				call this.m_speeches.popBack()
			endloop
			call this.m_speeches.destroy()
		endmethod
	endstruct

endlibrary