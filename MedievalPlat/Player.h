#pragma once

#include"agk.h"

#include "GF.h"
#include "World.h"
#include "Input.h"
#include "Item.h"
#include "Weapon.h"

//#include "Script.h"

/*class Limb
{
public:
	float getAngle();

	void setAngle(float angle);

	int imgID;
	int SID;
	float scale;

	float offsetX;
	float offsetY;

	float limbX;
	float limbY;

private:
	float angle;
};

class Arm
{
public:
	void create(uString limb1, uString limb2, int offsetX1, int offsetX2, int offsetY1, int offsetY2);

	void setPosition(float x, float y);
	void setHolding(float holdX, float holdY);
	void setLimbPos(float limbX, float limbY);
	void setHandPos(float handX, float handY);

	void updateLowerPos();
	void updateHolding();
private:
	Limb limb[2];

	float x;
	float y;

	float holdX;
	float holdY;
};*/

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
	void updateWeapon(ProjectileGroup* projGroup);
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

	struct Animation
	{
		int SID;
		int imgID;
		bool animated;

		int FPS;
		int start;
		int end;
	};

	Animation anim[2];

	//Arm arm;
};

