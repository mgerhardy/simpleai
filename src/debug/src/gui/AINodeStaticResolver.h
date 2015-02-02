#pragma once

#include <QHash>

namespace ai {

class AIStateNodeStatic;

namespace debug {

class AINodeStaticResolver {
private:
	QHash<int32_t, AIStateNodeStatic*> _data;
public:
	AINodeStaticResolver();
	const AIStateNodeStatic& get(int32_t id) const;
};

}
}
