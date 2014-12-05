package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public class AIChangeMessage extends ProtocolMessage {
    private final String _name;

    public AIChangeMessage(DataInput in) throws IOException {
        super(PROTO_CHANGE);
        _name = readString(in);
    }

    public AIChangeMessage(String name) {
        super(PROTO_CHANGE);
        _name = name;
    }

    public String getName() {
        return _name;
    }

    public void serialize(DataOutput out) throws IOException {
        out.writeByte(_id);
        writeString(out, _name);
    }
}
