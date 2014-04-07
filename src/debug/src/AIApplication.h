#pragma once

#include <QApplication>
#include <QMainWindow>

namespace ai {
namespace debug {

class AIDebugger;
class AIDebuggerWidget;

class AIApplication: public QApplication {
protected:
	AIDebugger* _debugger;
	AIDebuggerWidget* _widget;
	QMainWindow _window;
public:
	AIApplication(int argc, char** argv);
	virtual ~AIApplication();
};

}
}
