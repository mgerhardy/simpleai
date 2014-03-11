#include "MapItem.h"
#include "AIDebugger.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QFont>

namespace ai {
namespace debug {

MapItem::MapItem (const AIStateTree& tree, AIDebugger& aiDebugger) :
		QGraphicsItem(), _tree(tree), _aiDebugger(aiDebugger) {
	setFlags(ItemIsSelectable);
	setPos(worldToMap(tree.getPosition()));
	setZValue((qreal)tree.getPosition().z());
}

MapItem::~MapItem () {
}

QPointF MapItem::worldToMap(const AIPosition& position) const {
	// TODO:
	return QPointF((qreal)position.x(), (qreal)position.y());
}

QRectF MapItem::boundingRect() const {
	return QRectF(0, 0, 10, 10);
}

void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);
	QBrush b = painter->brush();
	const bool selected = _aiDebugger.isSelected(_tree);
	QColor color = selected ? QColor::fromRgb(200, 0, 0) : QColor::fromRgb(255, 0, 0);
	painter->setBrush(color);
	painter->drawEllipse(pos(), 10.0, 10.0);
	painter->setBrush(b);
}

void MapItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsItem::mousePressEvent(event);
	_aiDebugger.select(_tree);
	update();
}

void MapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsItem::mouseMoveEvent(event);
	update();
}

}
}
