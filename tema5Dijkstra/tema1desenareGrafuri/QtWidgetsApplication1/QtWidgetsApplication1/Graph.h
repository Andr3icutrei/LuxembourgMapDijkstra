#pragma once
#include "Edge.h"
#include <vector>

class Graph
{
private:
	std::vector<std::shared_ptr<Node>> m_nodes;
	std::vector<Edge> m_edges;
	std::vector<std::vector<Edge>> m_adjList;
	bool m_isOriented = true;
public:
	Graph()=default;
	Graph(const Graph&) = default;
	Graph& operator=(const Graph&) = default;
	~Graph()=default;

	void addNode(int x,int y);
	void addNode(std::shared_ptr<Node>& currentNode);
	void addEdge(std::shared_ptr<Node>& node1, std::shared_ptr<Node>& node2);
	void addEdge(std::shared_ptr<Node>& source,std::shared_ptr<Node>& dest, int weigth);

	bool nodesOverlap(std::shared_ptr<Node>& node1, std::shared_ptr<Node>& node2);

	void writeAdjList();

	void SetOrientation(bool orientation) { m_isOriented = orientation; }

	std::vector<std::vector<Edge>>& GetAdjList() { return m_adjList; }
	std::vector<std::shared_ptr<Node>>& GetNodes();
	std::vector<Edge>& GetEdges();
	bool GetIsOriented() const { return m_isOriented; }
};

