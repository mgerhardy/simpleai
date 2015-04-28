-- Packet dissector for the SimpleAI remote debugger protocol
-- Copy this to $HOME/.wireshark/plugins/

local default_settings = {
	port = 10001,
}

-- message id lookup table
PROTO = {}
PROTO[0] = "PROTO_PING"
PROTO[1] = "PROTO_STATE"
PROTO[2] = "PROTO_CHARACTER_STATIC"
PROTO[3] = "PROTO_CHARACTER_DETAILS"
PROTO[4] = "PROTO_SELECT"
PROTO[5] = "PROTO_PAUSE"
PROTO[6] = "PROTO_CHANGE"
PROTO[7] = "PROTO_NAMES"
PROTO[8] = "PROTO_RESET"
PROTO[9] = "PROTO_STEP"
PROTO[10] = "PROTO_UPDATENODE"
PROTO[11] = "PROTO_DELETENODE"
PROTO[12] = "PROTO_ADDNODE"

simpleai = Proto("simpleai", "SimpleAI remote debugger")

simpleai.prefs.port = Pref.uint("Port number", default_settings.port, "The TCP port number for the simpleai debug server")

function disPingMessage(buffer, tree)
	-- Nothing to do here
end

function disStateMessage(buffer, tree)
	tree:add(simpleai.fields.fieldStatesAmount, buffer(1, 4))
	local count = buffer(1, 4):uint()
	local offset = 5
	for i = 1, count do
		tree:add(simpleai.fields.fieldStatesId, buffer(offset, 4))
		offset = offset + 4;
		tree:add(simpleai.fields.fieldStatesX, buffer(offset, 4))
		offset = offset + 4;
		tree:add(simpleai.fields.fieldStatesY, buffer(offset, 4))
		offset = offset + 4;
		tree:add(simpleai.fields.fieldStatesZ, buffer(offset, 4))
		offset = offset + 4;
		tree:add(simpleai.fields.fieldStatesOrientation, buffer(offset, 4))
		offset = offset + 4;
		local attributesCount = buffer(offset, 2):uint()
		tree:add(simpleai.fields.fieldStatesAttributesCount, buffer(offset, 2))
		offset = offset + 2;
		for i = 1, attributesCount do
			tree:add(simpleai.fields.fieldStatesAttributesKey, buffer(offset))
			offset = offset + buffer(offset):stringz():len()
			tree:add(simpleai.fields.fieldStatesAttributesValue, buffer(offset))
			offset = offset + buffer(offset):stringz():len()
		end
	end
end

function disCharacterStaticMessage(buffer, tree)
	tree:add(simpleai.fields.fieldStaticCharacterId, buffer(1, 4))
	local count = buffer(5, 4):uint()
	tree:add(simpleai.fields.fieldStaticNodeCount, buffer(5, 4))
	local offset = 9
	for i = 1, count do
		tree:add(simpleai.fields.fieldStaticNodeId, buffer(offset, 4))
		offset = offset + 4;
		tree:add(simpleai.fields.fieldStaticNodeName, buffer(offset))
		offset = offset + buffer(offset):stringz():len()
		tree:add(simpleai.fields.fieldStaticNodeType, buffer(offset))
		offset = offset + buffer(offset):stringz():len()
		tree:add(simpleai.fields.fieldStaticParameters, buffer(offset))
		offset = offset + buffer(offset):stringz():len()
		tree:add(simpleai.fields.fieldStaticConditionType, buffer(offset))
		offset = offset + buffer(offset):stringz():len()
		tree:add(simpleai.fields.fieldStaticConditionParameters, buffer(offset))
		offset = offset + buffer(offset):stringz():len()
	end
end

function disCharacterDetailsMessageNode(buffer, tree, offset)
	tree:add(simpleai.fields.fieldDetailsNodeId, buffer(offset, 4))
	offset = offset + 4
	tree:add(simpleai.fields.fieldDetailsNodeCondition, buffer(offset))
	offset = offset + buffer(offset):stringz():len()
	tree:add(simpleai.fields.fieldDetailsNodeLastRun, buffer(offset, 8))
	offset = offset + 8
	local childrenCount = buffer(offset, 2):uint()
	tree:add(simpleai.fields.fieldDetailsChildrenCount, buffer(offset, 2))
	offset = offset + 2
	for i = 1, childrenCount do
		fields.advance = disCharacterDetailsMessageNode(buffer, tree, offset)
		offset = offset + advance
	end
	return offset
end

function disCharacterDetailsMessage(buffer, tree)
	tree:add(simpleai.fields.fieldDetailsCharacterId, buffer(1, 4))

	-- read the aggro
	local aggroCount = buffer(5, 2):uint()
	tree:add(simpleai.fields.fieldDetailsAggroCount, buffer(5, 2))
	local offset = 7
	for i = 1, aggroCount do
		tree:add(simpleai.fields.fieldDetailsAggroChrId, buffer(offset, 4))
		offset = offset + 4
		tree:add(simpleai.fields.fieldDetailsAggroAmount, buffer(offset, 4))
		offset = offset + 4
	end

	-- now read the nodes
	disCharacterDetailsMessageNode(buffer, tree, offset)
end

function disSelectMessage(buffer, tree)
	tree:add(simpleai.fields.fieldSelectCharacterId, buffer(1, 4))
end

function disPauseMessage(buffer, tree)
	tree:add(simpleai.fields.fieldPauseState, buffer(1, 1))
end

function disChangeMessage(buffer, tree)
	tree:add(simpleai.fields.fieldChangeName, buffer(1))
end

function disNamesMessage(buffer, tree)
	tree:add(simpleai.fields.fieldZoneNamesAmount, buffer(1, 4))
	local count = buffer(1, 4):uint()
	local offset = 5
	for i = 1, count do
		tree:add(simpleai.fields.fieldZoneNamesName, buffer(offset))
		offset = offset + buffer(offset):stringz():len()
	end
end

function disResetMessage(buffer, tree)
	-- Nothing to do here
end

function disStepMessage(buffer, tree)
	tree:add(simpleai.fields.fieldStepMillis, buffer(1, 8))
end

function disUpdateNodeMessage(buffer, tree)
end

function disDeleteNodeMessage(buffer, tree)
end

function disAddNodeMessage(buffer, tree)
end

function simpleai.dissector(buffer, pinfo, tree)
	local subtree = tree:add(simpleai, buffer())
	local id = buffer(0, 1):uint()

	local name = PROTO[id]
	if name == nil then
		name = "Unknown"
	end

	pinfo.cols.protocol = simpleai.name
	pinfo.cols.info = "Type: " .. name

	if id == 0 then
		disPingMessage(buffer, subtree)
	elseif id == 1 then
		disStateMessage(buffer, subtree)
	elseif id == 2 then
		disCharacterStaticMessage(buffer, subtree)
	elseif id == 3 then
		disCharacterDetailsMessage(buffer, subtree)
	elseif id == 4 then
		disSelectMessage(buffer, subtree)
	elseif id == 5 then
		disPauseMessage(buffer, subtree)
	elseif id == 6 then
		disChangeMessage(buffer, subtree)
	elseif id == 7 then
		disNamesMessage(buffer, subtree)
	elseif id == 8 then
		disResetMessage(buffer, subtree)
	elseif id == 9 then
		disStepMessage(buffer, subtree)
	elseif id == 10 then
		disUpdateNodeMessage(buffer, subtree)
	elseif id == 11 then
		disDeleteNodeMessage(buffer, subtree)
	elseif id == 12 then
		disAddNodeMessage(buffer, subtree)
	end
end

function simpleai.prefs_changed()
	if default_settings.port ~= simpleai.prefs.port then
		if default_settings.port ~= 0 then
			DissectorTable.get("tcp.port"):remove(default_settings.port, simpleai)
		end
		default_settings.port = simpleai.prefs.port
		if default_settings.port ~= 0 then
			DissectorTable.get("tcp.port"):add(default_settings.port, simpleai)
		end
	end
end

function simpleai.init()
	local fields = simpleai.fields
	fields.msgid = ProtoField.uint8("simpleai.msgid", "Message Id")
	fields.fieldZoneNamesAmount = ProtoField.uint32("simpleai.zonenames.amount", "Number of zone names")
	fields.fieldZoneNamesName = ProtoField.string("simpleai.zonenames.name", "Zone name")

	fields.fieldStatesAmount = ProtoField.uint32("simpleai.states.amount", "Number of states")
	fields.fieldStatesId = ProtoField.uint32("simpleai.states.id", "Node id")
	fields.fieldStatesX = ProtoField.float("simpleai.states.x", "X position of the entity")
	fields.fieldStatesY = ProtoField.float("simpleai.states.y", "Y position of the entity")
	fields.fieldStatesZ = ProtoField.float("simpleai.states.z", "Z position of the entity")
	fields.fieldStateOrientation = ProtoField.float("simpleai.states.orientation", "Orientation of the entity")
	fields.fieldStatesAttributesCount = ProtoField.uint16("simpleai.states.attribcount", "Attribute count")
	fields.fieldStatesAttributesKey = ProtoField.stringz("simpleai.states.attribkey", "Key")
	fields.fieldStatesAttributesValue = ProtoField.stringz("simpleai.states.attribvalue", "Value")

	fields.fieldStaticCharacterId = ProtoField.uint32("simpleai.static.chrid", "Character id")
	fields.fieldStaticNodeCount = ProtoField.uint32("simpleai.static.count", "Static node data count")
	fields.fieldStaticNodeId = ProtoField.uint32("simpleai.static.id", "Node id")
	fields.fieldStaticNodeName = ProtoField.stringz("simpleai.static.name", "Node name")
	fields.fieldStaticNodeType = ProtoField.stringz("simpleai.static.type", "Node type")
	fields.fieldStaticParameters = ProtoField.stringz("simpleai.static.parameters", "Node parameters")
	fields.fieldStaticConditionType = ProtoField.stringz("simpleai.static.conditiontype", "Character type")
	fields.fieldStaticConditionParameters = ProtoField.stringz("simpleai.static.conditionparameters", "Condition parameters")

	fields.fieldDetailsCharacterId = ProtoField.stringz("simpleai.detail.chrid", "Character id")
	fields.fieldDetailsAggroCount = ProtoField.uint16("simpleai.details.aggrocount", "Aggro entries")
	fields.fieldDetailsAggroChrId = ProtoField.uint32("simpleai.details.aggrochrid", "Character id")
	fields.fieldDetailsAggroAmount = ProtoField.float("simpleai.details.aggroamount", "Aggro amount")
	fields.fieldDetailsNodeId = ProtoField.uint32("simpleai.details.id", "Node id")
	fields.fieldDetailsNodeCondition = ProtoField.stringz("simpleai.details.condition", "Condition")
	fields.fieldDetailsNodeLastRun = ProtoField.int64("simpleai.details.lastrun", "Last run")
	fields.fieldDetailsChildrenCount = ProtoField.int64("simpleai.details.childrencount", "Amount of children")

	fields.fieldSelectCharacterId = ProtoField.uint32("simpleai.select.chrid", "Character id")

	fields.fieldPauseState = ProtoField.bool("simpleai.pause.state", "Pause state")

	fields.fieldChangeName = ProtoField.stringz("simpleai.change.name", "Zone name")

	fields.fieldStepMillis = ProtoField.int64("simpleai.step.millis", "Milliseconds")

	simpleai_table = DissectorTable.get("tcp.port")
	simpleai_table:add(default_settings.port, simpleai)
end

