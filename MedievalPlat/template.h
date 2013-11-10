#ifndef _H_SMACKIT_
#define _H_SMACKIT_

#include <time.h>

// Link to GDK libraries
#include "AGK.h"

#include "GF.h"
#include "Input.h"
#include "world.h"
#include "Player.h"
#include "Script.h"
#include "Item.h"
#include "IngameMenu.h"
#include "DebugConsole.h"
#include "DebugInput.h"
#include "NPC.h"
#include "Projectile.h"
#include "Particle.h"
#include "UI.h"

#include "Enemy.h"

#include <vector>

#ifdef DEVICE_BIG
	#define DEVICE_WIDTH 1600
	#define DEVICE_HEIGHT 900
#else
	#define DEVICE_WIDTH 1280
	#define DEVICE_HEIGHT 720
#endif
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

		void calculateLight();
	private:
		int programState;

		World world;
		Player player;

		NPCGroup npcGroup;
		ProjectileGroup projGroup;
		ParticleGroup partGroup;

		UI mainUI;

		bool initLoaded;

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