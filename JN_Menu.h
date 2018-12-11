#ifndef JN_MENU_H
#define JN_MENU_H

#include "JN_GameObject.h"
#include "JN_PerformanceTimer.h"

#include <vector>

class JN_Menu
{
public:
	bool GetQuit();

	void Run(SDL_Renderer* renderer, std::vector<JN_GameObject*>);
	void Input();
	void Render();

private:
	std::vector<JN_GameObject*> allTiles;
	SDL_Renderer* r = NULL;
	JN_PerformanceTimer pTimer;
	bool isQuit = false;
};

#endif // !JN_MENU_H
