#pragma once

#include <string>
#include <vector>

#include "Part.h"
#include "Input.h"
#include "DataEditor.h"
#include "DebugConsole.h"
#include "UI.h"

#include "zString.h"

#define e_defaultDepth 100
#define tool_select 0
#define tool_place 1
#define tool_move 2
#define tool_scale 3
#define tool_rotate 4

class Editor
{
public:
	Editor(void);
	~Editor(void);

	void setup();
	void update(int tool, bool uiActive);
	void updateSelectedWindow(UI* uiGroup);

	void setSelImage(std::string selImage);

	void saveMap(std::string filename);
	void loadMap(std::string filename);

	void incPhysState(int amount);
	void incDepth(int depth);
private:
	int nextPart;
	std::string selImage;

	std::vector< Part >* parts;
	std::vector< int >* selParts;

	Part* findPartByID(int ID);

	float lastRotation;

	float cameraX;
	float cameraY;
	float cameraZoom;

	int lastSelID;
};

