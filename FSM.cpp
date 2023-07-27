#include "FSM.h"
#include "Game.h"
GameState::GameState() :
	m_pGridTex(nullptr),
	m_pRedXTex(nullptr),
	arial11(nullptr),
	arial14(nullptr)

{
	sourceP = new int[2];
	destP = new int[2];
}
GameState::~GameState()
{
}

void GameState::Enter()
{
	cout << "Entering game state..." << endl;

	arial11 = TTF_OpenFont("arial.ttf", 11);
	arial14 = TTF_OpenFont("arial.ttf", 14);

	SDL_Surface* tempSurface;

	tempSurface = IMG_Load("Tiles.png");
	m_pGridTex = SDL_CreateTextureFromSurface(TheGame::Instance()->GetRenderer(), tempSurface);
	SDL_FreeSurface(tempSurface);

	tempSurface = IMG_Load("redx.png");
	m_pRedXTex = SDL_CreateTextureFromSurface(TheGame::Instance()->GetRenderer(), tempSurface);
	SDL_FreeSurface(tempSurface);

	sourceP[0] = 15;
	sourceP[1] = 3;


	destP[0] = 15;
	destP[1] = 5;

	Node* source = &m_pGraph[sourceP[0]][sourceP[1]];
	Node* dest = &m_pGraph[destP[0]][destP[1]];

	this->SetupGrid();
	this->Pathfind(source, dest);
}

void GameState::Update()
{
	// Update methods go here
}

void GameState::Render()
{
	this->ImGui_Render();

	// Render methods go here
	for (int x = 0; x < 32; x++)
	{
		for (int y = 0; y < 24; y++)
		{
			auto& node = m_pGraph[x][y];
			SDL_Rect dstRect = { node.m_x *32, node.m_y *32, 32, 32 };
			if (debugMode && node.m_tileState == tile_state::path)
				SDL_SetTextureColorMod(m_pGridTex, 255, 0, 255);
			else if (debugMode && node.m_tileState == tile_state::searched)
				SDL_SetTextureColorMod(m_pGridTex, 252, 179, 106);
			else
				SDL_SetTextureColorMod(m_pGridTex, node.GetColor().r, node.GetColor().g, node.GetColor().b);
			
			SDL_RenderCopy(TheGame::Instance()->GetRenderer(), m_pGridTex, node.GetSrc(), &dstRect);


			if (debugMode)
			{
				char buffer[50];
				SDL_Color textColor;
				SDL_Surface* ttfSurf;

				if ((node.m_tileState == tile_state::path ||
					node.m_tileState == tile_state::source ||
					node.m_tileState == tile_state::dest) && !showAllCoords)
				{
					textColor = { 0, 255, 255 };
					sprintf_s(buffer, "%i", node.m_iDist);
					ttfSurf = TTF_RenderText_Blended(arial14, buffer, textColor);
				}
				else if (node.m_tileState == tile_state::searched && !showAllCoords)
				{
					textColor = { 255, 255, 255 };
					sprintf_s(buffer, " ");
					ttfSurf = TTF_RenderText_Blended(arial11, buffer, textColor);
					SDL_RenderCopy(TheGame::Instance()->GetRenderer(), m_pRedXTex, NULL, &dstRect);
				}
				else
				{
					textColor = { 255, 255, 255 };
					sprintf_s(buffer, "%i,%i", node.m_x, node.m_y);
					ttfSurf = TTF_RenderText_Blended(arial11, buffer, textColor);
				}

				SDL_Texture* ttfText = SDL_CreateTextureFromSurface(TheGame::Instance()->GetRenderer(), ttfSurf);
				SDL_Rect dRect = { (float)(node.m_x * 32) + 16 - (float)ttfSurf->w / 2.0f, (float)(node.m_y * 32) + 16 - (float)ttfSurf->h / 2.0f, ttfSurf->w, ttfSurf->h };

				SDL_RenderCopy(TheGame::Instance()->GetRenderer(), ttfText, NULL, &dRect);
				SDL_FreeSurface(ttfSurf);
				SDL_DestroyTexture(ttfText);

				if (drawOutlines)
				{
					SDL_SetRenderDrawColor(TheGame::Instance()->GetRenderer(), 0, 255, 0, 255);
					SDL_RenderDrawRect(TheGame::Instance()->GetRenderer(), &dstRect);
				}
			}			
		}
	}

	SDL_SetRenderDrawColor(TheGame::Instance()->GetRenderer(), 0, 0, 255, 255);	
	for (int i = 0; i < path.size(); i++)
	{
		if (i < path.size() - 1)
		{
			SDL_RenderDrawLine(TheGame::Instance()->GetRenderer(), path[i]->m_x * 32+16, path[i]->m_y * 32 + 16, path[i + 1]->m_x * 32 + 16, path[i + 1]->m_y * 32 + 16);
		}
	}
}

void GameState::Exit()
{
	cout << "Exiting game state..." << endl;
}

void GameState::SetupGrid()
{
	fstream fs;
	fs.open("Level.txt");
	if (!fs.is_open())
		cout << "problem opening text file" << endl;


	for (int x = 0; x < 24; x++)
	{
		for (int y = 0; y < 32; y++)
		{
			auto& node = m_pGraph[y][x];
			char c;
			fs >> c;

			node.SetSprite(m_pGridTex, c);
			node.SetColor(255, 255, 255);
			node.SetCost(1.f);
		}
	}
	for (int x = 0; x < 32; x++)
	{
		for (int y = 0; y < 24; y++)
		{
			auto& node = m_pGraph[x][y];
			node.m_x = x;
			node.m_y = y;

			if (y > 0)
				node.AddNeighbour(&m_pGraph[x][y - 1]);		// up
			if (x < 32)	 					
				node.AddNeighbour(&m_pGraph[x + 1][y]);		// right
			if (y < 24)	 					
				node.AddNeighbour(&m_pGraph[x][y + 1]);		// down
			if (x > 0)
				node.AddNeighbour(&m_pGraph[x - 1][y]);		// left
		}
	}
}

void GameState::Pathfind(Node* source, Node* dest)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7);

	for (int x = 0; x < 32; x++)
	{
		for (int y = 0; y < 24; y++)
		{
			auto& node = m_pGraph[x][y];
			node.Reset();
		}
	}
	std::vector<Node*> visitedTiles;
	std::vector<Node*> nodesFront;
	std::vector<Node*> nodesBack;
	source->SetVisited(true);
	nodesFront.push_back(source);

	auto add_to_list = [&](Node* node, Node* neighbour, std::vector<Node*>& nodes, std::vector<Node*>& visitedTiles) {
		neighbour->m_tileState = tile_state::searched;
		neighbour->m_iDist = node->m_iDist + neighbour->GetCost();
		neighbour->m_pPreviousNode = node;
		neighbour->SetVisited(true);
		nodes.push_back(neighbour);
		visitedTiles.push_back(neighbour);
	};

	bool pathfound = false;
	while (!pathfound)
	{
		nodesBack.clear();
		for (auto& current : nodesFront)
		{
			for (auto& neighbour : current->GetNeighbours())
			{
				if (neighbour->GetNavigation() == navigation_type::open && !neighbour->IsVisited() && neighbour != dest)
				{
					add_to_list(current, neighbour, nodesBack, visitedTiles);
				}
				else if (neighbour == dest)
				{
					add_to_list(current, neighbour, nodesBack, visitedTiles);

					infoLog = "PATHFINDER:\nTarget found at: (" + to_string(neighbour->m_x) + "," + to_string(neighbour->m_y) + ")\nManhattan Distance: " + to_string(neighbour->m_iDist);
					pathfound = true;
					break;
				}
			}
		}
		if (pathfound)
			break;
		nodesFront.clear();
		for (auto& current : nodesBack)
		{
			for (auto& neighbour : current->GetNeighbours())
			{				
				if (neighbour->GetNavigation() == navigation_type::open && !neighbour->IsVisited() && neighbour != dest)
				{
					add_to_list(current, neighbour, nodesFront, visitedTiles);					
				}
				else if (neighbour == dest)
				{
					add_to_list(current, neighbour, nodesFront, visitedTiles);

					infoLog = "PATHFINDER:\nTarget found at: (" + to_string(neighbour->m_x) + "," + to_string(neighbour->m_y) + ")\nManhattan Distance: " + to_string(neighbour->m_iDist);
					pathfound = true;
					break;
				}
			}
		}
		if (pathfound)
			break;
	}

	path.clear();
	Node* currentNode = dest;
	currentNode->SetColor(255, 0, 255);
	path.push_back(currentNode);
	while (currentNode != source)
	{
		currentNode->m_tileState = tile_state::path;
		currentNode = currentNode->m_pPreviousNode;
		path.push_back(currentNode);
	}
	source->SetColor(255, 0, 0);
	source->m_tileState = tile_state::source;

	dest->SetColor(0, 0, 255);
	dest->m_tileState = tile_state::dest;
}

void GameState::ImGui_Render()
{
	auto& io = ImGui::GetIO();
	ImGui::Begin("Dijkstra Pathfinding");
	const ImVec2 wPos = ImVec2(0, 0);
	ImGui::SetWindowPos("Dijkstra Pathfinding", wPos);

	ImGui::Text("Pathfinding flags");
	ImGui::Indent();
	ImGui::Checkbox("Debug mode", &debugMode);
	ImGui::Checkbox("Draw outlines", &drawOutlines);
	ImGui::Checkbox("Show coords", &showAllCoords);
	if (ImGui::Button("Calculate path"))
	{
		Node* source = &m_pGraph[sourceP[0]][sourceP[1]];
		Node* dest = &m_pGraph[destP[0]][destP[1]];

		if (sourceP[0] < 0 || sourceP[0] > 31 || sourceP[1] < 0 || sourceP[1] > 23 ||
			destP[0] < 0 || destP[0] > 31 || destP[1] < 0 || destP[1] > 23)
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 12);

			cout << "Source or Dest out coords out of bounds!" << endl;
		}
		else
			this->Pathfind(source, dest);
	}
	ImGui::Unindent();
	ImGui::Separator();
	ImGui::InputInt2("Source", sourceP);
	ImGui::InputInt2("Dest", destP);
	ImGui::Separator();	
	ImGui::Text("Info Log");
	ImGui::Separator();
	auto col = ImVec4(1.f, 0.8f, 0.f, 1.f);
	ImGui::TextColored(col, infoLog.c_str());
		
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::End();

}

FSM::FSM() :
	m_pState(nullptr)
{
}

FSM::~FSM()
{
}

void FSM::Clean()
{
	m_pState->Exit();

	delete m_pState;
	m_pState = nullptr;
}

void FSM::ChangeState(State* changeTo)
{
	if (this->m_pState != nullptr)
		this->m_pState->Exit();
	
	delete m_pState;

	changeTo->Enter();
	this->m_pState = changeTo;
}


void FSM::Update()
{
	this->m_pState->Update();
}

void FSM::Render()
{
	this->m_pState->Render();
}
