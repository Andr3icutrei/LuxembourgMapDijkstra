#include "Edge.h"

Edge::Edge(std::shared_ptr<Node>& node1, std::shared_ptr<Node>& node2):
	m_first{node1},m_second{node2},m_weigth{0}
{
}

Edge::Edge(std::shared_ptr<Node>& node1, std::shared_ptr<Node>& node2, int weigth) :
	m_first{node1},m_second{node2},m_weigth{weigth}
{
}

std::shared_ptr<Node>& Edge::GetFirstNode()
{
	return m_first;
}

std::shared_ptr<Node>& Edge::GetSecondNode()
{
	return m_second;
}

bool Edge::operator==(const Edge& edge)
{
	return (m_first == edge.m_first && m_second == edge.m_second);
}

Edge::~Edge()
{
	m_first = nullptr;
	m_second = nullptr;
}

std::ostream& operator<<(std::ostream& os, const Edge& currEdge)
{
	os << currEdge.m_first->GetValue() << "," << currEdge.m_second->GetValue();
	return os;
}
