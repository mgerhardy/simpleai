package com.github.simpleai;

import java.io.DataOutput;
import java.io.IOException;

abstract class ProtocolMessage {
    public static final byte PROTO_STATE = 1;
    public static final byte PROTO_CHARACTER_DETAILS = 2;
    public static final byte PROTO_SELECT = 3;
    public static final byte PROTO_PAUSE = 4;
    public static final byte PROTO_CHANGE = 5;
    public static final byte PROTO_NAMES = 6;
    public static final byte PROTO_RESET = 7;
    public static final byte PROTO_STEP = 8;

    protected byte _id;

    protected ProtocolMessage(byte protoCharacterDetails) {
	_id = protoCharacterDetails;
    }

    public abstract void serialize(DataOutput out) throws IOException;
}
