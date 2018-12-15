
#include "stdafx.h"

#include <iostream>

#include "JN_Menu.h"
#include "JN_GameWorld.h"

#include "SDL_mixer.h"

bool JN_Menu::GetQuit()
{
	return isQuit;
}

void JN_Menu::Init()
{
	startBtn = new JN_Button();
	quitBtn = new JN_Button();
	settingsBtn = new JN_Button();
	sfxDownBtn = new JN_Button();
	stfVolUpBtn = new JN_Button();
	bgmVolDownBtn = new JN_Button();
	bgmVolUpBtn = new JN_Button();
	bgmVolTxt = new JN_Text();
	sfxVolTxt = new JN_Text();
	volTitle = new JN_Text();
	backButton = new JN_Button();
	muteBgmBtn = new JN_Button();
	muteSfxBtn = new JN_Button();
}


JN_Menu::~JN_Menu()
{

}

void JN_Menu::Reset()
{
	startBtn->Reset();
	quitBtn->Reset();
	muteBgmBtn->Reset();
	sfxDownBtn->Reset();
	bgmVolUpBtn->Reset();
	bgmVolDownBtn->Reset();
	stfVolUpBtn->Reset();
	settingsBtn->Reset();
	backButton->Reset();
	muteSfxBtn->Reset();
}

void JN_Menu::Run(SDL_Renderer* renderer, std::vector<JN_GameObject*> tiles, JN_GameWorld* gw, JN_Logging* logObj)
{
	Reset();

	isQuit = false;
	isFinished = false;

	allTiles = tiles;
	pTimer = JN_PerformanceTimer(JN_GameWorld::FPS);
	r = renderer;

	startBtn->Init("Start Round", (JN_GameWorld::WORLD_WIDTH / 2) - 200, (JN_GameWorld::WORLD_HEIGHT * 0.25f) - 75, 400, 150, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	settingsBtn->Init("Settings", (JN_GameWorld::WORLD_WIDTH / 2) - 200, (JN_GameWorld::WORLD_HEIGHT * 0.50f) - 75, 400, 150, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	quitBtn->Init("Quit Game", (JN_GameWorld::WORLD_WIDTH / 2) - 200, (JN_GameWorld::WORLD_HEIGHT * 0.75f) - 75, 400, 150, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	volTitle->Init((JN_GameWorld::WORLD_WIDTH / 2) - 190, (JN_GameWorld::WORLD_HEIGHT * 0.15f), 380, 100, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	sfxVolTxt->Init((JN_GameWorld::WORLD_WIDTH / 2) - 75, (JN_GameWorld::WORLD_HEIGHT * 0.25f) + 50, 150, 40, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	sfxDownBtn->Init("<", (JN_GameWorld::WORLD_WIDTH / 2) - 160, (JN_GameWorld::WORLD_HEIGHT * 0.25f) + 50, 40, 40, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	stfVolUpBtn->Init(">", (JN_GameWorld::WORLD_WIDTH / 2) + 120, (JN_GameWorld::WORLD_HEIGHT * 0.25f) + 50, 40, 40, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	bgmVolTxt->Init((JN_GameWorld::WORLD_WIDTH / 2) - 75, (JN_GameWorld::WORLD_HEIGHT * 0.25f) + 115, 150, 40, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	bgmVolUpBtn->Init(">", (JN_GameWorld::WORLD_WIDTH / 2) + 120, (JN_GameWorld::WORLD_HEIGHT * 0.25f) + 115, 40, 40, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	bgmVolDownBtn->Init("<", (JN_GameWorld::WORLD_WIDTH / 2) - 160, (JN_GameWorld::WORLD_HEIGHT * 0.25f) + 115, 40, 40, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	backButton->Init("Back to Menu", (JN_GameWorld::WORLD_WIDTH / 2) - 200, (JN_GameWorld::WORLD_HEIGHT * 0.75f) - 75, 400, 150, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	muteSfxBtn->Init("Disable SFX", (JN_GameWorld::WORLD_WIDTH / 2) - 220, (JN_GameWorld::WORLD_HEIGHT * 0.25f) + 180, 150, 80, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	muteBgmBtn->Init("Disable BGM", (JN_GameWorld::WORLD_WIDTH / 2) + 80, (JN_GameWorld::WORLD_HEIGHT * 0.25f) + 180, 150, 80, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	this->gw = gw;
	this->logObj = logObj;

	while ((!isQuit) && (!isFinished))
	{
		pTimer.Tick();
		Reset();
		Input();
		Update();

		if (stfVolUpBtn->IsClicked())
		{
			logObj->LogMethod("SFX volume up button clicked");
			gw->SetSFXVolume(fminf(100, gw->GetSFXVolume() + 10));
			Mix_Volume(-1, gw->GetSFXVolume());
		}

		if (sfxDownBtn->IsClicked())
		{
			logObj->LogMethod("SFX volume down button clicked");
			gw->SetSFXVolume(fmaxf(0, gw->GetSFXVolume() - 10));
			Mix_Volume(-1, gw->GetSFXVolume());
		}

		if (bgmVolUpBtn->IsClicked())
		{
			logObj->LogMethod("BGM volume up button clicked");
			gw->SetBGMVolume(fminf(100, gw->GetBGMVolume() + 10));
			Mix_VolumeMusic(gw->GetBGMVolume());
		}

		if (bgmVolDownBtn->IsClicked())
		{
			logObj->LogMethod("BGM volume down button clicked");
			gw->SetBGMVolume(fmaxf(0, gw->GetBGMVolume() - 10));
			Mix_VolumeMusic(gw->GetBGMVolume());
		}

		Render();

		this->logObj->Log();

		pTimer.Wait();
	}
}

void JN_Menu::Input()
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			isQuit = true;
			break;

		case SDL_KEYDOWN:
			if (e.key.keysym.scancode == SDL_SCANCODE_F1)
				this->logObj->ToggleLogging();

			if (!isOnSettings)
			{
				if (e.key.keysym.scancode == SDL_SCANCODE_W || e.key.keysym.scancode == SDL_SCANCODE_UP)
				{
					startBtn->SetActive(false);
					quitBtn->SetActive(false);
					settingsBtn->SetActive(false);
					usingMouse = false;
					currentMenu--;
				}

				else if (e.key.keysym.scancode == SDL_SCANCODE_S || e.key.keysym.scancode == SDL_SCANCODE_DOWN)
				{
					startBtn->SetActive(false);
					quitBtn->SetActive(false);
					settingsBtn->SetActive(false);
					usingMouse = false;
					currentMenu++;
				}

				else if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
				{
					switch (currentMenu)
					{
					case 0:
						isFinished = true;
						break;
					case 1:
						isOnSettings = true;
						break;
					case 2:
						isQuit = true;
						break;
					}
				}

				break;

		case SDL_MOUSEBUTTONDOWN:
			usingMouse = true;
			if (!isOnSettings)
			{
				startBtn->Input(e);
				quitBtn->Input(e);
				settingsBtn->Input(e);
			}
			else
			{
				muteSfxBtn->Input(e);
				sfxDownBtn->Input(e);
				muteBgmBtn->Input(e);
				stfVolUpBtn->Input(e);
				bgmVolUpBtn->Input(e);
				bgmVolDownBtn->Input(e);
				backButton->Input(e);
			}
			break;
			}
		}
	}
}


void JN_Menu::Update()
{
	currentMenu = fmin(2, fmax(0, currentMenu));

	if (!isOnSettings)
	{
		if (usingMouse)
		{
			startBtn->Update();
			quitBtn->Update();
			settingsBtn->Update();

			isOnSettings = isOnSettings || settingsBtn->IsClicked();
			isQuit = (isQuit || quitBtn->IsClicked());
			isFinished = startBtn->IsClicked();
		}
		else
		{
			switch (currentMenu)
			{
			case 0:
				startBtn->SetActive(true);
				break;
			case 1:
				settingsBtn->SetActive(true);
				break;
			case 2:
				quitBtn->SetActive(true);
				break;
			}
		}
	}
	else
	{
		stfVolUpBtn->Update();
		muteSfxBtn->Update();
		bgmVolUpBtn->Update();
		muteBgmBtn->Update();
		bgmVolDownBtn->Update();
		sfxDownBtn->Update();
		backButton->Update();

		isOnSettings = !backButton->IsClicked();

		if (muteSfxBtn->IsClicked())
		{
			JN_GameWorld::isSfxMute = !JN_GameWorld::isSfxMute;

			if (JN_GameWorld::isSfxMute)
				muteSfxBtn->SetText("Enable SFX");
			else
				muteSfxBtn->SetText("Disable SFX");
		}

		if (muteBgmBtn->IsClicked())
		{
			// Un-mute
			if (Mix_PausedMusic() == 1)
			{
				Mix_ResumeMusic();
				muteBgmBtn->SetText("Disable BGM");
			}
			// Mute
			else
			{
				Mix_PauseMusic();
				muteBgmBtn->SetText("Enable BGM");
			}
		}
	}
}


void JN_Menu::Render()
{
	for (auto t : allTiles)
		t->Render(r);

	if (!isOnSettings)
	{
		startBtn->Render(r);
		quitBtn->Render(r);
		settingsBtn->Render(r);
	}
	else
	{
		sfxDownBtn->Render(r);
		bgmVolUpBtn->Render(r);
		bgmVolDownBtn->Render(r);
		stfVolUpBtn->Render(r);
		backButton->Render(r);
		muteSfxBtn->Render(r);
		muteBgmBtn->Render(r);

		volTitle->Render(r, "Volume Controls");
		bgmVolTxt->Render(r, "BGM: " + std::to_string(this->gw->GetBGMVolume()));
		sfxVolTxt->Render(r, "SFX: " + std::to_string(this->gw->GetSFXVolume()));
	}


	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);	// Set background color
	SDL_RenderPresent(r);					// Flip the render
}