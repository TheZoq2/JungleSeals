#include "Particle.h"

char p_location[128] = "Data/Particles/";

Particle::Particle(void)
{
}


Particle::~Particle(void)
{
}

void Particle::setup()
{
	colorFrame = new std::vector< Keyframe >;


}
void Particle::update()
{
	if(timeout != -1)
	{
		if(createdAt + timeout < globaltime)
		{
			agk::SetParticlesFrequency(PID, 0);
		}

		//Checking if the particle should still be updated
		if(createdAt + timeout + life < globaltime)
		{
			agk::SetParticlesActive(PID, 0);
			agk::SetParticlesVisible(PID, 0);

			isFinished = true;
		}
	}
}

void Particle::addFromFile(int ID, uString filename, float x, float y)
{
	uString filePath;
	filePath.SetStr(p_location);
	filePath.Append(filename);

	//Making sure the file exists
	if(agk::GetFileExists(filePath))
	{
		//Setting default values for the particle
		image = GF::getPath("1x1.png");
		//image.SetStr(GF::getPath("1x1.png"));
		angle = 180;
		dirX = 1;
		dirY = 1;
		frequency = 10;
		life = 2;
		timeout = -1;
		size = 1.0f;
		startX = 0;
		startY = 0;

		//Creating temporary vectors for keeping track of color keyframes
		std::vector< float >* frameT = new std::vector< float >; frameT->push_back(0);
		std::vector< int >* frameR = new std::vector< int >; frameR->push_back(255);
		std::vector< int >* frameG = new std::vector< int >; frameG->push_back(255);
		std::vector< int >* frameB = new std::vector< int >; frameB->push_back(255);
		std::vector< int >* frameA = new std::vector< int >; frameA->push_back(255);

		int fileID = agk::OpenToRead(filePath);
		while(agk::FileEOF(fileID) == 0)
		{
			//Reading the line
			uString line;
			line.SetStr(agk::ReadLine(fileID));

			uString dataType = DataReader::getType(line);

			if(dataType.CompareTo("Image") == 0)
			{
				std::string imgName;
				imgName = GF::getPath( DataReader::getValue(line) );

				if(agk::GetFileExists(imgName.data()))
				{
					image = (GF::getPath(DataReader::getValue(line)));	
				}
				else
				{
					DebugConsole::addC("Image: "); DebugConsole::addC(imgName.data());
					DebugConsole::addC(" for particle "); DebugConsole::addC(filename);
					DebugConsole::addC(" --- "); DebugConsole::addToLog(filePath);
				}
			}
			if(dataType.CompareTo("Angle") == 0)
			{
				angle = float(atof(DataReader::getValue(line)));
			}
			if(dataType.CompareTo("DirectionX") == 0)
			{
				dirX = float(atof(DataReader::getValue(line)));
			}
			if(dataType.CompareTo("DirectionY") == 0)
			{
				dirY = float(atof(DataReader::getValue(line)));
			}
			if(dataType.CompareTo("Frequency") == 0)
			{
				frequency = float(atof(DataReader::getValue(line)));
			}
			if(dataType.CompareTo("Life") == 0)
			{
				life = float(atof(DataReader::getValue(line)));
			}
			if(dataType.CompareTo("Size") == 0)
			{
				size = float(atof(DataReader::getValue(line)));
			}
			if(dataType.CompareTo("startX") == 0)
			{
				startX = float(atof(DataReader::getValue(line)));
			}
			if(dataType.CompareTo("startY") == 0)
			{
				startY = float(atof(DataReader::getValue(line)));
			}
			if(dataType.CompareTo("Timeout") == 0)
			{
				timeout = float(atof(DataReader::getValue(line)));
			}

			if(dataType.CompareTo("KeyframeT") == 0)
			{
				int valueAmount = DataReader::getValueAmount(line); //Checking how many values there are
				for(int i = 0; i < valueAmount; i++) //Going thru all of the values
				{
					float value = float(atof(DataReader::getValue(line, i)));

					//Saving the value
					frameT->push_back(value);
				}
			}
			if(dataType.CompareTo("KeyframeR") == 0)
			{
				int valueAmount = DataReader::getValueAmount(line); //Checking how many values there are
				for(int i = 0; i < valueAmount; i++)
				{
					int value = atoi(DataReader::getValue(line, i));

					frameR->push_back(value);
				}
			}
			if(dataType.CompareTo("KeyframeG") == 0)
			{
				int valueAmount = DataReader::getValueAmount(line); //Checking how many values there are
				for(int i = 0; i < valueAmount; i++)
				{
					int value = atoi(DataReader::getValue(line, i));

					frameG->push_back(value);
				}
			}
			if(dataType.CompareTo("KeyframeB") == 0)
			{
				int valueAmount = DataReader::getValueAmount(line); //Checking how many values there are
				for(int i = 0; i < valueAmount; i++)
				{
					int value = atoi(DataReader::getValue(line, i));

					frameB->push_back(value);
				}
			}
			if(dataType.CompareTo("KeyframeA") == 0)
			{
				int valueAmount = DataReader::getValueAmount(line); //Checking how many values there are
				for(int i = 0; i < valueAmount; i++)
				{
					int value = atoi(DataReader::getValue(line, i));

					frameA->push_back(value);
				}
			}
		}
		agk::CloseFile(fileID);

		groupID = ID;

		//Everything has been loaded, create the particle
		PID = agk::CreateParticles(x, y);
		agk::SetParticlesAngle(PID, angle);
		agk::SetParticlesDirection(PID, dirX, dirY);
		agk::SetParticlesFrequency(PID, frequency);
		agk::SetParticlesMax(PID, -1);
		agk::SetParticlesLife(PID, life);
		agk::SetParticlesStartZone(PID, -startX, -startY, startX, startY);

		imgID = agk::LoadImage(image.data());
		agk::SetParticlesImage(PID, imgID);
		agk::SetParticlesSize(PID, size);
		//Hiding the particle
		agk::SetParticlesVisible(PID, 1);

		createdAt = globaltime;
		isFinished = false;

		//Checking forcolor keyframes keyframes
		for(unsigned int i = 0; i < frameT->size(); i++)
		{
			Keyframe tempFrame;

			tempFrame.t = frameT->at(i);

			if(i < frameR->size())
			{
				tempFrame.r = frameR->at(i);
			}
			else
			{
				tempFrame.r = frameR->back();

				//Notifying the user
				DebugConsole::addC("Missing red color for keyframe in particle: ");DebugConsole::addC(filename);
				DebugConsole::addC(" -- ");DebugConsole::addToLog(filePath);
			}

			if(i < frameG->size())
			{
				tempFrame.g = frameG->at(i);
			}
			else
			{
				tempFrame.g = frameG->back();

				//Notifying the user
				DebugConsole::addC("Missing green color for keyframe in particle: ");DebugConsole::addC(filename);
				DebugConsole::addC(" -- ");DebugConsole::addToLog(filePath);
			}

			if(i < frameB->size())
			{
				tempFrame.b = frameB->at(i);
			}
			else
			{
				tempFrame.b = frameB->back();

				//Notifying the user
				DebugConsole::addC("Missing blue color for keyframe in particle: ");DebugConsole::addC(filename);
				DebugConsole::addC(" -- ");DebugConsole::addToLog(filePath);
			}

			if(i < frameA->size())
			{
				tempFrame.a = frameA->at(i);
			}
			else
			{
				tempFrame.a = frameA->back();

				//Notifying the user
				DebugConsole::addC("Missing alpha color for keyframe in particle: ");DebugConsole::addC(filename);
				DebugConsole::addC(" -- ");DebugConsole::addToLog(filePath);
			}

			//Everything is done, add the keyframe
			colorFrame->push_back(tempFrame);
		}

		for(unsigned int i = 0; i < colorFrame->size(); i++)
		{
			agk::AddParticlesColorKeyFrame(PID, colorFrame->at(i).t, colorFrame->at(i).r, colorFrame->at(i).g, colorFrame->at(i).b, colorFrame->at(i).a);
		}

		//Cleaning up the garbage from the keyframes
		frameT->clear();
		delete frameT;
		frameR->clear();
		delete frameR;
		frameG->clear();
		delete frameG;
		frameB->clear();
		delete frameB;
		frameA->clear();
		delete frameA;
	}
	else 
	{
		//The file did not exist
		DebugConsole::addC("Particle file did not exist: "); DebugConsole::addC(filePath);
		DebugConsole::addC(" -- "); DebugConsole::addToLog(filename);
	}

	isFinished = false;
}
void Particle::cloneParticle(int ID, Particle* clonePart, float x, float y)
{
	this->groupID = ID;

	image = (clonePart->getImage());

	this->angle = clonePart->getAngle();
	this->dirX = clonePart->getDirX();
	this->dirY = clonePart->getDirY();
	this->frequency = clonePart->getFrequency();
	this->life = clonePart->getLife();
	this->size = clonePart->getSize();
	this->startX = clonePart->getStartX();
	this->startY = clonePart->getStartY();

	PID = agk::CreateParticles(x, y);
	imgID = agk::LoadImage(image.data());
	agk::SetParticlesImage(PID, imgID);

	agk::SetParticlesAngle(PID, angle);
	agk::SetParticlesDirection(PID, dirX, dirY);
	agk::SetParticlesFrequency(PID, frequency);
	agk::SetParticlesLife(PID, life);
	agk::SetParticlesSize(PID, size);
	agk::SetParticlesStartZone(PID, -startX, -startY, startX, startY);

	//Handeling keyframes
	std::vector< Keyframe >* cloneFrame = clonePart->getKeyframes();
	//Clear the keyframe vector
	colorFrame->clear();
	for(unsigned int i = 0; i < cloneFrame->size(); i++)
	{
		Keyframe tempFrame;
		tempFrame.t = cloneFrame->at(i).t;
		tempFrame.r = cloneFrame->at(i).r;
		tempFrame.g = cloneFrame->at(i).g;
		tempFrame.b = cloneFrame->at(i).b;
		tempFrame.a = cloneFrame->at(i).a;

		colorFrame->push_back(tempFrame);
	}

	for(unsigned int i = 0; i < colorFrame->size(); i++)
	{
		agk::AddParticlesColorKeyFrame(PID, colorFrame->at(i).t, colorFrame->at(i).r, colorFrame->at(i).g, colorFrame->at(i).b, colorFrame->at(i).a);
	}

	isFinished = false;
}
void Particle::remove()
{
	//Clearing pointers
	colorFrame->clear();
	delete colorFrame;

	agk::DeleteParticles(PID);
}
void Particle::removeParticle()
{
	agk::DeleteParticles(PID);
}

void Particle::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	agk::SetParticlesPosition(PID, x, y);
}
void Particle::setVisible(int visible)
{
	this->visible = visible;

	agk::SetParticlesVisible(PID, visible);
}
void Particle::setFinished(bool finished)
{
	this->isFinished = finished;
}

int Particle::getID()
{
	return groupID;
}

std::string Particle::getImage()
{
	return image;
}
float Particle::getAngle()
{
	return angle;
}
float Particle::getDirX()
{
	return dirX;
}
float Particle::getDirY()
{
	return dirY;
}
float Particle::getFrequency()
{
	return frequency;
}
float Particle::getLife()
{
	return life;
}
float Particle::getSize()
{
	return size;
}
float Particle::getStartX()
{
	return startX;
}
float Particle::getStartY()
{
	return startY;
}
bool Particle::getFinished()
{
	return isFinished;
}
std::vector< Particle::Keyframe >* Particle::getKeyframes()
{
	return this->colorFrame;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void ParticleGroup::setup()
{
	//Fixing vectors
	particles = new std::vector< Particle >;

	nextID = 0;
}
void ParticleGroup::update()
{
	for(unsigned int i = 0; i < particles->size();i++)
	{
		particles->at(i).update();
	}

	
	//Creating a vector if iterators to remove particles
	std::vector< int >* removal;
	removal = new std::vector< int >;

	if(particles->size() > 0)
	{
		std::vector< Particle >::iterator it;
		for(it = particles->begin(); it != particles->end(); it++)
		{
			if(it->getFinished() == true) //The particle is done playing, remove it
			{
				removal->push_back(it->getID());
			}
		}
	}

	for(unsigned int i = 0; i < removal->size(); i++)
	{
		std::vector< Particle >::iterator it;
		//Finding the particle with the ID
		if(particles->size() != 0)
		{
			for(it = particles->begin(); it != particles->end(); it++)
			{
				if(it->getID() == removal->at(i))
				{
					it->removeParticle();

					particles->erase(it);

					break; //Exiting the first loop
				}
			}
		}
		break;
	}

	//Clearing garbage
	removal->clear();
	delete removal;
	
}

int ParticleGroup::addFromFile(uString filename, float x, float y)
{
	Particle tempPart;
	tempPart.setup();
	tempPart.addFromFile(nextID, filename, x, y);

	particles->push_back(tempPart);

	//Increasing the ID
	nextID = nextID + 1;
	return nextID - 1;
}
int ParticleGroup::addFromClone(int cloneID)
{
	Particle* clonePart = findByID(cloneID);

	if(clonePart != NULL)
	{
		Particle tempPart;
		tempPart.setup();
		tempPart.cloneParticle(nextID, clonePart, 25, 25);
		
		particles->push_back(tempPart);

		nextID = nextID + 1;
		return nextID - 1;
	}
	else
	{
		return 0;
	}
}
void ParticleGroup::removeParticles(int ID)
{
	Particle* part = findByID(ID);

	if(part != NULL)
	{
		part->setFinished(true);
	}

	/*
	Particle* part = findByID(ID);

	if(part != NULL)
	{
		part->remove();

		//Creating an iterator to go though the particles array
		std::vector< Particle >::iterator it;
		for(it = particles->begin(); it != particles->end(); it++)
		{
			if(it->getID() == ID)
			{
				//Removing that element
				particles->erase(it);

				break;
			}
		}
	}
	*/
}

void ParticleGroup::setParticlePosition(int ID, float x, float y)
{
	Particle* part = findByID(ID);

	part->setPosition(x, y);
}
void ParticleGroup::setParticleVisible(int ID, int visible)
{
	Particle* part = findByID(ID);

	if(part != NULL)
	{
		part->setVisible(visible);
	}
}

Particle* ParticleGroup::findByID(int ID)
{
	Particle* result = NULL;
	for(unsigned int i = 0; i < particles->size(); i++)
	{
		if(particles->at(i).getID() == ID)
		{
			result = &particles->at(i);
		}
	}
	
	return result;
}