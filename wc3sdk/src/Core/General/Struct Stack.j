library AStructCoreGeneralStack requires optional ALibraryCoreDebugMisc

	//! textmacro A_STACK takes STRUCTPREFIX, NAME, TYPE
		$STRUCTPREFIX$ struct $NAME$DataNode
			//start members
			private $TYPE$ m_data
			private thistype m_next

			//start members

			public method data takes nothing returns $TYPE$
				return this.m_data
			endmethod

			public method next takes nothing returns thistype
				return this.m_next
			endmethod

			//methods

			public static method create takes $TYPE$ data, thistype next returns thistype
				local thistype this = thistype.allocate()
				//start members
				set this.m_data = data
				set this.m_next = next

				return this
			endmethod
		endstruct

		$STRUCTPREFIX$ struct $NAME$
			//dynamic members
			private integer m_maxSize
			//members
			private $NAME$DataNode m_dataNode
			private integer m_objects

			//dynamic members

			public method setMaxSize takes integer maxSize returns nothing
				set this.m_maxSize = maxSize
			endmethod

			public method maxSize takes nothing returns integer
				return this.m_maxSize
			endmethod

			//methods

			/// Adds a new elment to stack.
			public method push takes thistype data returns nothing
				local $NAME$DataNode dataNode
				if (this.m_objects < this.m_maxSize) then
					set dataNode = $NAME$DataNode.create(data, this.m_dataNode)
					debug if (dataNode != 0) then
						set this.m_dataNode = dataNode
						set this.m_objects = this.m_objects + 1
					debug else
						debug call Print("Stack is full - By Jass limit.")
					debug endif
				debug else
					debug call Print("Stack is full - By custom limit.")
				endif
			endmethod

			/// Returns the supreme element and removes it from stack.
			public method pop takes nothing returns integer
				local integer nodeData = 0
				local $NAME$DataNode oldDataNode
				debug if (this.m_dataNode != 0) then
					set oldDataNode = this.m_dataNode
					set this.m_dataNode = this.m_dataNode.next()
					set nodeData = oldDataNode.data()
					call oldDataNode.destroy()
					set this.m_objects = this.m_objects - 1
				debug else
					debug call Print("Stack is empty.")
				debug endif
				return nodeData
			endmethod

			public static method create takes integer maxSize returns thistype
				local thistype this = thistype.allocate()
				//dynamic members
				set this.m_maxSize = maxSize
				//members
				set this.m_dataNode = 0
				set this.m_objects = 0

				return this
			endmethod
		endstruct
	//! endtextmacro

	/// @todo Remove this after testing it.
	///! runtextmacro A_STACK("private", "AIntegerStack", "integer")

endlibrary