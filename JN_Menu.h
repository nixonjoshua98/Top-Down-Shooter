#ifndef JN_MENU_H
#define JN_MENU_H

#include "JN_GameObject.h"
#include "JN_PerformanceTimer.h"
#include "JN_Text.h"
#include "JN_Button.h"
#include "JN_Logging.h"
#include "JN_GameWorld.h"

#include <vector>

class JN_Menu
{
public:
	//JN_Menu();
	~JN_Menu();
	bool GetQuit();

	void Init();
	void Run(SDL_Renderer* renderer, std::vector<JN_GameObject*>, JN_GameWorld* gw, JN_Logging* logObj);
	void Input();
	void Update();
	void Reset();
	void Render();

private:
	int currentMenu = 0;
	bool usingMouse = true;

	bool isOnSettings = false;
	std::vector<JN_GameObject*> allTiles;

	SDL_Renderer* r = NULL;
	JN_Logging* logObj;

	JN_Button* startBtn = NULL;
	JN_Button* quitBtn = NULL;
	JN_Button* settingsBtn = NULL;

	JN_Button *sfxDownBtn = NULL;
	JN_Button* stfVolUpBtn = NULL;

	JN_Button* bgmVolUpBtn = NULL;
	JN_Button* bgmVolDownBtn = NULL;

	JN_Text* sfxVolTxt = NULL;
	JN_Text* bgmVolTxt = NULL;

	JN_Text* volTitle = NULL;

	JN_Button* backButton = NULL;

	JN_GameWorld* gw = NULL;

	JN_PerformanceTimer pTimer;

	bool isQuit = false;
	bool isFinished = false;

};

#endif // !JN_MENU_H
