#pragma once

#include "agk.h"

#include "DebugConsole.h"
#include "GF.h"
#include "DataReader.h"
#include "Particle.h"
#include "World.h"

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
	void loadFromName(uString name, ParticleGroup* partGroup);

	uString getName();
	bool getExists(); //Will return true if the projectile was loaded successfully
	int getSID();
	int getPartID();
	float getSpeed();
	int getType();
	float getRelativeMass();
	float getFriction();
	std::vector< uString >* getImpactPartNames();
private:
	uString name;

	int imgID;
	int SID;
	int partID;

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

	//End particle
	std::vector< uString >* impactPart;
};

class Projectile //Actual projectiles
{
public:
	Projectile(void);
	~Projectile(void);

	void createFromBase(ProjectileBase* projBase, float x, float y, float angle, float speedX, float speedY, ParticleGroup* partGroup);
	void update();
	void updateWorld(World* world);
	void updateParticle(ParticleGroup* partGroup);
	void remove(ParticleGroup* partGroup);
	void createImpactParticles();

	void setFlying(bool state);
	void setPosition(float x, float y);

	bool shouldBeRemoved( float centerX, float centerY, float removalDist );
private:
	int SID;
	int partID;

	float oldX;
	float oldY;
	float x;
	float y;
	float angle;
	float speedX;
	float speedY;

	float speed;

	int type;

	bool flying;
	int impactState;

	//Impact particles
	std::vector< uString >* impactPartName;
	std::vector< int >* impactPart;
};

class ProjectileGroup //Group of projectiles and bases
{
public:
	void setup();
	void update(float centerX, float centerY); //Center x and y are the center of the sphere which projectiles will be removed outside
	void updateWorld(World* world);

	void addByName(uString name, float x, float y, float angle, float speedX, float speedY);
private:
	std::list< Projectile >* projs;

	std::vector< ProjectileBase >* projBase;

	float projDist; //The diameter of the sphere where projectiles can be alive centered around the centerX and centerY cordinates passed in update

	ParticleGroup partGroup;
};