function subtreeExample (tree)
	tree:createRoot("Steer(Wander)", tree:getName())
end

function init ()
	subtreeExample(AI.createTree("wander"))

	local example = AI.createTree("example")
	local rootNodeExample = example:createRoot("PrioritySelector", "root")
		local group1Node = rootNodeExample:addNode("PrioritySelector", "group1")
			group1Node:setCondition("IsInGroup{1}")
			group1Node:addNode("Steer(Wander)", "wander"):setCondition("IsGroupLeader{1}")
			group1Node:addNode("Steer{0.6,0.4}(GroupFlee{2},SelectionSeek)", "steering"):setCondition("Filter(SelectGroupLeader{1})")
		local group2Node = rootNodeExample:addNode("PrioritySelector", "group2")
			group2Node:setCondition("IsInGroup{2}")
			group2Node:addNode("Steer(Wander)", "wander"):setCondition("IsGroupLeader{2}")
			group2Node:addNode("Steer(SelectionSeek)", "followgroupleader2"):setCondition("Filter(SelectGroupLeader{2})")
		local group3Node = rootNodeExample:addNode("PrioritySelector", "group3")
			group3Node:setCondition("IsInGroup{3}")
			group3Node:addNode("Steer(Wander)", "wander"):setCondition("IsGroupLeader{3}")
			group3Node:addNode("Steer(SelectionSeek)", "followgroupleader3"):setCondition("Filter(SelectGroupLeader{3})")
		local everythingElseNode = rootNodeExample:addNode("PrioritySelector", "otherornogroup")
			everythingElseNode:addNode("Steer(Wander)", "wander")

--	rootNode:addNode("Idle{3000}", "idle3000")
--	local prioSelector = rootNode:addNode("PrioritySelector", "prio2");
--	prioSelector:addNode("Idle{3000}", "idle3000"):setCondition("HasEnemies")
--	prioSelector:addNode("MoveTo", "movetoaggro"):setCondition("Filter(SelectHighestAggro)")
--	prioSelector:addNode("Idle{1000}", "idle1000")
--	rootNode:addNode("Idle{20}", "idle20")
end
