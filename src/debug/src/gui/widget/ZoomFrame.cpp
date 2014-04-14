#include "ZoomFrame.h"
#include "MapScene.h"
#include "MapView.h"
#include "ZoomWidget.h"
#include "AIDebugger.h"

namespace ai {
namespace debug {

ZoomFrame::ZoomFrame(QGraphicsView* graphicsView, QWidget* parent) :
		QFrame(parent), _zoomWidget(nullptr), _graphicsView(graphicsView) {
	setFrameStyle(Sunken | StyledPanel);
	_graphicsView->setRenderHint(QPainter::Antialiasing, false);
	_graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
	//_graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	_graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
	_graphicsView->setCacheMode(QGraphicsView::CacheBackground);
	//_graphicsView->setInteractive(true);
	_zoomWidget = new ZoomWidget(*_graphicsView);

	QGridLayout* topLayout = new QGridLayout;
	topLayout->addWidget(_graphicsView, 1, 0);
	topLayout->addWidget(_zoomWidget, 1, 1);
	setLayout(topLayout);
}

ZoomFrame::~ZoomFrame() {
	delete _graphicsView;
	delete _zoomWidget;
}

void ZoomFrame::zoomIn(int level) {
	_zoomWidget->zoomIn(level);
}

void ZoomFrame::zoomOut(int level) {
	_zoomWidget->zoomOut(level);
}

}
}
