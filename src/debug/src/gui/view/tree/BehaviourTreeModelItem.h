#pragma once

#include <QVariant>
#include <QIcon>
#include <SimpleAI.h>

namespace ai {
namespace debug {

class AINodeStaticResolver;

class BehaviourTreeModelItem {
private:
	AIStateNode* _node;
	const AIStateNodeStatic& _staticNodeData;
	bool _populated;
	int _rowCount;
	QList<BehaviourTreeModelItem*> _rows;
	BehaviourTreeModelItem* _parent;
	QIcon _icon;
public:
	BehaviourTreeModelItem(AIStateNode* node, AINodeStaticResolver& resolver, BehaviourTreeModelItem* parent = nullptr);
	virtual ~BehaviourTreeModelItem();

	QVariant data(int column);
	QString tooltip(int column);
	QIcon icon();

	inline int columnCount() const {
		return 4;
	}

	inline int childCount() const {
		return _rows.size();
	}

	int row();

	BehaviourTreeModelItem* child(int row);

	inline BehaviourTreeModelItem* parent() {
		return _parent;
	}

	inline AIStateNode* node() {
		return _node;
	}
};

}
}
