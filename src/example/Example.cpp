#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstdlib>
#include <AI.h>
#include <unistd.h>
#include <tree/loaders/lua/LUATreeLoader.h>
#include <server/Server.h>
#include "Pathfinder.h"
#include "actions/ExampleTask.h"
#include "actions/Move.h"
#include "GameEntity.h"

int main(int argc, char **argv) {
	if (argc <= 1) {
		std::cerr << "usage: example behaviourtree.lua [amount]" << std::endl;
		return EXIT_FAILURE;
	}

	ai::AIRegistry registry;

	registry.registerNodeFactory("Move", ai::example::Move::FACTORY);
	registry.registerNodeFactory("ExampleTask", ai::example::ExampleTask::FACTORY);

	ai::LUATreeLoader loader(registry);
	const std::string name = "walk";

	const std::string filename = argv[1];
	std::ifstream t(filename.c_str());
	if (!t) {
		std::cerr << "could not load " << filename << std::endl;
		return EXIT_FAILURE;
	}

	int amount = 100;
	if (argc >= 3) {
		amount = atoi(argv[2]);
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

	const short port = 12345;
	ai::Server server(port);
	if (!server.start())
		std::cerr << "Could not start the server" << std::endl;
	else
		std::cout << "Started the server and accept connections on port " << port << std::endl;

	ai::example::Pathfinder pathFinder(600, 600);

	std::vector<GameEntity*> entities;
	for (int i = 0; i < amount; ++i) {
		entities.push_back(new GameEntity(i, root, pathFinder, server));
	}

	long frames = 10000;
	long frame = frames;
	const int fps = 20;
	const long microseconds = 1000000 / fps;
	for (;--frame;) {
		const uint32_t dt = 1;
		for (std::vector<GameEntity*>::iterator i = entities.begin(); i != entities.end(); ++i) {
			(*i)->update(dt);
		}
		if ((frame % 10) == 0)
			server.update(dt);
		std::cout << (frames - frame) << "/" << frames << "    \r" << std::flush;
		usleep(microseconds);
	}

	for (std::vector<GameEntity*>::iterator i = entities.begin(); i != entities.end(); ++i) {
		delete *i;
	}

	return 0;
}
