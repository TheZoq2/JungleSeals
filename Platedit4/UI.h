#pragma once

#include <vector>
#include <string>

#include "agk.h"
#include "GF.h"
#include "DebugConsole.h"
#include "Input.h"

class UI_editbox
{
public:
	void create(std::string vecID, float x, float y, float sizeX, float sizeY);
	void remove();

	std::string getVecID();
	std::string getValue();
	
	void setValue(std::string value);
private:
	std::string vecID;

	int ID;

	float x;
	float y;
	float sizeX;
	float sizeY;
};
//////////////////////////////////////////////////////////////////////////

class Button
{
public:
	void createColorButton(std::string vecID, float x, float y, float width, float height, std::string text);
	void updateInput(float mx, float my);

	void reset();

	void setColors(int r, int g, int b, int hr, int hg, int hb);

	std::string getVecID();
private:
	std::string vecID;

	int type;

	int SID;
	int TID;

	float x;
	float y;

	int r;
	int g;
	int b;
	int hr;
	int hg;
	int hb;
};
//////////////////////////////////////////////////////////////////////////
class ImgListItem
{
public:
	void create(std::string image, float x, float y, float width, float height, std::string value);
	bool updateInput(float mx, float my);
	void remove();

	void setColor(int r, int g, int b, int a);
	void highlight(int r, int g, int b);

	std::string getImage();
	std::string getValue();
private:
	std::string image;
	std::string value;

	int imgID;
	int SID;

	float x;
	float y;
	float width;
	float height;
	float scaleX;
	float scaleY;
};

class ImgList
{
public:
	void create(std::string vecID, int cloneID, float x, float y, float width, float height, float imgWidth, float imgHeight, int r, int g, int b, int a);
	void updateInput(float mx, float my);
	void remove();

	void addImage(std::string image, std::string value);

	std::string getVecID();

	std::string getSelectedValue();
private:
	std::string vecID;

	std::string selected;

	int bgSID;

	float x;
	float y;

	float width;
	float height;
	
	float imgWidth;
	float imgHeight;

	std::vector< ImgListItem >* listItems;
	
	ImgListItem* getItemByImage(std::string ID);
};

//////////////////////////////////////////////////////////////////////////

//A cell in a list
class ListCell
{
public:
	void create(std::string value, float width, float x, float y);
	void remove();
	void setScissor(float x1, float y1, float x2, float y2);
private:
	int TID;

	float x;
	float y;
	
	std::string value;
};
//One row of data for a list view
class ListItem
{
public:
	void create(std::vector< float >* colWidth, std::vector< std::string >* values, float x, float y);
	void remove();

	void setScissor(float x1, float y1, float x2, float y2);
private:
	int bgID;

	std::vector< ListCell >* cells;
};
//A list view that contains rows of data
class List
{
public:
	void create(std::string ID, float x, float y, float width, float height, std::vector< float >* colWidth, std::vector< std::string >* colHeaders);
	void updateInput(float mx, float my);
	void updateScissors();
	void remove();

	void clear();

	int addItem(std::vector< std::string >* values);
	
	std::string getID();
private:
	std::string vecID;

	std::vector< ListItem >* listItems;
	std::vector< float >* colWidth;
	ListItem header;

	int cellAmount;

	float x;
	float y;
	float width;
	float height;

	float rowHeight;
	float nextItemY;
};

//////////////////////////////////////////////////
class Window
{
public:
	void create(std::string vecID, std::string bgImg, float x, float y, float sizeX, float sizeY);
	void setBgColor(int r, int g, int b, int a); //Set the color of the window background
	void remove();

	int update(float mx, float my);
	void updateInput(float mx, float my);
	void updateScissors();

	std::string getID();

	//UI elements
	void addList(std::string listID, float offsetX, float offsetY, float width, float height, std::vector< float >* colWidth, std::vector< std::string >* colHeaders);
	void addSimpleList(std::string listID, float x, float y, float width, float height, std::string header);
	void removeList(std::string listID);
	void clearList(std::string listID);

	void addToList(std::string listID, std::vector< std::string >* values);
	void addToSimpleList(std::string listID, std::string value);

	void addImgList(std::string vecID, int cloneID, float x, float y, float width, float height, float imgWidth, float imgHeight, int r, int g, int b, int a);
	void addImageToImgList(std::string vecID, std::string image, std::string value);
	std::string getImgListSelValue(std::string vecID);

	void addEditbox(std::string vecID, float x, float y, float sizeX, float sizeY);
	std::string getEditboxValue(std::string vecID);
	void setEditboxValue(std::string vecID, std::string value);

	void addColorButton(std::string buttonID, float x, float y, float width, float height, std::string text);
	void setButtonColor(std::string buttonID, int r, int g, int b, int hr, int hg, int hb);
private:
	std::string vecID;

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

	//UI elements
	std::vector< List >* lists;
	int nextList;
	std::vector< Button >* buttons;
	std::vector< ImgList >* imgLists;
	std::vector< UI_editbox >* editboxes;

	List* findListByID(std::string ID);
	ImgList* findImgListById(std::string ID);
	UI_editbox* findEditboxById(std::string ID);
	Button* findButtonById(std::string ID);
};

//Group that cointains UI elements
class UI
{
public:
	UI(void);
	~UI(void);

	void setup();

	void updateScissors();
	void update();

	bool getUIActive(); //Returns true if the mouse is not used by the UI group

	void addWindow(std::string ID, std::string bgName, float offsetX, float offsetY, float sizeX, float sizeY); //Creates a window which can be referened by the ID returned
	void removeWindow(std::string windowID);
	void setWindowColor(std::string ID, int r, int g, int b, int a);

	//UI elements
	void addListToWindow(std::string windowID, std::string listID, float x, float y, float width, float height, std::vector< float >* colWidth, std::vector< std::string >* colHeaders);
	void addSimpleListToWindow(std::string windowID, std::string listID, float x, float y, float width, float height, std::string header);
	void removeList(std::string windowID, std::string listID);
	void clearList(std::string windowID, std::string listID);

	void addToList(std::string windowID, std::string listID, std::vector< std::string >* values);
	void addToSimpleList(std::string windowID, std::string listID, std::string value);

	void addImgListToWindow(std::string windowID, std::string vecID, int cloneID, float x, float y, float width, float height, float imgWidth, float imgHeight, int r, int g, int b, int a);
	void addImageToImgList(std::string windowID, std::string vecID, std::string img, std::string value);
	std::string getImgListSelValue(std::string windowID, std::string vecID);

	void addEditboxToWindow(std::string windowID, std::string vecID, float x, float y, float sizeX, float sizeY);
	std::string getEditboxValue(std::string windowID, std::string vecID);
	void setEditboxValue(std::string windowID, std::string vecID, std::string value);

	void addColorButtonToWindow(std::string windowID, std::string buttonID, float x, float y, float width, float height, std::string text);
	void setButtonColor(std::string windowID, std::string buttonID, int r, int g, int b, int hr, int hg, int hb);

	bool getWindowExists(std::string ID);
private:
	int nextWindow;
	std::vector< Window >* windows;

	Window* getWindowByID(std::string ID);
	void deleteWindowByID(std::string ID);

	int windowHitAmount;
};
