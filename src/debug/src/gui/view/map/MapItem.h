#pragma once

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <server/AIStubTypes.h>

namespace ai {
namespace debug {

class AIDebugger;

class MapItem: public QGraphicsItem {
private:
	const AIStateWorld& _state;
	AIDebugger& _aiDebugger;
public:
	MapItem(const AIStateWorld& state, AIDebugger& aiDebugger);
	virtual ~MapItem();

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	QPointF worldToMap(const AIPosition& position) const;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

}
}
