#include "Server.h"
#include "AIStateMessage.h"
#include "AIStubTypes.h"
#include "AICharacterDetailsMessage.h"
#include "ProtocolHandlerRegistry.h"

namespace ai {

Server::Server(int port, const std::string& hostname) :
		_network(port, hostname), _selectedCharacterId(-1) {
	ProtocolHandlerRegistry& r = ai::ProtocolHandlerRegistry::get();
	r.registerHandler(ai::PROTO_SELECT, ProtocolHandlerPtr(this, ProtocolHandlerNopDeleter()));
}

Server::~Server() {
}

void Server::select(const ClientId& clientId, const CharacterId& id) {
	_selectedCharacterId = id;
}

bool Server::start() {
	return _network.start();
}

bool Server::addAI(AI& ai) {
	const CharacterId& id = ai.getCharacter().getId();
	if (_ais.find(id) != _ais.end())
		return false;
	_ais.insert(std::make_pair(id, &ai));
	return true;
}

bool Server::removeAI(AI& ai) {
	const CharacterId& id = ai.getCharacter().getId();
	AIMapIter i = _ais.find(id);
	if (i == _ais.end())
		return false;
	_ais.erase(i);
	return true;
}

void Server::addChildren(AIStateNode& parent, AI& ai, const TreeNodes& children) const {
	for (TreeNodes::const_iterator i = children.begin(); i != children.end(); ++i) {
		const TreeNodePtr& node = *i;
		const std::string& name = node->getName();
		const TreeNodes& children = node->getChildren();
		const ConditionPtr& condition = node->getCondition();
		const std::string conditionStr = condition ? condition->getNameWithConditions(ai) : "";
		// TODO: node->isActive
		const bool active = /* node->isActive(ai) */ false;
		AIStateNode child(name, conditionStr, node->getLastRunMillis(), active);
		addChildren(child, ai, children);
		parent.addChildren(child);
	}
}

void Server::broadcastState() {
	AIStateMessage msg;
	for (AIMapConstIter i = _ais.begin(); i != _ais.end(); ++i) {
		AI& ai = *i->second;
		const TreeNodePtr& behaviour = ai.getBehaviour();
		const std::string& name = behaviour->getName();
		const TreeNodes& children = behaviour->getChildren();
		const ConditionPtr& condition = behaviour->getCondition();
		const std::string conditionStr = condition ? condition->getNameWithConditions(ai) : "";
		// TODO: node->isActive
		const bool active = /* node->isActive(ai) */ false;
		AIStateNode root(name, conditionStr, behaviour->getLastRunMillis(), active);
		addChildren(root, ai, children);
		AIStateTree b(ai.getCharacter().getId(), ai.getCharacter().getPosition(), root);
		msg.addTree(b);
	}
	_network.broadcast(msg);
}

void Server::broadcastCharacterDetails() {
	if (_selectedCharacterId == -1)
		return;

	AIMapConstIter i = _ais.find(_selectedCharacterId);
	if (i != _ais.end()) {
		_selectedCharacterId = -1;
		return;
	}

	std::vector<AIStateAggroEntry> aggroVector;

	ai::AI *ai = i->second;
	const ai::AggroMgr::Entries& entries = ai->getAggroMgr().getEntries();
	for (ai::AggroMgr::Entries::const_iterator i = entries.begin(); i != entries.end(); ++i) {
		const EntryPtr& e = *i;
		aggroVector.push_back(AIStateAggroEntry(e->getCharacterId(), e->getAggro()));
	}

	const AIStateAggro aggro(_selectedCharacterId, aggroVector);
	const AICharacterDetailsMessage msg(_selectedCharacterId, aggro);
	_network.broadcast(msg);
}

void Server::update(uint32_t deltaTime) {
	broadcastState();
	broadcastCharacterDetails();
	_network.update(deltaTime);
}

}
