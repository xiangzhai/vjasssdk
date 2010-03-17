This file is deprecated since we should optimize declaration positions (using JassHelper option [forcemethodevaluate])

library A<Struct/Library><Module Name: Core/Objects/Systems><Sub Module Name><Name>

	//text macro calls

	//keywords

	globals
		//public constant globals
		//public globals
		//private constant globals
		//public globals
	endglobals

	//type definitions

	//function interfaces of interface

	//interfaces
	interface AInterfaceName
		//same as in struct definition
	endinterface

	//function interfaces of struct

	//structs
	struct AStructName
		//public static constant members
		//private static constant members
		//private static dynamic members
		//private static start members
		//private static start text members with the text prefix
		//private static members
		//private dynamic members
		//private start members
		//private members

		//module implementations

		//text macro calls

		//dynamic member methods

		//start member methods

		//member methods

		//convenience methods

		//public methods

		//private methods

		//create
		//You have to call the constructor with all start members
		//Dynamic members can be changed later (after construction) by using the set methods
		//If all members are dynamic, the constructor can take them instead of start members

		//additional create methods

		//private methods called from onDestroy

		//onDestroy

		//private methods called from onInit

		//onInit (usually private)

		//private methods called from init

		//public init method (init all static start members)
		//Use this instead of onInit if nothing should be initialized if the system is not be used

		//private methods called from cleanUp

		//public cleanUp method (uninit all static members)

		//public static set and get methods

		//public static methods

		//private static methods
	endstruct

	//functions

	//public functions

	//private functions

	//library initializer (usually private)

	//hooks

endlibrary