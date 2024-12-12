#include "Graph.h"
#include <fstream>
#include <qdebug.h>

void Graph::addNode(int x,int y)
{
	std::shared_ptr<Node> currNode{ new Node{ x,y } };
	currNode->SetValue(++Node::numberOfNodes);
	m_nodes.emplace_back(currNode);
	m_adjList.resize(m_nodes.size());

}

void Graph::addNode(std::shared_ptr<Node>& currentNode)
{
	m_nodes.emplace_back(currentNode);

	m_adjList.push_back({});
}

void Graph::addEdge(std::shared_ptr<Node>& node1, std::shared_ptr<Node>& node2)
{
	Edge currEdge{ node1, node2 };
	m_edges.emplace_back(currEdge);

	if (m_isOriented == false)
	{
		Edge invertedEdge(node2, node1);
		m_adjList[node2->GetValue() ].emplace_back(invertedEdge);
	}

	m_adjList[node1->GetValue()].emplace_back(currEdge);
}

void Graph::addEdge(std::shared_ptr<Node>& node1,std::shared_ptr<Node>& node2, int weigth)
{
	Edge currEdge{ node1,node2 ,weigth};
	m_edges.emplace_back(currEdge);

	if (m_isOriented == false)
	{
		Edge invertedEdge(node2, node1);
		m_adjList[node2->GetValue() ].emplace_back(invertedEdge);
	}
	m_adjList[node1->GetValue()].emplace_back(currEdge);
}

std::vector<std::shared_ptr<Node>>& Graph::GetNodes()
{
	return m_nodes;
}

std::vector<Edge>& Graph::GetEdges()
{
	return m_edges;
}

void Graph::writeAdjList()
{
	std::ofstream fout("adjList.txt");
	for (int i = 0; i < m_adjList.size(); ++i)
	{
		fout << i + 1 << ":";
		for (int j = 0; j < m_adjList[i].size(); ++j)
		{
			fout << '(' << m_adjList[i][j] << "),";
		}
		fout << '\n';
	}

	fout.close();
}


bool Graph::nodesOverlap(std::shared_ptr<Node>& node1, std::shared_ptr<Node>& node2)
{
    int x1 = node1->GetX();
    int y1 = node1->GetY();
    int x2 = node2->GetX();
    int y2 = node2->GetY();

    double distance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));

    return distance <= 2 * Node::m_radius;
}


