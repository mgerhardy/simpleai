#pragma once

#include <QFrame>
#include <QGraphicsView>

namespace ai {
namespace debug {

class MapZoomWidget;

class MapFrame: public QFrame {
	Q_OBJECT
protected:
	MapZoomWidget* _zoomWidget;
	QGraphicsView* _graphicsView;
public:
	MapFrame(QGraphicsView* graphicsView, QWidget* parent = nullptr);
	virtual ~MapFrame();

	void zoomIn(int level);
	void zoomOut(int level);
};

}
}
