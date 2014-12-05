package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

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

    private void writeAttributes(DataOutput out, final Map<String, String> a)
            throws IOException {
        out.writeShort((short) a.size());
        for (final Entry<String, String> i : a.entrySet()) {
            writeString(out, i.getKey());
            writeString(out, i.getValue());
        }
    }

    private void readAttributes(DataInput in, Map<String, String> attributes)
            throws IOException {
        final int size = in.readShort();
        for (int i = 0; i < size; ++i) {
            final String key = readString(in);
            final String value = readString(in);
            attributes.put(key, value);
        }
    }

    private void readState(DataInput in) throws IOException {
        final int id = in.readInt();
        final float x = in.readFloat();
        final float y = in.readFloat();
        final float z = in.readFloat();
        final float orientation = in.readFloat();
        final Map<String, String> attributes = new HashMap<String, String>();
        readAttributes(in, attributes);
        final Vector3f position = new Vector3f(x, y, z);
        final AIStateWorld tree = new AIStateWorld(id, position, orientation, attributes);
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
        writeAttributes(out, state.getAttributes());
    }
}