#pragma once

#include "agk.h"

#include "Part.h"
#include "DebugConsole.h"

#include <vector>
#include <list>
#include <string>

class NodeLink
{
public:
	void setNode(int index, int ID);

	int getNode(int index);
	
	bool isBadLink();
	bool compareTo(NodeLink link);
private:
	int node[2];

	int type;
};


class PathNode
{
public:
	void create(int ID, float x, float y);
	void setPos(float x, float y);

	void addLink(int ID, int type);

	int getID();
	float getX();
	float getY();
	unsigned int getLinkAmount();
	int getLinkID(unsigned int slot);
private:
	int ID;

	float x;
	float y;

	struct Link
	{
		int ID;
		int type;
	};

	std::vector< Link >* links;
};

class World
{
public:
	World(void);
	~World(void);

	struct worldSprite //There will be one of these per image in the world. They will be cloned to create the world parts
	{
		uString file;

		int SID;
		int imgID;
	};

	struct Entry
	{
		uString name;
		float x;
		float y;
	};

	void begin(); //Used as constructor

	int checkForWS(uString name); //Will look thru the list of world sprites and return the ID of any existing sprite with the same filename
	int createWS(uString name);	//Creates a hidden sprite and returns the ID in the worldSprite list

	Entry* findEntry(uString name);

	void loadBaseMedia();
	void load(uString name);
	void update(float playerX, float playerY);
	void clear();
	void generateBackgroundTextures();

	void loadV3(char* p, int fileID);
	void loadV4(char* p, int fileID);

	void setOvercast(float overcast);
	void setTime(float time);
	void setCloudLayers(int cloudLayers);
	void setLastActive(int lastActive);

	Part* getPartFromName(uString name); //This function goes thru all the parts and looks for one with the name specified //It will return the first part with the name
	Part* getPartFromID(int partID);
	int getPartAmount();
	int getPartSID(int partID); //Returns the sprite id of a part in the world
	int getPartPhysState(int partID);
	int getPartUsable(int partID);
	float getPartX(int partID);
	float getPartY(int partID);
	int getSkyID();
	float getTime();
	float getWidth();
	float getHeight();
	float getLeftEdge();
	float getBottomEdge();
	int getLastActive();

	bool isGround(float x, float y);
	
	//int getEntryAmount();
	//Entry* getEntry(int entryID);

	uString getName();

	//Backdrop
	void loadBG();
	void updateBG(float playerX, float playerY);
	float paralaxOffset(int depth); //Returns the procent that the sprite should be offset based on it's depth

	void setLightModeOn(); //Set to one will set the world in a mode for making light calculations
	void setLightModeOff();

	//Pathfinding functions
	void generateNodes();
	void displayNodes();
	PathNode* findNodeById(int ID);
	unsigned int getNodeAmount();
	PathNode* findNodeBySlot(unsigned int slot);

	NodeLink getClosestLink(float x, float y);

	void addPartToUpdate(Part* part);
private:
	struct Cloud
	{
		int SID;
		float x;
		float y;
		int depth;
	};

	uString name;

	std::vector< Part >* part;
	std::list< Part* >* partsToUpdate;

	std::vector< worldSprite >* wS;

	std::vector< Entry >* entry;

	int skyID;
	/*unsigned int skyR;
	unsigned int skyG;
	unsigned int skyB;*/
	int skyShader;
	int cloudLayers;

	int bgTexture[8];

	int starAmount;

	struct Star
	{
		int SID;
		float x;
		float y;
	};

	std::vector< Star >* stars;
	std::vector< Cloud >* clouds;

	struct Background
	{
		int SID;
		int imgID;
		float x;
		float y;

		int depth;
	};

	struct CloudBase
	{
		int img;
		int SID;
	};

	std::vector< CloudBase >* cloudBase;
	
	//The size of the world
	float minX;
	float maxX;
	float xDist;

	Background bg[4];
	Background dist[4];

	float overcast;

	float time;

	int lightSky;

	//Pathfinding variables
	std::vector< PathNode >* node;

	int lastActive; //The name of the part that was activated last
};