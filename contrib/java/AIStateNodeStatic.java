package com.github.simpleai;

public class AIStateNodeStatic {
	private final int _id;
	private final String _name;
	private final String _type;

	public AIStateNodeStatic ( final int id, final String name, final String type ) {
		this._id = id;
		this._name = name;
		this._type = type;
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
};
