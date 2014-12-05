package com.github.simpleai;

import java.util.Map;
import java.util.Map.Entry;

public class AIStateWorld {
    private final int _id;
    private final Vector3f _position;
    private final float _orientation;
    private final Map<String, String> _attributes;

    public AIStateWorld(int id, Vector3f position, float orientation, Map<String, String> attributes) {
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
