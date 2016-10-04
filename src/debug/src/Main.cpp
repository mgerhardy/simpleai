/**
 * @file
 */
#include "AIApplication.h"

#ifdef AI_PROFILER
#include <google/profiler.h>
#endif

int main(int argc, char **argv) {
#ifdef AI_PROFILER
	ProfilerStart("simpleai-debugger.prof");
#endif

	ai::debug::AIApplication app(argc, argv);
	app.init();
	return app.exec();
}
