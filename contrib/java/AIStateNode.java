package com.github.simpleai;

import java.util.List;

public class AIStateNode {
    private String _name;
    private String _condition;
    private List<AIStateNode> _children;
    private final long _lastRun;
    private final TreeNodeStatus _status;
    private final boolean _active;

    public AIStateNode() {
	_lastRun = 0L;
	_status = TreeNodeStatus.UNKNOWN;
	_active = false;
    }

    public AIStateNode(String name, String condition, long lastRun,
	    TreeNodeStatus status, boolean active) {
	_name = name;
	_condition = condition;
	_lastRun = 0L;
	_status = TreeNodeStatus.UNKNOWN;
	_active = false;
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
