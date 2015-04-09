package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.List;

public class AICharacterStaticMessage extends ProtocolMessage {
	private int _chrId;
	private List<AIStateNodeStatic> _nodeStaticData;

	public AICharacterStaticMessage ( DataInput in ) throws IOException {
		super( PROTO_CHARACTER_STATIC );
		_chrId = in.readInt();
		int size = in.readInt();
		for ( int i = 0; i < size; ++i ) {
			int id = in.readInt();
			String name = readString( in );
			String type = readString( in );
			String parameters = readString( in );
			String conditionType = readString( in );
			String conditionParameters = readString( in );
			AIStateNodeStatic staticData = new AIStateNodeStatic( id, name, type, parameters, conditionType, conditionParameters );
			_nodeStaticData.add( staticData );
		}
	}

	public AICharacterStaticMessage ( final int chrId, List<AIStateNodeStatic> nodeStaticData ) {
		super( PROTO_CHARACTER_STATIC );
		_chrId = chrId;
		_nodeStaticData = nodeStaticData;
	}

	public int getCharacterId() {
		return _chrId;
	}

	public List<AIStateNodeStatic> getNodeStaticData() {
		return _nodeStaticData;
	}

	@Override
	public void serialize( DataOutput out ) throws IOException {
		out.writeByte( _id );
		out.writeInt( _chrId );
		out.writeInt( _nodeStaticData.size() );
		for ( final AIStateNodeStatic i : _nodeStaticData ) {
			out.writeInt( i.getId() );
			writeString( out, i.getName() );
			writeString( out, i.getType() );
			writeString( out, i.getParameters() );
			writeString( out, i.getConditionType() );
			writeString( out, i.getConditionParameters() );
		}
	}
}
