library AStructCoreGeneralList requires optional ALibraryCoreDebugMisc

	/**
	* @author Tamino Dauth
	* Got some inspiration from @link http://www.cplusplus.com/reference/stl and @link http://www.cplusplus.com/reference/stl/list.
	*/
	//! textmacro A_LIST takes STRUCTPREFIX, NAME, ELEMENTTYPE, NULLVALUE, STRUCTSPACE, NODESPACE, ITERATORSPACE

		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$UnaryPredicate takes $ELEMENTTYPE$ value returns boolean

		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$BinaryPredicate takes $ELEMENTTYPE$ value0, $ELEMENTTYPE$ value1 returns boolean

		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$UnaryFunction takes $ELEMENTTYPE$ value returns nothing //Rückgabewert wurde vorerst rausgenommen, bis ich weiß, was er bringt

		/// Generator.
		/// Allows filling some elements with the return value.
		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$Generator takes nothing returns $ELEMENTTYPE$

		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$BinaryOperation  takes $ELEMENTTYPE$ value0, $ELEMENTTYPE$ value1 returns $ELEMENTTYPE$

		private struct $NAME$Node[$NODESPACE$]
			private thistype m_next
			private thistype m_previous
			private $ELEMENTTYPE$ m_data

			public method setNext takes thistype next returns nothing
				set this.m_next = next
			endmethod

			public method next takes nothing returns thistype
				return this.m_next
			endmethod

			public method setPrevious takes thistype previous returns nothing
				set this.m_previous = previous
			endmethod

			public method previous takes nothing returns thistype
				return this.m_previous
			endmethod

			public method setData takes $ELEMENTTYPE$ data returns nothing
				set this.m_data = data
			endmethod

			public method data takes nothing returns $ELEMENTTYPE$
				return this.m_data
			endmethod

			public static method create takes nothing returns thistype
				local thistype this = thistype.allocate()
				set this.m_next = 0
				set this.m_previous = 0
				set this.m_data = $NULLVALUE$

				return this
			endmethod

			public method onDestroy takes nothing returns nothing
				set this.m_data = $NULLVALUE$
			endmethod
		endstruct

		$STRUCTPREFIX$ struct $NAME$Iterator[$ITERATORSPACE$]
			private $NAME$Node m_node

			/// Required by list struct.
			public method setNode takes $NAME$Node node returns nothing
				set this.m_node = node
			endmethod

			/// Required by list struct.
			public method node takes nothing returns $NAME$Node
				return this.m_node
			endmethod

			public method isValid takes nothing returns boolean
				return not (this.m_node == 0)
			endmethod

			public method hasNext takes nothing returns boolean
				return this.m_node != 0 and this.m_node.next() != 0
			endmethod

			public method hasPrevious takes nothing returns boolean
				return this.m_node != 0 and this.m_node.previous() != 0
			endmethod

			/// Similar to C++'s ++ iterators operator.
			public method next takes nothing returns nothing
				if (this.m_node == 0) then
					return
				endif

				set this.m_node = this.m_node.next()
			endmethod

			/// Similar to C++'s -- iterators operator.
			public method previous takes nothing returns nothing
				if (this.m_node == 0) then
					return
				endif

				set this.m_node = this.m_node.next()
			endmethod

			public method setData takes $ELEMENTTYPE$ data returns nothing
				if (this.m_node == 0) then
					return
				endif
				call this.m_node.setData(data)
			endmethod

			public method data takes nothing returns $ELEMENTTYPE$
				if (this.m_node == 0) then
					return $NULLVALUE$
				endif
				return this.m_node.data()
			endmethod

			/**
			* @todo If you want to implement toBack and toFront (like Qt does) you'll have to save parent struct instance ...
			*/

			public static method create takes nothing returns thistype
				local thistype this = thistype.allocate()
				set this.m_node = 0

				return this
			endmethod

			public method operator== takes thistype iterator returns boolean
				return this.m_node == iterator.m_node
			endmethod
		endstruct

		$STRUCTPREFIX$ struct $NAME$[$STRUCTSPACE$]
			//members
			private $NAME$Node m_front
			private $NAME$Node m_back
			private integer m_size

			public method begin takes nothing returns $NAME$Iterator
				local $NAME$Iterator begin = $NAME$Iterator.create()
				call begin.setNode(this.m_front)

				return begin
			endmethod

			/// Does not reference the past-end element rather than the last one.
			public method end takes nothing returns $NAME$Iterator
				local $NAME$Iterator end = $NAME$Iterator.create()
				call end.setNode(this.m_back)

				return end
			endmethod

			public method size takes nothing returns integer
				return this.m_size
			endmethod

			/**
			* @return Returns the first element value of list.
			*/
			public method front takes nothing returns $ELEMENTTYPE$
				if (this.m_front == 0) then
					return $NULLVALUE$
				endif

				return this.m_front.data()
			endmethod

			/**
			* @return Returns the last element value of list.
			*/
			public method back takes nothing returns $ELEMENTTYPE$
				if (this.m_back == 0) then
					return $NULLVALUE$
				endif

				return this.m_back.data()
			endmethod

			public method empty takes nothing returns boolean
				return this.m_size == 0
			endmethod

			/**
			* Inserts a new element at the beginning of the list, right before its current first element. The content of this new element is initialized to @param value.
			* This effectively increases the list size by one.
			*/
			public method pushFront takes $ELEMENTTYPE$ value returns nothing
				local $NAME$Node node = this.m_front
				set this.m_front = $NAME$Node.create()
				call this.m_front.setData(value)
				if (node != 0) then
					call this.m_front.setNext(node)
					call node.setPrevious(this.m_front)
				else
					set this.m_back = this.m_front
				endif
				set this.m_size = this.m_size + 1
			endmethod

			/**
			* Removes the first element in the list container, effectively reducing the list size by one.
			* This doesn't call the removed element's destructor!
			*/
			public method popFront takes nothing returns nothing
				local $NAME$Node node = this.m_front
				if (node != 0) then
					if (node.next() != 0) then
						call node.next().setPrevious(0)
					else
						set this.m_back = 0
					endif
					set this.m_front = node.next() // could be 0!
					call node.destroy()
					set this.m_size = this.m_size - 1
				endif
			endmethod

			/**
			* Adds a new element at the end of the list, right after its current last
			* element. The content of this new element is initialized to @param value.
			* This effectively increases the list size by one.
			*/
			public method pushBack takes $ELEMENTTYPE$ value returns nothing
				local $NAME$Node node = this.m_back
				set this.m_back = $NAME$Node.create()
				call this.m_back.setData(value)
				if (node != 0) then
					call this.m_back.setPrevious(node)
					call node.setNext(this.m_back)
				else
					set this.m_front = this.m_back
				endif
				set this.m_size = this.m_size + 1
			endmethod

			/**
			* Removes the last element in the list container, effectively reducing the list size by one.
			* This calls the removed element's destructor.
			*/
			public method popBack takes nothing returns nothing
				local $NAME$Node node = this.m_back
				if (node != 0) then
					if (node.previous() != 0) then
						call node.previous().setNext(0)
					else
						set this.m_front = 0
					endif
					set this.m_back = node.previous() // could be 0!
					call node.destroy()
					set this.m_size = this.m_size - 1
				endif
			endmethod

			/**
			* The list container is extended by inserting new elements before the element at position @param position with value @param value.
			* This effectively increases the container size by @param number.
			*/
			public method insertNumber takes $NAME$Iterator position, integer number,  $ELEMENTTYPE$ value returns nothing
				local $NAME$Node node = position.node().previous()
				local $NAME$Node tmpNode
				local integer i = 0

				loop
					exitwhen (i == number)
					set tmpNode = $NAME$Node.create()
					call tmpNode.setData(value)
					call tmpNode.setPrevious(node)
					call tmpNode.setNext(position.node()) // this call actually can be removed
					if (node != 0) then
						call node.setNext(tmpNode)
					else
						set this.m_front = tmpNode
					endif
					set i = i + 1
				endloop
				set this.m_size = this.m_size + i
				if (i > 0) then
					call position.node().setPrevious(tmpNode)
				endif
			endmethod

			public method insert takes $NAME$Iterator position, $ELEMENTTYPE$ value returns nothing
				call this.insertNumber(position, 1, value)
			endmethod

			private method eraseNumberNode takes $NAME$Node first, $NAME$Node last returns nothing
				local $NAME$Node tmpNode
				loop
					exitwhen (first == 0)
					if (first == this.m_front) then
						set this.m_front = first.next()
					elseif (first == this.m_back) then
						set this.m_back = first.previous()
					endif
					if (first.next() != 0) then
						call first.next().setPrevious(first.previous())
					endif
					if (first.previous() != 0) then
						call first.previous().setNext(first.next())
					endif
					if (first == last) then
						call first.destroy()
						set first = 0
					else
						set tmpNode = first
						set first = first.next()
						call tmpNode.destroy()
					endif
					set this.m_size = this.m_size -1
				endloop
			endmethod

			/// No reverse erasing.
			public method eraseNumber takes $NAME$Iterator first, $NAME$Iterator last returns nothing
				call this.eraseNumberNode(first.node(), last.node())
			endmethod

			public method erase takes $NAME$Iterator position returns nothing
				call this.eraseNumber(position, position)
			endmethod

			/**
			* Searches the container for an element with a value of @param value and returns an iterator to it
			* if found, otherwise it returns 0.
			*/
			public method find takes $ELEMENTTYPE$ value returns $NAME$Iterator
				local $NAME$Node node = this.m_front
				local $NAME$Iterator result = 0
				loop
					exitwhen (node == 0)
					if (node.data() == value) then
						set result = $NAME$Iterator.create()
						call result.setNode(node)
						exitwhen (true)
					endif
					set node = node.next()
				endloop
				return result
			endmethod

			public method contains takes $ELEMENTTYPE$ value returns boolean
				local $NAME$Node node = this.m_front
				loop
					exitwhen (node == 0)
					if (node.data() == value) then
						return true
					endif
					set node = node.next()
				endloop
				return false
			endmethod

			/**
			* Removes from the list all the elements with a specific value.
			* This reduces the list size by the amount of elements removed.
			*
			* Unlike member function @method List.erase, which erases elements by their
			* position (iterator), this function (@method List.remove) removes elements by their value.
			*
			* A similar function, @method List.removeIf, exists, which allows for a condition other than a plain value comparison to be performed on each element in order to determine the elements to be removed.
			*
			* @param value Value of the elements to be removed.
			*/
			public method remove takes $ELEMENTTYPE$ value returns nothing
				local $NAME$Iterator iterator
				loop
					set iterator = this.find(value)
					exitwhen (iterator == 0)
					call this.erase(iterator)
					call iterator.destroy()
				endloop
			endmethod

			/**
			Remove elements fulfilling condition
			* Removes from the list all the elements for which predicate @param predicate
			* returns true.
			* This reduces the list size by the amount of elements removed.
			*
			* Predicate @param predicate can be implemented as any typed expression *
			* taking one argument of the same type as the list and returning a @type boolean
			*
			* The function calls predicate(i.data()) for each element (where i is an iterator to that element).
			* Any of the elements in the list for which this returns true, is removed from the container.
			*
			* @param predicate Unary predicate that, taking a value of the same type as those contained in the list object, returns true for those values to be removed from the container, and false for those remaining.
			*/
			public method removeIf takes $NAME$UnaryPredicate predicate returns nothing
				local $NAME$Iterator iterator = this.begin()
				local $NAME$Iterator tmpIterator
				loop
					exitwhen (not iterator.isValid())
					if (predicate.evaluate(iterator.data())) then
						set tmpIterator = iterator
						call iterator.next()
						call this.erase(tmpIterator)
					else
						call iterator.next()
					endif
				endloop
				call iterator.destroy()
			endmethod

			/// All the elements in the list container are dropped: they are removed from the list container, leaving it with a size of 0.
			public method clear takes nothing returns nothing
				local $NAME$Iterator first = this.begin()
				local $NAME$Iterator last = this.end()
				call this.eraseNumber(first, last)
				call first.destroy()
				call last.destroy()
			endmethod

			public method random takes nothing returns $ELEMENTTYPE$
				local integer index
				local integer i
				local $NAME$Iterator iterator
				local $ELEMENTTYPE$ result
				if (this.empty()) then
					return $NULLVALUE$
				endif
				set index = GetRandomInt(0, this.m_size - 1)
				set iterator = this.begin()
				set i = 0
				loop
					exitwhen (i == index)
					call iterator.next()
					set i = i + 1
				endloop
				set result = iterator.data()
				call iterator.destroy()
				return result
			endmethod

			public method randomIterator takes nothing returns $NAME$Iterator
				local integer index
				local integer i
				local $NAME$Iterator iterator
				if (this.empty()) then
					return 0
				endif
				set index = GetRandomInt(0, this.m_size - 1)
				set iterator = this.begin()
				set i = 0
				loop
					exitwhen (i == index)
					call iterator.next()
					set i = i + 1
				endloop
				return iterator
			endmethod

			public method operator< takes thistype other returns boolean
				debug if (this == other) then
					debug call Print("Same list.")
				debug endif
				return this.m_size < other.m_size
			endmethod

			public static method create takes nothing returns thistype
				local thistype this = thistype.allocate()
				set this.m_front = 0
				set this.m_back = 0
				set this.m_size = 0

				return this
			endmethod

			/// List will be cleared before destruction.
			public method onDestroy takes nothing returns nothing
				call this.clear()
			endmethod

			public static constant method maxInstances takes nothing returns integer
				return $STRUCTSPACE$
			endmethod
		endstruct

	//! endtextmacro

	/**
	* default lists, Jass data types
	* max instances = required struct space / biggest array member size
	* 400000 is struct space maximum
	* max instances = 8192 / 1 = 8192 since there is no array member
	*/
	//! runtextmacro A_LIST("", "AIntegerList", "integer", "0", "8192", "40000", "8192")
	//! runtextmacro A_LIST("", "AStringList", "string", "null", "8192", "40000", "8192")
	//! runtextmacro A_LIST("", "ABooleanList", "boolean", "false", "8192", "40000", "8192")
	//! runtextmacro A_LIST("", "ARealList", "real", "0.0", "8192", "40000", "8192")
	//! runtextmacro A_LIST("", "AHandleList", "handle", "null", "8192", "40000", "8192")

	//! runtextmacro A_LIST("", "APlayerList", "player", "null", "8192", "40000", "8192")

endlibrary