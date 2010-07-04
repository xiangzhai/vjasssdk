// vJass
struct Character
	public static constant classCleric = 0
	public static constant classWizard = 1
	public static constant classKnight = 2
	private integer m_class

	public static method create takes integer class returns thistype
		local thistype this = thistype.allocate()
		// Additionally you will usually have to print some debug error information and developer will recognize the error just if he tests map and not at compile time!
		if (class > thistype.classKnight) then
			set class = thistype.classKnight
		elseif (class < thistype.classCleric) then
			set class = thisype.classCleric
		endif
		return this
	endmethod

	public method className takes nothing returns string
		if (this.m_class == thistype.classCleric) then
			return "Cleric"
		elseif (this.m_class == thistype.classWizard) then
			return "Wizard"
		endif
		return "Knight"
	endmethod
endstruct

// Jass++
class Character
	public enum Class
	{
		Cleric,
		Wizard,
		Knight
	};

	private Class m_class;
	
	public new(Class class)
	{
		this.m_class = class;
	}

	public string className() const
	{
		enum Class.*;

		switch (this.m_class)
		{
			case (Cleric)
			{
				return "Cleric";
			}

			case (Wizard)
			{
				return "Wizard";
			}

			case (Knight)
			{
				return "Knight";
			}
		}
	}
};
