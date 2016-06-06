#include "Server.h"
#include "AIStateMessage.h"
#include "AINamesMessage.h"
#include "AIStubTypes.h"
#include "AICharacterDetailsMessage.h"
#include "AICharacterStaticMessage.h"
#include "ProtocolHandlerRegistry.h"
#include "../conditions/ConditionParser.h"
#include "../tree/TreeNodeParser.h"

namespace ai {

Server::Server(AIRegistry& aiRegistry, short port, const std::string& hostname) :
		_aiRegistry(aiRegistry), _network(port, hostname), _selectedCharacterId(AI_NOTHING_SELECTED), _time(0L), _selectHandler(*this), _pauseHandler(*this), _resetHandler(*this), _stepHandler(*this), _changeHandler(
				*this), _addNodeHandler(*this), _deleteNodeHandler(*this), _updateNodeHandler(*this), _pause(false), _zone(nullptr) {
	_network.addListener(this);
	ProtocolHandlerRegistry& r = ai::ProtocolHandlerRegistry::get();
	r.registerHandler(ai::PROTO_SELECT, &_selectHandler);
	r.registerHandler(ai::PROTO_PAUSE, &_pauseHandler);
	r.registerHandler(ai::PROTO_RESET, &_resetHandler);
	r.registerHandler(ai::PROTO_STEP, &_stepHandler);
	r.registerHandler(ai::PROTO_PING, &_nopHandler);
	r.registerHandler(ai::PROTO_CHANGE, &_changeHandler);
	r.registerHandler(ai::PROTO_ADDNODE, &_addNodeHandler);
	r.registerHandler(ai::PROTO_DELETENODE, &_deleteNodeHandler);
	r.registerHandler(ai::PROTO_UPDATENODE, &_updateNodeHandler);
}

Server::~Server() {
	_network.removeListener(this);
}

bool Server::updateNode(const CharacterId& characterId, int32_t nodeId, const std::string& name, const std::string& type, const std::string& condition) {
	Zone* zone = _zone;
	if (zone == nullptr) {
		return false;
	}
	const AIPtr& ai = zone->getAI(characterId);
	const TreeNodePtr& node = ai->getBehaviour()->getId() == nodeId ? ai->getBehaviour() : ai->getBehaviour()->getChild(nodeId);
	if (!node) {
		return false;
	}
	ConditionParser conditionParser(_aiRegistry, condition);
	const ConditionPtr& conditionPtr = conditionParser.getCondition();
	if (!conditionPtr) {
		ai_log_error("Failed to parse the condition '%s'", condition.c_str());
		return false;
	}
	TreeNodeParser treeNodeParser(_aiRegistry, type);
	TreeNodePtr newNode = treeNodeParser.getTreeNode(name);
	if (!newNode) {
		ai_log_error("Failed to parse the node '%s'", type.c_str());
		return false;
	}
	newNode->setCondition(conditionPtr);
	for (auto& child : node->getChildren()) {
		newNode->addChild(child);
	}

	const TreeNodePtr& root = ai->getBehaviour();
	if (node == root) {
		ai->setBehaviour(newNode);
	} else {
		const TreeNodePtr& parent = root->getParent(root, nodeId);
		if (!parent) {
			ai_log_error("No parent for non-root node '%i'", nodeId);
			return false;
		}
		parent->replaceChild(nodeId, newNode);
	}

	Event event;
	event.type = EV_UPDATESTATICCHRDETAILS;
	event.data.updateStaticCharacterDetails = zone;
	enqueueEvent(event);
	return true;
}

bool Server::addNode(const CharacterId& characterId, int32_t parentNodeId, const std::string& name, const std::string& type, const std::string& condition) {
	Zone* zone = _zone;
	if (zone == nullptr) {
		return false;
	}
	const AIPtr& ai = zone->getAI(characterId);
	TreeNodePtr node = ai->getBehaviour();
	if (node->getId() != parentNodeId) {
		node = node->getChild(parentNodeId);
	}
	if (!node) {
		return false;
	}
	ConditionParser conditionParser(_aiRegistry, condition);
	const ConditionPtr& conditionPtr = conditionParser.getCondition();
	if (!conditionPtr) {
		ai_log_error("Failed to parse the condition '%s'", condition.c_str());
		return false;
	}
	TreeNodeParser treeNodeParser(_aiRegistry, type);
	TreeNodePtr newNode = treeNodeParser.getTreeNode(name);
	if (!newNode) {
		ai_log_error("Failed to parse the node '%s'", type.c_str());;
		return false;
	}
	newNode->setCondition(conditionPtr);
	if (!node->addChild(newNode)) {
		return false;
	}

	Event event;
	event.type = EV_UPDATESTATICCHRDETAILS;
	event.data.updateStaticCharacterDetails = zone;
	enqueueEvent(event);
	return true;
}

bool Server::deleteNode(const CharacterId& characterId, int32_t nodeId) {
	Zone* zone = _zone;
	if (zone == nullptr) {
		return false;
	}
	const AIPtr& ai = zone->getAI(characterId);
	// don't delete the root
	const TreeNodePtr& root = ai->getBehaviour();
	if (root->getId() == nodeId) {
		return false;
	}

	const TreeNodePtr& parent = root->getParent(root, nodeId);
	if (!parent) {
		ai_log_error("No parent for non-root node '%i'", nodeId);
		return false;
	}
	parent->replaceChild(nodeId, TreeNodePtr());
	Event event;
	event.type = EV_UPDATESTATICCHRDETAILS;
	event.data.updateStaticCharacterDetails = zone;
	enqueueEvent(event);
	return true;
}

void Server::enqueueEvent(const Event& event) {
	ScopedWriteLock scopedLock(_lock);
	_events.push_back(event);
}

void Server::step(int64_t stepMillis) {
	Event event;
	event.type = EV_STEP;
	event.data.stepMillis = stepMillis;
	enqueueEvent(event);
}

void Server::reset() {
	Zone* zone = _zone;
	if (zone == nullptr)
		return;
	static auto func = [] (const AIPtr& ai) {
		ai->getBehaviour()->resetState(ai);
	};
	zone->executeParallel(func);
}

void Server::select(const ClientId& /*clientId*/, const CharacterId& id) {
	Event event;
	event.type = EV_SELECTION;
	event.data.characterId = id;
	enqueueEvent(event);
}

void Server::onConnect(Client* client) {
	Event event;
	event.type = EV_NEWCONNECTION;
	event.data.newClient = client;
	enqueueEvent(event);
}

void Server::onDisconnect(Client* /*client*/) {
	Zone* zone = _zone;
	if (zone == nullptr)
		return;

	// if there are still connected clients left, don't disable the debug mode for the zone
	if (_network.getConnectedClients() > 0)
		return;

	// restore the zone state if no player is left for debugging
	const bool pauseState = _pause;
	if (pauseState) {
		pause(0, false);
	}

	zone->setDebug(false);
	_zone = nullptr;
	resetSelection();
}

bool Server::start() {
	return _network.start();
}

void Server::pause(const ClientId& /*clientId*/, bool state) {
	Event event;
	event.type = EV_PAUSE;
	event.data.pauseState = state;
	enqueueEvent(event);
}

void Server::addChildren(const TreeNodePtr& node, std::vector<AIStateNodeStatic>& out) const {
	for (const TreeNodePtr& childNode : node->getChildren()) {
		const int32_t nodeId = childNode->getId();
		out.push_back(AIStateNodeStatic(nodeId, childNode->getName(), childNode->getType(), childNode->getParameters(), childNode->getCondition()->getName(), childNode->getCondition()->getParameters()));
		addChildren(childNode, out);
	}
}

void Server::addChildren(const TreeNodePtr& node, AIStateNode& parent, const AIPtr& ai) const {
	const TreeNodes& children = node->getChildren();
	std::vector<bool> currentlyRunning(children.size());
	node->getRunningChildren(ai, currentlyRunning);
	const int64_t aiTime = ai->_time;
	const std::size_t length = children.size();
	for (std::size_t i = 0; i < length; ++i) {
		const TreeNodePtr& childNode = children[i];
		const int32_t id = childNode->getId();
		const ConditionPtr& condition = childNode->getCondition();
		const std::string conditionStr = condition ? condition->getNameWithConditions(ai) : "";
		const int64_t lastRun = childNode->getLastExecMillis(ai);
		const int64_t delta = lastRun == -1 ? -1 : aiTime - lastRun;
		AIStateNode child(id, conditionStr, delta, childNode->getLastStatus(ai), currentlyRunning[i]);
		addChildren(childNode, child, ai);
		parent.addChildren(child);
	}
}

void Server::broadcastState(const Zone* zone) {
	AIStateMessage msg;
	auto func = [&] (const AIPtr& ai) {
		const ICharacterPtr& chr = ai->getCharacter();
		const AIStateWorld b(chr->getId(), chr->getPosition(), chr->getOrientation(), chr->getAttributes());
		msg.addState(b);
	};
	zone->execute(func);
	_network.broadcast(msg);
}

void Server::broadcastStaticCharacterDetails(const Zone* zone) {
	const CharacterId id = _selectedCharacterId;
	if (id == AI_NOTHING_SELECTED) {
		return;
	}

	static const auto func = [&] (const AIPtr& ai) {
		std::vector<AIStateNodeStatic> nodeStaticData;
		const TreeNodePtr& node = ai->getBehaviour();
		const int32_t nodeId = node->getId();
		nodeStaticData.push_back(AIStateNodeStatic(nodeId, node->getName(), node->getType(), node->getParameters(), node->getCondition()->getName(), node->getCondition()->getParameters()));
		addChildren(node, nodeStaticData);

		const AICharacterStaticMessage msgStatic(ai->getId(), nodeStaticData);
		_network.broadcast(msgStatic);
	};
	if (!zone->executeAsync(id, func)) {
		resetSelection();
	}
}

void Server::broadcastCharacterDetails(const Zone* zone) {
	const CharacterId id = _selectedCharacterId;
	if (id == AI_NOTHING_SELECTED) {
		return;
	}

	static const auto func = [&] (const AIPtr& ai) {
		const TreeNodePtr& node = ai->getBehaviour();
		const int32_t nodeId = node->getId();
		const ConditionPtr& condition = node->getCondition();
		const std::string conditionStr = condition ? condition->getNameWithConditions(ai) : "";
		AIStateNode root(nodeId, conditionStr, _time - node->getLastExecMillis(ai), node->getLastStatus(ai), true);
		addChildren(node, root, ai);

		AIStateAggro aggro;
		const ai::AggroMgr::Entries& entries = ai->getAggroMgr().getEntries();
		aggro.reserve(entries.size());
		for (const Entry& e : entries) {
			aggro.addAggro(AIStateAggroEntry(e.getCharacterId(), e.getAggro()));
		}

		const AICharacterDetailsMessage msg(ai->getId(), aggro, root);
		_network.broadcast(msg);
	};
	if (!zone->executeAsync(id, func)) {
		resetSelection();
	}
}

void Server::handleEvents(Zone* zone, bool pauseState) {
	std::vector<Event> events;
	{
		ScopedReadLock scopedLock(_lock);
		events = std::move(_events);
		_events.clear();
	}
	for (Event& event : events) {
		switch (event.type) {
		case EV_SELECTION: {
			if (zone == nullptr) {
				resetSelection();
			} else {
				_selectedCharacterId = event.data.characterId;
				broadcastStaticCharacterDetails(zone);
				if (pauseState) {
					broadcastState(zone);
					broadcastCharacterDetails(zone);
				}
			}
			break;
		}
		case EV_STEP: {
			const int64_t queuedStepMillis = event.data.stepMillis;
			auto func = [=] (const AIPtr& ai) {
				if (!ai->isPause())
					return;
				ai->setPause(false);
				ai->update(queuedStepMillis, true);
				ai->getBehaviour()->execute(ai, queuedStepMillis);
				ai->setPause(true);
			};
			zone->executeParallel(func);
			broadcastState(zone);
			broadcastCharacterDetails(zone);
			break;
		}
		case EV_PAUSE: {
			_pause = event.data.pauseState;
			if (zone != nullptr) {
				auto func = [=] (const AIPtr& ai) {
					ai->setPause(pauseState);
				};
				zone->executeParallel(func);
				_network.broadcast(AIPauseMessage(pauseState));
				// send the last time the most recent state until we unpause
				if (pauseState) {
					broadcastState(zone);
					broadcastCharacterDetails(zone);
				}
			}
			break;
		}
		case EV_UPDATESTATICCHRDETAILS:
			broadcastStaticCharacterDetails(event.data.updateStaticCharacterDetails);
			break;
		case EV_NEWCONNECTION: {
			_network.sendToClient(event.data.newClient, AIPauseMessage(pauseState));
			std::vector<std::string> names;
			{
				ScopedReadLock scopedLock(_lock);
				names = _names;
			}
			const AINamesMessage msg(names);
			_network.sendToClient(event.data.newClient, msg);
			break;
		}
		case EV_ZONECHANGE:
			broadcastZoneNames();
			break;
		case EV_MAX:
			break;
		}
	}
}

void Server::update(int64_t deltaTime) {
	// TODO: don't send stuff twice - maintain a bitmask to skip duplicated messages
	_time += deltaTime;
	const int clients = _network.getConnectedClients();
	Zone* zone = _zone;
	bool pauseState = _pause;

	handleEvents(zone, pauseState);

	if (clients > 0 && zone != nullptr) {
		if (!pauseState) {
			broadcastState(zone);
			broadcastCharacterDetails(zone);
		}
	} else if (pauseState) {
		pause(1, false);
		resetSelection();
	}
	_network.update(deltaTime);
}

void Server::resetSelection() {
	_selectedCharacterId = AI_NOTHING_SELECTED;
}

void Server::setDebug(const std::string& zoneName) {
	if (_pause) {
		pause(1, false);
	}

	_zone = nullptr;
	resetSelection();

	ScopedReadLock scopedLock(_lock);
	for (Zone* zone : _zones) {
		const bool debug = zone->getName() == zoneName;
		zone->setDebug(debug);
		if (debug)
			_zone = zone;
	}
}

void Server::broadcastZoneNames() {
	std::vector<std::string> names;
	{
		ScopedReadLock scopedLock(_lock);
		names = _names;
	}
	const AINamesMessage msg(names);
	_network.broadcast(msg);
}

void Server::addZone(Zone* zone) {
	{
		ScopedWriteLock scopedLock(_lock);
		if (!_zones.insert(zone).second)
			return;
	}
	{
		ScopedReadLock scopedLock(_lock);
		_names.clear();
		for (const Zone* z : _zones) {
			_names.push_back(z->getName());
		}
	}
	Event event;
	event.type = EV_ZONECHANGE;
	event.data.zoneChanges = true;
	enqueueEvent(event);
}

void Server::removeZone(Zone* zone) {
	if (_zone == zone)
		_zone = nullptr;
	{
		ScopedWriteLock scopedLock(_lock);
		if (_zones.erase(zone) != 1) {
			return;
		}
	}
	{
		ScopedReadLock scopedLock(_lock);
		_names.clear();
		for (const Zone* z : _zones) {
			_names.push_back(z->getName());
		}
	}
	Event event;
	event.type = EV_ZONECHANGE;
	event.data.zoneChanges = true;
	enqueueEvent(event);
}

}
