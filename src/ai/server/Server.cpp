#include "Server.h"
#include "AIStateMessage.h"
#include "AIStubTypes.h"
#include "AICharacterDetailsMessage.h"
#include "ProtocolHandlerRegistry.h"

namespace ai {

Server::Server(int port, const std::string& hostname) :
		_network(port, hostname), _selectedCharacterId(-1), _time(0L), _selectHandler(*this), _pauseHandler(*this), _pause(false) {
	ProtocolHandlerRegistry& r = ai::ProtocolHandlerRegistry::get();
	r.registerHandler(ai::PROTO_SELECT, ProtocolHandlerPtr(&_selectHandler, ProtocolHandlerNopDeleter()));
	r.registerHandler(ai::PROTO_PAUSE, ProtocolHandlerPtr(&_pauseHandler, ProtocolHandlerNopDeleter()));
}

Server::~Server() {
}

void Server::select(const ClientId& /*clientId*/, const CharacterId& id) {
	_selectedCharacterId = id;
}

bool Server::start() {
	return _network.start();
}

void Server::pause(const ClientId& /*clientId*/, bool pause) {
	_pause = pause;
	for (AIMapIter i = _ais.begin(); i != _ais.end(); ++i) {
		AI& ai = *i->second;
		ai.setPause(pause);
	}
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

void Server::addChildren(const TreeNodePtr& node, AIStateNode& parent, AI& ai) const {
	const TreeNodes& children = node->getChildren();
	std::vector<bool> active;
	node->getChildrenState(ai, active);
	const int length = children.size();
	for (int i = 0; i < length; ++i) {
		const TreeNodePtr& node = children[i];
		const std::string& name = node->getName();
		const ConditionPtr& condition = node->getCondition();
		const std::string conditionStr = condition ? condition->getNameWithConditions(ai) : "";
		AIStateNode child(name, conditionStr, _time - node->getLastExecMillis(), active[i]);
		addChildren(node, child, ai);
		parent.addChildren(child);
	}
}

void Server::broadcastState() {
	AIStateMessage msg;
	for (AIMapConstIter i = _ais.begin(); i != _ais.end(); ++i) {
		const AI& ai = *i->second;
		const ICharacter& chr = ai.getCharacter();
		AIStateWorld b(chr.getId(), chr.getPosition());
		msg.addState(b);
	}
	_network.broadcast(msg);
}

void Server::broadcastCharacterDetails() {
	if (_selectedCharacterId == -1)
		return;

	AIMapConstIter i = _ais.find(_selectedCharacterId);
	if (i == _ais.end()) {
		_selectedCharacterId = -1;
		return;
	}

	ai::AI& ai = *i->second;
	const TreeNodePtr& node = ai.getBehaviour();
	const std::string& name = node->getName();
	const ConditionPtr& condition = node->getCondition();
	const std::string conditionStr = condition ? condition->getNameWithConditions(ai) : "";
	AIStateNode root(name, conditionStr, _time - node->getLastExecMillis(), true);
	addChildren(node, root, ai);

	AIStateAggro aggro;
	const ai::AggroMgr::Entries& entries = ai.getAggroMgr().getEntries();
	for (ai::AggroMgr::Entries::const_iterator i = entries.begin(); i != entries.end(); ++i) {
		const EntryPtr& e = *i;
		aggro.addAggro(AIStateAggroEntry(e->getCharacterId(), e->getAggro()));
	}

	const AICharacterDetailsMessage msg(_selectedCharacterId, aggro, root, ai.getCharacter().getAttributes());
	_network.broadcast(msg);
}

void Server::update(uint32_t deltaTime) {
	_time += deltaTime;
	const int clients = _network.getConnectedClients();
	if (clients > 0) {
		broadcastState();
		broadcastCharacterDetails();
	} else if (_pause) {
		pause(1, false);
	}
	_network.update(deltaTime);
}

}
