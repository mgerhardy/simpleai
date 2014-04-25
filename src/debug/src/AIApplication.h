#pragma once

#include <QApplication>
#include <QMainWindow>

namespace ai {
namespace debug {

class AIDebugger;
class AIDebuggerWidget;

/**
 * @brief Use this class to run the debugger as a stand-alone application.
 *
 * @note If you embed the debugger into an already exsting QT application, you of course
 * don't need this.
 */
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
