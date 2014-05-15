#pragma once

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <server/AIStubTypes.h>

namespace ai {
namespace debug {

class AIDebugger;

/**
 * @brief Represents one entity in the world
 *
 * @note If you want to show additional details for an entity, see the @c MapView class
 */
class MapItem: public QGraphicsItem {
protected:
	const AIStateWorld& _state;
	AIDebugger& _aiDebugger;
	float _orientation;
	// the level of detail value that must not be underrun in
	// order to see the entity details rendered
	float _detailLod;
public:
	MapItem(QGraphicsItem* parent, const AIStateWorld& state, AIDebugger& aiDebugger);
	virtual ~MapItem();

	virtual QRectF boundingRect() const override;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	virtual QPointF worldToMap(const Vector3f& position) const;

protected:
	virtual void renderDetails(QPainter *painter, qreal lod);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

}
}
