#pragma once

#include <QGraphicsScene>

namespace ai {
namespace debug {

class MapScene: public QGraphicsScene {
public:
	MapScene(QObject *parent = 0);
	virtual ~MapScene();
};

}
}
