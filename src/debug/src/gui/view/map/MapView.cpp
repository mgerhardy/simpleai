#include "MapView.h"
#include "AIDebugger.h"
#include "MapScene.h"
#include "MapItem.h"

namespace ai {
namespace debug {

MapView::MapView(AIDebugger& debugger) :
		QGraphicsView(), _debugger(debugger) {
	_scene = new MapScene();
	setScene(_scene);
}

MapView::~MapView() {
	delete _scene;
}

void MapView::updateMapView() {
	_scene->clear();
	const AIDebugger::Entities& e = _debugger.getEntities();
	const CharacterId& id = _debugger.getSelected();
	MapItem* centerOnItem = nullptr;
	for (AIDebugger::EntitiesIter i = e.begin(); i != e.end(); ++i) {
		MapItem* item = new MapItem(*i, _debugger);
		_scene->addItem(item);
		if (i->getId() == id) {
			centerOnItem = item;
		}
	}
	QWidget* viewPort = viewport();
	viewPort->update();
	if (centerOnItem != nullptr)
		ensureVisible(centerOnItem);
}

}
}
