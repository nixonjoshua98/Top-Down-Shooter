#include "stdafx.h"


#include "JN_ReadWriteFunctions.h"
#include "JN_PlayerControls.h"

#include <iostream>

#include "json.hpp"

using json = nlohmann::json;

JN_PlayerControls::JN_PlayerControls()
{

}

JN_PlayerControls::~JN_PlayerControls()
{

}


void JN_PlayerControls::Init(JN_Logging *logObj)
{
	json j;

	ReadJson("Data/controls.json", j);

	// :(
	SDL_Scancode left      = SDL_GetScancodeFromKey(SDL_GetKeyFromName(j["left"].get<std::string>().c_str()));
	SDL_Scancode right     = SDL_GetScancodeFromKey(SDL_GetKeyFromName(j["right"].get<std::string>().c_str()));
	SDL_Scancode up        = SDL_GetScancodeFromKey(SDL_GetKeyFromName(j["up"].get<std::string>().c_str()));
	SDL_Scancode down      = SDL_GetScancodeFromKey(SDL_GetKeyFromName(j["down"].get<std::string>().c_str()));
	SDL_Scancode speedUp   = SDL_GetScancodeFromKey(SDL_GetKeyFromName(j["speedUp"].get<std::string>().c_str()));
	SDL_Scancode speedDown = SDL_GetScancodeFromKey(SDL_GetKeyFromName(j["speedDown"].get<std::string>().c_str()));

	keyboardControls[up] = ControlAction::UP;
	keyboardControls[left] = ControlAction::LEFT;
	keyboardControls[down] = ControlAction::DOWN;
	keyboardControls[right] = ControlAction::RIGHT;

	keyboardControls[speedUp] = ControlAction::SPEED_UP;
	keyboardControls[speedDown] = ControlAction::SPEED_DOWN;

	mouseControls[SDL_BUTTON_LEFT] = ControlAction::SHOOT;

	// No one cares about this 
	gamepadControls[1] = ControlAction::SHOOT;

	this->logObj = logObj;
}


bool JN_PlayerControls::ValidControl(InputDevice inputDevice, int scancode)
{
	switch (inputDevice)
	{
	case InputDevice::KEYBOARD:
		return keyboardControls.find(scancode) != keyboardControls.end();
		break;

	case InputDevice::MOUSE:
		return mouseControls.find(scancode) != mouseControls.end();
		break;

	default:
		return false;
	}
}


JN_PlayerControls::ControlAction JN_PlayerControls::GetControlAction(InputDevice inputDevice, int scancode)
{
	switch (inputDevice)
	{
	case InputDevice::KEYBOARD:
		return keyboardControls[scancode];
		break;

	case InputDevice::MOUSE:
		return mouseControls[scancode];
		break;

	default:
		return ControlAction::NONE;
		break;
	}
}


bool JN_PlayerControls::IsKeyDown(InputDevice inputDevice, int scancode)
{
	switch (inputDevice)
	{
	case InputDevice::KEYBOARD:
		return std::find(keyboardPresses.begin(), keyboardPresses.end(), keyboardControls[scancode]) != keyboardPresses.end();
		break;

	case InputDevice::MOUSE:
		return std::find(mousePresses.begin(), mousePresses.end(), mouseControls[scancode]) != mousePresses.end();
		break;

	case InputDevice::GAMEPAD:
		return std::find(gamepadPresses.begin(), gamepadPresses.end(), gamepadControls[scancode]) != gamepadPresses.end();
		break;

	default:
		return false;
		break;
	}
}


void JN_PlayerControls::AddKeyPress(InputDevice inputDevice, int scancode)
{
	switch (inputDevice)
	{
	case InputDevice::KEYBOARD:
		keyboardPresses.push_back(keyboardControls[scancode]);
		logObj->LogKeyboardInput(true, SDL_GetScancodeName(SDL_Scancode(scancode)));
		break;

	case InputDevice::MOUSE:
		mousePresses.push_back(mouseControls[scancode]);
		break;

	case InputDevice::GAMEPAD:
		gamepadPresses.push_back(gamepadControls[scancode]);
		break;
	}
}


void JN_PlayerControls::RemoveKeyPress(InputDevice inputDevice, int scancode)
{
	switch (inputDevice)
	{
	case InputDevice::KEYBOARD:
		logObj->LogKeyboardInput(false, SDL_GetScancodeName(SDL_Scancode(scancode)));
		keyboardPresses.erase(std::find(keyboardPresses.begin(), keyboardPresses.end(), keyboardControls[scancode]));
		break;

	case InputDevice::MOUSE:
		mousePresses.erase(std::find(mousePresses.begin(), mousePresses.end(), mouseControls[scancode]));
		break;
	}
}


bool JN_PlayerControls::IsKeyDown(InputDevice inputDevice, ControlAction action)
{
	switch (inputDevice)
	{
	case InputDevice::KEYBOARD:
		return std::find(keyboardPresses.begin(), keyboardPresses.end(), action) != keyboardPresses.end();
		break;

	case InputDevice::MOUSE:
		return std::find(mousePresses.begin(), mousePresses.end(), action) != mousePresses.end();
		break;

	case InputDevice::GAMEPAD:
		return std::find(gamepadPresses.begin(), gamepadPresses.end(), action) != gamepadPresses.end();
		break;

	default:
		return false;
		break;
	}
}


void JN_PlayerControls::EmptyInput()
{
	keyboardPresses = {};
	mousePresses = {};
	gamepadPresses = {};
}