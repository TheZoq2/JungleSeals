#include "Input.h"

bool i_up;
bool i_down;
bool i_left;
bool i_right;

bool i_activate;

uString i_activateName;

float i_mx;
float i_my;
float i_screenMX;
float i_screenMY;


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
		agk::AddVirtualJoystick(1, 200, agk::GetDeviceHeight() - 200, 250);
		agk::AddVirtualButton(1, agk::GetDeviceWidth() - 200, agk::GetDeviceHeight() - 200, 200);
	}
}

void Input::updateInput()
{
	if(agk::GetRawKeyPressed(87)) { i_up = true; }
	else if(agk::GetVirtualJoystickY(1) < -.5) { i_up = true; }
	else { i_up = false; }

	if(agk::GetRawKeyState(83)) { i_down = true; }
	else { i_down = false; }

	if(agk::GetRawKeyState(65)) { i_left = true; }
	else if(agk::GetVirtualJoystickX(1) < -0.5f) { i_left = true; }
	else { i_left = false; }

	if(agk::GetRawKeyState(68)) { i_right = true; }
	else if(agk::GetVirtualJoystickX(1) > 0.5f){ i_right = true; }
	else { i_right = false; }
	
	if(agk::GetRawKeyPressed(69) == 1){ i_activate = true; }
	else if( agk::GetVirtualButtonPressed(1) == 1 ){ i_activate = true; }
	else { i_activate = false; }

	i_screenMX = agk::GetPointerX();
	i_screenMY = agk::GetPointerY();

	i_mx = agk::ScreenToWorldX(i_screenMX);
	i_my = agk::ScreenToWorldY(i_screenMY);
}