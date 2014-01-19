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

void UI::update()
{
	std::vector< Window >::iterator windowIt;
	std::vector< Window >::iterator hitIt;

	bool windowHit = false;
	for(windowIt = windows->begin(); windowIt != windows->end(); windowIt++) //Going thru all the windows
	{
		if(windowIt->update(i_mx, i_my))
		{
			//The mouse is on this window
			hitIt = windowIt;

			windowHit = true;
		}
	}

	if(windowHit == true) //If a window was hit
	{
		hitIt->updateInput(i_mx, i_my);
	}
}
void UI::updateScissors()
{
	for(unsigned int i = 0; i < windows->size(); i++)
	{
		windows->at(i).updateScissors();
	}
}

void UI::addWindow(std::string ID, std::string bgName, float x, float y, float sizeX, float sizeY)
{
	Window tempWindow;
	tempWindow.create(ID, bgName, x, y, sizeX, sizeY);

	windows->push_back(tempWindow);
}
void UI::addListToWindow(std::string window, std::string listID, float offsetX, float offsetY, float width, float height, std::vector< float >* colWidth, std::vector< std::string >* colHeaders)
{
	//Adding the list
	getWindowByID(window)->addList(listID, offsetX, offsetY, width, height, colWidth, colHeaders);
}
void UI::addToList(std::string windowID, std::string listID, std::vector< std::string >* values)
{
	//Finding the window
	Window* window = getWindowByID(windowID);

	if(window != NULL)
	{
		window->addToList(listID, values);
	}
	else
	{
		DebugConsole::addC("Failed to add item to list, window: ");DebugConsole::addC(windowID.data());
		DebugConsole::addToLog(" did not exist");
	}
}

void UI::setWindowColor(std::string ID, int r, int g, int b, int a)
{
	Window* window = getWindowByID(ID); //Getting a pointer to the window
	
	if(window != NULL)
	{
		window->setBgColor(r, b, g, a);
	}
}

bool UI::getWindowExists(std::string ID)
{
	Window* window = getWindowByID(ID);

	if(window != NULL) //Checking if the window did exist
	{
		return true;
	}

	return false;
}

//Private function
Window* UI::getWindowByID(std::string ID)
{
	std::vector< Window >::iterator it;
	for(it = windows->begin(); it != windows->end(); it++) //Going through all the windows
	{
		if(strcmp(it->getID().data(), ID.data()) == 0) //Checking if the window has the correct ID
		{
			return it._Ptr; //Return the pointer for the window
		}
	}

	return NULL;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void Window::create(std::string vecID, std::string bgName, float x, float y, float sizeX, float sizeY)
{
	this->vecID = vecID;

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
	}
	else
	{
		DebugConsole::addC("Failed to create window, background image did not exist: ");DebugConsole::addToLog(bgPath.data());
	}

	nextList = 0;

	//Initialising vectors
	lists = new std::vector< List >;
}
int Window::update(float mx, float my)
{
	//Converting the cordinates to world cordinates
	float startX = agk::ScreenToWorldX(x);
	float endX = agk::ScreenToWorldX(x + sizeX);
	float startY = agk::ScreenToWorldY(y);
	float endY = agk::ScreenToWorldY(y + sizeY);

	if(mx > startX && mx < endX && my > startY && my < endY) //If the mouse is on the window
	{
		return true; 
	}
	return false;
}
void Window::updateInput(float mx, float my)
{
	//Updating individual UI elements

	//Lists
	for(unsigned int i = 0; i < lists->size(); i++)
	{
		lists->at(i).updateInput(mx, my);
	}
}
void Window::remove()
{
	//Removing vectors
	lists->clear();
	delete lists;
}
void Window::updateScissors()
{
	for(unsigned int i = 0; i < lists->size(); i++)
	{
		lists->at(i).updateScissors();
	}
}

void Window::addList(std::string listID, float offsetX, float offsetY, float width, float height, std::vector< float >* colWidth, std::vector< std::string >* colHeaders)
{
	//Caluclating the position of the list
	float listX = x + offsetX;
	float listY = y + offsetY;

	List tempList;
	tempList.create(listID, listX, listY, width, height, colWidth, colHeaders);
	
	lists->push_back(tempList);
	
}
void Window::addToList(std::string listID, std::vector< std::string >* values)
{
	List* list = findListByID(listID);
	
	if(list != NULL) //Making sure the list exists before modifying it
	{
		list->addItem(values);
	}
	else
	{
		DebugConsole::addC("Failed to add item to the list: ");DebugConsole::addC(listID.data());
		DebugConsole::addToLog(". List did not exist");
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

std::string Window::getID()
{
	return vecID;
}

List* Window::findListByID(std::string ID)
{
	//Looping thru the list vector
	for(unsigned int i = 0; i < lists->size(); i++)
	{
		if(strcmp(lists->at(i).getID().data(), ID.data()) == 0)
		{
			return &lists->at(i);
		}
	}
	
	return NULL;
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void List::create(std::string vecID, float x, float y, float width, float height, std::vector< float >* colWidth, std::vector< std::string >* colHeaders)
{
	this->vecID = vecID;

	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	//Initialising vectors
	listItems = new std::vector< ListItem >;
	this->colWidth = new std::vector< float > (*colWidth); //Cloning the colWidth vector

	//Creating the header
	header.create(colWidth, colHeaders, x, y);

	rowHeight = 13;
	nextItemY = y + rowHeight;
}

int List::addItem(std::vector< std::string >* values)
{
	ListItem tempItem; //Creating a tempororary item

	tempItem.create(colWidth, values, x, nextItemY);

	listItems->push_back(tempItem);

	nextItemY = nextItemY + rowHeight;

	return 0;
}
void List::updateInput(float mx, float my)
{
	//Checking if the list is hit
	//Converting the cordinates to world coords
	float startX = agk::ScreenToWorldX(x);
	float startY = agk::ScreenToWorldY(y);
	float endX = agk::ScreenToWorldX(x + width);
	float endY = agk::ScreenToWorldY(y + height);

	//Checking for collision
	if(mx > startX && my > startY && mx < endX && my < endY)
	{
		
	}
}
void List::updateScissors()
{
	for(unsigned int i = 0; i < listItems->size(); i++)
	{
		listItems->at(i).setScissor(agk::ScreenToWorldX(x), agk::ScreenToWorldY(y), agk::ScreenToWorldX(x + width), agk::ScreenToWorldY(y + height));
	}
}

std::string List::getID()
{
	return vecID;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void ListItem::create(std::vector< float >* colWidth, std::vector< std::string >* values, float x, float y)
{
	//initialising vectors
	cells = new std::vector< ListCell >;

	float nextXPos = x; //The next position to create a cell at
	//Going thru the colWidth vector and creating cells
	for(unsigned int i = 0; i < colWidth->size(); i++)
	{
		ListCell tempCell; //Temorary cell

		std::string slotValue;
		if(values->size() > i) //Making sure there is a value for the slot
		{
			slotValue = values->at(i);
		}
		else
		{
			slotValue = "";
		}

		tempCell.create(slotValue, colWidth->at(i), nextXPos, y);

		cells->push_back(tempCell);
		//Calculating the position of the next element
		nextXPos = nextXPos + colWidth->at(i);
	}
}

void ListItem::setScissor(float x1, float y1, float x2, float y2)
{
	for(unsigned int i = 0; i < cells->size(); i++)
	{
		cells->at(i).setScissor(x1, y1, x2, y2); //Setting the scissor for the cell
	}
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void ListCell::create(std::string value, float width, float x, float y)
{
	TID = agk::CreateText( value.data() );
	agk::FixTextToScreen(TID, 1);
	agk::SetTextPosition(TID, x, y);
	agk::SetTextSize(TID, 16);
}

void ListCell::setScissor(float x1, float y1, float x2, float y2)
{
	if(agk::GetTextExists(TID))
	{
		agk::SetTextScissor(TID, x1, y1, x2, y2);
	}
}