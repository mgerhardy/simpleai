package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public class AISelectMessage extends ProtocolMessage {
    private final int _chrId;

    public AISelectMessage(DataInput in) throws IOException {
	super(PROTO_SELECT);
	_chrId = in.readInt();
    }

    public AISelectMessage(int id) {
	super(PROTO_SELECT);
	_chrId = id;
    }

    public int getCharacterId() {
	return _chrId;
    }

    public void serialize(DataOutput out) throws IOException {
	out.writeByte(_id);
	out.writeInt(_chrId);
    }
}
