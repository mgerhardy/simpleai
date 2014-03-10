#include "AIDebugger.h"

#include <cstdlib>

int main(int argc, char **argv) {
	ai::debug::AIDebugger debugger(argc, argv);
	return debugger.run();
}
