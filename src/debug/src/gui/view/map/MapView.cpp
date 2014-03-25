#include "MapView.h"
#include "AIDebugger.h"
#include "MapScene.h"
#include "MapItem.h"

namespace ai {
namespace debug {

MapView::MapView(AIDebugger& debugger) :
		QGraphicsView(), _debugger(debugger) {
	setRenderHint(QPainter::Antialiasing, false);
	setDragMode(QGraphicsView::ScrollHandDrag);
	setOptimizationFlags(QGraphicsView::DontSavePainterState);
	setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setInteractive(false);

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
