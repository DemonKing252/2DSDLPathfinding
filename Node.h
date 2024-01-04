#pragma once
#include <vector>
#include <memory>
#include "Sprite.h"
#include <iostream>
#include <string>
#include <SDL_ttf.h>
using namespace std;

enum class navigation_type
{
	open,
	closed
};
enum class tile_state
{
	none,
	source,
	dest,
	path,
	searched
};

class Node : public Sprite
{
private:
	float m_fCost;
	SDL_Color m_color;
	navigation_type m_navType;
	std::vector<Node*> m_neighbours;
	bool m_visited = false;
public:
	tile_state m_tileState = tile_state::none;
	Node* m_pPreviousNode = nullptr;

	int m_x;
	int m_y;
	int m_iDist;

	Node();
	~Node();
	
	void Reset();
	bool IsVisited();
	void SetVisited(bool visited);
	
	std::vector<Node*>& GetNeighbours();
	SDL_Color GetColor();
	float GetCost() const;
	navigation_type GetNavigation() const;
	string GetName() const;

	void SetNavitation(navigation_type type);
	void SetCost(float cost);
	void SetColor(int r, int g, int b);
	void AddNeighbour(Node* node);

	bool operator==(Node& rhs)
	{
		return m_x == rhs.m_x && m_y == rhs.m_y;
	}
	bool operator!=(Node& rhs)
	{
		return m_x != rhs.m_x || m_y != rhs.m_y;
	}
};