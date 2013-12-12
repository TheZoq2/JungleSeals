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

	editor.setup();
}

void app::Loop (void)
{
	GF::updateTime();
	Input::updateInput();

	uiGroup.update();
	uiGroup.updateScissors();
	
	if(programState == 0) //Loading stuff
	{
		//Setting up the menus
		uiGroup.addWindow("toolbar", "1x1.png", 0, 0, 250.0f, (float)agk::GetVirtualHeight());
		uiGroup.setWindowColor("toolbar", 150, 150, 150, 255);
		std::vector< float > colWidth;
		colWidth.push_back(120.0f);
		colWidth.push_back(20.0f);
		std::vector< std::string > headers;
		headers.push_back("Parts");headers.push_back("key");
		uiGroup.addListToWindow("toolbar", "toolList", 5.0f, 5.0f, 140.0f, agk::GetVirtualHeight() - 5.0f, &colWidth, &headers);

		std::vector< std::string > tool;
		tool.push_back("");
		tool.push_back("");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Partpicker");
		tool.push_back("p");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("");
		tool.push_back("");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Create");
		tool.push_back("c");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Move");
		tool.push_back("m");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Scale");
		tool.push_back("s");
		uiGroup.addToList("toolbar", "toolList", &tool);

		tool.clear();
		tool.push_back("Rotate");
		tool.push_back("r");
		uiGroup.addToList("toolbar", "toolList", &tool);

		programState = 1;
	}
	if(programState == 1)
	{
		toolInput();

		editor.update(selTool, uiGroup.getUIActive());
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
		agk::PrintC("Last keystate: ");agk::Print(agk::GetRawLastKey()); 
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

void app::toolInput()
{
	if(agk::GetRawKeyPressed(80))
	{
		//part picker
		if(uiGroup.getWindowExists("partpicker") == false)
		{
			uiGroup.addWindow("partpicker", "1x1.png", 250.0f, 0, (float)agk::GetVirtualWidth() - 250.0f, (float)agk::GetVirtualHeight());
			uiGroup.setWindowColor("partpicker", 150, 150, 150, 255);
			//Adding the image list to the window
			uiGroup.addImgListToWindow("partpicker", "imgList", 1, 360, 5, (float)agk::GetVirtualWidth() - 350 - 10, (float)agk::GetVirtualHeight() - 10, 150, 150, 255, 255, 255, 150);
		
			//Adding the folder list
			uiGroup.addSimpleListToWindow("partpicker", "folderList", 5, 5, 100, (float) agk::GetVirtualHeight() - 10, "Folder");

			//Reading all images in the media folder
			std::string mediaFolder = "media";

			std::string oldDir = agk::GetCurrentDir();
			agk::SetCurrentDir("media");
			
			std::string file = agk::GetFirstFile();
			while(file.compare("") != 0)
			{
				std::string filepath = mediaFolder;
				filepath.append("/");
				filepath.append(file);

				uiGroup.addImageToImgList("partpicker", "imgList", filepath);

				file = agk::GetNextFile();
			}

			//Reading folers
			std::string folder = agk::GetFirstFolder();
			while(folder.compare("") != 0)
			{
				uiGroup.addToSimpleList("partpicker", "folderList", folder.data());
				
				folder = agk::GetNextFolder();
			}
		}
	}
}