package com.github.simpleai;

import java.util.List;

public class AIStateAggro {
    private List<AIStateAggroEntry> _aggro;

    public void addAggro(AIStateAggroEntry entry) {
	_aggro.add(entry);
    }

    public List<AIStateAggroEntry> getAggro() {
	return _aggro;
    }
}
