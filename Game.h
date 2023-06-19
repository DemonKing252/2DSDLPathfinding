#pragma once
#include "FSM.h"
#define FPS 60
class Game
{ 
private:
	Game();
	~Game();

	static Game* s_pInstance;
	SDL_Window* m_pWindow = NULL;
	SDL_Renderer* m_pRenderer = NULL;
	bool m_bRunning;
	bool m_bShowImGuiWindow;

	FSM* m_pFSM;

	bool Init_SDL(const char* title, int width, int height);
	bool Init_ImGui();

	void PollEvents();
	void Update();
	void Render();
	void Clean();
public:

	SDL_Renderer* GetRenderer();
	bool Run(const char* title, int width, int height);

	static Game* Instance() 
	{
		
		if (s_pInstance == nullptr) 
		{
			s_pInstance = new Game();
		}
		return s_pInstance;
	}

};

typedef class Game TheGame;