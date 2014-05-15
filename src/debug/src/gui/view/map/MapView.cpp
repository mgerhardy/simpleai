#include "MapView.h"
#include "AIDebugger.h"
#include "MapItem.h"

namespace ai {
namespace debug {

MapView::MapView(AIDebugger& debugger) :
		IGraphicsView(), _debugger(debugger) {
	_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
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
	for (AIDebugger::EntitiesIter i = e.begin(); i != e.end(); ++i) {
		MapItem* item = createMapItem(*i);
		_scene.addItem(item);
	}
}

}
}
