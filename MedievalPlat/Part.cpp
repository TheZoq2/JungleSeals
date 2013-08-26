#include "Part.h"


Part::Part(void)
{
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
			agk::SetSpritePhysicsFriction(SID, 0.8);
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