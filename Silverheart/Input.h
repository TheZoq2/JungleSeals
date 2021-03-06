#pragma once

#include "agk.h"

//Defining external variables
#ifndef _input_globals
#define _input_globals
	//extern bool i_left;
	//extern bool i_right;
	//extern bool i_up;
	//extern bool i_down;

	//extern bool i_activate;

	extern uString i_activateName;

	extern float i_mx;
	extern float i_my;
	extern float i_screenMX;
	extern float i_screenMY;

	//extern float i_moveX;
#endif

class Input
{
public:
	Input(void);
	~Input(void);

	static void setup();
	static void updateInput();

	static bool debugKey();

	static bool left();
	static bool right();

	static bool up();
	static bool down();

	static bool activate();

	static float moveX();

	static bool shoot(); //Return 1 for pointer state, 2 for press
};

