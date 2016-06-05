-- Packet dissector for the SimpleAI remote debugger protocol
-- Copy this to $HOME/.wireshark/plugins/ or ~/.config/wireshark/plugins
-- (check Help->About Wireshark->Folders->Personal Plugins)

-- wireshark -Xlua_script:wireshark-dissector.lua simpleaidump.pcap

-- filter 'tcp.port==10001 and tcp.len>0'

-- https://wiki.wireshark.org/LuaAPI
-- https://www.wireshark.org/docs/wsdg_html_chunked/wsluarm_modules.html

local default_settings = {
	port = 10001,
}

-- 4 bytes size - 1 byte id
local minlen = 5

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

local fields = simpleai.fields
fields.msgsize = ProtoField.uint32("simpleai.msgsize", "Message Size")
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

simpleai.prefs.port = Pref.uint("Port number", default_settings.port, "The TCP port number for the simpleai debug server")

function disPingMessage(buffer, tree)
	-- Nothing to do here
end

function disStateMessage(buffer, tree)
	tree:add(simpleai.fields.fieldStatesAmount, buffer(0, 4))
	local count = buffer(0, 4):le_uint()
	local offset = 4
	for i = 1, count do
		tree:add(simpleai.fields.fieldStatesId, buffer(offset, 4))
		offset = offset + 4
		tree:add(simpleai.fields.fieldStatesX, buffer(offset, 4))
		offset = offset + 4
		tree:add(simpleai.fields.fieldStatesY, buffer(offset, 4))
		offset = offset + 4
		tree:add(simpleai.fields.fieldStatesZ, buffer(offset, 4))
		offset = offset + 4
		tree:add(simpleai.fields.fieldStatesOrientation, buffer(offset, 4))
		offset = offset + 4
		local attributesCount = buffer(offset, 2):le_uint()
		tree:add(simpleai.fields.fieldStatesAttributesCount, buffer(offset, 2))
		offset = offset + 2
		for i = 1, attributesCount do
			tree:add(simpleai.fields.fieldStatesAttributesKey, buffer(offset))
			offset = offset + buffer(offset):stringz():len()
			tree:add(simpleai.fields.fieldStatesAttributesValue, buffer(offset))
			offset = offset + buffer(offset):stringz():len()
		end
	end
end

function disCharacterStaticMessage(buffer, tree)
	tree:add(simpleai.fields.fieldStaticCharacterId, buffer(0, 4))
	local count = buffer(4, 4):le_uint()
	tree:add(simpleai.fields.fieldStaticNodeCount, buffer(4, 4))
	local offset = 8
	for i = 1, count do
		tree:add(simpleai.fields.fieldStaticNodeId, buffer(offset, 4))
		offset = offset + 4
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
	local childrenCount = buffer(offset, 2):le_uint()
	tree:add(simpleai.fields.fieldDetailsChildrenCount, buffer(offset, 2))
	offset = offset + 2
	for i = 1, childrenCount do
		fields.advance = disCharacterDetailsMessageNode(buffer, tree, offset)
		offset = offset + advance
	end
	return offset
end

function disCharacterDetailsMessage(buffer, tree)
	tree:add(simpleai.fields.fieldDetailsCharacterId, buffer(0, 4))

	-- read the aggro
	local aggroCount = buffer(4, 2):le_uint()
	tree:add(simpleai.fields.fieldDetailsAggroCount, buffer(4, 2))
	local offset = 6
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
	tree:add(simpleai.fields.fieldSelectCharacterId, buffer(0, 4))
end

function disPauseMessage(buffer, tree)
	tree:add(simpleai.fields.fieldPauseState, buffer(0, 1))
end

function disChangeMessage(buffer, tree)
	tree:add(simpleai.fields.fieldChangeName, buffer(0))
end

function disNamesMessage(buffer, tree)
	tree:add(simpleai.fields.fieldZoneNamesAmount, buffer(0, 4))
	local count = buffer(0, 4):le_uint()
	local offset = 4
	for i = 1, count do
		tree:add(simpleai.fields.fieldZoneNamesName, buffer(offset))
		offset = offset + buffer(offset):stringz():len()
	end
end

function disResetMessage(buffer, tree)
	-- Nothing to do here
end

function disStepMessage(buffer, tree)
	tree:add(simpleai.fields.fieldStepMillis, buffer(0, 8))
end

function disUpdateNodeMessage(buffer, tree)
end

function disDeleteNodeMessage(buffer, tree)
end

function disAddNodeMessage(buffer, tree)
end

function disMessage(buffer, pinfo, tree)
	local subtree = tree:add(simpleai, buffer)
	local id = buffer(0, 1):le_uint()

	local name = PROTO[id]

	if name == nil then
		pinfo.cols.info:append("Type: unknown message type " .. id)
		subtree:add_expert_info(PI_MALFORMED, PI_ERROR, "Unknown message type " .. id)
		return
	end

	pinfo.cols.info:append("Type: " .. name .. ", Id_ " .. id)

	local treeitemName = subtree:add(simpleai.fields.msgtype, name)
	treeitemName:set_text("Type: " .. name .. ", Id_ " .. id)

	local msgBuffer = buffer(1)
	if id == 0 then
		disPingMessage(msgBuffer, subtree)
	elseif id == 1 then
		disStateMessage(msgBuffer, subtree)
	elseif id == 2 then
		disCharacterStaticMessage(msgBuffer, subtree)
	elseif id == 3 then
		disCharacterDetailsMessage(msgBuffer, subtree)
	elseif id == 4 then
		disSelectMessage(msgBuffer, subtree)
	elseif id == 5 then
		disPauseMessage(msgBuffer, subtree)
	elseif id == 6 then
		disChangeMessage(msgBuffer, subtree)
	elseif id == 7 then
		disNamesMessage(msgBuffer, subtree)
	elseif id == 8 then
		disResetMessage(msgBuffer, subtree)
	elseif id == 9 then
		disStepMessage(msgBuffer, subtree)
	elseif id == 10 then
		disUpdateNodeMessage(msgBuffer, subtree)
	elseif id == 11 then
		disDeleteNodeMessage(msgBuffer, subtree)
	elseif id == 12 then
		disAddNodeMessage(msgBuffer, subtree)
	end
end

function simpleai.dissector(buffer, pinfo, tree)
	local length = 0
	local offset = 0
	pinfo.cols.info = ""

	repeat
		if buffer:len() - length >= minlen then
			local msglength = buffer(length, 4):le_uint() + minlen
			length = length + msglength

			if length > buffer:len() then
				pinfo.desegment_len = length - buffer:len()
				return
			end


			pinfo.cols.protocol = simpleai.name

			disMessage(buffer(offset, msglength), pinfo, tree)
			offset = offset + msglength
		else
			pinfo.desegment_len = DESEGMENT_ONE_MORE_SEGMENT
			return
		end
	until length >= buffer:len()
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
	simpleai_table = DissectorTable.get("tcp.port")
	default_settings.port = simpleai.prefs.port
	print("register simpleai protocol for port: " .. simpleai.prefs.port)
	-- useful while debugging and applying the dissector multiple times
	simpleai_table:remove(simpleai.prefs.port, simpleai)
	simpleai_table:add(simpleai.prefs.port, simpleai)
end

