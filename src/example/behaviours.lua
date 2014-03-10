function init ()
	local tree = AI.createTree("walk")
	local rootNode = tree:createRoot("PrioritySelector", "root");
	rootNode:addNode("Move", "move"):setCondition("Not(HasEnemies)")
end
