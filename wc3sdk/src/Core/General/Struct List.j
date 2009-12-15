library AStructCoreGeneralList

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
		$STRUCTPREFIX$ function interface $NAME$UnaryFunction takes $ELEMENTTYPE$ element returns nothing //Rückgabewert wurde vorerst rausgenommen, bis ich weiß, was er bringt

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
					set this.m_size = this.m_size + 1
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

			/// No reverse erasing.
			public method eraseNumber takes $NAME$Iterator first, $NAME$Iterator last returns nothing
				local $NAME$Node node = first.node()
				local $NAME$Node tmpNode

				loop
					exitwhen (node == last.node().next() or not (node == 0))
					if (node == this.m_front) then
						set this.m_front = node.next()
					elseif (node == this.m_back) then
						set this.m_back = node.previous()
					endif
					if (node.next() != 0) then
						call node.next().setPrevious(node.previous())
					endif
					if (node.previous() != 0) then
						call node.previous().setNext(node.next())
					endif
					set tmpNode = node
					set node = node.next()
					call tmpNode.destroy()
					set this.m_size = this.m_size -1
				endloop
			endmethod

			public method erase takes $NAME$Iterator position returns nothing
				call this.eraseNumber(position, position)
			endmethod

			/// All the elements in the list container are dropped: they are removed from the list container, leaving it with a size of 0.
			public method clear takes nothing returns nothing
				local $NAME$Iterator first = this.begin()
				local $NAME$Iterator last = this.end()
				call this.eraseNumber(first, last)
				call first.destroy()
				call last.destroy()
			endmethod

			public static method create takes nothing returns thistype
				local thistype this = thistype.allocate()
				set this.m_front = 0
				set this.m_back = 0

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

endlibrary