package com.github.simpleai;

import java.util.Map;

public class AIStateWorld {
	private final int _id;
	private final Vector3f _position;
	private final float _orientation;
	private final Map<String, String> _attributes;

	public AIStateWorld ( final int id, final Vector3f position, final float orientation, final Map<String, String> attributes ) {
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

	public Vector3f getPosition() {
		return _position;
	}

	public Map<String, String> getAttributes() {
		return _attributes;
	}
}
