package com.github.simpleai;

import java.util.ArrayList;
import java.util.List;

public class AIStateNode {
    private String _name;
    private String _condition;
    private List<AIStateNode> _children = new ArrayList<AIStateNode>();
    private final long _lastRun;
    private final TreeNodeStatus _status;
    private final boolean _active;

    public AIStateNode() {
        this("", "", 0L, TreeNodeStatus.UNKNOWN, false);
    }

    public AIStateNode(String name, String condition, long lastRun,
            TreeNodeStatus status, boolean active) {
        _name = name;
        if (_name == null)
            _name = "";
        _condition = condition;
        _lastRun = lastRun;
        _status = status;
        _active = active;
    }

    public void addChildren(AIStateNode child) {
        _children.add(child);
    }

    public List<AIStateNode> getChildren() {
        return _children;
    }

    public String getCondition() {
        return _condition;
    }

    public long getLastRun() {
        return _lastRun;
    }

    public String getName() {
        return _name;
    }

    public TreeNodeStatus getStatus() {
        return _status;
    }

    public boolean isActive() {
        return _active;
    }
}
