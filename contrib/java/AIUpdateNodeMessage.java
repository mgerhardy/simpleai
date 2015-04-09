package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public class AIUpdateNodeMessageMessage extends ProtocolMessage {
	private int _nodeId;
	private String _name;
	private String _type;
	private String _condition;

	public AIUpdateNodeMessageMessage ( int nodeId, String name, String type, String condition ) {
		super( PROTO_UPDATENODE );
		_nodeId = nodeId;
		_name = name;
		_type = type;
		_condition = condition;
	}

	public AIUpdateNodeMessageMessage ( DataInput in ) throws IOException {
		super( PROTO_UPDATENODE );
		_nodeId = in.readInt();
		_name = in.readString();
		_type = in.readString();
		_condition = in.readString();
	}

	@Override
	public void serialize( DataOutput out ) throws IOException {
		out.writeByte( _id );
		out.writeInt( _nodeId );
		out.writeString( _name );
		out.writeString( _type );
		out.writeString( _condition );
	}

	public int getNodeId() {
		return _nodeId;
	}

	public String getName() {
		return _name;
	}

	public String getType() {
		return _type;
	}

	public String getCondition() {
		return _condition;
	}
}
