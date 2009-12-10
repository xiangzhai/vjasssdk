//! zinc

library AStructSystemsWorldLayer requires AModuleCoreGeneralSystemStruct, ALibraryCoreMathsHandle, ALibraryCoreMathsUnit
{

	type ALayerOnEnterFunction extends function(unit);

	type ALayerOnLeaveFunction extends function(unit);

	struct ALayer
	{
		private real m_height;
		private ALayerOnEnterFunction m_onEnterFunction;
		private ALayerOnLeaveFunction m_onLeaveFunction;
		private region m_entry;
		private region m_exit;
		private region m_region;
		private AUnitVector m_units;
		private trigger m_enterTrigger;
		private trigger m_leaveTrigger;
		private trigger m_boundsTrigger;

		module ASystemStruct;

		public method setHeight(real height)
		{
			integer i = 0;
			this.m_height = height;

			for (0 <= i < this.m_units.size())
			{
				SetUnitFlyHeight(this.m_units[i], this.m_height, 0.0);
				i = i + 1;
			}
		}

		public method height() -> real
		{
			return this.m_height;
		}

		public method setOnEnterFunction(ALayerOnEnterFunction onEnterFunction)
		{
			this.m_onEnterFunction = onEnterFunction;
		}

		public method onEnterFunction() -> ALayerOnEnterFunction
		{
			return this.m_onEnterFunction;
		}

		public method setOnLeaveFunction(ALayerOnLeaveFunction onLeaveFunction)
		{
			this.m_onLeaveFunction = onLeaveFunction;
		}

		public method onLeaveFunction() -> ALayerOnLeaveFunction
		{
			return this.m_onLeaveFunction;
		}

		public method addEntryRect(rect whichRect)
		{
			RegionAddRect(this.m_entry, whichRect);
		}

		public method removeEntryRect(rect whichRect)
		{
			RegionClearRect(this.m_entry, whichRect);
		}

		public method addExitRect(rect whichRect)
		{
			RegionAddRect(this.m_exit, whichRect);
		}

		public method removeExitRect(rect whichRect)
		{
			RegionClearRect(this.m_exit, whichRect);
		}

		public method addRect(rect whichRect)
		{
			RegionAddRect(this.m_region, whichRect);
		}

		public method removeRect(rect whichRect)
		{
			RegionClearRect(this.m_region, whichRect);
		}

		public method unitEnters(unit whichUnit)
		{
			this.m_units.pushBack(whichUnit);
			MakeUnitFlyable(whichUnit);
			SetUnitFlyHeight(whichUnit, this.m_height, 0.0);

			if (this.m_onEnterFunction != 0)
			{
				this.m_onEnterFunction.execute(whichUnit);
			}
		}

		public method unitLeaves(unit whichUnit)
		{
			this.m_units.remove(whichUnit);
			SetUnitFlyHeight(whichUnit, 0.0, 0.0);

			if (this.m_onLeaveFunction != 0)
			{
				this.m_onLeaveFunction.execute(whichUnit);
			}
		}

		public method unitLeaveAll()
		{
			integer i;

			while (!this.m_units.empty())
			{
				this.unitLeaves(this.m_units.back());
			}
		}

		public method enable()
		{
			EnableTrigger(this.m_enterTrigger);
			EnableTrigger(this.m_leaveTrigger);
			EnableTrigger(this.m_boundsTrigger);
		}

		/**
		* Does not change already added units!
		*/
		public method disable()
		{
			DisableTrigger(this.m_enterTrigger);
			DisableTrigger(this.m_leaveTrigger);
			DisableTrigger(this.m_boundsTrigger);
		}

		public method isEnabled() -> boolean
		{
			return IsTriggerEnabled(this.m_boundsTrigger);
		}

		private static method eventFilterEnter() -> boolean
		{
			thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this");

			return !this.m_units.contains(GetTriggerUnit());
		}

		private static method triggerActionEnter()
		{
			thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this");
			this.unitEnters(GetTriggerUnit());
		}

		private static method eventFilterLeave() -> boolean
		{
			thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this");

			return this.m_units.contains(GetTriggerUnit());
		}

		private static method triggerActionLeave()
		{
			thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this");
			this.unitLeaves(GetTriggerUnit());
		}

		private static method eventFilterBounds() -> boolean
		{
			thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this");

			return this.m_units.contains(GetTriggerUnit()) && !IsUnitInRegion(this.m_exit, GetTriggerUnit());
		}

		private static method triggerActionBounds()
		{
			thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this");
			SetUnitX(GetTriggerUnit(), GetUnitPolarProjectionX(GetTriggerUnit(), GetUnitFacing(GetTriggerUnit()) - 180.0, 100.0));
			SetUnitY(GetTriggerUnit(), GetUnitPolarProjectionY(GetTriggerUnit(), GetUnitFacing(GetTriggerUnit()) - 180.0, 100.0));
		}

		public static method create() -> thistype
		{
			thistype this = thistype.allocate();
			this.m_height = 0.0;
			this.m_onEnterFunction = 0;
			this.m_onLeaveFunction = 0;
			this.m_entry = CreateRegion();
			this.m_exit = CreateRegion();
			this.m_region = CreateRegion();
			this.m_units = AUnitVector.create();
			this.m_enterTrigger = CreateTrigger();
			TriggerRegisterEnterRegion(this.m_enterTrigger, this.m_entry, Filter(function thistype.eventFilterEnter));
			TriggerAddAction(this.m_enterTrigger, function thistype.triggerActionEnter);
			AHashTable.global().setHandleInteger(this.m_enterTrigger, "this", this);
			this.m_leaveTrigger = CreateTrigger();
			TriggerRegisterLeaveRegion(this.m_leaveTrigger, this.m_exit, Filter(function thistype.eventFilterLeave));
			TriggerAddAction(this.m_leaveTrigger, function thistype.triggerActionLeave);
			AHashTable.global().setHandleInteger(this.m_leaveTrigger, "this", this);
			this.m_boundsTrigger = CreateTrigger();
			TriggerRegisterLeaveRegion(this.m_boundsTrigger, this.m_region, Filter(function thistype.eventFilterBounds));
			TriggerAddAction(this.m_boundsTrigger, function thistype.triggerActionBounds);

			return this;
		}

		public method onDestroy()
		{
			RemoveRegion(this.m_entry);
			this.m_entry = null;
			RemoveRegion(this.m_exit);
			this.m_exit = null;
			RemoveRegion(this.m_region);
			this.m_region = null;
			this.m_units.destroy();
			AHashTable.global().destroyTrigger(this.m_enterTrigger);
			this.m_enterTrigger = null;
			AHashTable.global().destroyTrigger(this.m_leaveTrigger);
			this.m_enterTrigger = null;
			AHashTable.global().destroyTrigger(this.m_boundsTrigger);
			this.m_enterTrigger = null;
		}
	}

}

//! endzinc