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

local fieldZoneNamesAmount = ProtoField.uint32("simpleai.zonenames.amount", "Number of zone names")
local fieldZoneNamesName = ProtoField.string("simpleai.zonenames.name", "Zone name")

local fieldStatesAmount = ProtoField.uint32("simpleai.states.amount", "Number of states")
local fieldStatesId = ProtoField.uint32("simpleai.states.id", "Node id")
local fieldStatesX = ProtoField.float("simpleai.states.x", "X position of the entity")
local fieldStatesY = ProtoField.float("simpleai.states.y", "Y position of the entity")
local fieldStatesZ = ProtoField.float("simpleai.states.z", "Z position of the entity")
local fieldStateOrientation = ProtoField.float("simpleai.states.orientation", "Orientation of the entity")
local fieldStatesAttributesCount = ProtoField.uint16("simpleai.states.attribcount", "Attribute count")
local fieldStatesAttributesKey = ProtoField.stringz("simpleai.states.attribkey", "Key")
local fieldStatesAttributesValue = ProtoField.stringz("simpleai.states.attribvalue", "Value")

local fieldStaticCharacterId = ProtoField.uint32("simpleai.static.chrid", "Character id")
local fieldStaticNodeCount = ProtoField.uint32("simpleai.static.count", "Static node data count")
local fieldStaticNodeId = ProtoField.uint32("simpleai.static.id", "Node id")
local fieldStaticNodeName = ProtoField.stringz("simpleai.static.name", "Node name")
local fieldStaticNodeType = ProtoField.stringz("simpleai.static.type", "Node type")
local fieldStaticParameters = ProtoField.stringz("simpleai.static.parameters", "Node parameters")
local fieldStaticConditionType = ProtoField.stringz("simpleai.static.conditiontype", "Character type")
local fieldStaticConditionParameters = ProtoField.stringz("simpleai.static.conditionparameters", "Condition parameters")

local fieldDetailsCharacterId = ProtoField.stringz("simpleai.detail.chrid", "Character id")
local fieldDetailsAggroCount = ProtoField.uint16("simpleai.details.aggrocount", "Aggro entries")
local fieldDetailsAggroChrId = ProtoField.uint32("simpleai.details.aggrochrid", "Character id")
local fieldDetailsAggroAmount = ProtoField.float("simpleai.details.aggroamount", "Aggro amount")
local fieldDetailsNodeId = ProtoField.uint32("simpleai.details.id", "Node id")

local fieldSelectCharacterId = ProtoField.uint32("simpleai.select.chrid", "Character id")

local fieldPauseState = ProtoField.bool("simpleai.pause.state", "Pause state")

local fieldChangeName = ProtoField.stringz("simpleai.change.name", "Zone name")

local fieldStepMillis = ProtoField.int64("simpleai.step.millis", "Milliseconds")

function disPingMessage(buffer, tree)
	-- Nothing to do here
end

function disStateMessage(buffer, tree)
	tree:add(fieldStatesAmount, buffer(1, 4))
	local count = buffer(1, 4):uint()
	local offset = 5
	for i = 1, count do
		tree:add(fieldStatesId, buffer(offset, 4))
		offset = offset + 4;
		tree:add(fieldStatesX, buffer(offset, 4))
		offset = offset + 4;
		tree:add(fieldStatesY, buffer(offset, 4))
		offset = offset + 4;
		tree:add(fieldStatesZ, buffer(offset, 4))
		offset = offset + 4;
		tree:add(fieldStatesOrientation, buffer(offset, 4))
		offset = offset + 4;
		local attributesCount = buffer(offset, 2):uint()
		tree:add(fieldStatesAttributesCount, buffer(offset, 2))
		offset = offset + 2;
		for i = 1, attributesCount do
			tree:add(fieldStatesAttributesKey, buffer(offset))
			offset = offset + buffer(offset):stringz():len()
			tree:add(fieldStatesAttributesValue, buffer(offset))
			offset = offset + buffer(offset):stringz():len()
		end
	end
end

function disCharacterStaticMessage(buffer, tree)
	tree:add(fieldStaticCharacterId, buffer(1, 4))
	local count = buffer(5, 4):uint()
	tree:add(fieldStaticNodeCount, buffer(5, 4))
	local offset = 9
	for i = 1, count do
		tree:add(fieldStaticNodeId, buffer(offset, 4))
		offset = offset + 4;
		tree:add(fieldStaticNodeName, buffer(offset))
		offset = offset + buffer(offset):stringz():len()
		tree:add(fieldStaticNodeType, buffer(offset))
		offset = offset + buffer(offset):stringz():len()
		tree:add(fieldStaticParameters, buffer(offset))
		offset = offset + buffer(offset):stringz():len()
		tree:add(fieldStaticConditionType, buffer(offset))
		offset = offset + buffer(offset):stringz():len()
		tree:add(fieldStaticConditionParameters, buffer(offset))
		offset = offset + buffer(offset):stringz():len()
	end
end

function disCharacterDetailsMessage(buffer, tree)
	tree:add(fieldDetailsCharacterId, buffer(1, 4))

	-- read the aggro
	local aggroCount = buffer(5, 2):uint()
	tree:add(fieldStatesAttributesCount, buffer(5, 2))
	local offset = 7
	for i = 1, aggroCount do
		tree:add(fieldDetailsAggroChrId, buffer(offset, 4))
		offset = offset + 4
		tree:add(fieldDetailsAggroAmount, buffer(offset, 4))
		offset = offset + 4
	end

	-- now read the nodes
	-- TODO
end

function disSelectMessage(buffer, tree)
	tree:add(fieldSelectCharacterId, buffer(1, 4))
end

function disPauseMessage(buffer, tree)
	tree:add(fieldPauseState, buffer(1, 1))
end

function disChangeMessage(buffer, tree)
	tree:add(fieldChangeName, buffer(1))
end

function disNamesMessage(buffer, tree)
	tree:add(fieldZoneNamesAmount, buffer(1, 4))
	local count = buffer(1, 4):uint()
	local offset = 6
	for i = 1, count do
		tree:add(fieldZoneNamesName, buffer(offset):stringz())
		offset = offset + buffer(offset):stringz():len()
	end
end

function disResetMessage(buffer, tree)
	-- Nothing to do here
end

function disStepMessage(buffer, tree)
		tree:add(fieldStepMillis, buffer(1, 8))
end

function disUpdateNodeMessage(buffer, tree)
end

function disDeleteNodeMessage(buffer, tree)
end

function disAddNodeMessage(buffer, tree)
end

function simpleai.dissector(buffer, pinfo, tree)
	local id = buffer(0, 1):uint()

	local name = PROTO[id]
	if name == nil then
		name = "Unknown"
	end

	pinfo.cols.protocol = simpleai.name
	pinfo.cols.info = "Type: " .. name

	if id == 0 then
		disPingMessage(buffer, tree)
	elseif id == 1 then
		disStateMessage(buffer, tree)
	elseif id == 2 then
		disCharacterStaticMessage(buffer, tree)
	elseif id == 3 then
		disCharacterDetailsMessage(buffer, tree)
	elseif id == 4 then
		disSelectMessage(buffer, tree)
	elseif id == 5 then
		disPauseMessage(buffer, tree)
	elseif id == 6 then
		disChangeMessage(buffer, tree)
	elseif id == 7 then
		disNamesMessage(buffer, tree)
	elseif id == 8 then
		disResetMessage(buffer, tree)
	elseif id == 9 then
		disStepMessage(buffer, tree)
	elseif id == 10 then
		disUpdateNodeMessage(buffer, tree)
	elseif id == 11 then
		disDeleteNodeMessage(buffer, tree)
	elseif id == 12 then
		disAddNodeMessage(buffer, tree)
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

	simpleai_table = DissectorTable.get("tcp.port")
	simpleai_table:add(default_settings.port, simpleai)
end

