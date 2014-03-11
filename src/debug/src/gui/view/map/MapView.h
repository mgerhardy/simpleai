#pragma once

#include <QGraphicsView>
#include <AI.h>

namespace ai {
namespace debug {

class AIDebugger;
class MapScene;

class MapView: public QGraphicsView {
	Q_OBJECT
private:
	MapScene *_scene;
	AIDebugger& _debugger;
public:
	MapView(AIDebugger& debugger);
	virtual ~MapView();

	void updateMapView();
};

}
}
