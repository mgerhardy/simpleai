#pragma once

#include <QGraphicsView>

namespace ai {
namespace debug {

class IGraphicsView : public QGraphicsView {
private:
	Q_PROPERTY(bool renderGrid READ renderGrid WRITE setRenderGrid)
	Q_PROPERTY(bool renderBackground READ renderBackground WRITE setRenderBackground)

	bool _renderGrid;
	bool _renderBackground;
public:
	IGraphicsView(bool renderGrid = false, bool renderBackground = false, QWidget* parent = nullptr);
	virtual ~IGraphicsView();

	bool renderGrid() const {
		return _renderGrid;
	}
	bool renderBackground() const {
		return _renderBackground;
	}

	void setRenderGrid(bool renderGrid) {
		_renderGrid = renderGrid;
	}
	void renderBackground(bool renderBackground) {
		_renderBackground = renderBackground;
	}

	virtual void drawBackground(QPainter* painter, const QRectF& rect) override;
};

}
}
