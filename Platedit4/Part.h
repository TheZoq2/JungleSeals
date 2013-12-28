#pragma once

#include <string>
#include "agk.h"

class Part
{
public:
	Part(void);
	~Part(void);

	void create(int vecID, std::string imgName, float x, float y, int depth);
	
	void highlight(int r, int g, int b);
	void move(float mx, float my);
	void scale(float amountX, float amountY);

	void setAngle(float angle);
	void setPosition(float x, float y);
	void setScale(float scaleX, float scaleY);

	bool isHit(float xChk, float yChk);

	std::string getImgName();
	int getVecID();
	float getX();
	float getY();
	float getScaleX();
	float getScaleY();
	float getAngle();
private:
	int vecID;

	int SID;
	int imgID;

	std::string imgName;

	float x;
	float y;
	float xNoSnap;
	float yNoSnap;

	float scaleX;
	float scaleY;
	float scaleXNoSnap;
	float scaleYNoSnap;

	float angle;
	float angleNoSnap;

	int depth;
};

