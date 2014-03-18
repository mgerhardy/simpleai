#include "MapView.h"
#include "AIDebugger.h"
#include "MapScene.h"
#include "MapItem.h"

namespace ai {
namespace debug {

MapView::MapView(AIDebugger& debugger) :
		QGraphicsView(), _debugger(debugger) {
	setRenderHints(QPainter::Antialiasing);
	setInteractive(false);
	setDragMode(QGraphicsView::ScrollHandDrag);

	_scene = new MapScene();
	setScene(_scene);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MapView::~MapView() {
	delete _scene;
}

void MapView::updateMapView() {
	_scene->clear();
	const AIDebugger::Entities& e = _debugger.getEntities();
	for (AIDebugger::EntitiesIter i = e.begin(); i != e.end(); ++i) {
		MapItem* item = new MapItem(*i, _debugger);
		_scene->addItem(item);
	}

	QWidget* viewPort = viewport();
	viewPort->update();
}

}
}
