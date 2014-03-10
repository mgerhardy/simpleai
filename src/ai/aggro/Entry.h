#pragma once

#include <common/Pointers.h>
#include <common/Compiler.h>

namespace ai {

class AI;

enum ReductionType {
	DISABLED, RATIO, VALUE,
};

class Entry {
protected:
	float _aggro;
	float _minAggro;
	float _reduceRatioSecond;
	float _reduceValueSecond;
	ReductionType _reduceType;
	AI& _ai;

	void reduceByRatio(float ratio);
	void reduceByValue(float value);

public:
	Entry(AI& ai) :
			_aggro(0.0f), _minAggro(0.0f), _reduceRatioSecond(0.0f), _reduceValueSecond(0.0f), _reduceType(DISABLED), _ai(ai) {
	}
	virtual ~Entry() {
	}

	float getAggro() const;
	void addAggro(float aggro);
	void setReduceByRatio(float reductionRatioPerSecond, float minimumAggro);
	void setReduceByValue(float reductionValuePerSecond);
	void reduceByTime(long millis);
	void resetAggro();

	AI& getEntity();

	bool operator <(Entry& other) const;
};

typedef SharedPtr<Entry> EntryPtr;

inline void Entry::addAggro(float aggro) {
	_aggro += aggro;
}

inline void Entry::setReduceByRatio(float reduceRatioSecond, float minAggro) {
	_reduceType = RATIO;
	_reduceRatioSecond = reduceRatioSecond;
	_minAggro = minAggro;
}

inline void Entry::setReduceByValue(float degValueSecond) {
	_reduceType = VALUE;
	_reduceValueSecond = degValueSecond;
}

inline void Entry::reduceByTime(long millis) {
	switch (_reduceType) {
	case RATIO:
		reduceByRatio((millis / 1000.0f) * _reduceRatioSecond);
		break;
	case VALUE:
		reduceByValue((millis / 1000.0f) * _reduceValueSecond);
		break;
	default:
		break;
	}
}

inline void Entry::reduceByRatio(float ratio) {
	_aggro *= (1.0f - ratio);
	if (_aggro < _minAggro) {
		_aggro = 0.0f;
	}
}

inline void Entry::reduceByValue(float value) {
	_aggro -= value;

	if (_aggro < 0.000001f) {
		_aggro = 0.0f;
	}
}

inline float Entry::getAggro() const {
	return _aggro;
}

inline void Entry::resetAggro() {
	_aggro = 0.0f;
}

inline bool Entry::operator <(Entry& other) const {
	return _aggro < other._aggro;
}

inline AI& Entry::getEntity() {
	return _ai;
}

}
