#ifndef _H_SMACKIT_
#define _H_SMACKIT_

// Link to GDK libraries
#include "AGK.h"

#include "UI.h"
#include "DebugConsole.h"

#define DEVICE_WIDTH 1600
#define DEVICE_HEIGHT 900
#define DEVICE_POS_X 32
#define DEVICE_POS_Y 32
#define FULLSCREEN false

// Global values for the app
class app
{
	public:

		// constructor
		app() { memset ( this, 0, sizeof(app)); }

		// main app functions - mike to experiment with a derived class for this..
		void Begin( void );
		void Loop( void );
		void End( void );

		int programState;

		UI uiGroup;

		/////////////////////////////////////////////////////////////////////
		//						Debug variables
		/////////////////////////////////////////////////////////////////////
		bool debug;	
		bool fps;
		bool physDebug;

		bool console;
};

extern app App;

#endif

// Allow us to use the LoadImage function name
#ifdef LoadImage
 #undef LoadImage
#endif