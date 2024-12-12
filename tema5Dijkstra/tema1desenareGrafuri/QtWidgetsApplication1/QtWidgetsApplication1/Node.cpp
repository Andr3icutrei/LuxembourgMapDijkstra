#include "Node.h"
#include <QMouseEvent>
#include <qwidget.h>


uint8_t Node::numberOfNodes = 0;

Node::Node(int x, int y)
{
	m_positionX = x;
	m_positionY = y;
}

Node::Node(int value, double x,double y)
{
	m_value = value;
	m_positionX = x;
	m_positionY = y;
}

double Node::GetX() const
{
	return m_positionX;
}

double Node::GetY() const
{
	return m_positionY;
}

int Node::GetValue() const
{
	return m_value;
}

int Node::GetRadius() const
{
	return m_radius;
}

void Node::SetValue(int newValue)
{
	m_value = newValue;
}

void Node::SetPositionX(double x)
{
	m_positionX = x;
}

void Node::SetPositionY(double y)
{
	m_positionY = y;
}

bool Node::operator==(const Node*& node)
{
	return (m_positionX == node->m_positionX && m_positionY == node->m_positionY);
}
