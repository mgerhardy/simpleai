#pragma once

#include <AI.h>
#include <tree/TreeNode.h>
#include <set>
#include "Network.h"
#include "Zone.h"
#include "SelectHandler.h"
#include "PauseHandler.h"
#include "ResetHandler.h"
#include "StepHandler.h"
#include "ChangeHandler.h"

namespace ai {

class AIStateNode;

/**
 * @brief The server can serialize the state of the AI and broadcast it to all connected clients.
 *
 * If you start a server, you can add the @c AI instances to it by calling @c addAI. If you do so, make
 * sure to remove it when you remove that particular @c AI instance from your world. You should not do that
 * from different threads. The server should only be managed from one thread.
 *
 * The server will broadcast the world state - that is: It will send out an @c AIStateMessage to all connected
 * clients. If someone selected a particular @c AI instance by sending @c AISelectMessage to the server, it
 * will also broadcast an @c AICharacterDetailsMessage to all connected clients.
 */
class Server: public INetworkListener {
protected:
	typedef std::set<Zone*> Zones;
	typedef Zones::const_iterator ZoneConstIter;
	typedef Zones::iterator ZoneIter;
	Zones _zones;
	Network _network;
	CharacterId _selectedCharacterId;
	long _time;
	SelectHandler _selectHandler;
	PauseHandler _pauseHandler;
	ResetHandler _resetHandler;
	StepHandler _stepHandler;
	ChangeHandler _changeHandler;
	NopHandler _nopHandler;
	bool _pause;
	// the current active debugging zone
	Zone* _zone;

	void addChildren(const TreeNodePtr& node, AIStateNode& parent, AI& ai) const;
	void broadcastState();
	void broadcastCharacterDetails();
	void broadcastZoneNames();
	void onConnect(Client* client) override;
	void onDisconnect(Client* client) override;
public:
	Server(short port = 10001, const std::string& hostname = "0.0.0.0");
	virtual ~Server();

	/**
	 * @brief Start to listen on the specified port
	 */
	bool start();

	/**
	 * @brief Select a particular character (resp. @c AI instance) and send detail
	 * information to all the connected clients for this entity.
	 */
	void select(const ClientId& clientId, const CharacterId& id);
	/**
	 * @brief Will pause/unpause the execution of the behaviour trees for all watched @c AI instances.
	 */
	void pause(const ClientId& clientId, bool pause);

	/**
	 * @brief Performs one step of the ai in pause mode
	 */
	void step();

	/**
	 * @brief Adds a new zone to this server instance that can be debugged. The server does not own this pointer
	 * so it also doesn't free it. Every @c Zone that is added here, will be part of the @c AINamesMessage.
	 *
	 * @param zone The @c Zone that should be made available for debugging.
	 */
	void addZone(Zone* zone);

	/**
	 * @brief Removes a @c Zone from the server. After this call the given zone is no longer available for debugging
	 * purposes.
	 */
	void removeZone(Zone* zone);

	/**
	 * @brief Activate the debugging for this particular zone. And disables the debugging for every other zone
	 */
	void setDebug(const std::string& zoneName);

	/**
	 * @brief Resets the ai states
	 */
	void reset();
	/**
	 * @brief call this to update the server - should get called somewhere from your game tick
	 */
	void update(long deltaTime);
};

}
