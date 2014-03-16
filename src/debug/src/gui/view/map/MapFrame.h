#pragma once

#include <QFrame>

namespace ai {
namespace debug {

class AIDebugger;
class MapZoomWidget;
class MapView;

class MapFrame: public QFrame {
	Q_OBJECT
protected:
	MapView* _graphicsView;
	MapZoomWidget* _zoomWidget;
	AIDebugger& _debugger;
public:
	MapFrame(AIDebugger& debugger, QWidget* parent = nullptr);
	virtual ~MapFrame();

	void updateMapFrame();
	void zoomIn(int level);
	void zoomOut(int level);
};

}
}
