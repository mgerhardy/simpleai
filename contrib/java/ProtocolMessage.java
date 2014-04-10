package com.github.simpleai;

abstract class ProtocolMessage {
    protected static final byte PROTO_STATE = 1;
    protected static final byte PROTO_CHARACTER_DETAILS = 2;
    protected static final byte PROTO_SELECT = 3;
    protected static final byte PROTO_PAUSE = 4;

    protected byte _id;

    protected ProtocolMessage(byte protoCharacterDetails) {
	_id = protoCharacterDetails;
    }
}
