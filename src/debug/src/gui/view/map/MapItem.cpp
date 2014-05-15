#include "MapItem.h"
#include "AIDebugger.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QFont>

namespace ai {
namespace debug {

MapItem::MapItem (QGraphicsItem* parent, const AIStateWorld& state, AIDebugger& aiDebugger) :
		QGraphicsItem(parent), _state(state), _aiDebugger(aiDebugger), _detailLod(0.4f) {
	setFlag(QGraphicsItem::ItemIsSelectable);
	setPos(worldToMap(state.getPosition()));
	_orientation = state.getOrientation();
	setZValue((qreal)state.getPosition().z);
}

MapItem::~MapItem () {
}

QPointF MapItem::worldToMap(const Vector3f& position) const {
	return QPointF((qreal)position.x, (qreal)position.z);
}

QRectF MapItem::boundingRect() const {
	return QRectF(-30, -30, 30, 30);
}

void MapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->setClipRect(option->exposedRect);
	const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

	QBrush b = painter->brush();
	const bool selected = _aiDebugger.isSelected(_state);
	QColor color = selected ? QColor::fromRgb(200, 200, 0) : QColor::fromRgb(255, 0, 0);
	painter->setBrush(color);
	painter->drawEllipse(pos(), 10.0, 10.0);
	painter->setBrush(b);

	renderDetails(painter, lod);
}

void MapItem::renderDetails(QPainter* painter, qreal lod) {
	if (lod < _detailLod)
		return;

	if (_orientation >= 0.0) {
		QPoint endPos;
		QPointF direction(sin(_orientation), cos(_orientation));
		const float scaleV = 30.0f;
		endPos.setX(pos().x() + scaleV * direction.x());
		endPos.setY(pos().y() + scaleV * direction.y());
		painter->drawLine(pos(), endPos);
	}
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
