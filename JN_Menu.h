#ifndef JN_MENU_H
#define JN_MENU_H

#include "JN_GameObject.h"
#include "JN_PerformanceTimer.h"
#include "JN_Text.h"
#include "JN_Button.h"

#include <vector>

class JN_Menu
{
public:
	~JN_Menu();
	bool GetQuit();

	void Init();
	void Run(SDL_Renderer* renderer, std::vector<JN_GameObject*>);
	void Input();
	void Update();
	void Reset();
	void Render();

private:
	std::vector<JN_GameObject*> allTiles;
	SDL_Renderer* r = NULL;

	JN_Button* startBtn = NULL;
	JN_Button* quitBtn = NULL;
	JN_Button* settingsBtn = NULL;


	JN_PerformanceTimer pTimer;

	bool isQuit = false;
	bool isFinished = false;

};

#endif // !JN_MENU_H
