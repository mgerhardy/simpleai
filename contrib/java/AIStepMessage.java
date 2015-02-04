package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public class AIStepMessage extends ProtocolMessage {
	private long _millis;

	public AIStepMessage ( long millis ) {
		super( PROTO_STEP );
		_millis = millis;
	}

	public AIStepMessage ( DataInput in ) throws IOException {
		super( PROTO_PAUSE );
		_millis = in.readLong();
	}

	public long getMillis() {
		return _millis;
	}

	public void serialize( DataOutput out ) throws IOException {
		out.writeByte( _id );
		out.writeLong( _millis );
	}
}
