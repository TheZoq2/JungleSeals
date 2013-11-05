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
	agk::SetSpriteVisible(SID, 0);

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

	//Loading animation stuff, based on spritesheets untill we get spine support
	
	//Reading the animation file
	int fileID = agk::OpenToRead("Data/Player/Animation");
	
	uString idleImg;

	uString walkImg;
	int walkFPS = 30;
	int walkW;
	int walkH;
	int walkStart;
	int walkEnd;

	uString upperArm;
	uString lowerArm;
	int upperOffsetX;
	int upperOffsetY;
	int lowerOffsetX;
	int lowerOffsetY;
	int limbX;
	int limbY;
	int handX;
	int handY;

	while(agk::FileEOF(fileID) == 0)
	{
		char* line = agk::ReadLine(fileID);

		uString dataType;
		dataType.SetStr(DataReader::getType(line));

		if(dataType.CompareTo("Idle") == 0)
		{
			idleImg.SetStr(DataReader::getValue(line));
		}
		if(dataType.CompareTo("WalkImg") == 0)
		{
			walkImg.SetStr(DataReader::getValue(line));
		}
		if(dataType.CompareTo("WalkFPS") == 0)
		{
			walkFPS = atoi(DataReader::getValue(line));
		}
		if(dataType.CompareTo("WalkW") == 0)
		{
			walkW = atoi(DataReader::getValue(line));
		}
		if(dataType.CompareTo("WalkH") == 0)
		{
			walkH = atoi(DataReader::getValue(line));
		}
		if(dataType.CompareTo("WalkStart") == 0)
		{
			walkStart = atoi(DataReader::getValue(line));
		}
		if(dataType.CompareTo("WalkEnd") == 0)
		{
			walkEnd = atoi(DataReader::getValue(line));
		}

		//Arm stuff
		if(dataType.CompareTo("UpperArm") == 0)
		{
			upperArm.SetStr(DataReader::getValue(line));
		}
		if(dataType.CompareTo("LowerArm") == 0)
		{
			lowerArm.SetStr(DataReader::getValue(line));
		}
		
		if(dataType.CompareTo("UpperArmOffsetX") == 0)
		{
			upperOffsetX = atoi(DataReader::getValue(line));
		}
		if(dataType.CompareTo("UpperArmOffsetY") == 0)
		{
			upperOffsetY = atoi(DataReader::getValue(line));
		}
		if(dataType.CompareTo("LowerArmOffsetX") == 0)
		{
			lowerOffsetX = atoi(DataReader::getValue(line));
		}
		if(dataType.CompareTo("LowerArmOffsetX") == 0)
		{
			lowerOffsetY = atoi(DataReader::getValue(line));
		}

		if(dataType.CompareTo("ArmLimbX") == 0)
		{
			limbX = atoi(DataReader::getValue(line));
		}
		if(dataType.CompareTo("ArmLimbY") == 0)
		{
			limbY = atoi(DataReader::getValue(line));
		}
		if(dataType.CompareTo("HandX") == 0)
		{
			handX = atoi(DataReader::getValue(line));
		}
		if(dataType.CompareTo("HandY") == 0)
		{
			handY = atoi(DataReader::getValue(line));
		}

		delete[] line;
	}

	//Saving those animations
	anim[0].animated = false;
	anim[0].imgID = agk::LoadImage(GF::getPath(idleImg));
	anim[0].SID = agk::CreateSprite(anim[0].imgID);
	agk::SetSpriteScale(anim[0].SID, 0.012f, 0.012f);
	agk::SetSpriteDepth(anim[0].SID, 100);

	float walkScale = 0.019f;


	/*anim[1].animated = true;
	anim[1].imgID = agk::LoadImage(GF::getPath(walkImg));
	anim[1].SID = agk::CreateSprite(anim[1].imgID);
	agk::SetSpriteAnimation(anim[1].SID, walkW, walkH, walkEnd);
	agk::SetSpriteScale(anim[1].SID, walkScale, walkScale);
	agk::SetSpriteDepth(anim[1].SID, 100);

	anim[1].start = walkStart;
	anim[1].end = walkEnd;
	anim[1].FPS = walkFPS;*/
	
	///////////////////////////////
	//		   Arm stuff
	///////////////////////////////
/*	arm.create(upperArm, lowerArm, upperOffsetX, upperOffsetY, lowerOffsetX, lowerOffsetY);
	arm.setLimbPos(limbX, limbY);
	arm.setHandPos(handX, handY);
	*/
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

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	//Animation
	agk::SetSpritePositionByOffset(anim[0].SID, x, y - 0.5f);

	/*
	arm.setPosition(x, y - 1.2);
	arm.setHolding(cWeapon.getHandleWorldX(), cWeapon.getHandleWorldY());
	arm.updateHolding();
	*/
	
	/*if(moving == true) //When walking
	{
		agk::SetSpritePositionByOffset(anim[1].SID, x, y - 0.5f);

		//Starting the animation if its not playing already
		if(agk::GetSpritePlaying(anim[1].SID) == 0)
		{
			agk::PlaySprite(anim[1].SID, anim[1].FPS, 0);
		}

		agk::SetSpriteVisible(anim[0].SID, 0);
		agk::SetSpriteVisible(anim[1].SID, 1);
	}
	else
	{
		agk::SetSpriteVisible(anim[0].SID, 1);
		agk::SetSpriteVisible(anim[1].SID, 0);
	}*/
	
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

	//Checking where the target is and if the player sprite should be flipped
	if(targetX > x)
	{
		agk::SetSpriteFlip(anim[0].SID, 0, 0);
	}
	else
	{
		agk::SetSpriteFlip(anim[0].SID, 1, 0);
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

/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
/*
void Arm::create(uString limb1, uString limb2, int offsetX1, int offsetX2, int offsetY1, int offsetY2)
{
	float limbScale = 0.005f;
	limb[0].imgID = agk::LoadImage(GF::getPath(limb1));
	limb[0].SID = agk::CreateSprite(limb[0].imgID);
	agk::SetSpriteDepth(limb[0].SID, GF_BaseDepth);
	agk::SetSpriteOffset(limb[0].SID, offsetX1, offsetY1);
	agk::SetSpriteScale(limb[0].SID, limbScale, limbScale);
	limb[0].scale = limbScale;

	limb[1].imgID = agk::LoadImage(GF::getPath(limb2));
	limb[1].SID = agk::CreateSprite(limb[1].imgID);
	agk::SetSpriteDepth(limb[1].SID, GF_BaseDepth);
	agk::SetSpriteOffset(limb[1].SID, offsetX2, offsetY2);
	agk::SetSpriteScale(limb[1].SID, limbScale, limbScale);
	limb[1].scale = limbScale;

	limb[0].offsetX = offsetX1;
	limb[0].offsetY = offsetY1;
	limb[1].offsetX = offsetX2;
	limb[1].offsetY = offsetY2;
}

void Arm::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	
	agk::SetSpritePositionByOffset(limb[0].SID, x, y);
	float upperX = agk::GetSpriteX(limb[0].SID);
	float upperY = agk::GetSpriteY(limb[0].SID);
	agk::SetSpritePositionByOffset(limb[1].SID, upperX + limb[0].limbX * limb[0].scale, upperY + limb[0].limbY * limb[0].scale);
}
void Arm::setHolding(float holdX, float holdY)
{
	this->holdX = holdX;
	this->holdY = holdY;

	agk::SetSpritePositionByOffset(limb[1].SID, holdX, holdY);
}

void Arm::setLimbPos(float limbX, float limbY)
{
	limb[0].limbX = limbX;
	limb[0].limbY = limbY;
}
void Arm::setHandPos(float handX, float handY)
{
	limb[1].limbX = handX;
	limb[1].limbY = handY;
}

void Arm::updateHolding()
{
	//Using the law of cosines
	float uL = sqrt(pow(limb[0].offsetX * limb[0].scale - limb[0].limbX * limb[0].scale, 2) + pow(limb[0].offsetY * limb[0].scale - limb[0].limbY * limb[0].scale, 2));
	float lL = sqrt(pow(limb[1].offsetX * limb[1].scale - limb[1].limbX * limb[1].scale, 2) + pow(limb[1].offsetY * limb[1].scale - limb[1].limbY * limb[1].scale, 2));

	float weapDist = sqrt(pow(holdX - x, 2) + pow(holdY - y, 2));

	//calculating the cosine value of the limb angle
	float cosVal = (pow(lL, 2) - pow(uL, 2) - pow(weapDist, 2)) / (2 * uL * weapDist);
	
	float angle = agk::ACos(cosVal);

	//limb[0].setAngle(angle);
	//Setting the position of the lower arm based on the angle of the upper
	updateLowerPos();
} 

void Arm::updateLowerPos()
{
	float xPos = agk::GetSpriteX(limb[0].SID) + agk::Cos(limb[0].getAngle() - 90) * limb[0].offsetX * limb[0].scale;
	xPos = xPos + agk::Cos(limb[0].getAngle() + 90) * limb[0].offsetY * limb[0].scale;

	float yPos = agk::GetSpriteY(limb[0].SID) + agk::Sin(limb[0].getAngle() - 90) * limb[0].offsetX * limb[0].scale;
	yPos = yPos + agk::Sin(limb[0].getAngle() + 90) * limb[0].offsetY * limb[0].scale;

	agk::SetSpritePositionByOffset(limb[1].SID, xPos, yPos);
}

/////////////////////////////////////////////////////////////////////////////

void Limb::setAngle(float angle)
{
	this->angle = angle;

	agk::SetSpriteAngle(SID, angle);
}

float Limb::getAngle()
{
	return angle;
}
*/