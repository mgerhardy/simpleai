package com.github.simpleai;

public class AIStateWorld {
    private final int _id;
    private final Vector3f _position;
    private final float _orientation;

    public AIStateWorld(int id, Vector3f position, float orientation) {
	_id = id;
	_position = position;
	_orientation = orientation;
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
}
