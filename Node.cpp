#include "Node.h"

Node::Node() :
	m_x(0),
	m_y(0),
	m_fCost(0.f),
	m_iDist(0)
{
}

Node::~Node()
{
}


void Node::Reset()
{
	m_tileState = tile_state::none;
	SetColor(255, 255, 255);
	m_visited = false;
}

bool Node::IsVisited()
{
	return m_visited;
}
void Node::SetVisited(bool visited)
{
	this->m_visited = visited;
}

std::vector<Node*>& Node::GetNeighbours()
{
	return m_neighbours;
}

SDL_Color Node::GetColor()
{
	return m_color;
}

float Node::GetCost() const
{
	return m_fCost;
}

navigation_type Node::GetNavigation() const
{
	return m_navType;
}

void Node::SetNavitation(navigation_type type)
{
	this->m_navType = type;
}

void Node::SetCost(float cost)
{
	this->m_fCost = cost;
}

void Node::SetColor(int r, int g, int b)
{
	this->m_color.r = r;
	this->m_color.g = g;
	this->m_color.b = b;
}

void Node::AddNeighbour(Node* node)
{
	m_neighbours.push_back(node);
}

string Node::GetName() const
{
	return "Position: " + to_string(m_x) + "," + to_string(m_y) +
		", Color: " + to_string(m_color.r) + "," + to_string(m_color.b) + "," + to_string(m_color.b) +
		", neighbours size(): " + to_string(m_neighbours.size());
}
