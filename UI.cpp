#include "UI.h"



UI::UI(void)
{
}
UI::~UI(void)
{
}

void UI::setup()
{
	windows = new std::vector< Window >;
}

int UI::addWindow(std::string bgName, float x, float y, float sizeX, float sizeY)
{
	Window tempWindow;
	int success = tempWindow.create(nextWindow, bgName, x, y, sizeX, sizeY);
	
	if(success == 0) //If the window was created sucessfully
	{
		windows->push_back(tempWindow);

		//Increasing the window amount
		nextWindow++;

		return nextWindow - 1;
	}
	else
	{

	}

	return  - 1;
}

void UI::setWindowColor(int ID, int r, int g, int b, int a)
{
	Window* window = getWindowByID(ID); //Getting a pointer to the window
	
	if(window != NULL)
	{
		window->setBgColor(r, b, g, a);
	}
}

Window* UI::getWindowByID(int ID)
{
	std::vector< Window >::iterator it;
	for(it = windows->begin(); it != windows->end(); it++) //Going through all the windows
	{
		if(it->getID() == ID) //Checking if the window has the correct ID
		{
			return it._Ptr; //Return the pointer for the window
		}
	}

	return NULL;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

int Window::create(int ID, std::string bgName, float x, float y, float sizeX, float sizeY)
{
	this->vecID = ID;

	this->bgName = bgName.data();
	this->x = x;
	this->y = y;

	this->sizeX = sizeX;
	this->sizeY = sizeY;

	//Loading/creating the media

	std::string bgPath = GF::getPath(bgName.data());

	if(agk::GetFileExists(bgPath.data()))
	{
		bgImg = agk::LoadImage(bgPath.data());
		bgSID = agk::CreateSprite(bgImg);

		agk::FixSpriteToScreen(bgSID, 1);

		agk::SetSpritePosition(bgSID, x, y);
		agk::SetSpriteScale(bgSID, sizeX / agk::GetImageWidth(bgImg), sizeY / agk::GetImageHeight(bgImg));

		return 0;
	}
	else
	{
		DebugConsole::addC("Failed to create window, background image did not exist: ");DebugConsole::addToLog(bgPath.data());

		return 1;
	}
}

void Window::setBgColor(int r, int g, int b, int a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;

	agk::SetSpriteColor(bgSID, r, g, b, a);
}

int Window::getID()
{
	return vecID;
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////