package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.List;

public class AICharacterDetailsMessage extends ProtocolMessage {
	private final int _chrId;
	private AIStateAggro _aggro;
	private AIStateNode _root;

	public AICharacterDetailsMessage ( DataInput in ) throws IOException {
		super( PROTO_CHARACTER_DETAILS );
		_chrId = in.readInt();
		readAggro( in, _aggro );
		_root = readNode( in );
	}

	public AICharacterDetailsMessage ( final int id, final AIStateAggro aggro,
			final AIStateNode root ) {
		super( PROTO_CHARACTER_DETAILS );
		_chrId = id;
		_aggro = aggro;
		_root = root;
	}

	public AIStateAggro getAggro() {
		return _aggro;
	}

	public int getCharacterId() {
		return _chrId;
	}

	public AIStateNode getNode() {
		return _root;
	}

	private void readAggro( DataInput in, AIStateAggro aggro ) throws IOException {
		final int size = in.readShort();
		for ( int i = 0; i < size; ++i ) {
			final int chrId = in.readInt();
			final float aggroVal = in.readFloat();
			aggro.addAggro( new AIStateAggroEntry( chrId, aggroVal ) );
		}
	}

	private AIStateNode readNode( DataInput in ) throws IOException {
		final int nodeId = in.readInt();
		final String condition = readString( in );
		final long lastRun = in.readLong();
		final TreeNodeStatus status = TreeNodeStatus.values()[in.readByte()];
		final boolean active = in.readBoolean();
		final short childrenCount = in.readShort();
		final AIStateNode node = new AIStateNode( nodeId, condition, lastRun,
				status, active );
		for ( int i = 0; i < childrenCount; ++i ) {
			final AIStateNode child = readNode( in );
			node.addChildren( child );
		}
		return node;
	}

	public void serialize( DataOutput out ) throws IOException {
		out.writeByte( _id );
		out.writeInt( _chrId );
		writeAggro( out, _aggro );
		writeNode( out, _root );
	}

	private void writeAggro( DataOutput out, final AIStateAggro aggro )
			throws IOException {
		final List<AIStateAggroEntry> a = aggro.getAggro();
		out.writeShort( a.size() );
		for ( final AIStateAggroEntry i : a ) {
			out.writeInt( i.getId() );
			out.writeFloat( i.getAggro() );
		}
	}

	private void writeNode( DataOutput out, final AIStateNode node )
			throws IOException {
		out.writeInt( node.getNodeId() );
		writeString( out, node.getCondition() );
		out.writeLong( node.getLastRun() );
		out.writeByte( (byte) node.getStatus().ordinal() );
		out.writeBoolean( node.isActive() );
		final List<AIStateNode> children = node.getChildren();
		out.writeShort( (short) children.size() );
		for ( final AIStateNode i : children ) {
			writeNode( out, i );
		}
	}
}
