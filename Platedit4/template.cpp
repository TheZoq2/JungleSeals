// Includes
#include "template.h"

// Namespace
using namespace AGK;

app App;

void app::Begin(void)
{
	agk::SetVirtualResolution (1600, 900);
	agk::SetClearColor( 151,170,204 ); // light blue
	agk::SetSyncRate(60,0);
	agk::SetScissor(0,0,0,0);
	agk::SetPrintSize(16);
	agk::SetPrintColor(255, 0, 0);

	//Loading the 1x1 sprite
	agk::LoadImage(1,"media/1x1.png");
	agk::CreateSprite(1, 1);

	//Class setup
	GF::setupGlobals();

	DebugConsole::setup();
	uiGroup.setup();
}

void app::Loop (void)
{
	GF::updateTime();
	
	if(programState == 0) //Loading stuff
	{
		programState = 1;

		//Setting up the menus
		uiGroup.addWindow("toolbar", "1x1.png", 0, 0, 250, agk::GetVirtualHeight());
		uiGroup.setWindowColor("toolbar", 150, 150, 150, 255);
	}
	if(programState == 1)
	{

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
		agk::PrintC("Managed sprites: ");agk::Print(agk::GetManagedSpriteCount());
		agk::Print("");
		agk::PrintC("Speedmod: ");agk::Print(speedMod);
		agk::PrintC("Active keystate: "); agk::Print(Input::activate());
		agk::PrintC("Pointer x: ");agk::PrintC(i_mx);agk::PrintC("   PointerY: ");agk::Print(i_my);
		agk::PrintC("View zoom: ");agk::Print(agk::GetViewZoom());
		agk::PrintC("Global time: ");agk::Print(globaltime);
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

	agk::Sync();
}


void app::End (void)
{

}
