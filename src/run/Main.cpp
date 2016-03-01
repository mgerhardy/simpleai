#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <SimpleAI.h>
#include <tree/loaders/lua/LUATreeLoader.h>
#include "GameEntity.h"
#include "GameMap.h"
#include <chrono>
#include <algorithm>
#include <atomic>

#ifdef AI_PROFILER
#include <google/profiler.h>
#endif

class Loader {
private:
	ai::AIRegistry _registry;
	// add your own tasks and conditions here
	ai::LUATreeLoader _loader;
public:
	Loader() :
			_loader(_registry) {
	}

	ai::TreeNodePtr load(const std::string &name) {
		return _loader.load(name);
	}

	operator ai::AIRegistry& () {
		return _registry;
	}

	void getTrees(std::vector<std::string>& trees) const {
		_loader.getTrees(trees);
	}

	bool init(const std::string& lua) {
		return _loader.init(lua);
	}

	inline std::string getError() const {
		return _loader.getError();
	}
};

namespace {
std::atomic_int id(0);
std::atomic_bool autospawn(true);
std::atomic_bool shutdownThreads(false);
Loader loader;
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

static ai::example::GameMap *createMap(int amount, ai::Server& server, const std::string& name) {
	ai::example::GameMap* map = new ai::example::GameMap(600, name, server);

	std::vector<std::string> trees;
	loader.getTrees(trees);

	for (int i = 0; i < amount; ++i) {
		auto randomIter = ai::randomElement(trees.begin(), trees.end());
		ai::TreeNodePtr root = loader.load(*randomIter);
		ai::ICharacterPtr e(new ai::example::GameEntity(id++, map));
		ai::AIPtr ai(new ai::AI(root));
		ai->setCharacter(e);
		const ai::GroupId groupId = ai::random(1, 3);
		map->addEntity(ai, groupId);
	}

	map->initializeAggro();

	return map;
}

static void runDespawnSpawn(ai::example::GameMap* map) {
	if (!autospawn) {
		return;
	}
	const ai::AIPtr& rnd = map->getRandomEntity();
	if (rnd) {
		map->remove(rnd);
	}

	std::vector<std::string> trees;
	loader.getTrees(trees);
	auto randomIter = ai::randomElement(trees.begin(), trees.end());
	ai::TreeNodePtr root = loader.load(*randomIter);
	ai::ICharacterPtr e(new ai::example::GameEntity(id++, map));
	ai::AIPtr ai(new ai::AI(root));
	ai->setCharacter(e);
	const ai::GroupId groupId = ai::random(1, 3);
	map->addEntity(ai, groupId);
}

static bool load(const std::string filename) {
	std::ifstream btStream(filename);
	if (!btStream) {
		std::cerr << "could not load " << filename << std::endl;
		return false;
	}

	std::string str((std::istreambuf_iterator<char>(btStream)), std::istreambuf_iterator<char>());
	if (!loader.init(str)) {
		std::cerr << "could not load the tree" << std::endl;
		std::cerr << loader.getError() << std::endl;
		return false;
	}
	return true;
}

static void handleInput(const std::string& filename, const std::vector<ai::example::GameMap*>& maps) {
	std::cout << "hit q to quit or h for help" << std::endl;
	for (;;) {
		std::string c;
		std::getline(std::cin, c);

		if (c == "q") {
			std::cout << "quitting" << std::endl;
			break;
		} else if (c == "g") {
			for (ai::example::GameMap* map : maps) {
				std::cout << map->getName() << std::endl;
				const ai::Zone& zone = map->getZone();
				std::cout << "groups: " << std::endl;
				const ai::GroupMgr& groupMgr = zone.getGroupMgr();
				for (int groupId = 1; groupId <= 3; ++groupId) {
					const ai::AIPtr& leader = groupMgr.getLeader(groupId);
					if (!leader)
						continue;
					std::cout << " \\- group " << groupId << ":" << groupMgr.getPosition(groupId) << " - " << leader->getCharacter()->getPosition() << std::endl;
				}
			}
		} else if (c == "d") {
			for (ai::example::GameMap* map : maps) {
				std::cout << map->getName() << std::endl;
				const ai::Zone& zone = map->getZone();
				int count = 0;
				auto func = [&] (const ai::AIPtr& ai) {
					const ai::ICharacterPtr& chr = ai->getCharacter();
					const ai::GroupMgr& groupMgr = zone.getGroupMgr();
					std::cout << "id: " << chr->getId() << std::endl;
					std::cout << " \\- pos: " << chr->getPosition() << std::endl;
					std::cout << " \\- speed: " << chr->getSpeed() << std::endl;
					std::cout << " \\- group leader 1: " << (groupMgr.isGroupLeader(1, ai) ? "true" : "false") << std::endl;
					std::cout << " \\- group leader 2: " << (groupMgr.isGroupLeader(2, ai) ? "true" : "false") << std::endl;
					std::cout << " \\- group leader 3: " << (groupMgr.isGroupLeader(3, ai) ? "true" : "false") << std::endl;
					std::cout << " \\- group 1: " << (groupMgr.isInGroup(1, ai) ? "true" : "false") << std::endl;
					std::cout << " \\- group 2: " << (groupMgr.isInGroup(2, ai) ? "true" : "false") << std::endl;
					std::cout << " \\- group 3: " << (groupMgr.isInGroup(3, ai) ? "true" : "false") << std::endl;
					std::cout << " \\- orientation: " << chr->getOrientation() << std::endl;
					std::cout << " \\- attributes:" << std::endl;
					const ai::CharacterAttributes& attributes = chr->getAttributes();
					for (ai::CharacterAttributes::const_iterator attribIter = attributes.begin(); attribIter != attributes.end(); ++attribIter) {
						std::cout << "  \\- " << attribIter->first << ": \"" << attribIter->second << "\"" << std::endl;
					}
					++count;
				};
				zone.execute(func);
				std::cout << " - sum: " << count << " entities" << std::endl;
			}
		} else if (c == "t") {
			autospawn = !autospawn;
			std::cout << "automatic respawn: " << (autospawn ? "true" : "false") << std::endl;
		} else if (c == "r") {
			std::vector<std::string> trees;
			loader.getTrees(trees);

			for (ai::example::GameMap* map : maps) {
				const ai::AIPtr& rnd = map->getRandomEntity();
				if (rnd) {
					if (!map->remove(rnd)) {
						std::cout << "failed to remove " << rnd->getId() << " from map " << map->getName() << std::endl;
					} else {
						std::cout << "removed " << rnd->getId() << " from map " << map->getName() << std::endl;
					}
				}

				auto randomIter = ai::randomElement(trees.begin(), trees.end());
				const ai::TreeNodePtr& root = loader.load(*randomIter);

				ai::ICharacterPtr chr(new ai::example::GameEntity(id++, map));
				ai::AIPtr ai(new ai::AI(root));
				ai->setCharacter(chr);
				const ai::GroupId groupId = ai::random(1, 3);
				map->addEntity(ai, groupId);
				std::cout << "spawned " << ai->getId() << " on map " << map->getName() << std::endl;
			}
		} else if (c == "reload") {
			if (!load(filename))
				continue;
			auto func = [&] (const ai::AIPtr& ai) {
				const std::string& name = ai->getBehaviour()->getName();
				const ai::TreeNodePtr& newRoot = loader.load(name);
				if (!newRoot)
					return;
				ai->setBehaviour(newRoot);
			};
			for (ai::example::GameMap* map : maps) {
				map->getZone().executeParallel(func);
			}
			std::cout << "reloaded the behaviour trees" << std::endl;
		} else {
			std::cout << "q      - quit" << std::endl;
			std::cout << "name   - change name of the bt to use" << std::endl;
			std::cout << "r      - respawn" << std::endl;
			std::cout << "g      - group info" << std::endl;
			std::cout << "t      - trigger automatic respawn" << std::endl;
			std::cout << "d      - detail" << std::endl;
			std::cout << "reload - reload the behaviour tree from file" << std::endl;
		}
	}
	shutdownThreads = true;
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
	int seed = std::stoi(getOptParam(b, e, "-seed", "-1"));
	const int mapAmount = std::stoi(getOptParam(b, e, "-maps", "4"));
	const int amount = std::stoi(getOptParam(b, e, "-amount", "10"));
	const short port = static_cast<short>(std::stoi(getOptParam(b, e, "-port", "12345")));
	const std::string filename = getOptParam(b, e, "-file");
	const std::string interface = getOptParam(b, e, "-interface", "0.0.0.0");

#ifdef AI_PROFILER
	const std::string profilerOutput = getOptParam(b, e, "-profilerOutput", "simpleai-run.prof");
	ProfilerStart(profilerOutput.c_str());
#endif

	if (seed == -1) {
		seed = 1;
	}
	ai::randomSeed(seed);

	if (!load(filename)) {
		return EXIT_FAILURE;
	}

	std::cout << "successfully loaded the behaviour trees" << std::endl;
	std::cout << "now run this behaviour tree with " << amount << " entities on each map" << std::endl;
	std::cout << "spawn " << mapAmount << " maps with seed " << seed << std::endl;
	std::cout << "automatic respawn: " << (autospawn ? "true" : "false") << std::endl;

	ai::Server server(loader, port, interface);
	if (!server.start()) {
		std::cerr << "Could not start the server on port " << port << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Started server on " << interface << ":" << port << std::endl;

	std::vector<ai::example::GameMap*> maps;
	for (int i = 1; i <= mapAmount; ++i) {
		maps.push_back(createMap(amount, server, "map-" + std::to_string(i)));
	}

	{
		ai::ThreadScheduler threadScheduler;
		int i = 0;
		for (ai::example::GameMap* map : maps) {
			threadScheduler.scheduleAtFixedRate(std::chrono::milliseconds(i++ * 50), std::chrono::milliseconds(250), [=] () {map->update(250u);});
		}
		for (ai::example::GameMap* map : maps) {
			threadScheduler.scheduleAtFixedRate(std::chrono::milliseconds(800), std::chrono::milliseconds(15000), runDespawnSpawn, map);
		}
		threadScheduler.scheduleAtFixedRate(std::chrono::milliseconds(10), std::chrono::milliseconds(500), [&] () {server.update(500u);});

		handleInput(filename, maps);
	}
#ifdef AI_PROFILER
	ProfilerStop();
#endif
	return EXIT_SUCCESS;
}
