#pragma once

#include <vector>

#include "agk.h"

#include "Player.h"
#include "Input.h"
#include "Item.h"

class Button
{
public:
	Button(void);
	~Button(void);
	void create(uString path, int frameW, int frameH, float x, float y, float scaleX, float scaleY);
	int update();
	void remove();
private:
	int SID;
	int img;
	int state;
};

class MenuList
{
public:
	struct lItem
	{
		int TID;
		uString text;

		Item item;
	};

	void create(uString header, float x, float y, float width, float height);
	void addItem(uString text);
	void removeItem();
	void remove();
	void update();
private:
	std::vector< lItem >* listItem;

	uString header;
	int hText;
	int bgSprite;

	float x;
	float y;
	float width;
	float height;

	int itemHeight;

	bool exists;
};

class IngameMenu
{
public:
	IngameMenu(void);
	~IngameMenu(void);

	static void clear();

	static void createShop(Player* player, int type); //player is the player using the shop
	
	static void update();
private:
	static bool menuActive; //True if there is an active menu

	static int bgSprite;

	static Button button[10];
	static MenuList list[10];
};