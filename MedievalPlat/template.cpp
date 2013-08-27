// Includes
#include "template.h"

// Namespace
using namespace AGK;

app App;

void app::Begin(void)
{
	//Setting a random seed for the c++ random function
	srand(time(NULL));

	GF::setupGlobals();

	agk::SetWindowTitle("MedievalPlat");
	agk::SetVirtualResolution (agk::GetDeviceWidth(), agk::GetDeviceHeight());
	//agk::SetDisplayAspect(16.0f/9.0f);
	//agk::SetClearColor( 151,170,204 ); // light blue
	agk::SetSyncRate(0,1);
	//agk::SetScissor(0,0,0,0);

	agk::SetPrintSize(15);

	world.begin();

	player.begin(&world);

	Input::setup();

	//Setting up the item generator
	ItemGen::setup();

	fps = true;

	//Creating the 1x1 sprite
	agk::LoadImage(1, GF::getPath("1x1.png"));
	agk::CreateSprite(1, 1);

	DebugConsole::setup();
}

void app::Loop (void)
{
	Input::updateInput();
	GF::updateTime();

	//Main menu
	if(programState == 0)
	{
		programState = 2; //Starting to load a world
	}
	else if(programState == 1)
	{
		//Main game loop
		//agk::SetViewZoomMode(1);
		agk::SetViewZoom(15);
		player.update();
		world.update(player.getX(), player.getY());

		IngameMenu::update();
	}
	else if(programState == 2)
	{
		uString cWorldName;
		cWorldName.SetStr("levels/snow1");

		world.load(cWorldName);

		player.load(GF::getPath("Bound.png"));

		//player.spawn("start");
		//Running the first spawn script
		Script::run("scripts/firstSpawn", 0, &world, &player);

		programState = 1;

		player.addItem(ItemGen::generate(1));
	}


	///////////////////////////////////////////////////////////////
	//						Debug output
	///////////////////////////////////////////////////////////////
	if(agk::GetRawKeyPressed(112))
	{
		GF::changeState(&fps);
	}

	if(agk::GetRawKeyPressed(113))
	{
		GF::changeState(&debug);
	}

	if(agk::GetRawKeyPressed(114)){ GF::changeState(&physDebug); }

	//Printing fps info
	if(fps)
	{
		agk::PrintC("FPS:  ");agk::Print(agk::ScreenFPS());
	}

	//Printing debug info
	if(debug == true)
	{
		agk::Print("");
		agk::PrintC("Physics time: ");agk::Print(agk::GetPhysicsTime());
		agk::PrintC("Drawing setup time: ");agk::Print(agk::GetDrawingSetupTime());
		agk::PrintC("Drawing time: ");agk::Print(agk::GetDrawingTime());
		agk::Print("");
		agk::PrintC("Speedmod: ");agk::Print(speedMod);
		agk::PrintC("Active keystate: "); agk::Print(i_activate);
		agk::PrintC("Pointer x: ");agk::PrintC(i_mx);agk::PrintC("   PointerY: ");agk::Print(i_my);
	}

	if(physDebug == true)
	{
		agk::SetPhysicsDebugOn();
	}else
	{
		agk::SetPhysicsDebugOff();
	}

	//Updating the debug console
	if(Input::debugKey())
	{
		GF::changeState(&console);
	}

	if(console == true)
	{
		DebugConsole::setVisible(1);
	}
	else
	{
		DebugConsole::setVisible(0);
	}

	//Syncing

	agk::Update2D();
	//agk::Render();
	agk::Render2DBack();
	agk::Render2DFront();
	agk::Swap();
	//agk::Sync();
}

void app::End (void)
{

}
