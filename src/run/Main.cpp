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
		const ai::TreeNodePtr& root = loader.load(*randomIter);
		const ai::ICharacterPtr& e = std::make_shared<ai::example::GameEntity>(id++, map);
		const ai::AIPtr& ai = std::make_shared<ai::AI>(root);
		ai->setCharacter(e);
		const ai::GroupId groupId = ai::random(1, 3);
		map->addEntity(ai, groupId);
	}

	map->initializeAggro();
	ai_log("map created %s (%i npcs)", name.c_str(), amount);

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
	const ai::TreeNodePtr& root = loader.load(*randomIter);
	const ai::ICharacterPtr& e = std::make_shared<ai::example::GameEntity>(id++, map);
	const ai::AIPtr& ai = std::make_shared<ai::AI>(root);
	ai->setCharacter(e);
	const ai::GroupId groupId = ai::random(1, 3);
	map->addEntity(ai, groupId);
}

static bool load(const std::string filename) {
	std::ifstream btStream(filename);
	if (!btStream) {
		ai_log_error("could not load %s", filename.c_str());
		return false;
	}

	std::string str((std::istreambuf_iterator<char>(btStream)), std::istreambuf_iterator<char>());
	if (!loader.init(str)) {
		ai_log_error("could not load the tree: %s", loader.getError().c_str());
		return false;
	}
	return true;
}

static void handleInput(const std::string& filename, const std::vector<ai::example::GameMap*>& maps) {
	ai_log("hit q to quit or h for help");
	for (;;) {
		std::string c;
		std::getline(std::cin, c);

		if (c == "q") {
			ai_log("quitting");
			break;
		} else if (c == "g") {
			for (ai::example::GameMap* map : maps) {
				ai_log("%s", map->getName().c_str());
				const ai::Zone& zone = map->getZone();
				ai_log("groups: ");
				const ai::GroupMgr& groupMgr = zone.getGroupMgr();
				for (int groupId = 1; groupId <= 3; ++groupId) {
					const ai::AIPtr& leader = groupMgr.getLeader(groupId);
					if (!leader) {
						continue;
					}
					const glm::vec3& pos = groupMgr.getPosition(groupId);
					const glm::vec3& leaderPos = leader->getCharacter()->getPosition();
					ai_log(" \\- group %i: %f:%f:%f - %f:%f:%f", groupId, pos.x, pos.y, pos.z, leaderPos.x, leaderPos.y, leaderPos.z);
				}
			}
		} else if (c == "d") {
			for (ai::example::GameMap* map : maps) {
				ai_log("%s", map->getName().c_str());
				const ai::Zone& zone = map->getZone();
				int count = 0;
				auto func = [&] (const ai::AIPtr& ai) {
					const ai::ICharacterPtr& chr = ai->getCharacter();
					const ai::GroupMgr& groupMgr = zone.getGroupMgr();
					ai_log("id: %i", chr->getId());
					ai_log(" \\- pos: %s", ai::Str::toString(chr->getPosition()).c_str());
					ai_log(" \\- speed: %f", chr->getSpeed());
					ai_log(" \\- group leader 1: %s", (groupMgr.isGroupLeader(1, ai) ? "true" : "false"));
					ai_log(" \\- group leader 2: %s", (groupMgr.isGroupLeader(2, ai) ? "true" : "false"));
					ai_log(" \\- group leader 3: %s", (groupMgr.isGroupLeader(3, ai) ? "true" : "false"));
					ai_log(" \\- group 1: %s", (groupMgr.isInGroup(1, ai) ? "true" : "false"));
					ai_log(" \\- group 2: %s", (groupMgr.isInGroup(2, ai) ? "true" : "false"));
					ai_log(" \\- group 3: %s", (groupMgr.isInGroup(3, ai) ? "true" : "false"));
					ai_log(" \\- orientation: %f", chr->getOrientation());
					ai_log(" \\- attributes:");
					const ai::CharacterAttributes& attributes = chr->getAttributes();
					for (const auto& entry : attributes) {
						ai_log("  \\- %s: \"%s\"", entry.first.c_str(), entry.second.c_str());
					}
					++count;
				};
				zone.execute(func);
				ai_log(" - sum: %i entities", count);
			}
		} else if (c == "t") {
			autospawn = !autospawn;
			ai_log("automatic respawn: %s", (autospawn ? "true" : "false"));
		} else if (c == "r") {
			std::vector<std::string> trees;
			loader.getTrees(trees);

			for (ai::example::GameMap* map : maps) {
				const ai::AIPtr& rnd = map->getRandomEntity();
				if (rnd) {
					if (!map->remove(rnd)) {
						ai_log("failed to remove %i from map %s", rnd->getId(), map->getName().c_str());
					} else {
						ai_log("removed %i from map %s", rnd->getId(), map->getName().c_str());
					}
				}

				auto randomIter = ai::randomElement(trees.begin(), trees.end());
				const ai::TreeNodePtr& root = loader.load(*randomIter);
				const ai::ICharacterPtr& e = std::make_shared<ai::example::GameEntity>(id++, map);
				const ai::AIPtr& ai = std::make_shared<ai::AI>(root);
				ai->setCharacter(e);
				const ai::GroupId groupId = ai::random(1, 3);
				map->addEntity(ai, groupId);
				ai_log("spawned %i on map %s", rnd->getId(), map->getName().c_str());
			}
		} else if (c == "reload") {
			if (!load(filename))
				continue;
			auto func = [&] (const ai::AIPtr& ai) {
				const std::string& name = ai->getBehaviour()->getName();
				const ai::TreeNodePtr& newRoot = loader.load(name);
				if (!newRoot) {
					return;
				}
				ai->setBehaviour(newRoot);
			};
			for (ai::example::GameMap* map : maps) {
				map->getZone().executeParallel(func);
			}
			ai_log("reloaded the behaviour trees");
		} else {
			ai_log("q      - quit");
			ai_log("name   - change name of the bt to use");
			ai_log("r      - respawn");
			ai_log("g      - group info");
			ai_log("t      - trigger automatic respawn");
			ai_log("d      - detail");
			ai_log("reload - reload the behaviour tree from file");
		}
	}
	shutdownThreads = true;
}

int main(int argc, char **argv) {
	char **b = argv;
	char **e = argv + argc;
	if (argc <= 1 || optExists(b, e, "-h") || optExists(b, e, "-help") || !optExists(b, e, "-file")) {
		ai_log_error("usage: simpleai-run -file behaviourtree.lua [options]");
		ai_log_error("Valid options are (default values are given here):");
		ai_log_error("-amount 10            - how many entities are spawned on each map");
		ai_log_error("-maps 4               - how many maps should get spawned");
		ai_log_error("-autospawn true|false - automatic respawn (despawn random and respawn) of entities");
		ai_log_error("-seed 1               - use a fixed seed for all the random actions");
		ai_log_error("-help -h              - show this help screen");
		ai_log_error("Network related options");
		ai_log_error("-interface 0.0.0.0    - the interface the server will listen on");
		ai_log_error("-port 12345           - the port of the server to listen on");
#ifdef AI_PROFILER
		ai_log_error("-profilerOutput       - google profiler output file");
#endif
		return EXIT_FAILURE;
	}

	autospawn = getOptParam(b, e, "-autospawn", "true") == "true";
	int seed = std::stoi(getOptParam(b, e, "-seed", "-1"));
	const int mapAmount = std::stoi(getOptParam(b, e, "-maps", "4"));
	const int amount = std::stoi(getOptParam(b, e, "-amount", "10"));
	const short port = static_cast<short>(std::stoi(getOptParam(b, e, "-port", "10001")));
	const std::string& filename = getOptParam(b, e, "-file");
	const std::string& interface = getOptParam(b, e, "-interface", "0.0.0.0");

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

	ai_log("successfully loaded the behaviour trees");
	ai_log("now run this behaviour tree with %i entities on each map", amount);
	ai_log("spawn %i maps with seed %i", mapAmount, seed);
	ai_log("automatic respawn: %s", (autospawn ? "true" : "false"));

	ai::Server server(loader, port, interface);
	if (!server.start()) {
		ai_log_error("Could not start the server on port %i", port);
		return EXIT_FAILURE;
	}

	ai_log("Started server on %s:%i", interface.c_str(), port);

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

	for (ai::example::GameMap* map : maps) {
		delete map;
	}

#ifdef AI_PROFILER
	ProfilerStop();
#endif
	return EXIT_SUCCESS;
}
