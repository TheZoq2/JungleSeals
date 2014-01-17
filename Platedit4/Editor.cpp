#include "Editor.h"


Editor::Editor(void)
{
	
}


Editor::~Editor(void)
{
}

void Editor::setup()
{
	parts = new std::vector< Part >;
	selParts = new std::vector< int >;

	cameraX = (float)agk::GetVirtualWidth();
	cameraY = (float)agk::GetVirtualWidth();
	cameraZoom = 1;
}

void Editor::update(int selTool, bool uiActive)
{
	float sMoveX = i_sMoveX;
	float sMoveY = i_sMoveY;
	float moveX = i_moveX;
	float moveY = i_moveY;
	float mouseX = i_mx;
	float mouseY = i_my;
	
	float scrollAmount = Input::scrollAmount();

	if(Input::mouseClick()) //If the mouse was just clicked
	{
		//Preventing movement from happening when the user was not holding the mouse
		moveX = 0;
		moveY = 0;
	}

	if(uiActive == false)
	{
		if(Input::mouseClick())
		{
			//Selecting new parts
			for(unsigned int i = 0; i < parts->size(); i++)
			{
				//Checking for collision
				if(parts->at(i).isHit(mouseX, mouseY))
				{
					if(Input::selectMultiple() == false)
					{
						selParts->clear();
					}

					//checking if the part is already selected
					bool alreadySelected = false;
					for(unsigned int n = 0; n < selParts->size(); n++)
					{
						if(selParts->at(n) == parts->at(i).getVecID())
						{
							alreadySelected = true;
						}
					}
					if(alreadySelected == false)
					{
						selParts->push_back(parts->at(i).getVecID());
					}
				}
			}

			if(selTool == tool_place)
			{
				if(agk::GetFileExists(selImage.data()))
				{
					Part part;
					part.create(nextPart, selImage, mouseX, mouseY, e_defaultDepth);

					parts->push_back(part);

					nextPart++;
				}
			}
		}
	}
	if(Input::mouseHold())
	{
		if(selTool == tool_move)
		{
			//Going through all the selected parts
			for(unsigned int i = 0; i < selParts->size(); i++)
			{
				Part* part = this->findPartByID(selParts->at(i));
				
				if(part != NULL)
				{
					part->move(moveX, moveY);
				}
			}
		}
		if(selTool == tool_scale)
		{
			Part* part0;
			if(selParts->size() > 0)
			{
				part0 = findPartByID(selParts->at(0));
			}
			//Going thru all the selected parts
			for(unsigned int i = 0; i < selParts->size(); i++)
			{
				Part* part = this->findPartByID(selParts->at(i));
				
				if(part != NULL)
				{
					float xScale = moveX / 500.0f;
					float yScale = moveX / 500.0f;
					float xScaleChange = 1 - xScale;
					float yScaleChange = 1 - yScale;
					part->scale(xScaleChange, xScaleChange);

					//Moving parts towards the center
					if(part0 != NULL)
					{
						float xDist = part->getX() - part0->getX();
						float yDist = part->getY() - part0->getY();

						float newXDist = xDist * xScaleChange;
						float newYDist = yDist * yScaleChange;

						float newX = part0->getX() + newXDist;
						float newY = part0->getY() + newYDist;

						part->setPosition(newX, newY);
					}
				}
			}
		}
		if(selTool == tool_rotate)
		{
			float lowestX = 1000000;
			float lowestY = 1000000;
			float highestX = - 100000;
			float highestY = - 100000;
			float xCenter = 0;
			float yCenter = 0;
			for(unsigned int i = 0; i < selParts->size(); i++)
			{
				Part* part = findPartByID(selParts->at(i));
				if(part != NULL)
				{
					if(part->getX() < lowestX) lowestX = part->getX();
					if(part->getY() < lowestY) lowestY = part->getY();
					if(part->getX() > highestX) highestX = part->getX();
					if(part->getX() > highestY) highestY = part->getY();
				}
			}

			//Calculating the center point of the parts
			xCenter = lowestX + (highestX - lowestX) / 2.0f;
			yCenter = lowestY + (highestY - lowestY) / 2.0f;

			float xDiff = mouseX - xCenter;
			float yDiff = mouseY - yCenter;
			float angle = agk::ATanFull(xDiff, yDiff);

			//Rotating those parts
			for(unsigned int i = 0; i < selParts->size(); i++)
			{
				Part* part = findPartByID(selParts->at(i));
				if(part != NULL)
				{
					float angleDiff = angle - part->getAngle();

					part->setAngle(angle);

					//Positioning the sprites based on the rotation and offset from the center
					float offsetX = part->getX() - xCenter;
					float offsetY = part->getY() - yCenter;
					float offsetDist = sqrt(pow(offsetX, 2) + pow(offsetY, 2));

					float offsetAngle = agk::ATanFull(offsetX, offsetY);
					agk::Print(offsetAngle);

					float newX = xCenter + agk::Cos(offsetAngle - 90 + angleDiff) * offsetDist;
					float newY = yCenter + agk::Sin(offsetAngle - 90 + angleDiff) * offsetDist;
				}
			}
		}
	}

	//Highlighting the selected parts
	for(unsigned int i = 0; i < selParts->size(); i++)
	{
		Part* part = findPartByID(selParts->at(i));

		if(part != NULL)
		{
			part->highlight(255, 255, 255);
		}
	}

	//Camera movement
	if(Input::cameraMove())
	{
		cameraX = cameraX + -sMoveX / cameraZoom;
		cameraY = cameraY + -sMoveY / cameraZoom;
	}
	cameraZoom = cameraZoom + 0.01f * scrollAmount;

	agk::SetViewZoom(cameraZoom);
	agk::SetViewOffset(cameraX - agk::GetVirtualWidth() / 2, cameraY - agk::GetVirtualHeight() / 2);
}
void Editor::updateSelectedWindow( UI* uiGroup)
{
	if(selParts->size() == 0) //If no parts are selected
	{
		if(uiGroup->getWindowExists("selected"))
		{
			uiGroup->removeWindow("selected");
		}
	}
	else
	{
		if(selParts->size() == 1)
		{
			if(lastSelID != selParts->at(0))
			{
				uiGroup->removeWindow("selected");
			}

			lastSelID = selParts->at(0);
			Part* part = findPartByID(selParts->at(0));

			if(uiGroup->getWindowExists("selected") == 0)
			{
				uiGroup->addWindow("selected", "1x1.png", (float)agk::GetVirtualWidth() - 250.0f, 0, 250.0f, (float)agk::GetVirtualHeight());
				uiGroup->setWindowColor("selected", 150, 150, 150, 255);

				uiGroup->addEditboxToWindow("selected", "name", 5, 5, 250, 16);
				uiGroup->setEditboxValue("selected", "name", part->getData());

				uiGroup->addSimpleListToWindow("selected", "dataList", 5, 25, 240, 200, "Data");
			}

			//Adding the data to the data list
			uiGroup->clearList("selected", "dataList");
			std::string listVal = "Depth: ";
			char* p = agk::Str(part->getDepth());
			listVal.append(p);
			delete p;
			uiGroup->addToSimpleList("selected", "dataList", listVal);
			p = agk::Str(part->getPhysState());
			listVal = "Phys state: ";
			listVal.append(p);
			delete p;
			uiGroup->addToSimpleList("selected", "dataList", listVal);


			part->setData(uiGroup->getEditboxValue("selected", "name"));

			
		}
		else //If there is more than 1 part selected
		{
			if(uiGroup->getWindowExists("selected"))
			{
				uiGroup->removeWindow("selected");
			}
		}
	}
}

void Editor::incPhysState(int amount)
{
	for(unsigned int i = 0; i < selParts->size(); i++)
	{
		Part* part = findPartByID(selParts->at(i));

		part->incPhysState(amount);
	}
}
void Editor::incDepth(int amount)
{
	for(unsigned int i = 0; i < selParts->size(); i++)
	{
		Part* part = findPartByID(selParts->at(i));

		part->incDepth(amount);
	}
}

void Editor::setSelImage(std::string selImage)
{
	this->selImage = selImage;
}

Part* Editor::findPartByID(int ID)
{
	for(unsigned int i = 0; i < parts->size(); i++)
	{
		if(parts->at(i).getVecID() == ID)
		{
			return &parts->at(i);
		}
	}

	return NULL;
}

void Editor::saveMap(std::string filename)
{
	//Creating the file
	int fileID = agk::OpenToWrite(filename.data());
		//Writing the filename
		agk::WriteString(fileID, "1"); //Version
		//Writing the amount of data
		agk::WriteString(fileID, "0"); //Special data

		//Writing the actual level data
		std::string partAmount = agk::Str((signed)parts->size());
		agk::WriteString(fileID, partAmount.data());
		for(unsigned int i = 0; i < parts->size(); i++)
		{
			std::string imgName = parts->at(i).getImgName();
			float xPos = parts->at(i).getX();
			float yPos = parts->at(i).getY();
			float scaleX = parts->at(i).getScaleX();
			float scaleY = parts->at(i).getScaleY();
			float angle = parts->at(i).getAngle();
			int physState = parts->at(i).getPhysState();
			int depth = parts->at(i).getDepth();

			//Starting a new part
			agk::WriteString(fileID, "|");
			
			std::string partString = DataEditor::createData("imgName", imgName);
			partString.append(";");
			partString.append(DataEditor::createData("xPos", xPos));
			partString.append(";");
			partString.append(DataEditor::createData("yPos", yPos));
			partString.append(";");
			partString.append(DataEditor::createData("scaleX", scaleX));
			partString.append(";");
			partString.append(DataEditor::createData("scaleY", scaleY));
			partString.append(";");
			partString.append(DataEditor::createData("angle", angle));
			partString.append(";");
			partString.append(DataEditor::createData("physState", physState));
			partString.append(";");
			partString.append(DataEditor::createData("depth", depth));
			partString.append(";");
			partString.append(parts->at(i).getData());

			//Writing the data
			agk::WriteString(fileID, partString.data());
		}

		agk::WriteString(fileID, "0");
	agk::CloseFile(fileID);
}

void Editor::loadMap(std::string filename)
{
	//Checking if the file exists
	if(agk::GetFileExists(filename.data()))
	{
		//Starting to read the file
		int fileID = agk::OpenToRead(filename.data());

		//Pointer for removing the data once it has been read
		char* p;
		p = agk::ReadString(fileID); //Version
		delete[] p;

		p = agk::ReadString(fileID); //Special data
		delete[] p;

		p = agk::ReadString(fileID); //Part amount
		int partAmount = agk::Val(p);
		delete[] p;
		for(int i = 0; i < partAmount; i++) //Reading all the parts
		{
			//Default values
			std::string imgName = "ERROR:NoValue";
			float xPos;
			float yPos;
			float scaleX = 1;
			float scaleY = 0;
			float angle = 0;
			int depth = 100;
			int physState = 1;

			std::string additionalData;

			p = agk::ReadString(fileID); // '|'
			delete[] p;

			std::string partData = agk::ReadString(fileID);

			//Handleing the part data
			std::vector< std::string > dataSegments = zString::split(partData, ";");

			for(unsigned int i = 0; i < dataSegments.size(); i++)
			{
				//Splitting the data into type and value
				std::vector< std::string > dataParts = zString::split(dataSegments.at(i), ":");

				//Making sure the data is correct
				if(dataParts.size() == 2)
				{
					std::string dataType = dataParts.at(0);
					std::string dataValue = dataParts.at(1);

					if(dataType.compare("imgName") == 0)
					{
						imgName = dataValue;
					}
					else if(dataType.compare("xPos") == 0)
					{
						xPos = agk::ValFloat(dataValue.data());
					}
					else if(dataType.compare("yPos") == 0)
					{
						yPos = agk::ValFloat(dataValue.data());
					}
					else if(dataType.compare("scaleX") == 0)
					{
						scaleX = agk::ValFloat(dataValue.data());
					}
					else if(dataType.compare("scaleY") == 0)
					{
						scaleY = agk::ValFloat(dataValue.data());
					}
					else if(dataType.compare("angle") == 0)
					{
						angle = agk::ValFloat(dataValue.data());
					}
					else if(dataType.compare("physState") == 0)
					{
						physState = agk::Val(dataValue.data());
					}
					else if(dataType.compare("depth") == 0)
					{
						depth = agk::Val(dataValue.data());
					}
					else
					{
						additionalData.append(dataType);
						additionalData.append(":");
						additionalData.append(dataValue);
						additionalData.append(";");
					}
				}
			}

			//All the data has been loaded from the file
			//Making sure important paramaters were set
			if(imgName.compare("ERROR:NoValue") != 0)
			{
				Part tempPart; //Creating the new part
				tempPart.create(nextPart, imgName, xPos, yPos, depth);
				tempPart.setScale(scaleX, scaleY);
				tempPart.setPosition(xPos, yPos);
				tempPart.setAngle(angle);
				tempPart.setPhysState(physState);
				tempPart.setDepth(depth);
				tempPart.setData(additionalData);

				nextPart++;
				parts->push_back(tempPart);
			}
		}
	}
	else
	{
		DebugConsole::addC("Failed to load level: ");DebugConsole::addC(filename.data());
		DebugConsole::addToLog(" -- file did not exist");
	}
}