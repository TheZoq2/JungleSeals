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
	agk::SetSyncRate(0,1);
	agk::SetClearColor(255, 0, 150);

	agk::SetPrintSize(15);
	agk::SetPrintColor(255, 0, 0, 255);

	world.begin();

	player.begin(&world);

	Input::setup();

	npcGroup.setup();

	//Setting up the item generator
	ItemGen::setup();

	fps = true;

	//Creating the 1x1 sprite
	agk::LoadImage(1, GF::getPath("1x1.png"));
	agk::CreateSprite(1, 1);
	agk::SetSpriteVisible(1, 0);

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
		player.update();

		world.update(player.getX(), player.getY());

		IngameMenu::update();
	}
	else if(programState == 2)
	{
		agk::SetViewZoom(15);

		uString cWorldName;
		cWorldName.SetStr("levels/snow1");

		world.load(cWorldName);
		world.loadBaseMedia();

		player.load(GF::getPath("Bound.png"));

		//enemy->at(0).load(GF::getPath("Bound.png"));
		//enemy->at(0).spawn("entrance");

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
		agk::PrintC("Active keystate: "); agk::Print(Input::activate());
		agk::PrintC("Pointer x: ");agk::PrintC(i_mx);agk::PrintC("   PointerY: ");agk::Print(i_my);
		agk::PrintC("View zoom: ");agk::Print(agk::GetViewZoom());
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
		DebugConsole::update(&world, &player, &npcGroup);
	}
	else
	{
		DebugConsole::setVisible(0);
	}

	//Syncing

	agk::Update2D(); //Updating the game

	/////////////////////////////////////////////////////////////////////////
	//							Light calculation
	/////////////////////////////////////////////////////////////////////////'

	/*
	//Turning the light mode on
	world.setLightModeOn();

	agk::Render(); //Rendering the image
	//Changing the display aspect
	float lightScale = 10.0f;
	agk::SetViewZoom(15 / lightScale);

	//Calculating the cordinates of the new image
	float h = agk::GetVirtualHeight(); float w = agk::GetVirtualWidth();
	float imgHeight = h / lightScale;
	float imgWidth = w / lightScale;

	//Getting the image
	int lightImg = agk::GetImage(w / 2.0f - imgWidth / 2.0f, h / 2.0f - imgHeight / 2.0f, imgWidth, imgHeight);

	int sprite = agk::CreateSprite(lightImg);
	agk::FixSpriteToScreen(sprite, 1);
	//agk::SetSpriteScale(sprite, lightScale, lightScale);

	//Resetting the world
	world.setLightModeOff();*/
	//agk::SetViewZoom(15);

	//calculateLight();
	/////////////////////////////////////////////////////////////////////////

	//agk::Render();

	agk::Render2DBack();
	agk::Render3D();
	agk::Render2DFront();
	agk::Swap();

	/*agk::DeleteImage(lightImg);
	agk::DeleteSprite(sprite);*/
}

void app::End (void)
{

}

void app::calculateLight()
{
	/*
	//Turning the light mode on
	world.setLightModeOn();

	agk::Render2DFront(); //Rendering the image
	//Changing the display aspect
	float lightScale = 10.0f;
	agk::SetViewZoom(15 / lightScale);

	//Calculating the cordinates of the new image
	float h = agk::GetVirtualHeight(); float w = agk::GetVirtualWidth();
	float imgHeight = h / lightScale;
	float imgWidth = w / lightScale;

	//Getting the image
	int lightImg = agk::GetImage(w / 2.0f - imgWidth / 2.0f, h / 2.0f - imgHeight / 2.0f, imgWidth, imgHeight);

	agk::ClearScreen();

	int sprite = agk::CreateSprite(lightImg);
	agk::FixSpriteToScreen(sprite, 1);
	//agk::SetSpriteScale(sprite, lightScale, lightScale);

	//Resetting the world
	world.setLightModeOff();

	agk::SetViewZoom(15);
	*/
}