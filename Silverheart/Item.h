#pragma once

#include <vector>

#include "agk.h"

class Item
{
public:
	Item(void);
	~Item(void);

	void generate( int type );
	void createFromData( int type, int stage1, int stage2, int stage3, int stage4 );//Creates an item from specific parts
	void addStage(int value);

	void setItem(Item item);
	void setName(uString name);

	uString getName();
	uString getCommand(); //This will return the script command for giving an entity the item
private:
	uString name;

	int type;

	std::vector< int > stage;

	int equipSlot; //The slot that the item gets equiped to. 0 if the item is not equippable

	float damage; //The amount of damage that the item makes
	float armor; //The armor provided by the item
	float heal; //The amount healed if consumed
	float speed; //The speed gained if consumed
};


class ItemProt //A prototype for an item to be generated
{
public:
	ItemProt(void);
	~ItemProt(void);

	uString name;

	int type;
	int equipSlot;

	float damage;
	float attSpeed;
	int hands;

	float armor;
	float heal;
	float speed;
};

class ItemGen
{
public:
	ItemGen(void);
	~ItemGen(void);

	static Item generate(int type);
	static void setup();

	
	static int weaponAmount;
	static ItemProt weapon[10];
	static int materialAmount;
	static ItemProt material[10];
	static int attribAmount;
	static ItemProt attrib[10];
};

#ifndef _ITEM_globals
#define _Item_globals
	extern std::vector< Item >* items;
#endif