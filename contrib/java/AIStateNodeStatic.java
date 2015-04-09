package com.github.simpleai;

public class AIStateNodeStatic {
	private final int _id;
	private final String _name;
	private final String _type;
	private final String _parameter;
	private final String _conditionType;
	private final String _conditionParameters;

	public AIStateNodeStatic ( final int id, final String type, final String name, final String parameters, final String conditionType, final String conditionParameters ) {
		this._id = id;
		this._name = name;
		this._type = type;
		this._parameters = parameters;
		this._conditionType = conditionType;
		this._conditionParameters = conditionParameters;
	}

	public int getId() {
		return _id;
	}

	public String getName() {
		return _name;
	}

	public String getType() {
		return _type;
	}

	public String getParameters() {
		return _parameters;
	}

	public String getCondition() {
		return _conditionType + "(" + _conditionParameters + ")";
	}

	public String getConditionType() {
		return _conditionType;
	}

	public String getConditionParameters() {
		return _conditionParameters;
	}
};
