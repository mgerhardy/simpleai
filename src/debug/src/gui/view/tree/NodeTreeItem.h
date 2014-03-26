#pragma once

#include <QColor>
#include <QPainter>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>

#include <server/AIStubTypes.h>

namespace ai {
namespace debug {

class AIDebugger;

class NodeTreeItem : public QGraphicsItem {
protected:
	const AIStateNode& _node;
	NodeTreeItem* _parent;
	QString _condition;
	QString _name;
	int _width;
	int _height;
	int _lineHeight;

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

public:
	NodeTreeItem (int maxChild, int child, const AIStateNode& node, NodeTreeItem* parent);
	virtual ~NodeTreeItem ();
};

}
}
