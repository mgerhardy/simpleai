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
	typedef std::vector<AIStateTree> Entities;
	typedef Entities::const_iterator EntitiesIter;
protected:
	typedef Entities::iterator Iter;
	Entities _entities;

	ai::streamContainer _stream;

	bool _running;
	long _time;

	ai::CharacterId _selectedId;
	std::vector<AIStateAggroEntry> _aggro;

	QTcpSocket _socket;

private slots:
	void readTcpData();
public:
	AIDebugger(int argc, char** argv);
	virtual ~AIDebugger();

	const Entities& getEntities() const;
	void setEntities(const Entities& entities);
	void setCharacterDetails(const CharacterId& id, const AIStateAggro& aggro);

	int run();
	bool connectToAIServer(const QString& hostname, short port);

	bool isSelected(const ai::AIStateTree& ai) const;
	const ai::AIStateTree* getSelected();
	void select(const ai::AIStateTree& ai);
	void unselect();

	virtual MapView* createMapWidget();

	void stop();
};

inline void AIDebugger::stop() {
	_running = false;
}

inline void AIDebugger::setCharacterDetails(const CharacterId& id, const AIStateAggro& aggro) {
	_selectedId = id;
	_aggro = aggro.getAggro();
}

inline void AIDebugger::setEntities(const Entities& entities) {
	_entities = entities;
}

inline const AIDebugger::Entities& AIDebugger::getEntities() const {
	return _entities;
}

}
}
