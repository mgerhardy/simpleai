#include "NodeTreeItem.h"
#include "AIDebugger.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QFont>

namespace ai {
namespace debug {

namespace {
const qreal padding = 3;
const qreal horizontalSpacing = 40;
const qreal verticalSpacing = 10;
const qreal width = 130;
const qreal height = 50;
const qreal fontSize = 10;
const qreal conditionIndent = 3;
const QColor backgroundColor = QColor::fromRgb(32, 32, 32, 64);
const QColor activeBackgroundColor = QColor::fromRgb(255, 0, 0, 128);
const QFont font("Times", fontSize);
}

// TODO: items might still overlap
NodeTreeItem::NodeTreeItem (int maxChild, int child, const AIStateNode& node, NodeTreeItem* parent) :
		QGraphicsItem(), _node(node), _parent(parent) {
	const size_t size = _parent == nullptr ? 1 : _parent->_node.getChildren().size();
	const qreal maxHeight = maxChild * height;
	const qreal half = maxHeight / 2.0;
	qreal y;
	if (size <= 1) {
		y = 0.0;
	} else {
		const qreal boxHeight = height + verticalSpacing;
		const qreal parentCenter = height / 2.0;
		const qreal startY = ((size * height) + (size - 1) * verticalSpacing) / -2.0;
		y = parentCenter + startY + child * boxHeight;
	}
	if (_parent != nullptr) {
		setPos(_parent->pos() + QPointF(width + horizontalSpacing, y));
	} else {
		y = half - height / 2.0;
		setPos(0.0, y);
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
	const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
	const QRectF bounding = boundingRect();
	const bool active = _node.getState();
	QBrush b = painter->brush();
	if (active)
		painter->setBrush(activeBackgroundColor);
	else
		painter->setBrush(backgroundColor);
	painter->drawRect(0.0, 0.0, width, height);

	const std::vector<AIStateNode>& c = _node.getChildren();
	if (!c.empty()) {
		const qreal verticalLine = width + horizontalSpacing / 2.0;
		// horizontal line
		painter->drawLine(width, height / 2.0, verticalLine, height / 2.0);
		const int size = c.size();
		const qreal boxHeight = height + verticalSpacing;
		const qreal parentCenter = height / 2.0;
		const qreal startY = ((size * height) + (size - 1) * verticalSpacing) / -2.0;
		const qreal verticalLineTop = parentCenter + startY + height / 2.0;
		const qreal verticalLineBottom = parentCenter - startY - height / 2.0;
		// vertical line
		painter->drawLine(verticalLine, verticalLineTop, verticalLine, verticalLineBottom);
		// upper horizontal line
		const qreal horizontalLine = verticalLine + horizontalSpacing / 2.0;
		painter->drawLine(verticalLine, verticalLineTop, horizontalLine, verticalLineTop);
		for (int i = 1; i < size - 1; ++i) {
			const qreal verticalLineToChildrenY = verticalLineTop + boxHeight * i;
			painter->drawLine(verticalLine, verticalLineToChildrenY, horizontalLine, verticalLineToChildrenY);
		}
		// upper horizontal line
		painter->drawLine(verticalLine, verticalLineBottom, horizontalLine, verticalLineBottom);
	}

	painter->setBrush(b);

	if (lod < 0.4)
		return;

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
}

}
}
