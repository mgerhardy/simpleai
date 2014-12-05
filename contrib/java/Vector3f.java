package com.github.simpleai;

public class Vector3f {
    private final float _x;
    private final float _y;
    private final float _z;

    public Vector3f() {
        this(0.0f, 0.0f, 0.0f);
    }

    public Vector3f(float x, float y) {
        this(x, y, 0.0f);
    }

    public Vector3f(float x, float y, float z) {
        _x = x;
        _y = y;
        _z = z;
    }

    public Vector3f(Vector3f pos) {
        _x = pos._x;
        _y = pos._y;
        _z = pos._z;
    }

    public double angle() {
        final double angle = Math.atan2(_z, _x);
        return angle;
    }

    public float length() {
        return (float) Math.sqrt(squareLength());
    }

    public double orientation() {
        final double orientation = Math.atan2(_x, _z);
        return orientation;
    }

    public float squareLength() {
        return _x * _x + _y * _y + _z * _z;
    }

    public float x() {
        return _x;
    }

    public float y() {
        return _y;
    }

    public float z() {
        return _z;
    }
}
