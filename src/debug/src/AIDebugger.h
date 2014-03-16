#pragma once

#include <vector>
#include <AI.h>
#include <server/AIStateMessage.h>
#include <server/AICharacterDetailsMessage.h>
#include <server/AIStubTypes.h>
#include <server/IProtocolHandler.h>
#include <QTcpSocket>
#include <QApplication>
#include <QSettings>
#include <QFile>

namespace ai {
namespace debug {

class MapView;

class AIDebugger: public QApplication {
	Q_OBJECT
public:
	typedef std::vector<AIStateWorld> Entities;
	typedef Entities::const_iterator EntitiesIter;
protected:
	typedef Entities::iterator Iter;
	Entities _entities;

	ai::streamContainer _stream;

	bool _running;
	long _time;

	ai::CharacterId _selectedId;
	std::vector<AIStateAggroEntry> _aggro;
	AIStateNode _node;

	QTcpSocket _socket;

	bool writeMessage(const IProtocolMessage& msg);

private slots:
	void readTcpData();
public:
	AIDebugger(int argc, char** argv);
	virtual ~AIDebugger();

	const Entities& getEntities() const;
	void setEntities(const Entities& entities);
	void setCharacterDetails(const CharacterId& id, const AIStateAggro& aggro, const AIStateNode& node);
	const AIStateNode& getNode() const;
	const std::vector<AIStateAggroEntry>& getAggro() const;

	int run();
	bool connectToAIServer(const QString& hostname, short port);

	bool isSelected(const ai::AIStateWorld& ai) const;
	const CharacterId& getSelected() const;
	void select(const ai::AIStateWorld& ai);
	void unselect();

	virtual MapView* createMapWidget();

	void stop();
};

inline void AIDebugger::stop() {
	_running = false;
}

inline void AIDebugger::setCharacterDetails(const CharacterId& id, const AIStateAggro& aggro, const AIStateNode& node) {
	_selectedId = id;
	_aggro = aggro.getAggro();
	_node = node;
}

inline const std::vector<AIStateAggroEntry>& AIDebugger::getAggro() const {
	return _aggro;
}

inline const AIStateNode& AIDebugger::getNode() const {
	return _node;
}

inline void AIDebugger::setEntities(const Entities& entities) {
	_entities = entities;
}

inline const AIDebugger::Entities& AIDebugger::getEntities() const {
	return _entities;
}

}
}
