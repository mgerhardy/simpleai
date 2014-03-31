function init ()
	local example = AI.createTree("example")
	local rootNodeExample = example:createRoot("PrioritySelector", "root")
		local attackerParallel = rootNodeExample:addNode("Parallel", "attacker")
			attackerParallel:setCondition("IsInGroup{1}")
			attackerParallel:addNode("Wander", "wander")
			attackerParallel:addNode("FollowGroup{2}", "followvictimgroup")
		local victimParallel = rootNodeExample:addNode("Parallel", "victim")
			victimParallel:setCondition("IsInGroup{2}")
			victimParallel:addNode("Wander", "wander")
			victimParallel:addNode("FollowGroup{1}", "followowngroup")
			victimParallel:addNode("FleeGroup{1}", "victim")

--	rootNode:addNode("Idle{3000}", "idle3000")
--	local prioSelector = rootNode:addNode("PrioritySelector", "prio2");
--	prioSelector:addNode("Idle{3000}", "idle3000"):setCondition("HasEnemies")
--	prioSelector:addNode("Idle{1000}", "idle1000")
--	rootNode:addNode("Idle{20}", "idle20")
end
