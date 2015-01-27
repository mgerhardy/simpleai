#pragma once

#include <QVariant>
#include <SimpleAI.h>

namespace ai {
namespace debug {

class BehaviourTreeModelItem {
private:
	AIStateNode* _node;
	bool _populated;
	int _rowCount;
	QList<BehaviourTreeModelItem*> _rows;
	BehaviourTreeModelItem* _parent;
public:
	BehaviourTreeModelItem(AIStateNode* node, BehaviourTreeModelItem* parent = nullptr);
	virtual ~BehaviourTreeModelItem();

	QVariant data(int column);

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
};

}
}
