#include "NodeTreeItem.h"
#include "AIDebugger.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QFont>

namespace ai {
namespace debug {

namespace {
const int padding = 3;
const int width = 100;
const int height = 50;
const int fontSize = 10;
const int conditionIndent = 3;
const QColor backgroundColor = QColor::fromRgb(32, 32, 32, 64);
const QFont font("Times", fontSize);
}

NodeTreeItem::NodeTreeItem (const AIStateNode& node, NodeTreeItem* parent) :
		QGraphicsItem(), _node(node), _parent(parent) {
	if (_parent != nullptr) {
		setPos(_parent->pos() + QPointF(width, 0.0));
	}
}

NodeTreeItem::~NodeTreeItem () {
}

QRectF NodeTreeItem::boundingRect () const {
	return QRectF(0, 0, width, height);
}

void NodeTreeItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);
	const QRectF bounding = boundingRect();
	QFontMetrics fontMetrics(font, painter->device());
	const int lineGap = fontMetrics.lineSpacing();
	painter->setFont(font);
	painter->save();
	QRect rect(bounding.x() + padding, bounding.y() + padding, bounding.width() - 2 * padding,
			bounding.height() - 2 * padding);
	painter->drawText(rect, QString::fromStdString(_node.getName()));
	rect.setY(rect.y() + fontSize + lineGap);
	rect.setX(rect.x() + conditionIndent);
	painter->drawText(rect, QString::fromStdString(_node.getCondition()));
	painter->restore();

	QBrush b = painter->brush();
	painter->setBrush(backgroundColor);
	painter->drawRect(0, 0, width, height);
	painter->setBrush(b);
}

}
}
