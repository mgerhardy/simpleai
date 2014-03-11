#pragma once

#include <QColor>
#include <QPainter>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>

#include <server/AIStubTypes.h>

namespace ai {
namespace debug {

class AIDebugger;

class NodeTreeItem : public QGraphicsItem {
private:
	const AIStateNode& _node;
	NodeTreeItem* _parent;

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

public:
	NodeTreeItem (const AIStateNode& node, NodeTreeItem* parent);
	virtual ~NodeTreeItem ();
};

}
}
