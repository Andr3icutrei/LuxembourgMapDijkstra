#pragma once
#include <qapplication.h>
#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"   
#include <QMouseEvent>
#include "Graph.h"
#include <qpushbutton.h>
#include "tinyxml2.h"

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtWidgetsApplication1(QWidget *parent = nullptr);
    ~QtWidgetsApplication1();

    void mouseReleaseEvent(QMouseEvent* m) override;
    void paintEvent(QPaintEvent* ev) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event)override;

    void drawArrow(QPainter& p, const QPoint& start, const QPoint& end);
    bool canDrawNode(int xCurr,int yCurr);
    bool isMultigraph(const Edge& currEdge);
    
    int windowWidth,windowHeight;

    double m_minLat,m_maxLat,m_minLong,m_maxLong;
    double xScaleFactor, yScaleFactor;

    void resizeEvent(QResizeEvent* event) override;
private:
    QPushButton* pushButton1;
    QPushButton* pushButtonReadGraphFromFile;
    Ui::QtWidgetsApplication1Class* ui;

    QPoint m_mousePos;

    Graph m_graph;

    std::shared_ptr<Node> selectedNode1 = nullptr;
    std::shared_ptr<Node> selectedNode2 = nullptr; 

    std::shared_ptr<Node> m_draggedNode = nullptr;

    std::unordered_map<int, std::shared_ptr<Node>> m_nodesMap;

private slots:
    void onPushButton1Clicked();
    void onPushButtonReadGraphFromFile();
};
