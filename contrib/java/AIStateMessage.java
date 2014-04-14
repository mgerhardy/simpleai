package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class AIStateMessage extends ProtocolMessage {
    private List<AIStateWorld> _states = new ArrayList<AIStateWorld>();

    public AIStateMessage() {
	super(PROTO_STATE);
    }

    public AIStateMessage(DataInput in) throws IOException {
	super(PROTO_STATE);
	final int treeSize = in.readInt();
	for (int i = 0; i < treeSize; ++i) {
	    readState(in);
	}
    }

    public void addState(AIStateWorld tree) {
	_states.add(tree);
    }

    public List<AIStateWorld> getStates() {
	return _states;
    }

    private void readState(DataInput in) throws IOException {
	final int id = in.readInt();
	final float x = in.readFloat();
	final float y = in.readFloat();
	final float z = in.readFloat();
	final float orientation = in.readFloat();
	final Vector3f position = new Vector3f(x, y, z);
	final AIStateWorld tree = new AIStateWorld(id, position, orientation);
	_states.add(tree);
    }

    public void serialize(DataOutput out) throws IOException {
	out.writeByte(_id);
	out.writeInt(_states.size());
	for (final AIStateWorld i : _states) {
	    writeState(out, i);
	}
    }

    private void writeState(DataOutput out, AIStateWorld state)
	    throws IOException {
	out.writeInt(state.getId());
	final Vector3f position = state.getPosition();
	out.writeFloat(position.x());
	out.writeFloat(position.y());
	out.writeFloat(position.z());
	out.writeFloat(state.getOrientation());
    }
}