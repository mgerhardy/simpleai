#include "AIApplication.h"
#include "AIDebugger.h"
#include "AIDebuggerWidget.h"

int main(int argc, char **argv) {
	ai::debug::AIApplication app(argc, argv);
	return app.exec();
}
