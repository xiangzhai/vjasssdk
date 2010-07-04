// Without any range checks just as example.

// vJass
//! textmacro GENERIC_CONTAINER takes NAME, TYPE, SIZE
struct $NAME$
	private $TYPE$ array m_values[$SIZE$]
	public static constant integer size = $SIZE$

	public static method create takes $TYPE$ initValue, integer values returns thistype
		local thistype this = thistype.allocate()
		local integer i = 0
		loop
			exitwhen (i == values)
			set this.m_values[i] = initValue
			set i =i + 1
		endloop
		return this
	endmethod

	public static method create2 takes $TYPE$ initValue returns thistype
		return thistype.create(initValue, thistype.size)
	endmethod

	public method operator [] takes integer index returns $TYPE$
		return this.m_values[index]
	endmethod

	public method operator []= takes integer index, $TYPE$ value returns nothing
		set this.m_values[index] = value
	endmethod
endstruct
//! endtextmacro

//! runtextmacro GENERIC_CONTAINER("IntegerContaner", "integer", "8192")

globals
	IntegerContainer container = IntegerContainer.create2(10)
endglobals

// Jass++
template < T = type integer, integer valuesSize = 8192 >
class Container
{
	private T m_values[valuesSize]

	public new(T initValue, integer values = valuesSize)
	{
		integer i;

		for (i = 0; i < values; i++)
			this.m_values[i] = initValue;
	}

	public const T operator[](integer index) const
	{
		return this.m_values[index];
	}

	public T operator[](integer index)
	{
		return this.m_values[index];
	}

	public operator[]=(integer index, T value)
	{
		this.m_values[index] = value;
	}
};

class Container container = new (10);
	
