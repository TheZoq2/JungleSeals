#include "IngameMenu.h"


bool IngameMenu::menuActive;
int IngameMenu::bgSprite;
Button IngameMenu::button[10];
MenuList IngameMenu::list[10];

IngameMenu::IngameMenu(void)
{
}


IngameMenu::~IngameMenu(void)
{
}

void IngameMenu::createShop(Player* player, int type)
{
	if(menuActive == true) //If there is a menu up already
	{
		clear(); //Clearing the old menu
	}

	menuActive = true; //Activating the menu
	//Creating the background
	bgSprite = agk::CloneSprite(1);
	agk::FixSpriteToScreen(bgSprite, 1);
	agk::SetSpriteScale(bgSprite, agk::GetVirtualWidth() - 20.0f, agk::GetVirtualHeight() - 20.0f);
	agk::SetSpritePosition(bgSprite, 10, 10);
	agk::SetSpriteDepth(bgSprite, 2);
	agk::SetSpriteColor(bgSprite, 50, 50, 50, 200);


	//Creating the back button
	button[0].create(GF::getPath("GUI/Button_back.png"), 128, 32, agk::GetVirtualWidth() - 30.0f - 256.0f, agk::GetVirtualHeight() - 30.0f - 64.0f, 2.0f, 2.0f);

	list[0].create("Buy", 30, 30, (agk::GetVirtualWidth() - 60.0f) / 2 - 10, agk::GetVirtualHeight() - 60.0f - 64.0f - 20.0f);
	list[1].create("Sell", 30 + (agk::GetVirtualWidth() - 60.0f) / 2 + 10, 30, (agk::GetVirtualWidth() - 60.0f) / 2 - 10, agk::GetVirtualHeight() - 60.0f  - 64.0f - 20.0f);

	//Checking what type of shop this is
	if(type == 1) //It's a smith
	{
		for(int i = 0; i < 10; i++)
		{
			//Generating items for the seller
			Item tempItem;
			tempItem.setItem(ItemGen::generate(1));

			//Adding the item to the list
			list[0].addItem(tempItem.getName());
		}

		//Adding the players inventory to the list
		for(int i = 0; i < player->getInvAmount(); i++)
		{
			list[1].addItem(player->getItemFromSlot(i).getName());
		}
	}
}

void IngameMenu::clear()
{
	if(agk::GetSpriteExists(bgSprite))
	{
		agk::DeleteSprite(bgSprite);
	}
	for(int i = 0; i < 10; i++)
	{
		button[i].remove();
	}
	for(int i = 0; i < 10; i++)
	{
		list[i].remove();
	}
}

void IngameMenu::update()
{
	if(menuActive == true)
	{
		if(button[0].update() == 1)
		{
			clear();
		}

		//Updating the lists
		list[0].update();
		list[1].update();
	}
}

Button::Button(void)
{
	
}
Button::~Button(void)
{
	
}
void Button::create(uString path, int frameW, int frameH, float x, float y, float scaleX, float scaleY)
{
	//Creating the sprite
	img = agk::LoadImage(path);
	SID = agk::CreateSprite(img);
	agk::FixSpriteToScreen(SID, 1);
	agk::SetSpriteAnimation(SID, frameW, frameH, 3);
	agk::SetSpriteScale(SID, scaleX, scaleY);
	agk::SetSpritePosition(SID, x, y);
	agk::SetSpriteDepth(SID, 1);
}
void Button::remove()
{
	if(agk::GetImageExists(img)) agk::DeleteImage(img);
	if(agk::GetSpriteExists(SID)) agk::DeleteSprite(SID);
}
int Button::update()
{
	//Making sure the sprite exists
	if(agk::GetSpriteExists(SID))
	{
		if(agk::GetSpriteHitTest(SID, i_mx, i_my))
		{
			agk::SetSpriteFrame(SID, 2);
			if(agk::GetPointerState() == 1)
			{
				agk::SetSpriteFrame(SID, 2);
				state = 2;
				return 2;
			}else if(state == 2)
			{
				state = 0;
				return 1;
			}
			
		}else
		{
			agk::SetSpriteFrame(SID, 1);
		}
	}

	return 0;
}

void MenuList::create(uString header, float x, float y, float width, float height)
{
	exists = true;

	this->header.SetStr(header);

	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	bgSprite = agk::CloneSprite(1);
	agk::FixSpriteToScreen(bgSprite, 1);
	agk::SetSpriteScale(bgSprite, width, height - 40.0f);
	agk::SetSpritePosition(bgSprite, x, y + 40.0f);
	agk::SetSpriteColor(bgSprite, 50, 50, 50, 200);

	hText = agk::CreateText(header);
	agk::FixTextToScreen(hText, 1);
	agk::SetTextDepth(hText, 1);
	agk::SetTextPosition(hText, x, y);
	agk::SetTextSize(hText, 32);

	listItem = new std::vector< lItem >;
	itemHeight = 32; //Setting the height of the items in the list
}
void MenuList::remove()
{
	if(exists == true)
	{
		if(agk::GetTextExists(hText)) agk::DeleteText(hText);
		if(agk::GetSpriteExists(bgSprite)) agk::DeleteSprite(bgSprite);

		//Removing the text
		for(unsigned int i = 0; i < listItem->size(); i++)
		{
			if(agk::GetTextExists(listItem->at(i).TID)) agk::DeleteText(listItem->at(i).TID);
		}


		if(listItem != NULL)
		{
			listItem->clear();

			delete listItem;
			listItem = NULL;
		}

		exists = false;
	}
}
void MenuList::addItem(uString text)
{
	lItem tempItem;
	tempItem.text.SetStr(text);
	tempItem.TID = agk::CreateText(text);
	agk::FixTextToScreen(tempItem.TID, 1);
	agk::SetTextPosition(tempItem.TID, x, y + 40.0f + itemHeight * listItem->size());
	agk::SetTextSize(tempItem.TID, 30);
	agk::SetTextDepth(tempItem.TID, 1);
	//Adding the text to the vector
	listItem->push_back(tempItem);
}
void MenuList::update()
{
	if(exists == true)
	{
		for(unsigned int i = 0; i < listItem->size(); i++)
		{
			if(agk::GetTextHitTest(listItem->at(i).TID, i_mx, i_my))
			{
				agk::SetTextColor(listItem->at(i).TID, 200,  200, 255, 255);
			}else
			{
				agk::SetTextColor(listItem->at(i).TID, 255, 255, 255, 255);	
			}
		}
	}
}