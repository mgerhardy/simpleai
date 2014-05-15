#pragma once

#include <QGraphicsView>

namespace ai {
namespace debug {

class IGraphicsView : public QGraphicsView {
public:
	IGraphicsView();
	virtual ~IGraphicsView();

	virtual void drawBackground(QPainter* painter, const QRectF& rect) override;
};

}
}
