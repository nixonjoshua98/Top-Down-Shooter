#ifndef JN_BUTTON_H
#define JN_BUTTON_H

#include "JN_Text.h"
#include "JN_Square.h"
#include "JN_WindowData.h"

#include <string>

class JN_Button
{
public:
	JN_Button();

	~JN_Button();
	

	void Init(std::string txt, int x, int y, int w, int h, SDL_Color btnCol, SDL_Color txtCol, char *fontPath, int size);


	void Render(SDL_Renderer *renderer);


	void Input(SDL_Event e);


	void Resize(int x, int y);


	void Update();


	void SetText(std::string str);


	void Reset();


	bool IsClicked();

private:
	int R, G, B;

	bool isActive = false;
	bool isClicked = false;

	std::string labelText;

	JN_Text* text = NULL;

	JN_Square square;
	JN_Square borderSquare;
};

#endif // !JN_BUTTON_H
