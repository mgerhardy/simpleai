function init ()
	local tree = AI.createTree("walk")
	local rootNode = tree:createRoot("PrioritySelector", "root");
	rootNode:addNode("Idle{1000}", "idle1000")
	rootNode:addNode("Move", "move"):setCondition("Not(HasEnemies)")
	local prioSelector = rootNode:addNode("PrioritySelector", "foobar");
	prioSelector:addNode("Idle{10}", "idle10")
end
