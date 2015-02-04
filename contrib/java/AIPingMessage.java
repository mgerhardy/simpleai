package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public class AIPingMessage extends ProtocolMessage {
	public AIPingMessage () {
		super( PROTO_PING );
	}

	public AIPauseMessage ( DataInput in ) throws IOException {
		super( PROTO_PING );
	}

	public void serialize( DataOutput out ) throws IOException {
		out.writeByte( _id );
	}
}
