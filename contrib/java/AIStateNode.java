package com.github.simpleai;

import java.util.ArrayList;
import java.util.List;

public class AIStateNode {
	private int _nodeId;
	private String _condition;
	private List<AIStateNode> _children = new ArrayList<AIStateNode>();
	private final long _lastRun;
	private final TreeNodeStatus _status;
	private final boolean _active;

	public AIStateNode () {
		this( -1, "", 0L, TreeNodeStatus.UNKNOWN, false );
	}

	public AIStateNode ( int id, String condition, long lastRun,
			TreeNodeStatus status, boolean active ) {
		_nodeId = id;
		_condition = condition;
		_lastRun = lastRun;
		_status = status;
		_active = active;
	}

	public void addChildren( AIStateNode child ) {
		_children.add( child );
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

	public int getNodeId() {
		return _nodeId;
	}

	public TreeNodeStatus getStatus() {
		return _status;
	}

	public boolean isActive() {
		return _active;
	}
}
