#include "Item.h"


std::vector< Item >* item;
std::vector< ItemProt >* itemProt;

int ItemGen::attribAmount;
int ItemGen::weaponAmount;
int ItemGen::materialAmount;
ItemProt ItemGen::material[10];
ItemProt ItemGen::attrib[10];
ItemProt ItemGen::weapon[10];

Item::Item(void)
{
	
}

Item::~Item(void)
{
}

void Item::setName(uString name)
{
	this->name.SetStr(name);
}
void Item::setItem(Item item)
{
	this->armor = item.armor;
	this->damage = item.damage;
	this->equipSlot = item.equipSlot;
	this->heal = item.heal;
	this->name.SetStr(item.name);
	this->speed = item.speed;
	this->type = item.type;
}
void Item::addStage(int value)
{
	stage.push_back(value); //Add the stage to the value
}

uString Item::getName()
{
	return this->name;
}
uString Item::getCommand()
{
	unsigned int stageAmount = stage.size();

	return "";
}

void ItemGen::setup() //Setting up the item generator
{
	//Setting up the item vectors
	item = new std::vector< Item >;
	itemProt = new std::vector< ItemProt >;

	//Creating item prototypes
	weapon[0].name.SetStr("sword");
	weapon[0].damage = 1.0f;
	weapon[0].attSpeed = 1.0f;
	weapon[0].equipSlot = 1;
	
	weapon[1].name.SetStr("axe");
	weapon[1].damage = 1.1f;
	weapon[1].attSpeed = 0.9f;

	weapon[2].name.SetStr("spear");
	weapon[2].damage = 0.9f;
	weapon[2].attSpeed = 0.85f;

	weapon[3].name.SetStr("dagger");
	weapon[3].damage = .75f;
	weapon[3].attSpeed = 1.2f;

	weapon[4].name.SetStr("hammer");
	weapon[4].damage = 1.2f;
	weapon[4].attSpeed = 0.7f;
	
	weapon[5].name.SetStr("greatsword");
	weapon[5].damage = 1.4f;
	weapon[5].attSpeed = 1.0f;

	weapon[6].name.SetStr("war axe");
	weapon[6].damage = 1.5f;
	weapon[6].attSpeed = 0.8f;

	weapon[7].name.SetStr("katana");
	weapon[7].damage = 1.1f;
	weapon[7].attSpeed = 1.1f;

	weapon[8].name.SetStr("flail");
	weapon[8].damage = 1.5f;
	weapon[8].attSpeed = 0.6f;

	weapon[9].name.SetStr("shortsword");
	weapon[9].damage = 0.9f;
	weapon[9].attSpeed = 1.1f;

	weaponAmount = 10;
	//Materials
	material[0].name.SetStr("steel");
	material[0].damage = 1.0f;
	material[0].armor = 1.0f;
	material[0].attSpeed = 1.0f;
	
	material[1].name.SetStr("iron");
	material[1].damage = 0.8f;
	material[1].armor = 0.7f;
	material[1].attSpeed = 1.0f;

	material[2].name.SetStr("clay");
	material[2].damage = 0.5f;
	material[2].armor = 0.5f;
	material[2].attSpeed = 0.9f;
	
	material[3].name.SetStr("bronze");
	material[3].damage = .7f;
	material[3].armor = .7f;
	material[3].attSpeed = 1.0f;

	material[4].name.SetStr("uranium");
	material[4].damage = 1.0f;
	material[4].armor = 1.0f;
	material[4].attSpeed = 0.7f;

	materialAmount = 5;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	attrib[0].name.SetStr("soft");
	attrib[0].damage = -0.3f;
	attrib[0].armor = -0.4f;
	attrib[0].attSpeed = 0.0f;

	attrib[1].name.SetStr("cracked");
	attrib[1].damage = -0.4f;
	attrib[1].armor = -0.5f;

	attrib[2].name.SetStr("hard");
	attrib[2].damage = 0.1f;
	attrib[2].armor = 0.1f;
	
	attrib[3].name.SetStr("flaming");
	attrib[3].damage = .2f;
	attrib[3].armor = -.2f;

	attrib[4].name.SetStr("light");
	attrib[4].damage = -0.1f;
	attrib[4].attSpeed = 0.3f;

	attribAmount = 5;
}

Item ItemGen::generate(int type)
{
	Item newItem;

	//Generating a weapon
	if(type == 1)
	{
		uString name;
		//Selecting a material
		int weap = rand() % weaponAmount;

		//Selecting a material
		int mat = rand() % materialAmount;
		
		//Selecting attributes
		int atAmount = rand() % 3;
		for( int i = 0; i < atAmount; i++ )
		{
			int att = rand() % attribAmount;
			name.Append(attrib[att].name);
			name.Append(" ");

			//adding it to the stage variable
			newItem.addStage(att);
		}
		
		name.Append(material[mat].name);name.Append(" ");
		name.Append(weapon[weap].name);name.Append(" ");

		//Adding the stages to the stage variable
		newItem.addStage(mat);
		newItem.addStage(weap);

		newItem.setName(name);
	}

	return newItem;
}

ItemGen::ItemGen(void)
{
	
}
ItemGen::~ItemGen(void)
{
	
}

ItemProt::ItemProt(void)
{
	
}
ItemProt::~ItemProt(void)
{
	
}