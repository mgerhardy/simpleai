#pragma once

#include <QGraphicsView>
#include <AI.h>
#include <server/AIStubTypes.h>

namespace ai {
namespace debug {

class AIDebugger;
class MapScene;

class MapView: public QGraphicsView {
	Q_OBJECT
private:
	MapScene *_scene;
public:
	MapView(AIDebugger& debugger);
	virtual ~MapView();

	void updateMapView();

private:
	AIDebugger& _debugger;
};

}
}
