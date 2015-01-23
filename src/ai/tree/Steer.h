#pragma once

#include "tree/ITask.h"
#include "common/String.h"
#include "common/Types.h"
#include "common/Math.h"
#include "movement/Steering.h"

namespace ai {

class Steer: public ITask {
protected:
	movement::WeightedSteerings _weightedSteerings;
	const movement::Steerings _steerings;
public:
	Steer(const std::string& name, const std::string& parameters, const ConditionPtr& condition, const movement::Steerings& steerings) :
			ITask(name, parameters, condition), _steerings(steerings) {
		if (_parameters.empty()) {
			for (const SteeringPtr& s : _steerings) {
				_weightedSteerings.push_back(movement::WeightedData(s, 1.0f));
			}
		} else {
			std::vector<std::string> tokens;
			Str::splitString(_parameters, tokens, ",");
			ai_assert(tokens.size() == _steerings.size(), "weights doesn't match steerings methods count");
			const int tokenAmount = tokens.size();
			for (int i = 0; i < tokenAmount; ++i) {
				_weightedSteerings.push_back(movement::WeightedData(_steerings[i], ::atof(tokens[i].c_str())));
			}
		}
	}
	class Factory: public ISteerNodeFactory {
	public:
		TreeNodePtr create (const SteerNodeFactoryContext *ctx) const;
	};
	static Factory FACTORY;

	TreeNodeStatus doAction(AI& entity, long deltaMillis) override;
};

}
