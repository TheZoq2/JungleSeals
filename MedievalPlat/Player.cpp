#include "Player.h"

#include "Script.h"

Player::Player(void)
{
}


Player::~Player(void)
{
}

void Player::begin(World* world)
{
	this->world = world;

	inventory = new std::vector< Item >;
}

void Player::load(uString name)
{
	img = agk::LoadImage(name); //Loading the sprite
	SID = agk::CreateSprite(img);

	spriteScale = .04f;

	agk::SetSpriteScale(SID, spriteScale, spriteScale); //Setting the position

	agk::SetSpritePhysicsOn(SID, 2);

	//agk::SetSpritePhysicsAngularDamping(SID, .999);
	agk::SetSpriteShape(SID, 3);
	agk::SetSpritePhysicsFriction(SID, 1.0f);
	agk::SetSpritePhysicsRestitution(SID, 0);
	agk::SetSpritePhysicsMass(SID, 0.1f);
	agk::SetSpriteDepth(SID, GF_BaseDepth);

	//Preventing collisioin between this and other characters
	agk::SetSpriteCategoryBit(SID, 1, 0);
	agk::SetSpriteCategoryBit(SID, GF_charGroup, 1);
	agk::SetSpriteCollideBit(SID, GF_charGroup, 0);

	speed = 15;
	jumpHeight = 3;

	//Creating the activation message
	activateSprite = agk::CloneSprite(1);
	agk::SetSpriteColor(activateSprite, 100, 100, 100, 200);
	agk::SetSpriteDepth(activateSprite, 3);
	agk::FixSpriteToScreen(activateSprite, 1);
	activateText = agk::CreateText("E) Activate");
	agk::FixTextToScreen(activateText, 1);
	agk::SetTextDepth(activateText, 2);
	agk::SetTextSize(activateText, 20);

	weapOffsetX = 0;
	weapOffsetY = -1.5;
}

void Player::spawn(uString name)
{
	//Finding the spawnpoint
	/*World::Entry spawnPoint = *world->findEntry(name);

	setPosition(spawnPoint.x, spawnPoint.y);*/

	//Finding the part with the name of the spawnpoint
	Part* spawnPoint = world->getPartFromName( name );

	//We got the spawnpoint, let's set the player position
	setPosition(spawnPoint->getX(), spawnPoint->getY());

	lastTravel = globaltime;
}

void Player::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	agk::SetSpritePositionByOffset(SID, x, y);
}

void Player::update()
{
	bool onGround = checkOnGround(); //Checking if the player is on the ground right now

	//Making sure the player does not fall over
	agk::SetSpriteAngle(SID, 0);

	x = agk::GetSpriteXByOffset(SID);
	y = agk::GetSpriteYByOffset(SID);

	if(Input::up() == true)
	{
		if(onGround && globaltime > lastJump + 0.1) //Checking if the player can jump
		{
			agk::SetSpritePhysicsImpulse(SID, x, y, 0, -jumpHeight);

			lastJump = globaltime;
		}
	}

	bool moving = false; //Checking if the player is moving this frame
	//Going left/right
	float moveForce = 30.0f;

	if(onGround == false)
	{
		//The player is in the air and can not move as much
		moveForce = 10.0f;
	}

	if(Input::left() == true)
	{
		//agk::SetSpritePhysicsImpulse(SID, x, y, -0.5f, 0);
		agk::SetSpritePhysicsForce(SID, x, y, -moveForce, 0);

		moving = true;
	}
	
	if(Input::right() == true)
	{
		//agk::SetSpritePhysicsImpulse(SID, x, y, 0.5f, 0);
		agk::SetSpritePhysicsForce(SID, x, y, moveForce, 0);

		moving = true;
	}

	//Adapting the speed checking for use with floats to move
	float chkSpeed = speed;

	if(Input::moveX() != 0)
	{
		chkSpeed = abs(speed * Input::moveX());

		agk::SetSpritePhysicsForce(SID, x, y, moveForce * Input::moveX(), 0);
		moving = true;
	}
	
	if(moving == false) //The player didn't move
	{
		//Checking if the player is on ground
		if(onGround)
		{
			//Stopping the movment
			if(agk::GetSpritePhysicsVelocityX(SID) > 0) //If the velocity is greater than 0
			{
				//agk::SetSpritePhysicsImpulse(SID, x, y, -1.0f, 0);
				agk::SetSpritePhysicsVelocity(SID, 0, agk::GetSpritePhysicsVelocityY(SID));
			}
			else if(agk::GetSpritePhysicsVelocityX(SID) < 0) //If it is lower than 0
			{
				//agk::SetSpritePhysicsImpulse(SID, x, y, 1.0f, 0);
				agk::SetSpritePhysicsVelocity(SID, 0, agk::GetSpritePhysicsVelocityY(SID));
			}
		}
	}

	//Making sure that the left/right speed is not to big
	if(agk::GetSpritePhysicsVelocityX(SID) > chkSpeed)
	{
		agk::SetSpritePhysicsVelocity(SID, chkSpeed, agk::GetSpritePhysicsVelocityY(SID));
	}
	if(agk::GetSpritePhysicsVelocityX(SID) < -chkSpeed)
	{
		agk::SetSpritePhysicsVelocity(SID, -chkSpeed, agk::GetSpritePhysicsVelocityY(SID));
	}

	//

	activation(); //Function for looking thru all the parts to find close usable parts

	//Positioning the camera
	cameraX = x - (agk::GetVirtualWidth() / agk::GetViewZoom() / 2);
	cameraY = y - (agk::GetVirtualHeight() / agk::GetViewZoom() / 2);

	agk::SetViewOffset(cameraX, cameraY);
}
void Player::updateWeapon(ProjectileGroup* projGroup)
{
	cWeapon.setPosition(x + weapOffsetX, y + weapOffsetY);

	//Aiming
	float targetX = i_mx;
	float targetY = i_my;
	
	cWeapon.targetPos(targetX, targetY);

	if(Input::shoot() == true)
	{
		cWeapon.shoot(projGroup);
	}
}

void Player::activation()
{
	float lowestDist = 6;
	bool partFound = false;
	Part* closestPart;

	//Looping thru all of the parts
	for(int i = 0; i < world->getPartAmount(); i++)
	{
		if(world->getPartUsable(i) == 1) //Making sure we are only looking at parts that can actually be used
		{
			float partX = world->getPartX(i);
			float partY = world->getPartY(i);

			float distX = partX - x;
			float distY = partY - y;
			float dist = agk::Sqrt(distX * distX + distY * distY);

			int useRange = 6;
			if(dist < useRange) //Checking if the part is within reach
			{
				partFound = true;
				
				if(dist < lowestDist) //CHecking if this is the closest part found
				{
					lowestDist = dist; //Changing the lowest distance

					closestPart = world->getPartFromID(i); //Changing the lowest part we have found
				}
			}
		}
	}

	if(partFound == true)
	{
		//Positioning the activation text
		agk::SetTextVisible(activateText, 1);
		agk::SetTextPosition(activateText, agk::WorldToScreenX( closestPart->getX() ), agk::WorldToScreenY( closestPart->getY() ));

		//Changing the text to the use text of the part
		uString fText;
		fText.SetStr(i_activateName);
		fText.Append(") ");
		fText.Append(closestPart->getUseMsg());

		agk::SetTextString(activateText, fText);

		//Adding the background
		agk::SetSpritePosition(activateSprite, agk::WorldToScreenX(closestPart->getX()) -5.0f, agk::WorldToScreenY(closestPart->getY()) - 2.5f);
		agk::SetSpriteVisible(activateSprite, 1);
		agk::SetSpriteScale(activateSprite, agk::GetTextTotalWidth(activateText) + 10, agk::GetTextTotalHeight(activateText) + 5);

		//Checking if the part is activated
		if(Input::activate() == true)
		{
			//Getting the script of the item
			uString actScript;
			actScript.SetStr("scripts/");
			actScript.Append(closestPart->getActScript());

			//Starting the script
			Script::run(actScript, closestPart, world, this);
		}
	}
	else //No parts where found, hide the text
	{
		agk::SetTextVisible(activateText, 0);
		agk::SetSpriteVisible(activateSprite, 0);
	}
}

bool Player::checkOnGround()
{
	bool canJump = false;
	//Calculating the height of the sprite
	float plrWidth = agk::GetImageWidth(img) * spriteScale;
	float plrHeight = agk::GetImageHeight(img) * spriteScale;
	for(int i = 0; i < world->getPartAmount(); i++)
	{
		int physState = world->getPartPhysState(i);
		if(physState != 0) //making sure that the sprite is setup for physics and part of the world
		{
			//Checking if the lower part of the sprite is coliding with anything
			int wID = world->getPartSID(i);
			for(float xChk = -0.5f; xChk < 0.5f; xChk = xChk + 0.1f)
			{
				if(agk::GetSpriteHitTest(wID, x+ xChk, y + (plrHeight / 2) + 0.5f) == 1)
				{
					canJump = true; //Looks like the player can jump
				}
			}
		}
	}

	return canJump; //Returning the result
}

void Player::addItem(Item item)
{
	Item tempItem;
	tempItem.setItem(item);

	inventory->push_back(tempItem);
}

bool Player::canTravel()
{
	/*
	if(justLoaded == false)
	{
		return true;
	}
	*/
	if(globaltime + 0.5 > lastTravel)
	{
		return true;
	}
	
	return false;
}
void Player::setJustLoaded(bool justLoaded)
{
	this->justLoaded = justLoaded;
}
void Player::setVisible(int visible)
{
	agk::SetSpriteVisible(SID, visible);
}

float Player::getX()
{
	return x;
}
float Player::getY()
{
	return y;
}

int Player::getInvAmount()
{
	return inventory->size();
}
Item Player::getItemFromSlot(int slot)
{
	return inventory->at(slot);
}
void Player::setCurrentWeaponByName(uString weaponName)
{
	cWeapon.loadWeaponByName(weaponName);
}