#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Node.h"
#include <unordered_map>
#include <map>
#include <queue>
#include <fstream>
#include <Windows.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl2.h"
#include "ImGui/imgui_impl_sdlrenderer2.h"

using std::cout;
using std::endl;
class State
{
	friend class FSM;
private:
protected:
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
};

class GameState : public State
{
private:
	Node m_pGraph[32][24];
	SDL_Texture* m_pGridTex;
	SDL_Texture* m_pRedXTex;

	TTF_Font* arial14;
	TTF_Font* arial11;

	std::vector<Node*> path;

	string infoLog;
	bool debugMode = false;
	bool drawOutlines = false;
	bool showAllCoords = false;
	int* sourceP;
	int* destP;

public:
	GameState();
	~GameState();

	void Enter() override;
	void Update() override;
	void Render() override;
	void Exit() override;

	void SetupGrid();
	void Pathfind(Node* source, Node* dest);
	void ImGui_Render();
};

class FSM
{
private:
	State* m_pState;

public:
	FSM();
	~FSM();

	void Clean();
	void ChangeState(class State* changeTo);
	void Update();
	void Render();
};