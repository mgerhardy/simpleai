#pragma once

#include <QGraphicsView>
#include <QSlider>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

namespace ai {
namespace debug {

class MapZoomWidget: public QWidget {
	Q_OBJECT
public:
	MapZoomWidget(QGraphicsView& gview, QWidget* parent = nullptr);
	~MapZoomWidget();

	void setAutoRepeatInterval(int interval);
	void setValueRange(int min, int max);
	void setValue(int value);

public slots:
	void zoomIn(int level = 1);
	void zoomOut(int level = 1);

private slots:
	QVBoxLayout* createLayout();
	void setupZoomMatrix();

private:
	QGraphicsView& _graphicsView;
	QSlider _zoomSlider;
	QToolButton _zoomInButton;
	QToolButton _zoomOutButton;
};

}
}
