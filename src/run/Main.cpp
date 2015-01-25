#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <SimpleAI.h>
#include "GameEntity.h"
#include "GameMap.h"
#include <chrono>
#include <algorithm>
#include <atomic>

#ifdef AI_PROFILER
#include <google/profiler.h>
#endif

namespace {
std::atomic_int id(1);
std::atomic_bool autospawn(true);
std::atomic_bool shutdownThreads(false);
}

static std::string getOptParam(char** begin, char** end, const std::string& option, const std::string& defaultVal = "") {
	char** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end) {
		return *itr;
	}
	return defaultVal;
}

static bool optExists(char** begin, char** end, const std::string& option) {
	return std::find(begin, end, option) != end;
}

static ai::example::GameMap *createMap(int amount, ai::Server& server, const ai::TreeNodePtr& root, const std::string& name) {
	ai::example::GameMap* map = new ai::example::GameMap(600, name, server);

	for (int i = 0; i < amount; ++i) {
		map->addEntity(new ai::example::GameEntity(id++, map, root));
	}

	map->initializeAggro();

	return map;
}

#ifndef AI_NO_THREADING
static void runMap(ai::example::GameMap* map) {
	const std::chrono::milliseconds delay(250);
	auto timeLast = std::chrono::steady_clock::now();
	while (!shutdownThreads) {
		const auto timeNow = std::chrono::steady_clock::now();
		const auto dt = std::chrono::duration_cast < std::chrono::milliseconds > (timeNow - timeLast).count();
		timeLast = timeNow;
		map->update(static_cast<uint32_t>(dt));
		std::this_thread::sleep_for(delay);
	}
}

static void runServer(ai::Server* server) {
	const std::chrono::milliseconds delay(500);
	auto timeLast = std::chrono::steady_clock::now();
	while (!shutdownThreads) {
		const auto timeNow = std::chrono::steady_clock::now();
		const auto dt = std::chrono::duration_cast < std::chrono::milliseconds > (timeNow - timeLast).count();
		timeLast = timeNow;
		server->update(dt);
		std::this_thread::sleep_for(delay);
	}
}

static void runDespawnSpawn(ai::example::GameMap* map, const ai::TreeNodePtr* root) {
	const std::chrono::milliseconds delay(15000);
	while (!shutdownThreads) {
		if (autospawn) {
			ai::example::GameEntity *rnd = map->getRandomEntity();
			if (rnd != nullptr) {
				map->remove(rnd);
				delete rnd;
			}

			map->addEntity(new ai::example::GameEntity(id++, map, *root));
		}
		std::this_thread::sleep_for(delay);
	}
}

#endif

static ai::TreeNodePtr load(const std::string filename, const std::string& name) {
	ai::AIRegistry registry;
	// add your own tasks and conditions here
	ai::LUATreeLoader loader(registry);

	std::ifstream btStream(filename);
	if (!btStream) {
		std::cerr << "could not load " << filename << std::endl;
		return ai::TreeNodePtr();
	}

	std::string str((std::istreambuf_iterator<char>(btStream)), std::istreambuf_iterator<char>());
	if (!loader.init(str)) {
		std::cerr << "could not load the tree" << std::endl;
		std::cerr << loader.getError() << std::endl;
		return ai::TreeNodePtr();
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
		return ai::TreeNodePtr();
	}

	return root;
}

int main(int argc, char **argv) {
	char **b = argv;
	char **e = argv + argc;
	if (argc <= 1 || optExists(b, e, "-h") || optExists(b, e, "-help") || !optExists(b, e, "-file")) {
		std::cerr << "usage: simpleai-run -file behaviourtree.lua [options]" << std::endl;
		std::cerr << "Valid options are (default values are given here):" << std::endl;
		std::cerr << "-amount 10            - how many entities are spawned on each map" << std::endl;
		std::cerr << "-maps 4               - how many maps should get spawned" << std::endl;
		std::cerr << "-autospawn true|false - automatic respawn (despawn random and respawn) of entities" << std::endl;
		std::cerr << "-name example         - the name of the behaviour tree in the given script" << std::endl;
		std::cerr << "-seed 1               - use a fixed seed for all the random actions" << std::endl;
		std::cerr << "-help -h              - show this help screen" << std::endl;
		std::cerr << std::endl;
		std::cerr << "Network related options" << std::endl;
		std::cerr << "-interface 0.0.0.0    - the interface the server will listen on" << std::endl;
		std::cerr << "-port 12345           - the port of the server to listen on" << std::endl;
#ifdef AI_PROFILER
		std::cerr << "-profilerOutput       - google profiler output file" << std::endl;
#endif
		return EXIT_FAILURE;
	}

	autospawn = getOptParam(b, e, "-autospawn", "true") == "true";
	const int seed = std::stoi(getOptParam(b, e, "-seed", "1"));
	const int mapAmount = std::stoi(getOptParam(b, e, "-maps", "4"));
	const int amount = std::stoi(getOptParam(b, e, "-amount", "10"));
	const short port = static_cast<short>(std::stoi(getOptParam(b, e, "-port", "12345")));
	const std::string name = getOptParam(b, e, "-name", "example");
	const std::string filename = getOptParam(b, e, "-file");
	const std::string interface = getOptParam(b, e, "-interface", "0.0.0.0");

#ifdef AI_PROFILER
	const std::string profilerOutput = getOptParam(b, e, "-profilerOutput", "simpleai-run.prof");
	ProfilerStart(profilerOutput.c_str());
#endif

	ai::randomSeed(seed);

	ai::TreeNodePtr root = load(filename, name);
	if (!root) {
		return EXIT_FAILURE;
	}

	std::cout << *root.get() << std::endl;
	std::cout << "successfully loaded the behaviour tree " << name << std::endl;
	std::cout << "now run this behaviour tree with " << amount << " entities on each map" << std::endl;
	std::cout << "spawn " << mapAmount << " maps with seed " << seed << std::endl;
	std::cout << "automatic respawn: " << (autospawn ? "true" : "false") << std::endl;
#ifdef AI_NO_THREADING
	std::cout << "compiled without threading support" << std::endl;
#else
	std::cout << "compiled with threading support" << std::endl;
#endif

	ai::Server server(port, interface);
	if (!server.start()) {
		std::cerr << "Could not start the server on port " << port << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Started server on " << interface << ":" << port << std::endl;

	typedef std::vector<ai::example::GameMap*> Maps;
	Maps maps;
	for (int i = 0; i < mapAmount; ++i) {
		maps.push_back(createMap(amount, server, root, "Map" + std::to_string(i)));
	}

#ifdef AI_NO_THREADING
	const std::chrono::milliseconds delay(100);
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
	for (auto i = maps.begin(); i != maps.end(); ++i) {
		threads.push_back(std::thread(runMap, *i));
		const std::string threadName = "tick-" + (*i)->getName();
		pthread_setname_np(threads.back().native_handle(), threadName.c_str());
	}

	for (auto i = maps.begin(); i != maps.end(); ++i) {
		threads.push_back(std::thread(runDespawnSpawn, *i, &root));
		const std::string threadName = "spawn-" + (*i)->getName();
		pthread_setname_np(threads.back().native_handle(), threadName.c_str());
	}

	threads.push_back(std::thread(runServer, &server));
	pthread_setname_np(threads.back().native_handle(), "tick-server");

	std::cout << "hit q to quit or h for help" << std::endl;
	for (;;) {
		std::string c;
		std::getline(std::cin, c);

		if (c == "q") {
			std::cout << "quitting - waiting for threads" << std::endl;
			break;
		} else if (c == "d") {
			for (std::vector<ai::example::GameMap*>::const_iterator i = maps.begin(); i != maps.end(); ++i) {
				ai::example::GameMap *map = *i;
				std::cout << map->getName() << std::endl;
				const ai::Zone& zone = map->getZone();
				int count = 0;
				auto func = [&] (const ai::AI& ai) {
					std::cout << "id: " << ai.getCharacter().getId() << std::endl;
					// std::cout << *root.get() << std::endl;
					std::cout << " \\- pos: " << ai.getCharacter().getPosition() << std::endl;
					std::cout << " \\- speed: " << ai.getCharacter().getSpeed() << std::endl;
					std::cout << " \\- orientation: " << ai.getCharacter().getOrientation() << std::endl;
					std::cout << " \\- attributes:" << std::endl;
					const ai::CharacterAttributes& attributes = ai.getCharacter().getAttributes();
					for (ai::CharacterAttributes::const_iterator attribIter = attributes.begin(); attribIter != attributes.end(); ++attribIter) {
						std::cout << "  \\- " << attribIter->first << ": \"" << attribIter->second << "\"" << std::endl;
					}
					++count;
				};
				zone.visit(func);
				std::cout << " - sum: " << count << " entities" << std::endl;
			}
		} else if (c == "t") {
			autospawn = !autospawn;
			std::cout << "automatic respawn: " << (autospawn ? "true" : "false") << std::endl;
		} else if (c == "r") {
			for (std::vector<ai::example::GameMap*>::const_iterator i = maps.begin(); i != maps.end(); ++i) {
				ai::example::GameMap *map = *i;
				ai::example::GameEntity *rnd = map->getRandomEntity();
				if (rnd != nullptr) {
					if (!map->remove(rnd)) {
						std::cout << "failed to remove " << rnd->getId() << " from map " << map->getName() << std::endl;
					} else {
						std::cout << "removed " << rnd->getId() << " from map " << map->getName() << std::endl;
					}
				}

				ai::example::GameEntity* ent = map->addEntity(new ai::example::GameEntity(id++, map, root));
				std::cout << "spawned " << ent->getId() << " on map " << map->getName() << std::endl;
			}
		} else if (c == "reload") {
			ai::TreeNodePtr newRoot = load(filename, name);
			if (!newRoot)
				continue;
			root = newRoot;
			auto func = [&] (ai::AI& ai) {
				ai.setBehaviour(root);
			};
			for (ai::example::GameMap* map : maps) {
				map->getZone().visit(func);
			}
			std::cout << "reloaded the behaviour tree: " << name << std::endl;
			std::cout << *root.get() << std::endl;
		} else {
			std::cout << "q      - quit" << std::endl;
			std::cout << "r      - respawn" << std::endl;
			std::cout << "t      - trigger automatic respawn" << std::endl;
			std::cout << "d      - detail" << std::endl;
			std::cout << "reload - reload the behaviour tree from file" << std::endl;
		}
	}

	shutdownThreads = true;
	for (std::thread& t : threads) {
		t.join();
	}
#endif

#ifdef AI_PROFILER
	ProfilerStop();
#endif
	return EXIT_SUCCESS;
}
