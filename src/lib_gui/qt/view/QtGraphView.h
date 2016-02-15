#ifndef QT_GRAPH_VIEW_H
#define QT_GRAPH_VIEW_H

#include <QGraphicsView>
#include <QPointF>

#include "component/view/GraphView.h"
#include "qt/utility/QtThreadedFunctor.h"
#include "utility/types.h"

struct DummyEdge;
struct DummyNode;
class QMouseEvent;
class QSequentialAnimationGroup;
class QtGraphEdge;
class QtGraphNode;

class QtGraphicsView
	: public QGraphicsView
{
	Q_OBJECT

public:
	QtGraphicsView(QWidget* parent);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

signals:
	void emptySpaceClicked();

private:
	QPoint m_last;
};


class QtGraphView
	: public QObject
	, public GraphView
{
	Q_OBJECT

public:
	QtGraphView(ViewLayout* viewLayout);
	virtual ~QtGraphView();

	// View implementation
	virtual void createWidgetWrapper();
	virtual void initView();
	virtual void refreshView();

	virtual void rebuildGraph(std::shared_ptr<Graph> graph, const std::vector<DummyNode>& nodes, const std::vector<DummyEdge>& edges);
	virtual void clear();

	virtual void focusTokenIds(const std::vector<Id>& focusedTokenIds);
	virtual void defocusTokenIds(const std::vector<Id>& defocusedTokenIds);

	virtual void resizeView();

	virtual Vec2i getViewSize() const;

private slots:
	void centerScrollBars();
	void finishedTransition();
	void clickedInEmptySpace();

private:
	void switchToNewGraphData();

	QGraphicsView* getView() const;

	void doRebuildGraph(std::shared_ptr<Graph> graph, const std::vector<DummyNode>& nodes, const std::vector<DummyEdge>& edges);
	void doClear();
	void doResize();
	void doRefreshView();

	void doFocusIn(const std::vector<Id>& tokenIds);
	void doFocusOut(const std::vector<Id>& tokenIds);

	std::shared_ptr<QtGraphNode> findNodeRecursive(const std::list<std::shared_ptr<QtGraphNode>>& nodes, Id tokenId);

	std::shared_ptr<QtGraphNode> createNodeRecursive(
		QGraphicsView* view, std::shared_ptr<QtGraphNode> parentNode, const DummyNode& node, bool multipleActive);
	std::shared_ptr<QtGraphEdge> createEdge(QGraphicsView* view, const DummyEdge& edge);

	QRectF itemsBoundingRect(const std::list<std::shared_ptr<QtGraphNode>>& items) const;
	QRectF getSceneRect(const std::list<std::shared_ptr<QtGraphNode>>& items) const;

	void compareNodesRecursive(
		std::list<std::shared_ptr<QtGraphNode>> newSubNodes,
		std::list<std::shared_ptr<QtGraphNode>> oldSubNodes,
		std::list<QtGraphNode*>* appearingNodes,
		std::list<QtGraphNode*>* vanishingNodes,
		std::vector<std::pair<QtGraphNode*, QtGraphNode*>>* remainingNodes);

	void createTransition();

	QtThreadedFunctor<std::shared_ptr<Graph>, const std::vector<DummyNode>&, const std::vector<DummyEdge>&> m_rebuildGraphFunctor;
	QtThreadedFunctor<void> m_clearFunctor;
	QtThreadedFunctor<void> m_resizeFunctor;
	QtThreadedFunctor<void> m_refreshFunctor;
	QtThreadedFunctor<const std::vector<Id>&> m_focusInFunctor;
	QtThreadedFunctor<const std::vector<Id>&> m_focusOutFunctor;

	std::shared_ptr<Graph> m_graph;
	std::shared_ptr<Graph> m_oldGraph;

	std::list<std::shared_ptr<QtGraphEdge>> m_edges;
	std::list<std::shared_ptr<QtGraphEdge>> m_oldEdges;

	std::list<std::shared_ptr<QtGraphNode>> m_nodes;
	std::list<std::shared_ptr<QtGraphNode>> m_oldNodes;

	std::shared_ptr<QSequentialAnimationGroup> m_transition;
	QPointF m_sceneRectOffset;
};

#endif // QT_GRAPH_VIEW_H