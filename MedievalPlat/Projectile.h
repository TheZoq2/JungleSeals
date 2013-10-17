#pragma once

#include "agk.h"

#include "DebugConsole.h"
#include "GF.h"
#include "DataReader.h"

#include <list>
#include <vector>

#ifndef PROJ_GLOBALS
#define PROJ_GLOBALS
	extern char projPath[128];

	extern int proj_physType;
#endif

class ProjectileBase //Base used to clone projectiles from
{
public:
	void loadFromName(uString name);

	uString getName();
	bool getExists(); //Will return true if the projectile was loaded successfully
	int getSID();
	float getSpeed();
	int getType();
	float getRelativeMass();
	float getFriction();
private:
	uString name;

	int imgID;
	int SID;

	float scaleX;
	float scaleY;

	bool exists;

	float speed;

	float speedX;
	float speedY;

	int colorR;
	int colorG;
	int colorB;

	int type;

	float friction;
	float RelativeMass;
};

class Projectile //Actual projectiles
{
public:
	Projectile(void);
	~Projectile(void);

	void createFromBase(ProjectileBase* projBase, float x, float y, float angle, float speedX, float speedY);
	void update();
	void remove();

	bool shouldBeRemoved( float centerX, float centerY, float removalDist );
private:
	int SID;

	float x;
	float y;
	float angle;
	float speedX;
	float speedY;

	float speed;

	int type;
};

class ProjectileGroup //Group of projectiles and bases
{
public:
	void setup();
	void update(float centerX, float centerY); //Center x and y are the center of the sphere which projectiles will be removed outside

	void addByName(uString name, float x, float y, float angle, float speedX, float speedY);
private:
	std::list< Projectile >* projs;

	std::vector< ProjectileBase >* projBase;

	float projDist; //The diameter of the sphere where projectiles can be alive centered around the centerX and centerY cordinates passed in update
};