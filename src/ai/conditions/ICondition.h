/**
 * @file
 * @brief Condition related stuff
 *
 * @sa ICondition
 */

#pragma once

#include <string>
#include <vector>
#include <sstream>

#include <common/Compiler.h>
#include <common/Pointers.h>

#include <AIRegistry.h>

namespace ai {

class AI;

/**
 * @brief Macro to simplify the condition creation. Just give the class name of the condition as parameter.
 */
#define CONDITION_CLASS(ConditionName) \
	ConditionName(const std::string& parameters = "") : \
		ICondition(#ConditionName, parameters) { \
	} \
public: \
	virtual ~ConditionName() { \
	}

#define CONDITION_FACTORY \
public: \
	class Factory: public IConditionFactory { \
	public: \
		ConditionPtr create (const ConditionFactoryContext *ctx) const; \
	}; \
	static Factory FACTORY;

#define CONDITION_FACTORY_SINGLETON \
public: \
	class Factory: public IConditionFactory { \
		ConditionPtr create (const ConditionFactoryContext *ctx) const { \
			return get(); \
		} \
	}; \
	static Factory FACTORY;

#define CONDITION_FACTORY_IMPL(ConditionName) \
	ConditionPtr ConditionName::Factory::create(const ConditionFactoryContext *ctx) const { \
		return ConditionPtr(new ConditionName(ctx->parameters)); \
	} \
	ConditionName::Factory ConditionName::FACTORY;

/**
 * @brief Macro to create a singleton conditions for very easy conditions without a state.
 */
#define CONDITION_CLASS_SINGLETON(ConditionName) \
private: \
	struct Deleter { \
		void operator()(ConditionName* /* ptr */) { \
		} \
	}; \
	CONDITION_CLASS(ConditionName) \
public: \
	static ConditionPtr& get() { \
		static ConditionName _conditionInstance; \
		static ConditionPtr _instance(&_conditionInstance, Deleter()); \
		return _instance; \
	} \
	CONDITION_FACTORY_SINGLETON

class ICondition;
typedef SharedPtr<ICondition> ConditionPtr;
typedef std::vector<ConditionPtr> Conditions;

/**
 * A condition can be placed on a @c TreeNode to decide which node is going to get executed. In general they are stateless.
 * If they are not, it should explicitly get noted.
 */
class ICondition {
protected:
	static int _nextId;
	int _id;
	const std::string _name;
	const std::string _parameters;

	/**
	 * @brief Override this method to get a more detailed result in @c getNameWithConditions
	 *
	 * @param[out] s The string stream to write your details to
	 * @param[in,out] entity The entity that is used to evaluate a condition
	 * @sa getNameWithConditions
	 */
	virtual void getConditionNameWithValue(std::stringstream& /* s */, AI& /* entity */) {
	}
public:
	ICondition(const std::string& name, const std::string& parameters) :
			_id(_nextId++), _name(name), _parameters(parameters) {
	}

	virtual ~ICondition() {
	}

	/**
	 * @brief Checks whether the condition evaluates to @c true for the given @c entity.
	 * @param[in,out] entity The entity that is used to evaluate the condition
	 * @return @c true if the condition is fulfilled, @c false otherwise.
	 */
	virtual bool evaluate(AI& entity) = 0;

	/**
	 * @brief Returns the short name of the condition - without any related conditions or results.
	 */
	const std::string& getName() const;

	/**
	 * @brief Returns the full condition string with all related conditions and results of the evaluation method
	 * @param[in,out] entity The entity that is used to evaluate the condition
	 * @sa getConditionNameWithValue
	 */
	inline std::string getNameWithConditions(AI& entity) {
		std::stringstream s;
		s << getName();
		getConditionNameWithValue(s, entity);
		s << "[";
		s << (evaluate(entity) ? "1" : "0");
		s << "]";
		return s.str();
	}

	friend inline std::ostream& operator<<(std::ostream& stream, const ICondition& condition) {
		stream << condition._name;
		stream << "[";
		stream << condition._parameters;
		stream << "]";
		return stream;
	}
};

inline const std::string& ICondition::getName() const {
	return _name;
}


}
