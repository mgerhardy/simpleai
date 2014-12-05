package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

public class AICharacterDetailsMessage extends ProtocolMessage {
    private final int _chrId;
    private AIStateAggro _aggro;
    private AIStateNode _root;
    private Map<String, String> _attributes;

    public AICharacterDetailsMessage(DataInput in) throws IOException {
        super(PROTO_CHARACTER_DETAILS);
        _chrId = in.readInt();
        readAggro(in, _aggro);
        _root = readNode(in);
        readAttributes(in, _attributes);
    }

    public AICharacterDetailsMessage(final int id, final AIStateAggro aggro,
            final AIStateNode root, final Map<String, String> attributes) {
        super(PROTO_CHARACTER_DETAILS);
        _chrId = id;
        _aggro = aggro;
        _root = root;
        _attributes = attributes;
    }

    public AIStateAggro getAggro() {
        return _aggro;
    }

    public Map<String, String> getAttributes() {
        return _attributes;
    }

    public int getCharacterId() {
        return _chrId;
    }

    public AIStateNode getNode() {
        return _root;
    }

    private void readAggro(DataInput in, AIStateAggro aggro) throws IOException {
        final int size = in.readShort();
        for (int i = 0; i < size; ++i) {
            final int chrId = in.readInt();
            final float aggroVal = in.readFloat();
            aggro.addAggro(new AIStateAggroEntry(chrId, aggroVal));
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

    private AIStateNode readNode(DataInput in) throws IOException {
        final String name = readString(in);
        final String condition = readString(in);
        final long lastRun = in.readLong();
        final TreeNodeStatus status = TreeNodeStatus.values()[in.readByte()];
        final boolean active = in.readBoolean();
        final short childrenCount = in.readShort();
        final AIStateNode node = new AIStateNode(name, condition, lastRun,
                status, active);
        for (int i = 0; i < childrenCount; ++i) {
            final AIStateNode child = readNode(in);
            node.addChildren(child);
        }
        return node;
    }

    public void serialize(DataOutput out) throws IOException {
        out.writeByte(_id);
        out.writeInt(_chrId);
        writeAggro(out, _aggro);
        writeNode(out, _root);
        writeAttributes(out, _attributes);
    }

    private void writeAggro(DataOutput out, final AIStateAggro aggro)
            throws IOException {
        final List<AIStateAggroEntry> a = aggro.getAggro();
        out.writeShort(a.size());
        for (final AIStateAggroEntry i : a) {
            out.writeInt(i.getId());
            out.writeFloat(i.getAggro());
        }
    }

    private void writeAttributes(DataOutput out, final Map<String, String> a)
            throws IOException {
        out.writeShort((short) a.size());
        for (final Entry<String, String> i : a.entrySet()) {
            writeString(out, i.getKey());
            writeString(out, i.getValue());
        }
    }

    private void writeNode(DataOutput out, final AIStateNode node)
            throws IOException {
        writeString(out, node.getName());
        writeString(out, node.getCondition());
        out.writeLong(node.getLastRun());
        out.writeByte((byte) node.getStatus().ordinal());
        out.writeBoolean(node.isActive());
        final List<AIStateNode> children = node.getChildren();
        out.writeShort((short) children.size());
        for (final AIStateNode i : children) {
            writeNode(out, i);
        }
    }
}
