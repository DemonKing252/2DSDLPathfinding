#include "Game.h"

#include "ImGui/imgui.h"
Game* Game::s_pInstance = nullptr;

Game::Game() : 
	m_bRunning(false),
	m_pFSM(nullptr),
	m_bShowImGuiWindow(false)
{

}

Game::~Game()
{
}

bool Game::Run(const char* title, int width, int height)
{
	if (!this->Init_SDL(title, width, height))
		return false;

	if (!this->Init_ImGui())
		return false;
	else
	{
		m_bShowImGuiWindow = true;
		m_bRunning = true;
		m_pFSM = new FSM();
		m_pFSM->ChangeState(new GameState());

		while (m_bRunning)
		{
			SDL_Delay(16);
			this->PollEvents();
			this->Update();
			this->Render();
		}
		this->Clean();
	}
	return true;
}


bool Game::Init_SDL(const char* title, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
		return false;

	m_pWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

	if (m_pWindow == NULL)
		return false;

	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	
	if (m_pRenderer == NULL)
		return false;

	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == NULL)
		return false;

	if (TTF_Init() != 0)
		return false;

	cout << "SDL Init success!" << endl;

	return true;
}

bool Game::Init_ImGui()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();

	io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(m_pWindow, m_pRenderer);
	ImGui_ImplSDLRenderer2_Init(m_pRenderer);

	return true;
}


void Game::PollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			m_bRunning = false;			
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_bRunning = false;
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			
			}
			break;
		}
	}
}

SDL_Renderer* Game::GetRenderer()
{
	return m_pRenderer;
}

void Game::Update()
{
	m_pFSM->Update();
}

void Game::Render()
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	SDL_RenderClear(m_pRenderer);
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

	m_pFSM->Render();

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(m_pRenderer);
}

void Game::Clean()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	m_pFSM->Clean();
	delete m_pFSM;
	m_pFSM = nullptr;

	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}
