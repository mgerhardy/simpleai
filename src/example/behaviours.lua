function init ()
	local tree = AI.createTree("example")
	local rootNode = tree:createRoot("Sequence", "root");
--	rootNode:addNode("Idle{3000}", "idle3000")
--	local prioSelector = rootNode:addNode("PrioritySelector", "prio2");
--	prioSelector:addNode("Idle{3000}", "idle3000"):setCondition("HasEnemies")
--	prioSelector:addNode("Idle{1000}", "idle1000")
	rootNode:addNode("Idle{20}", "idle20")
	rootNode:addNode("Wander", "move")
end
