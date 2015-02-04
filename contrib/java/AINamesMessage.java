package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;

public class AINamesMessage extends ProtocolMessage {
	private Collection<String> _names;

	public AINamesMessage ( Collection<String> names ) {
		super( PROTO_NAMES );
		_names = names;
	}

	public AINamesMessage ( DataInput in ) throws IOException {
		super( PROTO_NAMES );
		int size = in.readInt();
		_names = new ArrayList<String>( size );
		for ( int i = 0; i < size; ++i ) {
			_names.add( readString( in ) );
		}
	}

	@Override
	public void serialize( DataOutput out ) throws IOException {
		out.writeByte( _id );
		out.writeInt( _names.size() );
		for ( final String i : _names ) {
			writeString( out, i );
		}
	}

	public Collection<String> getNames() {
		return _names;
	}
}
