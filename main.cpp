#include "Game.h"
int main(int argc, char* args[])
{
	if (!TheGame::Instance()->Run("C++/SDL - Dijkstra's pathfinding algorithm", 1024, 768)) {
		std::cout << "Problem initializing SDL!" << std::endl;
		return -1;
	}
	return 0;
}