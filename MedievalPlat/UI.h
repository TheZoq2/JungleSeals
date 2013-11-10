#pragma once

#include <vector>
#include <string>

#include "agk.h"
#include "GF.h"
#include "DebugConsole.h"

class ListItem
{
public:
	int bgID;
private:

}
class List
{
public:

private:
	std::vector< ListItem >* listItems;
	65

class Window
{
public:
	int create(int vecID, std::string bgImg, float x, float y, float sizeX, float sizeY);
	void setBgColor(int r, int g, int b, int a); //Set the color of the window background

	int getID();
private:
	int vecID;

	int bgSID;
	int bgImg;
	std::string bgName;

	float x;
	float y;

	float sizeX;
	float sizeY;

	//Colors
	int r;
	int g;
	int b;
	int a;
};

//Group that cointains UI elements
class UI
{
public:
	UI(void);
	~UI(void);

	void setup();

	int addWindow(std::string bgName, float x, float y, float sizeX, float sizeY); //Creates a window which can be referened by the ID returned
	void setWindowColor(int ID, int r, int g, int b, int a);
private:
	int nextWindow;
	std::vector< Window >* windows;

	Window* getWindowByID(int ID);
};
