#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <SimpleAI.h>
#include "actions/ExampleTask.h"
#include "GameEntity.h"
#include "GameMap.h"
#include <chrono>

namespace {
int id = 1;
ai::GroupMgr groupManager;
}

static ai::example::GameMap *createMap(ai::GroupMgr& groupManager, int amount, ai::Server& server, const ai::TreeNodePtr& root, const std::string& name) {
	ai::example::GameMap* map = new ai::example::GameMap(300, name, server);

	for (int i = 0; i < amount; ++i) {
		ai::example::GameEntity* e = map->addEntity(new ai::example::GameEntity(id++, root, groupManager));
		e->setPosition(map->getStartPosition());
	}

	map->initializeAggro();

	return map;
}

#ifndef AI_NO_THREADING
static void runMap(ai::example::GameMap* map) {
	const std::chrono::milliseconds delay(10);
	auto timeLast = std::chrono::steady_clock::now();
	for (;;) {
		const auto timeNow = std::chrono::steady_clock::now();
		const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - timeLast).count();
		timeLast = timeNow;
		map->update(static_cast<uint32_t>(dt));
		std::this_thread::sleep_for(delay);
	}
}

static void runServer(ai::Server* server) {
	const std::chrono::milliseconds delay(100);
	auto timeLast = std::chrono::steady_clock::now();
	for (;;) {
		const auto timeNow = std::chrono::steady_clock::now();
		const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - timeLast).count();
		timeLast = timeNow;
		server->update(dt);
		std::this_thread::sleep_for(delay);
	}
}

static void runRespawn(ai::example::GameMap* map, const ai::TreeNodePtr* root) {
	const std::chrono::milliseconds delay(10000);
	std::this_thread::sleep_for(delay);
	ai::example::GameEntity* e = map->addEntity(new ai::example::GameEntity(id++, *root, groupManager));
	e->setPosition(map->getStartPosition());
}

static void runDespawn(ai::example::GameMap* map, const ai::TreeNodePtr* root) {
	const std::chrono::milliseconds delay(10000);
	std::this_thread::sleep_for(delay);
	ai::example::GameEntity* e = map->addEntity(new ai::example::GameEntity(id++, *root, groupManager));
	e->setPosition(map->getStartPosition());
}

#endif

int main(int argc, char **argv) {
	if (argc <= 1) {
		std::cerr << "usage: example behaviourtree.lua [amount]" << std::endl;
		return EXIT_FAILURE;
	}

	srand(1);

	// define your own tasks and conditions
	ai::AIRegistry registry;
	registry.registerNodeFactory("ExampleTask", ai::example::ExampleTask::FACTORY);

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

	const short port = 12345;
	ai::Server server(port, "0.0.0.0");
	if (!server.start()) {
		std::cerr << "Could not start the server on port " << port << std::endl;
		return EXIT_FAILURE;
	} else {
		std::cout << "Started server on port " << port << std::endl;
	}

	int mapAmount = 4;
	std::vector<ai::example::GameMap*> maps;
	for (int i = 0; i < mapAmount; ++i) {
		maps.push_back(createMap(groupManager, amount, server, root, "Map" + std::to_string(i)));
	}

#ifdef AI_NO_THREADING
	const std::chrono::milliseconds delay(10);
	auto timeLast = std::chrono::steady_clock::now();
	for (;;) {
		const auto timeNow = std::chrono::steady_clock::now();
		const auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - timeLast).count();
		timeLast = timeNow;
		for (std::vector<ai::example::GameMap*>::const_iterator i = maps.begin(); i != maps.end(); ++i) {
		   (*i)->update(static_cast<uint32_t>(dt));
		}
		server.update(dt);
		std::this_thread::sleep_for(delay);
	}
#else
	typedef std::vector<std::thread> Threads;
	Threads threads;
	for (std::vector<ai::example::GameMap*>::const_iterator i = maps.begin(); i != maps.end(); ++i) {
		threads.push_back(std::thread(runMap, *i));
	}

	for (std::vector<ai::example::GameMap*>::const_iterator i = maps.begin(); i != maps.end(); ++i) {
		threads.push_back(std::thread(runRespawn, *i, &root));
	}

	threads.push_back(std::thread(runServer, &server));
#endif
	std::cout << "hit enter to quit" << std::flush;
	std::cin.get();
	std::cout << "quitting" << std::endl;

#ifndef AI_NO_THREADING
	for (Threads::iterator i = threads.begin(); i != threads.end(); ++i) {
		i->detach();
	}
#endif

	return EXIT_SUCCESS;
}
