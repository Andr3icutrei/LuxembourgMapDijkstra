#pragma once
#include <qpoint.h>
#include <cstdint>
#include <QMouseEvent>

class Node
{
private:
	double m_positionX;
	double m_positionY;
	int m_value;
	bool m_isDragging;
public:

	static uint8_t numberOfNodes;
	static const uint8_t m_radius=15;

	Node(int x,int y);
	Node(int value,double x,double y);

	double GetX() const;
	double GetY() const;
	int GetValue() const;
	int GetRadius() const;

	void SetValue(int newValue);
	void SetPositionX(double x);
	void SetPositionY(double y);

	bool operator==(const Node * &node);
};
