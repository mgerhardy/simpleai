#include "Server.h"
#include "AIStateMessage.h"
#include "AINamesMessage.h"
#include "AIStubTypes.h"
#include "AICharacterDetailsMessage.h"
#include "ProtocolHandlerRegistry.h"

namespace ai {

Server::Server(short port, const std::string& hostname) :
		_network(port, hostname), _selectedCharacterId(-1), _time(0L), _selectHandler(*this), _pauseHandler(*this), _resetHandler(*this), _stepHandler(*this), _changeHandler(
				*this), _pause(false), _zone(nullptr) {
	_network.addListener(this);
	ProtocolHandlerRegistry& r = ai::ProtocolHandlerRegistry::get();
	r.registerHandler(ai::PROTO_SELECT, &_selectHandler);
	r.registerHandler(ai::PROTO_PAUSE, &_pauseHandler);
	r.registerHandler(ai::PROTO_RESET, &_resetHandler);
	r.registerHandler(ai::PROTO_STEP, &_stepHandler);
	r.registerHandler(ai::PROTO_CHANGE, &_changeHandler);
}

Server::~Server() {
	_network.removeListener(this);
}

void Server::step() {
	if (_zone == nullptr)
		return;
	for (Zone::AIMapIter i = _zone->begin(); i != _zone->end(); ++i) {
		AI& ai = *i->second;
		if (!ai.isPause())
			continue;
		ai.setPause(false);
		ai.update(1L);
		ai.setPause(true);
	}
}

void Server::reset() {
	if (_zone == nullptr)
		return;
	for (Zone::AIMapIter i = _zone->begin(); i != _zone->end(); ++i) {
		AI& ai = *i->second;
		ai.getBehaviour()->resetState(ai);
	}
}

void Server::select(const ClientId& /*clientId*/, const CharacterId& id) {
	_selectedCharacterId = id;
}

void Server::onConnect(Client* client) {
	_network.sendToClient(client, AIPauseMessage(_pause));
	std::vector<std::string> names;
	for (ZoneConstIter i = _zones.begin(); i != _zones.end(); ++i) {
		const Zone* zone = *i;
		names.push_back(zone->getName());
	}
	const AINamesMessage msg(names);
	_network.sendToClient(client, msg);
}

bool Server::start() {
	return _network.start();
}

void Server::pause(const ClientId& /*clientId*/, bool state) {
	if (_zone == nullptr)
		return;
	_pause = state;
	for (Zone::AIMapIter i = _zone->begin(); i != _zone->end(); ++i) {
		AI& ai = *i->second;
		ai.setPause(_pause);
	}
	_network.broadcast(AIPauseMessage(_pause));
}

void Server::addChildren(const TreeNodePtr& node, AIStateNode& parent, AI& ai) const {
	const TreeNodes& children = node->getChildren();
	std::vector<bool> currentlyRunning;
	node->getRunningChildren(ai, currentlyRunning);
	const long aiTime = ai._time;
	const std::size_t length = children.size();
	for (std::size_t i = 0; i < length; ++i) {
		const TreeNodePtr& childNode = children[i];
		const std::string& name = childNode->getName();
		const ConditionPtr& condition = childNode->getCondition();
		const std::string conditionStr = condition ? condition->getNameWithConditions(ai) : "";
		const long lastRun = childNode->getLastExecMillis(ai);
		const long delta = lastRun == -1 ? -1 : aiTime - lastRun;
		AIStateNode child(name, conditionStr, delta, childNode->getLastStatus(ai), currentlyRunning[i]);
		addChildren(childNode, child, ai);
		parent.addChildren(child);
	}
}

void Server::broadcastState() {
	AIStateMessage msg;
	for (Zone::AIMapConstIter i = _zone->begin(); i != _zone->end(); ++i) {
		const AI& ai = *i->second;
		const ICharacter& chr = ai.getCharacter();
		const AIStateWorld b(chr.getId(), chr.getPosition(), chr.getOrientation(), chr.getAttributes());
		msg.addState(b);
	}
	_network.broadcast(msg);
}

void Server::broadcastCharacterDetails() {
	if (_selectedCharacterId == -1)
		return;

	Zone::AIMapConstIter i = _zone->find(_selectedCharacterId);
	if (i == _zone->end()) {
		_selectedCharacterId = -1;
		return;
	}

	ai::AI& ai = *i->second;
	const TreeNodePtr& node = ai.getBehaviour();
	const std::string& name = node->getName();
	const ConditionPtr& condition = node->getCondition();
	const std::string conditionStr = condition ? condition->getNameWithConditions(ai) : "";
	AIStateNode root(name, conditionStr, _time - node->getLastExecMillis(ai), node->getLastStatus(ai), true);
	addChildren(node, root, ai);

	AIStateAggro aggro;
	const ai::AggroMgr::Entries& entries = ai.getAggroMgr().getEntries();
	for (ai::AggroMgr::Entries::const_iterator it = entries.begin(); it != entries.end(); ++it) {
		const EntryPtr& e = *it;
		aggro.addAggro(AIStateAggroEntry(e->getCharacterId(), e->getAggro()));
	}

	const AICharacterDetailsMessage msg(_selectedCharacterId, aggro, root);
	_network.broadcast(msg);
}

void Server::update(long deltaTime) {
	_time += deltaTime;
	const int clients = _network.getConnectedClients();
	if (clients > 0 && _zone != nullptr) {
		broadcastState();
		broadcastCharacterDetails();
	} else if (_pause) {
		pause(1, false);
		_selectedCharacterId = -1;
	}
	_network.update(deltaTime);
}

void Server::setDebug(const std::string& zoneName) {
	_zone = nullptr;
	for (ZoneIter i = _zones.begin(); i != _zones.end(); ++i) {
		Zone* zone = *i;
		const bool debug = zone->getName() == zoneName;
		zone->setDebug(debug);
		if (debug)
			_zone = zone;
	}
}

void Server::broadcastZoneNames() {
	std::vector<std::string> names;
	for (ZoneConstIter i = _zones.begin(); i != _zones.end(); ++i) {
		const Zone* zone = *i;
		names.push_back(zone->getName());
	}
	const AINamesMessage msg(names);
	_network.broadcast(msg);
}

void Server::addZone(Zone* zone) {
	_zones.insert(zone);
	broadcastZoneNames();
}

void Server::removeZone(Zone* zone) {
	if (_zone == zone)
		_zone = nullptr;
	if (_zones.erase(zone) == 1) {
		broadcastZoneNames();
	}
}

}
