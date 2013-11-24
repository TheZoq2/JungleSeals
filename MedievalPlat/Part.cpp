#include "Part.h"


Part::Part(void)
{
	operations = NULL;
}


Part::~Part(void)
{
}


void Part::setVisible(int state)
{
	agk::SetSpriteVisible(SID, state);
}
void Part::cloneSprite(int cloneID)
{
	this->SID = agk::CloneSprite(cloneID);
}

void Part::setAngle(float angle)
{
	this->angle = angle;

	agk::SetSpriteAngle(SID, angle);
}
void Part::setDepth(int depth)
{
	this->depth = depth;

	agk::SetSpriteDepth(SID, depth);
}
void Part::setPhysState(int physState)
{
	this->physState = physState;

	if(physState != 0)
	{
		agk::SetSpritePhysicsOn(SID, physState);

		if(physState == 2)
		{
			agk::SetSpritePhysicsFriction(SID, 0.8f);
		}
	}

	//Also giving it a shape
	agk::SetSpriteShape(SID, 3);
}
void Part::setScale(float scaleX, float scaleY)
{
	this->scaleX = scaleX;
	this->scaleY = scaleY;

	agk::SetSpriteScale(SID, scaleX, scaleY);
}
void Part::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	agk::SetSpritePositionByOffset(SID, x, y);
}

void Part::remove()
{
	if(agk::GetSpriteExists(SID))
	{
		agk::DeleteSprite(SID);
	}

	if(operations != NULL) //If the list of operations exists
	{
		//Clean it up
		operations->clear();
		delete operations;
		operations = NULL;
	}
}

int Part::getSID()
{
	return SID;
}
int Part::getPhysState()
{
	return physState;
}
float Part::getX()
{
	return x;
}
float Part::getY()
{
	return y;
}

void Part::setActScript(uString activScript)
{
	this->activScript.SetStr(activScript);
}
void Part::setName(uString name)
{
	this->name.SetStr(name);
}
void Part::setUsable(int usable)
{
	this->usable = usable;
}
void Part::setUseMsg(uString useMsg)
{
	this->useMsg.SetStr(useMsg);
}
void Part::setLabel(int slot, uString label)
{
	this->label[slot].SetStr(label);
}
void Part::setTarget(float targetX, float targetY)
{
	this->targetX = targetX;
	this->targetY = targetY;
}
void Part::setSpeed(float speed)
{
	this->speed = speed;
}

uString Part::getActScript()
{
	return activScript;
}
uString Part::getName()
{
	return name;
}
int Part::getUsable()
{
	return usable;
}
uString Part::getUseMsg()
{
	return useMsg;
}
uString Part::getLabel(int slot)
{
	return label[slot];
}
bool Part::getHit(float x, float y)
{
	if(agk::GetSpriteHitTest(SID, x, y))
	{
		return true;
	}
	return false;
}
float Part::getEdgeRadius()
{
	float xSize = agk::GetImageWidth(agk::GetSpriteImageID(SID)) * scaleX;
	float ySize = agk::GetImageHeight(agk::GetSpriteImageID(SID)) * scaleY;

	float diameter = sqrt(pow(xSize,2) + pow(ySize,2));

	return diameter/2;
}

void Part::update()
{
	std::list< std::string >::iterator it; //Iterator for going thru all of the operations

	for(it = operations->begin(); it != operations->end(); it++)
	{
		if(strcmp(it->data(), "moveToTarget") == 0) //If the part should move towards something
		{
			//Calculating the distance to the target
			float xDist = targetX - x;
			float yDist = targetY - y;
			float totDist = sqrt(pow(xDist, 2) + pow(yDist, 2));

			if(totDist < speed * 0.6) //If the target has been reached
			{
				//Add it to a vector of elements to be removed
			}
			else
			{
				//Calculating the angle to the target
				float angle = agk::ATanFull(xDist, yDist);
				float xAdd = agk::Cos(angle - 90) * speed * speedMod;
				float yAdd = agk::Sin(angle - 90) * speed * speedMod;

				setPosition(x + xAdd, y + yAdd);
			}
		}
	}
}
void Part::addOperation(std::string operation)
{
	if(operations == NULL) //If the operations list has not been initiated
	{
		operations = new std::list< std::string >;
	}

	//Checking if that operation already exists
	bool exists = false;
	std::list< std::string >::iterator it;
	for(it = operations->begin(); it != operations->end(); it++)
	{
		if(strcmp(it->data(), operation.data()) == 0)
		{
			exists = true;
		}
	}

	if(exists == false) //The operation didnt already exist
	{
		operations->push_back(operation); //Add it
	}
}