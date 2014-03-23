function init ()
	local tree = AI.createTree("example")
	local rootNode = tree:createRoot("Sequence", "root");
	rootNode:addNode("Idle{500}", "idle500")
	rootNode:addNode("Move", "move"):setCondition("Not(HasEnemies)")
	local prioSelector = rootNode:addNode("Parallel", "foobar");
	prioSelector:addNode("Idle{10}", "idle10")
end
