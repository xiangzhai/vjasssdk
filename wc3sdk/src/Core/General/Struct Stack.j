library AStructCoreGeneralStack requires ALibraryCoreDebugMisc

	//! textmacro A_STACK takes TYPE, TYPENAME
		private struct A$TYPENAME$DataNode
			//start members
			private $TYPE$ data
			private A$TYPENAME$DataNode next

			//start members

			public method getData takes nothing returns $TYPE$
				return this.data
			endmethod

			public method getNext takes nothing returns A$TYPENAME$DataNode
				return this.next
			endmethod

			//methods

			public static method create takes $TYPE$ data, A$TYPENAME$DataNode next returns A$TYPENAME$DataNode
				local A$TYPENAME$DataNode this = A$TYPENAME$DataNode.allocate()
				set this.data = data
				set this.next = next

				return this
			endmethod
		endstruct

		struct A$TYPENAME$Stack
			//dynamic members
			private integer maxSize
			//properties
			private A$TYPENAME$DataNode dataNode
			private integer objects

			//dynamic members

			public method setMaxSize takes integer maxSize returns nothing
				set this.maxSize = maxSize
			endmethod

			public method getMaxSize takes nothing returns integer
				return this.maxSize
			endmethod

			//methods

			/// Adds a new elment to stack.
			public method push takes $TYPE$ data returns nothing
				local A$TYPENAME$DataNode dataNode
				if (this.objects < this.maxSize) then
					set dataNode = A$TYPENAME$DataNode.create(data, this.dataNode)
					debug if (dataNode != 0) then
						set this.dataNode = dataNode
						set this.objects = this.objects + 1
					debug else
						debug call Print("Stack " + I2S(this) + " is full - By jass limit.") //ALibraryGeneralDebug
					debug endif
				debug else
					debug call Print("Stack " + I2S(this) + " is full - By custom limit.") //ALibraryGeneralDebug
				endif
			endmethod

			/// Returns the supreme element and removes it from stack.
			public method pop takes nothing returns integer
				local integer nodeData = 0
				local A$TYPENAME$DataNode oldDataNode
				debug if (this.dataNode != 0) then
					set oldDataNode = this.dataNode
					set this.dataNode = this.dataNode.getNext()
					set nodeData = oldDataNode.getData()
					call oldDataNode.destroy()
					set this.objects = this.objects - 1
				debug else
					debug call Print("Stack " + I2S(this) + " is empty.") //ALibraryGeneralDebug
				debug endif
				return nodeData
			endmethod

			public static method create takes integer maxSize returns A$TYPENAME$Stack
				local A$TYPENAME$Stack this = A$TYPENAME$Stack.allocate()
				//dynamic members
				set this.maxSize = maxSize
				//members
				set this.dataNode = 0
				set this.objects = 0

				return this
			endmethod
		endstruct
	//! endtextmacro

	/// @todo Remove this after testing it.
	//! runtextmacro A_STACK("integer", "Integer")

endlibrary