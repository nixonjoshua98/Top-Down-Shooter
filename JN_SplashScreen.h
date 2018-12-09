#ifndef JN_SPLASHSCREEN_H
#define JN_SPLASHSCREEN_H

#include "JN_Text.h"
#include "JN_GameObject.h"
#include "JN_ReadWriteFunctions.h"

#include "json.hpp"

#include <SDL.h>
#include <vector>

using json = nlohmann::json;

class JN_SplashScreen
{
public:
	void Run(SDL_Renderer* render, std::vector<JN_GameObject*> tiles, int duration);

	bool GetQuit()
	{
		return isQuit;
	}

protected:

private:
	json controls;

	bool isQuit = false;

	void Input();
	void Render(SDL_Renderer* render);

	JN_Text* titleTxt = NULL;
	JN_Text* shootTxt = NULL;
	JN_Text* speedControlsTxt = NULL;
	JN_Text* movementControlsTxt = NULL;

	std::vector<JN_GameObject*> allTiles;

};

#endif // !JN_SPLASHSCREEN_H
