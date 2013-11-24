#pragma once

#include "agk.h"
#include "GF.h"

#include <list>
#include <string>

class Part
{
public:
	Part(void);
	~Part(void);

	void cloneSprite(int cloneSprite);
	void setScale(float scaleX, float scaleY);
	void setPosition(float x, float y);
	void setAngle(float angle);
	void setDepth(int depth);
	void setPhysState(int physState);

	void update(); //Performs all operations that should be performed on the part
	void remove();

	void setVisible(int state);

	int getSID();
	int getPhysState();
	float getX();
	float getY();

	void setName(uString name);
	void setUsable(int usable);
	void setActScript(uString activScript);
	void setUseMsg(uString msg);
	void setLabel(int slot, uString label);
	void setTarget(float targetX, float targetY);
	void setSpeed(float speed);

	uString getName();
	int getUsable();
	uString getActScript();
	uString getUseMsg();
	uString getLabel(int slot);
	bool getHit(float x, float y);
	float getEdgeRadius();

	void addOperation(std::string operation);
private:
	int SID;
	uString file;

	int physState;
	int depth;
	float x;
	float y;
	float scaleX;
	float scaleY;
	float angle;

	uString name;
	int usable;
	uString activScript;
	uString useMsg;
	
	uString label[5];

	std::list< std::string >* operations; //List of things that the part should do each update

	float targetX;
	float targetY;
	float speed;
};