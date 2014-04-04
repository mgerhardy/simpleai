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
	int _horizontalSpacing;
	int _verticalSpacing;

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

public:
	NodeTreeItem (const AIStateNode& node, NodeTreeItem* parent, int height, int horizontalSpacing, int verticalSpacing);
	virtual ~NodeTreeItem ();

	inline NodeTreeItem* getParent() { return _parent; }
	inline const AIStateNode& getNode() { return _node; }
	inline int width() const { return _width; }
	inline int height() const { return _height; }
	inline int getSize() const { return _node.getChildren().size(); }
};

}
}
