#include "MapView.h"
#include "AIDebugger.h"
#include "MapItem.h"

namespace ai {
namespace debug {

MapView::MapView(AIDebugger& debugger) :
		QGraphicsView(), _debugger(debugger) {
	setScene(&_scene);
}

MapView::~MapView() {
}

MapItem* MapView::createMapItem(const AIStateWorld& state) {
	return new MapItem(state, _debugger);
}

void MapView::updateMapView() {
	_scene.clear();
	const AIDebugger::Entities& e = _debugger.getEntities();
	const CharacterId& id = _debugger.getSelected();
	MapItem* centerOnItem = nullptr;
	for (AIDebugger::EntitiesIter i = e.begin(); i != e.end(); ++i) {
		MapItem* item = createMapItem(*i);
		_scene.addItem(item);
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
