#pragma once

#include <vector>
#include "MapView.h"
#include <server/AIStateMessage.h>
#include <server/AIStubTypes.h>
#include <server/IProtocolHandler.h>
#include <QTcpSocket>
#include <QApplication>
#include <QSettings>
#include <QFile>

namespace ai {

namespace debug {

class AIDebugger;

class Handler: public ai::IProtocolHandler {
private:
	AIDebugger* _debugger;
public:
	Handler(AIDebugger* debugger) :
			_debugger(debugger) {
	}

	void execute(const ClientId& clientId, const IProtocolMessage& message) override;
};

class AIDebugger: public QApplication {
	Q_OBJECT
public:
	typedef std::vector<AIStateTree> Entities;
	typedef Entities::const_iterator EntitiesIter;
protected:
	typedef Entities::iterator Iter;
	Entities _entities;

	bool _running;
	long _time;

	ai::CharacterId _selectedId;

	QTcpSocket _socket;
	Handler _handler;

private slots:
	void readTcpData();
public:
	AIDebugger(int argc, char** argv);
	virtual ~AIDebugger();

	const Entities& getEntities() const;

	int run();
	bool connectToAIServer(const QString& hostname, short port);

	void setEntities(const Entities& entities);
	bool isSelected(const ai::AIStateTree& ai) const;
	const ai::AIStateTree* getSelected() const;
	void select(const ai::AIStateTree& ai);
	void unselect();

	virtual MapView* createMapWidget();

	void stop();
};

inline MapView* AIDebugger::createMapWidget() {
	return new MapView(*this);
}

inline void AIDebugger::stop() {
	_running = false;
}

inline const AIDebugger::Entities& AIDebugger::getEntities() const {
	return _entities;
}

}
}
