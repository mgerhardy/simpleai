package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public class AIDeleteNodeMessageMessage extends ProtocolMessage {
	private int _nodeId;

	public AIDeleteNodeMessageMessage ( int nodeId ) {
		super( PROTO_DELETENODE );
		_nodeId = nodeId;
	}

	public AIDeleteNodeMessageMessage ( DataInput in ) throws IOException {
		super( PROTO_DELETENODE );
		_nodeId = in.readInt();
	}

	@Override
	public void serialize( DataOutput out ) throws IOException {
		out.writeByte( _id );
		out.writeInt( _nodeId );
	}

	public int getNodeId() {
		return _nodeId;
	}
}
