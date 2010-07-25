library AStructCoreGeneralMap requires AInterfaceCoreGeneralContainer, optional ALibraryCoreDebugMisc

	/**
	* @author Tamino Dauth
	* Got some inspiration from @link http://www.cplusplus.com/reference/stl and @link http://www.cplusplus.com/reference/stl/map.
	* Note that maps of the ASL aren't sorted automatically.
	* Maps are containers which contain pairs of values and their corresponding keys.
	* When creating a new instance of A_MAP user can define element and key type.
	* E. g. you can create your custom unit map with string keys for accessing units by name:
	* @code
	* library MyLibrary initializer init
	* //! runtextmacro A_MAP("private", "MyUnitMap", "unit", "string", "null", "null", 8192, 20000, 8192)
	* globals
	* MyUnitMap units = MyUnitMap.create()
	* endglobals
	*
	* function init takes nothing returns nothing
	* call MyUnit["Peter"] = gg_unit_n0001
	* call MyUnit["Heinz"] = gg_unit_n0001
	* call MyUnit["Franz"] = gg_unit_n0001
	* endfunction
	* @endcode
	* By using methods @method $STRUCTNAME$.findKey and @method $STRUCTNAME$.findValue user can get iterator
	* which can be used to iterate all contained map elements.
	*/
	//! textmacro A_MAP takes STRUCTPREFIX, NAME, ELEMENTTYPE, KEYTYPE, NULLVALUE, KEYNULLVALUE, STRUCTSPACE, NODESPACE, ITERATORSPACE

		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$UnaryPredicate takes $ELEMENTTYPE$ value, $KEYTYPE$ key returns boolean

		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$BinaryPredicate takes $ELEMENTTYPE$ value0, $KEYTYPE$ key0, $ELEMENTTYPE$ value1, $KEYTYPE$ key1 returns boolean

		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$UnaryFunction takes $ELEMENTTYPE$ value, $KEYTYPE$ key returns nothing

		/// Generator.
		/// Allows filling some elements with the return value.
		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$Generator takes nothing returns $ELEMENTTYPE$

		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$BinaryOperation  takes $ELEMENTTYPE$ value0, $KEYTYPE$ key0, $ELEMENTTYPE$ value1, $KEYTYPE$ key1 returns $ELEMENTTYPE$

		private struct $NAME$Node[$NODESPACE$]
			private thistype m_next
			private thistype m_previous
			private $KEYTYPE$ m_key
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

			public method setKey takes $KEYTYPE$ key returns nothing
				set this.m_key = key
			endmethod

			public method key takes nothing returns $KEYTYPE$
				return this.m_key
			endmethod

			public method setData takes $ELEMENTTYPE$ data returns nothing
				set this.m_data = data
			endmethod

			public method data takes nothing returns $ELEMENTTYPE$
				return this.m_data
			endmethod

			public method hasNext takes nothing returns boolean
				return this.m_next != 0
			endmethod

			public method hasPrevious takes nothing returns boolean
				return this.m_previous != 0
			endmethod

			public static method create takes nothing returns thistype
				local thistype this = thistype.allocate()
				set this.m_next = 0
				set this.m_previous = 0
				set this.m_key = $KEYNULLVALUE$
				set this.m_data = $NULLVALUE$

				return this
			endmethod

			public method onDestroy takes nothing returns nothing
				set this.m_key = $KEYNULLVALUE$
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

			public method setKey takes $KEYTYPE$ key returns nothing
				if (this.m_node == 0) then
					return
				endif
				call this.m_node.setKey(key)
			endmethod

			public method key takes nothing returns $KEYTYPE$
				if (this.m_node == 0) then
					return $KEYNULLVALUE$
				endif
				return this.m_node.key()
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
			// members
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

			public method frontKey takes nothing returns $KEYTYPE$
				if (this.m_front == 0) then
					return $KEYNULLVALUE$
				endif

				return this.m_front.key()
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

			public method backKey takes nothing returns $KEYTYPE$
				if (this.m_back == 0) then
					return $KEYNULLVALUE$
				endif

				return this.m_back.key()
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
			* Searches the container for key @param key x and returns an iterator to it if
			* found, otherwise it returns 0.
			*/
			public method findKey takes $KEYTYPE$ key returns $NAME$Iterator
				local $NAME$Node node = this.m_front
				local $NAME$Iterator result = 0
				loop
					exitwhen (node == 0)
					if (node.key() == key) then
						set result = $NAME$Iterator.create()
						call result.setNode(node)
						exitwhen (true)
					endif
					set node = node.next()
				endloop
				return result
			endmethod

			public method findValue takes $ELEMENTTYPE$ value returns $NAME$Iterator
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

			/**
			* Searches the container for an element with a key of @param key and returns the number of
			* elements having that key. Because map containers do not allow for duplicate keys,
			* this means that the function actually returns 1 if an element with that key is found
			* and zero otherwise.
			*/
			public method countKeys takes $KEYTYPE$ key returns integer
				local $NAME$Node node = this.m_front
				loop
					exitwhen (node == 0)
					if (node.key() == key) then
						return 1
					endif
					set node = node.next()
				endloop
				return 0
			endmethod

			public method containsKey takes $KEYTYPE$ key returns boolean
				return this.countKeys(key) == 1
			endmethod

			public method countValues takes $ELEMENTTYPE$ value returns integer
				local $NAME$Node node = this.m_front
				local integer result = 0
				loop
					exitwhen (node == 0)
					if (node.data() == value) then
						set result = result + 1
					endif
					set node = node.next()
				endloop
				return result
			endmethod

			public method containsValue takes $ELEMENTTYPE$ value returns boolean
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

			public method contains takes $NAME$Iterator iterator returns boolean
				local $NAME$Node node = this.m_front
				loop
					exitwhen (node == 0)
					if (node == iterator.node()) then
						return true
					endif
					set node = node.next()
				endloop
				return false
			endmethod

			/**
			* The list container is extended by inserting a new element before the element at position @param position with value @param value and key @param key.
			* This effectively increases the container size by @param number.
			* The element can't be inserted if key @param key is already used by another one.
			* @param position If this value is 0 or less the new inserted element will become front and back (only element in map) if map is empty. Otherwise it won't be added.
			*/
			public method insert takes $NAME$Iterator position, $KEYTYPE$ key, $ELEMENTTYPE$ value returns nothing
				local $NAME$Node node
				local $NAME$Node tmpNode
				if (this.containsKey(key) or (position == 0 and not this.empty()) or (position != 0 and not this.contains(position))) then
					return
				endif
				set node = $NAME$Node.create()
				call node.setKey(key)
				call node.setData(value)
				if (position == 0) then
					set this.m_front = node
					set this.m_back = node
				else
					set tmpNode = position.node().previous()
					call position.node().setPrevious(node)

					if (tmpNode != 0) then
						call tmpNode.setNext(node)
					else
						set this.m_front = node
					endif
				endif
				set this.m_size = this.m_size + 1
			endmethod

			private method eraseNumberNode takes $NAME$Node first, $NAME$Node last returns nothing
				local $NAME$Node tmpNode
				loop
					exitwhen (first == 0)
					// check both since it can be front and back at the same time!
					if (first == this.m_front) then
						set this.m_front = first.next()
					endif
					if (first == this.m_back) then
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

			/// All the elements in the list container are dropped: they are removed from the list container, leaving it with a size of 0.
			public method clear takes nothing returns nothing
				local $NAME$Iterator first = this.begin()
				local $NAME$Iterator last = this.end()
				call this.eraseNumber(first, last)
				call first.destroy()
				call last.destroy()
			endmethod

			public method randomValue takes nothing returns $ELEMENTTYPE$
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

			public method randomKey takes nothing returns $KEYTYPE$
				local integer index
				local integer i
				local $NAME$Iterator iterator
				local $KEYTYPE$ result
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
				set result = iterator.key()
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

			public method operator[]= takes $KEYTYPE$ key, $ELEMENTTYPE$ value returns nothing
				local $NAME$Iterator iterator = this.findKey(key)
				if (iterator == 0) then
					set iterator = this.begin()
					call this.insert(iterator, key, value)
					call iterator.destroy()
				else
					call iterator.setData(value)
					call iterator.destroy()
				endif
			endmethod

			public method operator[] takes $KEYTYPE$ key returns $ELEMENTTYPE$
				local $NAME$Node node = this.m_front
				loop
					exitwhen (node == 0)
					if (node.key() == key) then
						return node.data()
					endif
					set node = node.next()
				endloop
				return $NULLVALUE$
			endmethod

			public method operator< takes thistype other returns boolean
				debug if (this == other) then
					debug call Print("Same map.")
				debug endif
				return this.m_size < other.m_size
			endmethod

			public static constant method maxInstances takes nothing returns integer
				return $STRUCTSPACE$
			endmethod
		endstruct

	//! endtextmacro

	/**
	* default maps, Jass data types
	* max instances = required struct space / biggest array member size
	* 400000 is struct space maximum
	* max instances = 8192 / 1 = 8192 since there is no array member
	*/
	//! runtextmacro A_MAP("", "AUnitMap", "unit", "string", "null", "null", "8192", "40000", "8192")

endlibrary