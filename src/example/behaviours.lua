function init ()
	local example = AI.createTree("example")
	local rootNodeExample = example:createRoot("PrioritySelector", "root")
		local attackerNode = rootNodeExample:addNode("Parallel", "attacker")
			attackerNode:setCondition("IsInGroup{1}")
			attackerNode:addNode("Wander", "wander")
		local victimNode = rootNodeExample:addNode("PrioritySelector", "victim")
			victimNode:setCondition("IsInGroup{2}")
			victimNode:addNode("FollowGroup{2}", "followowngroup"):setCondition("Not(IsCloseToGroup{1,10})")
			victimNode:addNode("FleeGroup{1}", "victim")

--	rootNode:addNode("Idle{3000}", "idle3000")
--	local prioSelector = rootNode:addNode("PrioritySelector", "prio2");
--	prioSelector:addNode("Idle{3000}", "idle3000"):setCondition("HasEnemies")
--	prioSelector:addNode("Idle{1000}", "idle1000")
--	rootNode:addNode("Idle{20}", "idle20")
end
