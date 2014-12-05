package com.github.simpleai;

import java.util.ArrayList;
import java.util.List;

public class AIStateAggro {
    private List<AIStateAggroEntry> _aggro = new ArrayList<AIStateAggroEntry>();

    public void addAggro(AIStateAggroEntry entry) {
        _aggro.add(entry);
    }

    public List<AIStateAggroEntry> getAggro() {
        return _aggro;
    }
}
