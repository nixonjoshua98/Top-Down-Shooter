#ifndef JN_PLAYER_CONTROLS_H
#define JN_PLAYER_CONTROLS_H

#include <map>
#include <vector>

#include "JN_Logging.h"

#include "SDL.h"

class JN_PlayerControls {

public:
	///<summary>Default constructor</summary>
	JN_PlayerControls();


	enum class ControlAction { LEFT, RIGHT, UP, DOWN, NONE, SHOOT, SPEED_UP, SPEED_DOWN };	// Enum which stores all player input actions
	enum class InputDevice { KEYBOARD, MOUSE };												// Input types


	///<summary>Init the controls and the logging</summary>
	void Init(JN_Logging *logObj);

	///<summary>Returns a bool based on if the scancode is a valid control</summary>
	bool ValidControl(InputDevice inputDevice, int scancode);


	///<summary>Return the action of the control</summary>
	ControlAction GetControlAction(InputDevice inputDevice, int scancode);


	///<summary>Returns a bool based on if the key is being held down</summary>
	bool IsKeyDown(InputDevice inputDevice, int scancode);


	///<summary>Returns a bool based on if the acion is currently down</summary>
	bool IsKeyDown(InputDevice inputDevice, ControlAction action);


	///<summary>Adds a new key press to the vector</summary>
	void AddKeyPress(InputDevice inputDevice, int scancode);


	///<summary>Remove a key press form the vector</summary>
	void RemoveKeyPress(InputDevice inputDevice, int scancode);


	///<summary>Returns the keyboard control map</summary>
	std::map<int, ControlAction> GetKeyboardControls() { return keyboardControls; }


	///<summary>Returns the mouse control map</summary>
	std::map<int, ControlAction> GetMouseControls() { return mouseControls; }


	///<summary>Returns the keyboard presses</summary>
	std::vector<ControlAction> GetKeyboardPresses() { return keyboardPresses; }


	///<summary>Returns the mouse presses</summary>
	std::vector<ControlAction> GetMousePresses() { return mousePresses; }

	///<summary>Drops all input so the player has a fresh start</summary>
	void EmptyInput();

private:
	std::map<int, ControlAction> keyboardControls = {};	// Map to assign keyboard controls to actions
	std::map<int, ControlAction> mouseControls    = {};	// Assigns mouse buttons to actions

	std::vector<ControlAction> keyboardPresses = {};	// Stores the keyboard presses
	std::vector<ControlAction> mousePresses    = {};	// Stores the mouse clicks which have a command attached

	JN_Logging *logObj = NULL;	// Logging object

};

#endif // !JN_PLAYER_CONTROLS_H
