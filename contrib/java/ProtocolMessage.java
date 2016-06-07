package com.github.simpleai;

import java.io.ByteArrayOutputStream;
import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

abstract class ProtocolMessage {
	public static final byte PROTO_PING = 0;
	public static final byte PROTO_STATE = 1;
	public static final byte PROTO_CHARACTER_DETAILS = 2;
	public static final byte PROTO_CHARACTER_STATIC = 3;
	public static final byte PROTO_SELECT = 4;
	public static final byte PROTO_PAUSE = 5;
	public static final byte PROTO_CHANGE = 6;
	public static final byte PROTO_NAMES = 7;
	public static final byte PROTO_RESET = 8;
	public static final byte PROTO_STEP = 9;
	public static final byte PROTO_UPDATENODE = 10;
	public static final byte PROTO_DELETENODE = 11;
	public static final byte PROTO_ADDNODE = 12;

	protected byte _id;

	protected ProtocolMessage ( byte protoCharacterDetails ) {
		_id = protoCharacterDetails;
	}

	public byte getId() {
		return _id;
	}

	// DataInput must be little endian
	protected String readString( DataInput in ) throws IOException {
		ByteArrayOutputStream s = new ByteArrayOutputStream();
		for ( ;; ) {
			byte readByte = in.readByte();
			if ( readByte == '\0' )
				break;
			s.write( readByte );
		}
		return new String( s.toByteArray() );
	}

	protected void writeString( DataOutput out, String str ) throws IOException {
		byte[] bytes = str.getBytes();
		out.write( bytes );
		out.write( '\0' );
	}

	// DataOutput must be little endian
	public abstract void serialize( DataOutput out ) throws IOException;
}
