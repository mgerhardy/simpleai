#include "MapItem.h"
#include "AIDebugger.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QFont>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <math.h>
#include <QVector2D>

namespace ai {
namespace debug {

MapItem::MapItem (QGraphicsItem* parent, const AIStateWorld& state, AIDebugger& aiDebugger) :
		QGraphicsItemGroup(parent), _state(state), _aiDebugger(aiDebugger) {
	const bool selected = _aiDebugger.isSelected(state);
	setFlag(QGraphicsItem::ItemIsSelectable);

	const qreal size = 30.0;
	const QColor& color = selected ? QColor::fromRgb(200, 200, 0) : QColor::fromRgb(255, 0, 0);
	QGraphicsEllipseItem *body = new QGraphicsEllipseItem(0.0, 0.0, size, size);
	body->setBrush(color);
	addToGroup(body);

	QVector2D end(::sinf(state.getOrientation()), ::cosf(state.getOrientation()));
	end.normalize();
	const qreal center = size / 2.0;
	QGraphicsLineItem *direction = new QGraphicsLineItem(center, center, center + center * end.x(), center + center * end.y());
	addToGroup(direction);

	direction->setSelected(selected);
	body->setSelected(selected);
}

MapItem::~MapItem () {
}

void MapItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsItem::mouseDoubleClickEvent(event);
	_aiDebugger.select(_state);
	update();
}

}
}
