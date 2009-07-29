library AStructSystemsCharacterShrine requires ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, ALibraryCoreEnvironmentSound, ALibraryCoreEnvironmentSpecialEffect, AStructSystemsCharacterCharacter, AStructSystemsCharacterRevival

	struct AShrine
		//static start members
		private static string effectPath
		private static string soundPath
		private static string textMessage
		//start members
		private destructable m_destructable
		private rect m_discoverRect
		private rect m_revivalRect
		private real m_facing
		//members
		private trigger m_shrineTrigger
		private region m_discoverRegion
		private effect m_discoverEffect

		//! runtextmacro A_STRUCT_DEBUG("\"AShrine\"")

		//start members

		public method destructable takes nothing returns destructable
			return this.m_destructable
		endmethod

		public method discoverRect takes nothing returns rect
			return this.m_discoverRect
		endmethod

		public method revivalRect takes nothing returns rect
			return this.m_revivalRect
		endmethod
		
		public method facing takes nothing returns real
			return this.m_facing
		endmethod

		//methods

		public method enableForCharacter takes ACharacter character, boolean showMessage returns nothing
			local player user = character.user()
			if (ACharacter.playerCharacter(user).shrine() != 0) then
				call ACharacter.playerCharacter(user).shrine().disableForCharacter(ACharacter.playerCharacter(user)) //disable old
			endif
			call character.setShrine(this)
			call character.revival().setX(GetRandomReal(GetRectMinX(this.m_revivalRect), GetRectMaxX(this.m_revivalRect)))
			call character.revival().setY(GetRandomReal(GetRectMinY(this.m_revivalRect), GetRectMaxY(this.m_revivalRect)))
			call character.revival().setFacing(this.m_facing)
			if (thistype.effectPath != null) then
				set this.m_discoverEffect = CreateSpecialEffectForPlayer(user, thistype.effectPath, GetDestructableX(this.m_destructable), GetDestructableY(this.m_destructable))
			endif
			if (thistype.soundPath != null) then
				call PlaySoundPathForPlayer(user, thistype.soundPath)
			endif
			if (showMessage) then
				call character.displayMessage(ACharacter.messageTypeInfo, thistype.textMessage)
			endif
			set user = null
		endmethod

		private method disableForCharacter takes ACharacter character returns nothing
			debug if (character.shrine() == this) then
				call character.setShrine(0)
				if (thistype.effectPath != null) then
					call DestroyEffect(this.m_discoverEffect)
					set this.m_discoverEffect = null
				endif
			debug else
				debug call this.print("Is not the shrine of character " + I2S(character) + ".")
			debug endif
		endmethod

		private method createDiscoverRegion takes nothing returns nothing
			set this.m_discoverRegion = CreateRegion()
			call RegionAddRect(this.m_discoverRegion, this.m_discoverRect)
		endmethod

		private static method triggerConditionEnable takes nothing returns boolean
			local trigger triggeringTrigger
			local unit triggerUnit = GetTriggerUnit()
			local player owner = GetOwningPlayer(triggerUnit)
			local thistype this
			local boolean result = false
			debug call Print("Shrine Condition")
			if (triggerUnit == ACharacter.playerCharacter(owner).unit()) then
				set triggeringTrigger = GetTriggeringTrigger()
				set this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
				set result = ACharacter.playerCharacter(owner).shrine() != this
				//if (ACharacter.playerCharacter(owner).shrine() != this) then
					//set result = ACharacter.playerCharacter(owner).isMovable()
				//endif
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
			local thistype this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			call this.enableForCharacter(ACharacter.playerCharacter(owner), true)
			set triggeringTrigger = null
			set triggerUnit = null
			set owner = null
		endmethod

		private method createShrineTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.m_shrineTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterEnterRegion(this.m_shrineTrigger, this.m_discoverRegion, null)
			set conditionFunction = Condition(function thistype.triggerConditionEnable)
			set triggerCondition = TriggerAddCondition(this.m_shrineTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.m_shrineTrigger, function thistype.triggerActionEnable)
			call AHashTable.global().storeHandleInteger(this.m_shrineTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		static method create takes destructable usedDestructable, rect discoverRect, rect revivalRect, real facing returns thistype
			local thistype this = thistype.allocate()
			//start members
			set this.m_destructable = usedDestructable
			debug if (usedDestructable == null) then
				debug call this.print("Destructable is null!")
			debug endif
			set this.m_discoverRect = discoverRect
			debug if (discoverRect == null) then
				debug call this.print("Discover rect is null!")
			debug endif
			set this.m_revivalRect = revivalRect
			debug if (revivalRect == null) then
				debug call this.print("Revival rect is null!")
			debug endif
			set this.m_facing = facing

			call this.createDiscoverRegion()
			call this.createShrineTrigger()
			return this
		endmethod

		private method destroyDiscoverRegion takes nothing returns nothing
			call RemoveRegion(this.m_discoverRegion)
			set this.m_discoverRegion = null
		endmethod

		private method destroyShrineTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_shrineTrigger)
			set this.m_shrineTrigger = null
		endmethod

		private method destroyDiscoverRect takes nothing returns nothing
			call RemoveRect(this.m_discoverRect)
			set this.m_discoverRect = null
		endmethod

		private method destroyDiscoverEffect takes nothing returns nothing
			if (thistype.effectPath != null) then
				if (this.m_discoverEffect != null) then
					call DestroyEffect(this.m_discoverEffect)
					set this.m_discoverEffect = null
				endif
			endif
		endmethod

		public method onDestroy takes nothing returns nothing
			//start members
			set this.m_destructable = null

			call this.destroyDiscoverRegion()
			call this.destroyShrineTrigger()
			call this.destroyDiscoverRect()
			call this.destroyDiscoverEffect()
		endmethod

		/// @param effectPath If this value is null there is no effect
		/// @param soundPath If this value is null there is no sound.
		public static method init takes string effectPath, string soundPath, string textMessage returns nothing
			//static start members
			set thistype.effectPath = effectPath
			set thistype.soundPath = soundPath
			set thistype.textMessage = textMessage

			if (soundPath != null) then
				call PreloadSoundPath(soundPath)
			endif
		endmethod
	endstruct

endlibrary