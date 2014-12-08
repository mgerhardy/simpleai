#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <SimpleAI.h>
#include "Pathfinder.h"
#include "actions/ExampleTask.h"
#include "actions/Move.h"
#include "conditions/IsMoving.h"
#include "GameEntity.h"
#include "GameMap.h"
#include <chrono>
#include <thread>

static ai::example::GameMap *createMap(ai::GroupMgr& groupManager, ai::Network& network, int amount, const ai::TreeNodePtr& root, const std::string& name) {
	ai::example::GameMap* map = new ai::example::GameMap(network, 300, name);
	ai::example::Pathfinder* pathFinder = new ai::example::Pathfinder(*map);

	for (int i = 0; i < amount; ++i) {
		ai::example::GameEntity* e = map->addEntity(new ai::example::GameEntity(i, root, *pathFinder, groupManager));
		e->setPosition(map->getStartPosition());
	}

	// TODO: remove me once we have an attack
	std::vector<ai::example::GameEntity*>& entities = map->getEntities();
	for (std::vector<ai::example::GameEntity*>::iterator i = entities.begin() + 1; i != entities.end(); ++i) {
		ai::Entry* e = entities[0]->addAggro(**i, 1000.0f + static_cast<float>(rand() % 1000));
		e->setReduceByValue(1.0f + static_cast<float>(rand() % 3));
	}

	return map;
}

int main(int argc, char **argv) {
	if (argc <= 1) {
		std::cerr << "usage: example behaviourtree.lua [amount]" << std::endl;
		return EXIT_FAILURE;
	}

	srand(1);

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
	std::cout << "now run this behaviour tree with " << amount << " entities for some time: " << std::endl << *root.get() << std::endl;

	const int port = 12345;
	ai::Network network(port, "0.0.0.0");
	if (!network.start()) {
		std::cerr << "Failed to initialize the network" << std::endl;
		return EXIT_FAILURE;
	} else {
		std::cout << "Started the server and accept connections on port " << port << std::endl;
	}
	ai::GroupMgr groupManager;
	std::vector<ai::example::GameMap*> maps;
	maps.push_back(createMap(groupManager, network, amount, root, "Map1"));
	maps.push_back(createMap(groupManager, network, amount, root, "Map2"));

	std::vector<std::string> names;
	for (std::vector<ai::example::GameMap*>::const_iterator i = maps.begin(); i != maps.end(); ++i) {
		names.push_back((*i)->getName());
	}
	const ai::AINamesMessage msg(names);

	const std::chrono::milliseconds delay(10);
	auto timeLast = std::chrono::steady_clock::now();
	for (;;) {
		const auto timeNow = std::chrono::steady_clock::now();
		const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - timeLast).count();
		timeLast = timeNow;
		for (std::vector<ai::example::GameMap*>::const_iterator i = maps.begin(); i != maps.end(); ++i) {
			(*i)->update(static_cast<uint32_t>(dt));
		}
		network.broadcast(msg);
		std::this_thread::sleep_for(delay);
	}
}
