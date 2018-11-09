#include "stdafx.h"

#include "JN_PlayerControls.h"

#include <iostream>

void JN_PlayerControls::SetControls()
{
	keyboardControls[SDL_SCANCODE_W] = ControlAction::UP;
	keyboardControls[SDL_SCANCODE_A] = ControlAction::LEFT;
	keyboardControls[SDL_SCANCODE_S] = ControlAction::DOWN;
	keyboardControls[SDL_SCANCODE_D] = ControlAction::RIGHT;

	mouseControls[SDL_BUTTON_LEFT] = ControlAction::SHOOT;
}


bool JN_PlayerControls::ValidControl(InputDevice inputDevice, int scancode)
{
	switch (inputDevice)
	{
	case InputDevice::KEYBOARD:
		return keyboardControls.find(scancode) != keyboardControls.end();

	case InputDevice::MOUSE:
		return mouseControls.find(scancode) != mouseControls.end();

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

	case InputDevice::MOUSE:
		return mouseControls[scancode];

	default:
		return ControlAction::NONE;
	}
}


bool JN_PlayerControls::IsKeyDown(InputDevice inputDevice, int scancode)
{
	switch (inputDevice)
	{
	case InputDevice::KEYBOARD:
		return std::find(keyboardPresses.begin(), keyboardPresses.end(), keyboardControls[scancode]) != keyboardPresses.end();

	case InputDevice::MOUSE:
		return std::find(mousePresses.begin(), mousePresses.end(), mouseControls[scancode]) != mousePresses.end();

	default:
		return false;
	}
}


void JN_PlayerControls::AddKeyPress(InputDevice inputDevice, int scancode)
{
	switch (inputDevice)
	{
	case InputDevice::KEYBOARD:
		keyboardPresses.push_back(keyboardControls[scancode]);
		break;

	case InputDevice::MOUSE:
		mousePresses.push_back(mouseControls[scancode]);
		break;
	}
}


void JN_PlayerControls::RemoveKeyPress(InputDevice inputDevice, int scancode)
{
	switch (inputDevice)
	{
	case InputDevice::KEYBOARD:
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

	case InputDevice::MOUSE:
		return std::find(mousePresses.begin(), mousePresses.end(), action) != mousePresses.end();

	default:
		return false;
	}
}