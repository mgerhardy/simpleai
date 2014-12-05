package com.github.simpleai;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;

public class AIPauseMessage extends ProtocolMessage {
    private final boolean _pause;

    public AIPauseMessage(boolean pause) {
        super(PROTO_PAUSE);
        _pause = pause;
    }

    public AIPauseMessage(DataInput in) throws IOException {
        super(PROTO_PAUSE);
        _pause = in.readBoolean();
    }

    public boolean isPause() {
        return _pause;
    }

    public void serialize(DataOutput out) throws IOException {
        out.writeByte(_id);
        out.writeBoolean(_pause);
    }
}
