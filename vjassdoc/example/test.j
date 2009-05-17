/// Hello world.
/// Peter.
/// Hans.
/// In this phrase interface @interface MyStructInterface
// TEST
library AnotherLibrary initializer init
	private keyword Jesus
	private keyword Auto
	private keyword Lalala

	globals
		integer tut1 = 0
		public integer tut2 = GetRandomInt(0, 10)
		private integer tut3 = -3
		constant string myString = "Hallo"
		public constant string myOtherString = "Bis dann"
		private constant string andAnotherOne = "Brumm brumm, die Feuerwehr"
	endglobals

	function interface TestFunctionInterface takes nothing returns nothing

	interface MyStructInterface
		public method aMethod takes integer a, integer b, boolean c returns real defaults 10.0
	endinterface
	
	struct MyStruct extends MyStructInterface
		private real x
		real y
		public real z
		
		public method setX takes real x returns nothing
			set this.x = x
		endmethod
		
		public method getX takes nothing returns real
			return this.x
		endmethod
		
		public stub method stubMethod takes nothing returns nothing
		endmethod
		
		/// @param x x is x.
		public static method create takes real x returns MyStruct
			local MyStruct this = MyStruct.allocate()
			set this.x = x
			return this
		endmethod
			
		public method onDestroy takes nothing returns nothing
		endmethod
			
		private static method onInit takes nothing returns nothing
		endmethod
	endstruct
	
	/// This function is public
	public function init takes nothing returns nothing
	endfunction
	
	function aLibraryFunction takes nothing returns boolean
		return true
	endfunction
	
	private function aPrivateLibraryFunction takes nothing returns nothing
	endfunction
endlibrary

/// This library is so cool.
/// @author Baradé
/// @todo Nothing
library MyLibrary requires AnotherLibrary /// another comment
	//! textmacro A_MACRO takes TYPE, TYPENAME
		function A$TYPENAME$Function takes $TYPE$ $TYPENAME$ returns $TYPE$
			return $TYPE$
		endfunction
	//! endtextmacro
	
	//! runtextmacro A_MACRO("integer", "Integer") /// TEST!
	/// @author Baradé
	//! runtextmacro A_MACRO("boolean", "Boolean")
endlibrary

type MyType extends integer array[10]

library TypeLibrary requires MyLibrary, AnotherLibrary
	type MyOtherType extends MyType
endlibrary

scope MyScope initializer privateInitializer

	private function privateInitializer takes nothing returns nothing
	endfunction
	
endscope

library_once LastLibrary requires MyLibrary, AnotherLibrary

	scope NestedScope
	endscope
	
	private scope PrivateNestedScope
	endscope

	private interface PrivateInterface
		public method test takes nothing returns integer defaults 10
	endinterface
	
endlibrary

struct ArrayStruct //extends array[10]
	delegate integer delegateMember = 0
endstruct

module MyModule
	public static method Bla takes nothing returns nothing
		local integer i = 0
		loop
			exitwhen (i == bj_MAX_PLAYERS)
			call DisplayTimedTextToPlayer(Player(i), 0.0, 0.0, 20.0, "Peter und ein Huhn, die hatten viel zu tun.")
			set i = i + 1
		endloop
	endmethod
endmodule

module AndTheOtherModule
	implement optional MyModule
endmodule

library ModuleLibrary

	private scope ModuleScope

		private module PrivateModule
		endmodule

	endscope

endlibrary

struct ModuleImplementingStruct
	implement AndTheOtherModule
endstruct

struct TestStruct1
	public static integer peter = 10
	public static integer hans = thistype.peter
endstruct

struct TestStruct2 extends TestStruct1
	public static integer herbert = super.peter + super.hans
	public static thistype aValue
	public static super bValue
endstruct

struct OperatorStruct
	private real m_test

	method operator test takes nothing returns real
		return this.m_test
	endmethod

	method operator test= takes real test returns nothing
		set this.m_test = test
	endmethod
endstruct


globals
	integer randomInteger = GetRandomInt(0, 10) + GetRandomInt(11, 100)
endglobals


interface ExtensionInterface
endinterface

struct ExtensionStruct1 extends ExtensionInterface
endstruct

struct ExtensionStruct2 extends ExtensionStruct1
endstruct

struct ExtensionStruct3 extends ExtensionStruct2
endstruct

//Parameter check
native Bla takes integer x returns nothing
native Bla2 takes integer x , integer y returns nothing
native Bla3 takes integer x, integer y returns nothing
native Bla4 takes integer x,integer y returns nothing 