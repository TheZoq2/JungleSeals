#pragma once

#include"agk.h"

#include "GF.h"
#include "World.h"
#include "Input.h"
#include "Item.h"
#include "Weapon.h"
//#include "Script.h"

class Player
{
public:
	Player(void);
	~Player(void);
	
	void begin(World* world);

	void load(uString name);

	void spawn(uString point);

	void setPosition(float x, float y);

	void update();
	void activation();

	bool canTravel();
	void setJustLoaded(bool justLoaded);
	void setVisible(int visible);

	void addItem(Item item);
	void setCurrentWeaponByName(uString name);

	float getX();
	float getY();

	int getInvAmount();
	Item getItemFromSlot(int slot);
private:
	int img;
	int SID;

	int activateText;
	int activateSprite;

	float x;
	float y;

	float weapOffsetX;
	float weapOffsetY;

	float cameraX;
	float cameraY;

	int health;

	float lastJump;

	float speed;
	float jumpHeight;

	World* world;

	float spriteScale;

	bool checkOnGround();

	bool justLoaded;
	float lastTravel;

	std::vector< Item >* inventory;

	Weapon cWeapon;
};

