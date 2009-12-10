//! zinc

library AStructCoreGeneralList
{

	/**
	* @author Tamino Dauth
	* Got some inspiration from @link http://www.cplusplus.com/reference/stl and @link http://www.cplusplus.com/reference/stl/list.
	*/
	//! textmacro A_LIST takes STRUCTPREFIX, NAME, ELEMENTTYPE, NULLVALUE, STRUCTSPACE, ELEMENTSPACE, ITERATORSPACE

		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ type $NAME$UnaryPredicate extends function($ELEMENTTYPE$ value) -> boolean

		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ type $NAME$BinaryPredicate extends function($ELEMENTTYPE$ value0, $ELEMENTTYPE$ value1) -> boolean

		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ type $NAME$UnaryFunction extends function($ELEMENTTYPE$ element) //Rückgabewert wurde vorerst rausgenommen, bis ich weiß, was er bringt

		/// Generator.
		/// Allows filling some elements with the return value.
		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ type $NAME$Generator extends function() -> $ELEMENTTYPE$

		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ type $NAME$BinaryOperation extends function($ELEMENTTYPE$ value0, $ELEMENTTYPE$ value1) -> $ELEMENTTYPE$

		private struct $NAME$Node[$ELEMENTSPACE$]
		{
			private thistype m_next;
			private thistype m_previous;
			private $ELEMENTTYPE$ m_data;

			public method setNext($ELEMENTTYPE$ next)
			{
				this.m_next = next;
			}

			public method next() -> $ELEMENTTYPE$
			{
				return this.m_next;
			}

			public method setPrevious($ELEMENTTYPE$ previous)
			{
				this.m_previous = previous;
			}

			public method previous() -> $ELEMENTTYPE
			{
				return this.m_previous;
			}

			public method setData($ELEMENTTYPE$ data)
			{
				this.m_data = data;
			}

			public method data() -> $ELEMENTTYPE$
			{
				return this.m_data;
			}

			public static method create() -> thistype
			{
				thistype this = thistype.allocate();
				this.m_next = 0;
				this.m_previous = 0;
				this.m_data = $NULLVALUE$;

				return this;
			}

			public method onDestroy()
			{
				this.m_data = $NULLVALUE$;
			}
		}

		$STRUCTPREFIX$ struct $NAME$Iterator[$ITERATORSPACE$]
		{
			private $NAME$Node m_node;

			/// Required by list struct.
			public method setNode($NAME$Node node)
			{
				this.m_node = node;
			}

			/// Required by list struct.
			public method node() -> $NAME$Node
			{
				return this.m_node;
			}

			public method isValid() -> boolean
			{
				return !this.m_node == 0;
			}

			public method hasNext() -> boolean
			{
				return this.m_node != 0 && this.m_node.next() != 0;
			}

			public method hasPrevious() -> boolean
			{
				return this.m_node != 0 && this.m_node.previous() != 0;
			}

			/// Similar to C++'s ++ iterators operator.
			public method next()
			{
				if (this.m_node == 0)
				{
					return;
				}

				this.m_node = this.m_node.next();
			}

			/// Similar to C++'s -- iterators operator.
			public method previous()
			{
				if (this.m_node == 0)
				{
					return;
				}

				this.m_node = this.m_node.next();
			}

			/**
			* @todo If you want to implement toBack and toFront (like Qt does) you'll have to save parent struct instance ...
			*/

			public static method create() -> thistype
			{
				thistype this = thistype.allocate();
				this.m_node = 0;

				return this;
			}

			public method operator==(thistype iterator) -> boolean
			{
				return this.m_node == iterator.m_node;
			}
		}


		$STRUCTPREFIX$ struct $NAME$[$STRUCTSPACE$]
			//members
			private $NAME$Node m_front;
			private $NAME$Node m_back;
			private integer m_size;

			public method begin() -> $NAME$Iterator
			{
				$NAME$Iterator begin = $NAME$Iterator.create();
				begin.setNode(this.m_front);

				return begin;
			}

			/// Does not reference the past-end element rather than the last one.
			public method end() -> $NAME$Iterator
			{
				$NAME$Iterator end = $NAME$Iterator.create();
				end.setNode(this.m_back);

				return end;
			}

			public method size() -> integer
			{
				return this.m_size;
			}

			/**
			* @return Returns the first element value of list.
			*/
			public method front() -> $ELEMENTTYPE$
			{
				if (this.m_front == 0)
				{
					return $NULLVALUE$;
				}

				return this.m_front.data();
			}

			/**
			* @return Returns the last element value of list.
			*/
			public method back() -> $ELEMENTTYPE$
			{
				if (this.m_back == 0)
				{
					return $NULLVALUE$;
				}

				return this.m_back.data();
			}

			public method empty() -> boolean
			{
				return this.m_size == 0;
			}

			public method eraseNumber($NAME$Iterator first, $NAME$Iterator last)
			{
				$NAME$Node node;

				do
				{
					if (first.node() == this.m_back)
					{
						this.m_back = first.node().previous();
					}
					else if (first.node().next() != 0)
					{
						first.node().next().setPrevious(first.node().previous());
					}

					if (first.node() == this.m_front)
					{
						this.m_front = first.node().next();
					}
					else if (first.node().previous() != 0)
					{
						first.node().previous().setNext(first.node().next());
					}

					if (first == last)
					{
						first.node().destroy();
					}
					else
					{
						node = first.node().next();
						first.node().destroy();
						first.setNode(node);
					}

					this.m_size -= 1;
				}
				while (first.isValid());
			}

			/// All the elements in the list container are dropped: they are removed from the list container, leaving it with a size of 0.
			public method clear()
			{
				$NAME$Iterator first = this.begin();
				$NAME$Iterator last = this.end();
				this.eraseNumber(first, last)
				first.destroy();
				last.destroy();
			}

			public static method create() -> thistype
			{
				thistype this = thistype.allocate();
				this.m_front = 0;
				this.m_back = 0;

				return this;
			}

			/// List will be cleared before destruction.
			public method onDestroy()
			{
				this.clear();
			}

			public static constant method maxInstances() -> integer
			{
				return $STRUCTSPACE$;
			}
		}

	//! endtextmacro
}

//! endzinc