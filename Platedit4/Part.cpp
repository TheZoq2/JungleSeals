#include "Part.h"


Part::Part(void)
{
}


Part::~Part(void)
{
}


void Part::create(int vecID, std::string imgName, float x, float y, int depth)
{
	this->vecID = vecID;

	this->imgName = imgName;
	this->x = x;
	this->y = y;
	this->xNoSnap = x;
	this->yNoSnap = y;
	this->scaleX = 1;
	this->scaleY = 1;
	this->scaleXNoSnap = 1;
	this->scaleYNoSnap = 1;

	this->angle = 0;

	imgID = agk::LoadImage(imgName.data());
	SID = agk::CreateSprite(imgID);

	agk::SetSpritePositionByOffset(SID, x, y);
	agk::SetSpriteDepth(SID, depth);
	agk::SetSpriteShape(SID, 3);
}

int Part::getVecID()
{
	return vecID;
}
bool Part::isHit(float xChk, float yChk)
{
	if(agk::GetSpriteHitTest(SID, xChk, yChk) == 1)
	{
		return true;
	}
	return false;
}

void Part::highlight(int r, int g, int b)
{
	float x1 = agk::WorldToScreenX(agk::GetSpriteX(SID));
	float x2 = agk::WorldToScreenX(agk::GetSpriteX(SID) + agk::GetImageWidth(imgID) * scaleX);
	float y1 = agk::WorldToScreenY(agk::GetSpriteY(SID));
	float y2 = agk::WorldToScreenY(agk::GetSpriteY(SID) + agk::GetImageHeight(imgID) * scaleY);

	agk::DrawLine(x1, y1, x1, y2, r, g, b);
	agk::DrawLine(x1, y2, x2, y2, r, g, b);
	agk::DrawLine(x2, y2, x2, y1, r, g, b);
	agk::DrawLine(x2, y1, x1, y1, r, g, b);
}

void Part::move(float mx, float my)
{
	this->x = x + mx;
	this->y = y + my;

	agk::SetSpritePositionByOffset(SID, x, y);
}

void Part::scale(float amountX, float amountY)
{
	scaleX = scaleX * amountX;
	scaleY = scaleY * amountY;

	agk::SetSpriteScale(SID, scaleX, scaleY);
}

void Part::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	agk::SetSpritePositionByOffset(SID, x, y);
}
void Part::setAngle(float angle)
{
	this->angle = angle;
	this->angleNoSnap = angle;

	agk::SetSpriteAngle(SID, angle);
}
void Part::setScale(float scaleX, float scaleY)
{
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->scaleXNoSnap = scaleX;
	this->scaleYNoSnap = scaleY;

	agk::SetSpriteScale(SID, scaleX, scaleY);
}

std::string Part::getImgName()
{
	return imgName;
}
float Part::getX()
{
	return x;
}
float Part::getY()
{
	return y;
}
float Part::getAngle()
{
	return angle;
}
float Part::getScaleX()
{
	return scaleX;
}
float Part::getScaleY()
{
	return scaleY;
}