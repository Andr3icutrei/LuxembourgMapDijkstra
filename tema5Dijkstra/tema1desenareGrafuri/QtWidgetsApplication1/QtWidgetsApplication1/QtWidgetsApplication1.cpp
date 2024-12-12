#include "QtWidgetsApplication1.h"
#include <qpainter.h>
#include <qpushbutton.h>
#include "Graph.h"
#include <QVBoxLayout>

#include <fstream>
#include <string>

using namespace tinyxml2;

#pragma once

QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::QtWidgetsApplication1Class) {
    ui->setupUi(this);

    pushButton1 = new QPushButton("isOriented", this);
    pushButton1->setGeometry(900, 30, 100, 30);

    pushButtonReadGraphFromFile = new QPushButton("Read graph", this);
    pushButtonReadGraphFromFile->setGeometry(900, 100, 100, 30);

    connect(pushButton1, &QPushButton::clicked, this, &QtWidgetsApplication1::onPushButton1Clicked);
    connect(pushButtonReadGraphFromFile, &QPushButton::clicked, this, &QtWidgetsApplication1::onPushButtonReadGraphFromFile);

    windowWidth = this->width();
    windowHeight = this->height();
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{
    delete ui;
}

bool QtWidgetsApplication1::canDrawNode(int xCurr, int yCurr)
{
    std::shared_ptr<Node> nCurr{ new Node{ xCurr,yCurr } };
    for (std::shared_ptr<Node>& n : m_graph.GetNodes())
    {
        if (m_graph.nodesOverlap(n, nCurr) == true)
        {
            nCurr.reset();
            return false;
        }
    }
    return true;
}

bool QtWidgetsApplication1::isMultigraph(const Edge& currEdge)
{
    std::vector<Edge> vEdges = m_graph.GetEdges();
    for (int i=0;i<vEdges.size();++i)
    {
        if (vEdges[i] == currEdge)
            return false;
    }
    return false;
}

void QtWidgetsApplication1::resizeEvent(QResizeEvent* event)
{
    QSize newSize = event->size();
    int newWidth = newSize.width();
    int newHeight = newSize.height();

    // Print the new size
    qDebug() << "New width:" << newWidth << ", New height:" << newHeight;

    // Call the base class implementation (optional, but recommended)
    QMainWindow::resizeEvent(event);
}

void QtWidgetsApplication1::onPushButtonReadGraphFromFile()
{
    XMLDocument doc;
    if (doc.LoadFile("Harta_Luxemburg.xml") != XML_SUCCESS)
    {
        qDebug() << "error loading xml file";
        return;
    }

    XMLElement* map = doc.FirstChildElement("map");
    if (!map)
    {
        qDebug() << "error not finding map";
        return;
    }

    XMLElement* nodes=map->FirstChildElement("nodes");

    if (nodes) 
    {
        m_maxLat = m_maxLong = std::numeric_limits<double>::min();
        m_minLong = m_minLat = std::numeric_limits<double>::max();
        for (XMLElement* node = nodes->FirstChildElement("node"); node != nullptr; node = node->NextSiblingElement("node"))
        {
            int idValue;
            node->QueryIntAttribute("id", &idValue);

            double xValue;
            node->QueryDoubleAttribute("latitude", &xValue);

            double yValue;
            node->QueryDoubleAttribute("longitude", &yValue);

            if (xValue > m_maxLat)
                m_maxLat = xValue;
            if (xValue < m_minLat)
                m_minLat = xValue;
            if (yValue > m_maxLong)
                m_maxLong = yValue;
            if (yValue < m_minLong)
                m_minLong = yValue;

            std::shared_ptr<Node> nodeToAdd = std::make_shared<Node>(idValue,xValue,yValue);

            m_graph.addNode(nodeToAdd);

            m_nodesMap[idValue] = nodeToAdd;
        }
    }
    else { qDebug() << "nodes does not exist"; return; }

    xScaleFactor = windowWidth / (m_maxLat - m_minLat);
    yScaleFactor = windowHeight / (m_maxLong - m_minLong);

    for (auto& node : m_graph.GetNodes())
    {
        node->SetPositionX((node->GetX()-m_minLong)/100.0*xScaleFactor);
        node->SetPositionY((node->GetY()-m_minLat) /100.0* yScaleFactor); 
           // qDebug() << node->GetX() << " " << node->GetY() << '\n';
    }

    XMLElement* arcs = map->FirstChildElement("arcs");
    if (arcs)
    {
        for (XMLElement* arc = arcs->FirstChildElement("arc"); arc != nullptr; arc = arc->NextSiblingElement("arc"))
        {
            int fromValue;
            arc->QueryIntAttribute("from", &fromValue);

            int toValue;
            arc->QueryIntAttribute("to", &toValue);

            int length;
            arc->QueryIntAttribute("length", &length);

            m_graph.addEdge(m_nodesMap[fromValue], m_nodesMap[toValue], length);

        }
    }
    else { qDebug() << "arcs does not exist"; return; }
    
}

void QtWidgetsApplication1::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        for (std::shared_ptr<Node>& node : m_graph.GetNodes()) {
            int dx = event->pos().x() - node->GetX();
            int dy = event->pos().y() - node->GetY();
            if (dx * dx + dy * dy <= Node::m_radius * Node::m_radius) {
                m_draggedNode = node;
                m_mousePos = event->pos();
                break;
            }
        }
    }
}

void QtWidgetsApplication1::mouseMoveEvent(QMouseEvent* event)
{
    if (m_draggedNode != nullptr) {
        int dx = event->pos().x() - m_mousePos.x();
        int dy = event->pos().y() - m_mousePos.y();

        m_draggedNode->SetPositionX(m_draggedNode->GetX() + dx);
        m_draggedNode->SetPositionY(m_draggedNode->GetY() + dy);
        
        m_mousePos = event->pos();
        
        update();
    }
}

void QtWidgetsApplication1::mouseReleaseEvent(QMouseEvent* m)
{
    if (m->button() == Qt::RightButton)
    {
        if (m_draggedNode != nullptr) {
            qDebug() << "Drag ended." << m_draggedNode->GetX() << " " << m_draggedNode->GetY() << '\n';
            m_draggedNode = nullptr;
            update();
        }
        else
        {
            m_mousePos = m->pos();
            int xCurr = m_mousePos.x();
            int yCurr = m_mousePos.y();
            std::shared_ptr<Node> nCurr{ new Node{ xCurr,yCurr } };
            for (std::shared_ptr<Node>& n : m_graph.GetNodes())
            {
                if (m_graph.nodesOverlap(n, nCurr) == true)
                {
                    nCurr.reset();
                    return;
                }
            }
            m_graph.addNode(xCurr, yCurr);
            update();
        }
    }
    else
    {
        m_mousePos = m->pos();
        for (std::shared_ptr<Node>& currNode : m_graph.GetNodes())
        {
            int dx = abs(m_mousePos.x() - currNode->GetX());
            int dy = abs(m_mousePos.y() - currNode->GetY());
            int sqDistance = dx * dx + dy * dy;

            if (sqDistance <= currNode->GetRadius()* currNode->GetRadius())
            {
                if (selectedNode1 == nullptr)
                    selectedNode1 = currNode;
                else if (selectedNode2 == nullptr)
                {
                    selectedNode2 = currNode;
                    Edge currEdge(selectedNode1, selectedNode2);
                    if (isMultigraph(currEdge) == false)
                    {
                        pushButton1->setDisabled(true);
                        m_graph.addEdge(selectedNode1, selectedNode2);
                    }
                    selectedNode1 = nullptr;
                    selectedNode2 = nullptr;
                    update();
                }

            }

        }

    }
}

void QtWidgetsApplication1::paintEvent(QPaintEvent* ev)
{
    if (m_mousePos.x() && m_mousePos.y())
    {
        QPainter p(this);
        p.setPen(Qt::blue);
        std::vector<Edge> vEdges = m_graph.GetEdges();
        for (Edge& currEdge : vEdges)
        {
            QPointF pos1(currEdge.GetFirstNode()->GetX(), currEdge.GetFirstNode()->GetY());
            QPointF pos2(currEdge.GetSecondNode()->GetX(), currEdge.GetSecondNode()->GetY());
            double dx = pos2.x() - pos1.x();
            double dy = pos2.y() - pos1.y();
            double distance = std::sqrt(dx * dx + dy * dy);

            if (distance > 0) {
                double nx = dx / distance;
                double ny = dy / distance;

                const QPoint start(pos1.x() , pos1.y());
                const QPoint end(pos2.x(), pos2.y());

                p.drawLine(start, end);
                if(m_graph.GetIsOriented() == true)
                    drawArrow(p, start, end);
                //m_graph.writeAdjList();
                
            }
        }
    }
}

void QtWidgetsApplication1::drawArrow(QPainter& p, const QPoint& start, const QPoint& end)
{
    const double arrowSize = 10.0;
    const double angle = M_PI / 6;

    double dx = end.x() - start.x();
    double dy = end.y() - start.y();

    double lineAngle = std::atan2(dy, dx);

    QPointF arrowP1 = end - QPointF(arrowSize * std::cos(lineAngle - angle), arrowSize * std::sin(lineAngle - angle));
    QPointF arrowP2 = end - QPointF(arrowSize * std::cos(lineAngle + angle), arrowSize * std::sin(lineAngle + angle));

    p.drawLine(end, arrowP1);
    p.drawLine(end, arrowP2);
}

void QtWidgetsApplication1::onPushButton1Clicked()
{
    if (pushButton1->text() == "isOriented")
    {
        pushButton1->setText("isNonoriented");
        m_graph.SetOrientation(false);
    }
    else
    {
        pushButton1->setText("isOriented");
        m_graph.SetOrientation(true);
    }
}




