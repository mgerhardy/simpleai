package com.github.simpleai;

import java.util.Map;

import com.github.simpleai.util.WorldVector;

public class AIStateWorld {
	private final int _id;
	private final WorldVector _position;
	private final float _orientation;
	private final Map<String, String> _attributes;

	public AIStateWorld ( final int id, final WorldVector position, final float orientation, final Map<String, String> attributes ) {
		_id = id;
		_position = position;
		_orientation = orientation;
		_attributes = attributes;
	}

	public int getId() {
		return _id;
	}

	public float getOrientation() {
		return _orientation;
	}

	public WorldVector getPosition() {
		return _position;
	}

	public Map<String, String> getAttributes() {
		return _attributes;
	}
}
