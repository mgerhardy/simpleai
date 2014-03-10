#include "MapView.h"
#include "AIDebugger.h"
#include "MapScene.h"

namespace ai {
namespace debug {

MapView::MapView(AIDebugger& debugger) :
		QGraphicsView(), _debugger(debugger) {
	setRenderHints(QPainter::Antialiasing);

	_scene = new MapScene();
	setScene(_scene);
}

MapView::~MapView() {
	delete _scene;
}

void MapView::updateMapView() {
	delete _scene;
	_scene = new MapScene();
	setScene(_scene);

	const AIDebugger::Entities& e = _debugger.getEntities();
	for (AIDebugger::EntitiesIter i = e.begin(); i != e.end(); ++i) {
		_scene->addRect(0, 0, 10, 10);
	}

	QWidget* viewPort = viewport();
	viewPort->update();
}

}
}
