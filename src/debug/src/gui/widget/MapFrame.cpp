#include "MapFrame.h"
#include "MapScene.h"
#include "MapView.h"
#include "MapZoomWidget.h"
#include "AIDebugger.h"

namespace ai {
namespace debug {

MapFrame::MapFrame(QGraphicsView* graphicsView, QWidget* parent) :
		QFrame(parent), _zoomWidget(nullptr), _graphicsView(graphicsView) {
	setFrameStyle(Sunken | StyledPanel);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	_graphicsView->setRenderHint(QPainter::Antialiasing, false);
	_graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
	_graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	_graphicsView->setInteractive(false);
	_zoomWidget = new MapZoomWidget(*_graphicsView);

	QGridLayout* topLayout = new QGridLayout;
	topLayout->addWidget(_graphicsView, 1, 0);
	topLayout->addWidget(_zoomWidget, 1, 1);
	setLayout(topLayout);
}

MapFrame::~MapFrame() {
	delete _graphicsView;
	delete _zoomWidget;
}

void MapFrame::zoomIn(int level) {
	_zoomWidget->zoomIn(level);
}

void MapFrame::zoomOut(int level) {
	_zoomWidget->zoomOut(level);
}

}
}
