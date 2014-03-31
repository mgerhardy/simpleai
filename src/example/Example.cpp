#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <sys/time.h>
#include <unistd.h>
#include <SimpleAI.h>
#include "Pathfinder.h"
#include "actions/ExampleTask.h"
#include "actions/Move.h"
#include "conditions/IsMoving.h"
#include "GameEntity.h"
#include "GameMap.h"

static inline uint32_t getCurrentMillis () {
	struct timeval now;
	gettimeofday(&now, NULL);
	const uint32_t ms = now.tv_sec * 1000 + now.tv_usec / 1000;
	return ms;
}

int main(int argc, char **argv) {
	if (argc <= 1) {
		std::cerr << "usage: example behaviourtree.lua [amount]" << std::endl;
		return EXIT_FAILURE;
	}

	srand(getCurrentMillis());

	// define your own tasks and conditions
	ai::AIRegistry registry;
	registry.registerNodeFactory("Move", ai::example::Move::FACTORY);
	registry.registerNodeFactory("ExampleTask", ai::example::ExampleTask::FACTORY);
	registry.registerConditionFactory("IsMoving", ai::example::IsMoving::FACTORY);

	ai::LUATreeLoader loader(registry);
	const std::string name = "example";

	const std::string filename = argv[1];
	std::ifstream t(filename.c_str());
	if (!t) {
		std::cerr << "could not load " << filename << std::endl;
		return EXIT_FAILURE;
	}

	int amount = 1;
	if (argc >= 3) {
		amount = atoi(argv[2]);
		if (amount <= 0) {
			std::cerr << "invalid amount given" << std::endl;
			return EXIT_FAILURE;
		}
	}

	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	if (!loader.init(str)) {
		std::cerr << "could not load the tree" << std::endl;
		std::cerr << loader.getError() << std::endl;
		return EXIT_FAILURE;
	}
	std::cerr << "loaded the tree: " << filename << std::endl;
	const ai::TreeNodePtr& root = loader.load(name);
	if (!root) {
		std::cerr << "could not find behaviour with the name " << name << std::endl;
		std::vector<std::string> trees;
		loader.getTrees(trees);
		std::cerr << "found " << trees.size() << " trees" << std::endl;
		for (std::vector<std::string>::const_iterator i = trees.begin(); i != trees.end(); ++i) {
			std::cerr << "found " << *i << std::endl;
		}
		return EXIT_FAILURE;
	}

	std::cout << "successfully loaded the behaviour tree " << name << std::endl;
	std::cout << "now run this behaviour tree for some time: " << std::endl << *root.get() << std::endl;

	ai::GroupMgr groupManager;
	ai::example::GameMap gameMap(300);
	ai::example::Pathfinder pathFinder(gameMap);

	for (int i = 0; i < amount; ++i) {
		ai::example::GameEntity* e = gameMap.addEntity(new ai::example::GameEntity(i, root, pathFinder, groupManager));
		if (i == 0)
			groupManager.add(1, e);
		e->setPosition(gameMap.getStartPosition());
	}

	// TODO: remove me once we have an attack
	std::vector<ai::example::GameEntity*>& entities = gameMap.getEntities();
	for (std::vector<ai::example::GameEntity*>::iterator i = entities.begin() + 1; i != entities.end(); ++i) {
		ai::Entry* e = entities[0]->addAggro(**i, 1000.0f + (rand() % 1000));
		e->setReduceByValue(1.0f + rand() % 3);
	}

	uint32_t timeLast = getCurrentMillis();
	for (;;) {
		const uint32_t timeNow = getCurrentMillis();
		const uint32_t dt = timeNow - timeLast;
		timeLast = timeNow;
		if (dt > 0)
			gameMap.update(dt);
		sleep(0);
	}

	return 0;
}
