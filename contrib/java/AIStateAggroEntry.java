package com.github.simpleai;

public class AIStateAggroEntry {
    private final int id;
    private final float aggro;

    public AIStateAggroEntry(int _id, float _aggro) {
        id = _id;
        aggro = _aggro;
    }

    public float getAggro() {
        return aggro;
    }

    public int getId() {
        return id;
    }
}
