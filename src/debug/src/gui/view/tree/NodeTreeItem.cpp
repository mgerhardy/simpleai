#include "NodeTreeItem.h"
#include "AIDebugger.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QFont>

namespace ai {
namespace debug {

namespace {
const qreal padding = 1;
const qreal fontSize = 10;
const QColor backgroundColor = QColor::fromRgb(32, 32, 32, 64);
const QColor activeBackgroundColor = QColor::fromRgb(255, 0, 0, 128);
const QFont font("Times", fontSize);
}

NodeTreeItem::NodeTreeItem (const AIStateNode& node, NodeTreeItem* parent, int height, int horizontalSpacing, int verticalSpacing) :
		QGraphicsItem(), _node(node), _parent(parent), _height(height), _horizontalSpacing(
				horizontalSpacing), _verticalSpacing(verticalSpacing) {
	_condition = QString::fromStdString(_node.getCondition());
	_name = QString::fromStdString(_node.getName());
	QFontMetrics fontMetrics(font);
	_width = std::max(130, std::max(fontMetrics.width(_name), fontMetrics.width(_condition)));
	_lineHeight = fontMetrics.lineSpacing();
}

NodeTreeItem::~NodeTreeItem () {
}

QRectF NodeTreeItem::boundingRect () const {
	return QRectF(0, 0, _width, _height);
}

void NodeTreeItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);
	const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
	const bool active = _node.isActive();
	QBrush b = painter->brush();
	if (active)
		painter->setBrush(activeBackgroundColor);
	else
		painter->setBrush(backgroundColor);
	painter->drawRect(0.0, 0.0, _width, _height);

	const std::vector<AIStateNode>& c = _node.getChildren();
	if (!c.empty()) {
		const qreal verticalLine = _width + _horizontalSpacing / 2.0;
		// horizontal line
		painter->drawLine(_width, _height / 2.0, verticalLine, _height / 2.0);
		const int size = c.size();
		const qreal boxHeight = _height + _verticalSpacing;
		const qreal parentCenter = _height / 2.0;
		const qreal startY = ((size * _height) + (size - 1) * _verticalSpacing) / -2.0;
		const qreal verticalLineTop = parentCenter + startY + _height / 2.0;
		const qreal verticalLineBottom = parentCenter - startY - _height / 2.0;
		// vertical line
		painter->drawLine(verticalLine, verticalLineTop, verticalLine, verticalLineBottom);
		// upper horizontal line
		const qreal horizontalLine = verticalLine + _horizontalSpacing / 2.0;
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

	painter->setFont(font);
	painter->save();
	QRect rect(padding, padding, _width - 2 * padding, _height - 2 * padding);
	painter->drawText(rect, _name);
	rect.setY(rect.y() + _lineHeight);
	const TreeNodeStatus status = _node.getStatus();
	QString stateString;
	switch (status) {
	case UNKNOWN:
		stateString = "UNKNOWN";
		break;
	case CANNOTEXECUTE:
		stateString = "CANNOTEXECUTE";
		break;
	case RUNNING:
		stateString = "RUNNING";
		break;
	case FAILED:
		stateString = "FAILED";
		break;
	case FINISHED:
		stateString = "FINISHED";
		break;
	case EXCEPTION:
		stateString = "EXCEPTION";
		break;
	}
	painter->drawText(rect, stateString);
	rect.setY(rect.y() + _lineHeight);
	painter->drawText(rect, _condition);
	painter->restore();
}

}
}
