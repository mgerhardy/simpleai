#include "MapItem.h"
#include "AIDebugger.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QFont>

namespace ai {
namespace debug {

MapItem::MapItem (const AIStateWorld& state, AIDebugger& aiDebugger) :
		QGraphicsItem(), _state(state), _aiDebugger(aiDebugger) {
	setFlags(ItemIsSelectable);
	setPos(worldToMap(state.getPosition()));
	_orientation = state.getOrientation();
	setZValue((qreal)state.getPosition().z());
}

MapItem::~MapItem () {
}

QPointF MapItem::worldToMap(const Vector3f& position) const {
	// TODO:
	return QPointF((qreal)position.x(), (qreal)position.y());
}

QRectF MapItem::boundingRect() const {
	return QRectF(0, 0, 10, 10);
}

void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);
	const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

	QBrush b = painter->brush();
	const bool selected = _aiDebugger.isSelected(_state);
	QColor color = selected ? QColor::fromRgb(200, 200, 0) : QColor::fromRgb(255, 0, 0);
	painter->setBrush(color);
	painter->drawEllipse(pos(), 10.0, 10.0);
	painter->setBrush(b);

	renderDetails(painter, lod);
}

void MapItem::renderDetails(QPainter* /*painter*/, qreal /*lod*/) {
}

void MapItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsItem::mousePressEvent(event);
	_aiDebugger.select(_state);
	update();
}

void MapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsItem::mouseMoveEvent(event);
	update();
}

}
}
