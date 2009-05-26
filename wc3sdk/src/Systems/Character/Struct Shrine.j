library AStructSystemsCharacterShrine requires ALibraryCoreDebugMisc, ALibraryCoreEnvironmentSound, ALibraryCoreEnvironmentSpecialEffect, AStructSystemsCharacterCharacterHashTable, AStructSystemsCharacterCharacter, AStructSystemsCharacterRevival

	struct AShrine
		//static start members
		private static string effectPath
		private static string soundPath
		private static string textMessage
		//start members
		private destructable usedDestructable
		private rect discoverRect
		private rect revivalRect
		private real m_facing
		//members
		private trigger shrineTrigger
		private region discoverRegion
		private effect discoverEffect

		//! runtextmacro A_STRUCT_DEBUG("\"AShrine\"")

		//start members

		public method getUsedDestructable takes nothing returns destructable
			return this.usedDestructable
		endmethod

		public method getDiscoverRect takes nothing returns rect
			return this.discoverRect
		endmethod

		public method getRevivalRect takes nothing returns rect
			return this.revivalRect
		endmethod
		
		public method facing takes nothing returns real
			return this.m_facing
		endmethod

		//methods

		public method enableForCharacter takes ACharacter character returns nothing
			local player user = character.getUser()
			if (ACharacter.getPlayerCharacter(user).getShrine() != 0) then
				call ACharacter.getPlayerCharacter(user).getShrine().disableForCharacter(ACharacter.getPlayerCharacter(user)) //disable old
			endif
			call character.setShrine(this)
			call character.getRevival().setX(GetRandomReal(GetRectMinX(this.revivalRect), GetRectMaxX(this.revivalRect)))
			call character.getRevival().setY(GetRandomReal(GetRectMinY(this.revivalRect), GetRectMaxY(this.revivalRect)))
			call character.getRevival().setFacing(this.m_facing)
			if (AShrine.effectPath != null) then
				set this.discoverEffect = CreateSpecialEffectForPlayer(user, AShrine.effectPath, GetDestructableX(this.usedDestructable), GetDestructableY(this.usedDestructable))
			endif
			if (AShrine.soundPath != null) then
				call PlaySoundPathForPlayer(user, AShrine.soundPath)
			endif
			call character.displayMessage(ACharacter.messageTypeInfo, AShrine.textMessage)
			set user = null
		endmethod

		private method disableForCharacter takes ACharacter character returns nothing
			debug if (character.getShrine() == this) then
				call character.setShrine(0)
				if (AShrine.effectPath != null) then
					call DestroyEffect(this.discoverEffect)
					set this.discoverEffect = null
				endif
			debug else
				debug call this.print("Is not the shrine of character " + I2S(character) + ".")
			debug endif
		endmethod

		private method createDiscoverRegion takes nothing returns nothing
			set this.discoverRegion = CreateRegion()
			call RegionAddRect(this.discoverRegion, this.discoverRect)
		endmethod

		private static method triggerConditionEnable takes nothing returns boolean
			local trigger triggeringTrigger
			local unit triggerUnit = GetTriggerUnit()
			local player owner = GetOwningPlayer(triggerUnit)
			local AShrine this
			local boolean result = false
			debug call Print("Shrine Condition")
			if (triggerUnit == ACharacter.getPlayerCharacter(owner).getUsedUnit()) then
				set triggeringTrigger = GetTriggeringTrigger()
				set this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this")
				debug call this.print("Shrine is already enabled?")
				if (ACharacter.getPlayerCharacter(owner).getShrine() != this) then
					set result = ACharacter.getPlayerCharacter(owner).getMovable()
				endif
				set triggeringTrigger = null
			endif
			set triggerUnit = null
			set owner = null
			return result
		endmethod

		private static method triggerActionEnable takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local unit triggerUnit = GetTriggerUnit()
			local player owner = GetOwningPlayer(triggerUnit)
			local AShrine this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this")
			call this.enableForCharacter(ACharacter.getPlayerCharacter(owner))
			set triggeringTrigger = null
			set triggerUnit = null
			set owner = null
		endmethod

		private method createShrineTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.shrineTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterEnterRegion(this.shrineTrigger, this.discoverRegion, null)
			set conditionFunction = Condition(function AShrine.triggerConditionEnable)
			set triggerCondition = TriggerAddCondition(this.shrineTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.shrineTrigger, function AShrine.triggerActionEnable)
			call AGetCharacterHashTable().storeHandleInteger(this.shrineTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		static method create takes destructable usedDestructable, rect discoverRect, rect revivalRect, real facing returns AShrine
			local AShrine this = AShrine.allocate()
			//start members
			set this.usedDestructable = usedDestructable
			set this.discoverRect = discoverRect
			set this.revivalRect = revivalRect
			set this.m_facing = facing

			call this.createDiscoverRegion()
			debug call this.print("After created discover region")
			call this.createShrineTrigger()
			debug call this.print("After created trigger")
			return this
		endmethod

		private method destroyDiscoverRegion takes nothing returns nothing
			call RemoveRegion(this.discoverRegion)
			set this.discoverRegion = null
		endmethod

		private method destroyShrineTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.shrineTrigger)
			set this.shrineTrigger = null
		endmethod

		private method destroyDiscoverRect takes nothing returns nothing
			call RemoveRect(this.discoverRect)
			set this.discoverRect = null
		endmethod

		private method destroyDiscoverEffect takes nothing returns nothing
			if (AShrine.effectPath != null) then
				if (this.discoverEffect != null) then
					call DestroyEffect(this.discoverEffect)
					set this.discoverEffect = null
				endif
			endif
		endmethod

		public method onDestroy takes nothing returns nothing
			debug call this.print("Destroy shrine")
			//start members
			set this.usedDestructable = null

			call this.destroyDiscoverRegion()
			call this.destroyShrineTrigger()
			call this.destroyDiscoverRect()
			call this.destroyDiscoverEffect()
		endmethod

		/// @param effectPath If this value is null there is no effect
		/// @param soundPath If this value is null there is no sound.
		public static method init takes string effectPath, string soundPath, string textMessage returns nothing
			//static start members
			set AShrine.effectPath = effectPath
			set AShrine.soundPath = soundPath
			set AShrine.textMessage = textMessage

			if (soundPath != null) then
				call PreloadSoundPath(soundPath)
			endif
		endmethod
	endstruct

endlibrary