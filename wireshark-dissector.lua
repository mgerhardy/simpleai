-- Packet dissector for the SimpleAI remote debugger protocol
-- Copy this to $HOME/.wireshark/plugins/

simpleai = Proto("simpleai", "SimpleAI remote debugger")

function simpleai.dissector(buffer, pinfo, tree)
	local id = buffer(0, 1):uint()

	pinfo.cols.protocol = simpleai.name
	pinfo.cols.info = "Type: "
	if id == 0 then
		pinfo.cols.info:append("PING")
	elseif id == 1 then
		pinfo.cols.info:append("STATE")
	elseif id == 2 then
		pinfo.cols.info:append("CHARACTER_STATIC")
	elseif id == 3 then
		pinfo.cols.info:append("CHARACTER_DETAILS")
	elseif id == 4 then
		pinfo.cols.info:append("SELECT")
	elseif id == 5 then
		pinfo.cols.info:append("PAUSE")
	elseif id == 6 then
		pinfo.cols.info:append("CHANGE")
	elseif id == 7 then
		pinfo.cols.info:append("NAMES")
	elseif id == 8 then
		pinfo.cols.info:append("RESET")
	elseif id == 9 then
		pinfo.cols.info:append("STEP")
	elseif id == 10 then
		pinfo.cols.info:append("UPDATENODE")
	elseif id == 11 then
		pinfo.cols.info:append("DELETENODE")
	elseif id == 12 then
		pinfo.cols.info:append("ADDNODE")
	else
		pinfo.cols.info:append("Unknown")
	end
end

function simpleai.init()
	local fields = simpleai.fields
	f.msgid = ProtoFields.uint8("simpleai.msgid", "Message Id")

	simpleai_table = DissectorTable.get("tcp.port")
	simpleai_table:add(10001, SIMPLEAI)
end

