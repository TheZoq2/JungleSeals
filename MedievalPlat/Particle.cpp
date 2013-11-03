#include "Particle.h"

char p_location[128] = "";

Particle::Particle(void)
{
}


Particle::~Particle(void)
{
}

void Particle::addFromFile(int ID, uString filename, float x, float y)
{
	//Making sure the file exists
	if(agk::GetFileExists(filename))
	{

	}
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
int ParticleGroup::addFromFile(uString filename, float x, float y)
{

}