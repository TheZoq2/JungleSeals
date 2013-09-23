#include "Input.h"

#include "DebugConsole.h"
/*bool i_up;
bool i_down;*/
//bool i_left;
//bool i_right;

//bool i_activate;

uString i_activateName;

float i_mx;
float i_my;
float i_screenMX;
float i_screenMY;

//float i_moveX;


Input::Input(void)
{
}


Input::~Input(void)
{
}

void Input::setup()
{
	i_activateName = "E";

	char* deviceName = agk::GetDeviceName();
	if(strcmp(deviceName, "WSindows"))
	{
		//agk::AddVirtualJoystick(1, 200.0f, agk::GetDeviceHeight() - 200.0f, 250.0f);
		//agk::AddVirtualButton(1, agk::GetDeviceWidth() - 200.0f, agk::GetDeviceHeight() - 200.0f, 200.0f);
	}
}

void Input::updateInput()
{

	/*if(agk::GetRawKeyState(83)) { i_down = true; }
	else { i_down = false; }*/

	i_screenMX = agk::GetPointerX();
	i_screenMY = agk::GetPointerY();

	i_mx = agk::ScreenToWorldX(i_screenMX);
	i_my = agk::ScreenToWorldY(i_screenMY);
}

bool Input::debugKey()
{
	if(agk::GetRawKeyPressed(220))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Input::left()
{
	bool left = false;
	//A key
	if(agk::GetRawKeyState(65)) { left = true; }
	//Virtual joystick
	if(agk::GetVirtualJoystickExists(1))
	{
		if(agk::GetVirtualJoystickX(1) < -0.5f) { left = true; }
	}
	//Raw joystick
	if(agk::GetRawJoystickExists(1))
	{
		if(agk::GetRawJoystickX(1) < -0.5f) { left = true; }
	}

	return left;
}
bool Input::right()
{
	bool right = false;
	//Right
	if(agk::GetRawKeyState(68)) { right = true; }
	//Virtual joystick
	if(agk::GetVirtualJoystickExists(1))
	{
		if(agk::GetVirtualJoystickX(1) > 0.5f){ right = true; }
	}
	//Real joystick
	if(agk::GetRawJoystickExists(1))
	{
		if(agk::GetRawJoystickX(1) > 0.5f) { right = true; }
	}

	return right;
}

bool Input::up()
{
	//Jumping
	bool up = false;

	//W
	if(agk::GetRawKeyPressed(87)) { up = true; }
	//Space
	if(agk::GetRawKeyState(32)) { up = true; }
	
	//Joystick button
	if(agk::GetRawJoystickExists(1))
	{
		if(agk::GetRawJoystickButtonPressed(1, 1)) { up = true; }
	}
	//Virtual joystick up
	if(agk::GetVirtualJoystickExists(1))
	{
		if(agk::GetVirtualJoystickY(1) < -.5) { up = true; }
	}

	return up;
}

bool Input::activate()
{
	bool activate = false;

	//A button on controller
	if(agk::GetRawJoystickExists(1))
	{
		if( agk::GetRawJoystickButtonPressed(1, 4) ){ activate = true; }
	}

	//E key
	if(agk::GetRawKeyPressed(69) == 1){ activate = true; }
	//Virtual joystick
	if(agk::GetVirtualButtonExists(1))
	{
		if( agk::GetVirtualButtonPressed(1) == 1 ){ activate = true; }
	}

	return activate;
}

float Input::moveX()
{
	float moveX = 0;
	if(agk::GetRawJoystickExists(1))
	{
		moveX = agk::GetRawJoystickX(1);
	}
	return moveX;
}