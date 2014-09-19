#pragma once

#include <QGraphicsView>

namespace ai {
namespace debug {

class IGraphicsView : public QGraphicsView {
private:
	bool _renderGrid;
	bool _renderBackground;
public:
	IGraphicsView(bool renderGrid = false, bool renderBackground = false);
	virtual ~IGraphicsView();

	virtual void drawBackground(QPainter* painter, const QRectF& rect) override;
};

}
}
