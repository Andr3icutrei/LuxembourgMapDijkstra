#pragma once
#include "Node.h"
#include <iostream>
#include <memory>

class Edge
{
private:
	std::shared_ptr<Node> m_first;
	std::shared_ptr<Node> m_second;
	int m_weigth;
public:
	Edge();
	Edge(std::shared_ptr<Node>& node1, std::shared_ptr<Node>& node2);
	Edge(std::shared_ptr<Node>& node1, std::shared_ptr<Node>& node2, int weigth);

	std::shared_ptr<Node>& GetFirstNode() ;
	std::shared_ptr<Node>& GetSecondNode() ;

	bool operator==(const Edge& edge);
	~Edge();
	friend std::ostream& operator<<(std::ostream& os, const Edge& currEdge);
};

