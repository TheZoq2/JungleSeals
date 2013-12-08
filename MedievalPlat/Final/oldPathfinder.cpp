//Finding the closest nodes to the goal
	NodeLink* goalLink;
	
	goalLink = world->getClosestLink(goalX, goalY);

	//Getting the current link
	NodeLink* cLink;
	cLink = world->getClosestLink(chr.getFeetX(), chr.getFeetY());

	//If one of the positions are to far away
	if(cLink->isBadLink() || goalLink->isBadLink())
	{
		return false; //Exiting the function
	}

	//Checking if the links are the same
	if(goalLink->compareTo(cLink) == true)
	{
		return true;
	}

	//No nodes were bad, start to look for a path between the nodes
	float lowestDist = 1000000000;

	PathNode* goalNode;
	PathNode* startNode;

	//Finding a good starting node
	for(int i = 0; i < 2; i++)
	{
		PathNode* cGoalNode = world->findNodeById(goalLink.getNode(i));
		for(int n = 0; n < 2; n++)
		{
			PathNode* cStartNode = world->findNodeById(cLink.getNode(n));

			float xDist = cGoalNode->getX() - cStartNode->getX();
			float yDist = cGoalNode->getY() - cStartNode->getY();

			float dist = sqrt(pow(xDist, 2) + pow(yDist, 2));

			//If these nodes are closer than the old ones
			if(dist < lowestDist)
			{
				//Saving the new nodes
				lowestDist = dist;
				goalNode = cGoalNode;
				startNode = cStartNode;
			}
		}
	}

	//Making sure nothing weird happned
	if(goalNode == NULL || startNode == NULL)
	{
		return false;
	}

	//Everything is good, lets start looking for a good node
	int openState = 1;
	int closedState = 2;
	

	std::vector< listElement >* nodeList;
	nodeList = new std::vector< listElement >;

	//Adding the starting node to the open list
	listElement tempElement;
	tempElement.node = startNode->getID();
	tempElement.state = openState;
	tempElement.FScore = 0; //Total score
	tempElement.HScore = 0; //Estemated distance left
	tempElement.GScore = 0; //Walk cost
	tempElement.parent = -1;

	nodeList->push_back(tempElement); //Add the first node to the list

	bool pathFound = false;
	int finalNode = -1; //The final node in the chain

	while(pathFound == false) //Running the actual pathfinder
	{
		float lowestFScore = 100000000;

		int nextNode = -1;
		int listSlot = -1;

		//Finding the node with the lowest FScore
		for(unsigned int i = 0; i < nodeList->size(); i++)
		{
			if(nodeList->at(i).state == openState && nodeList->at(i).FScore < lowestFScore)
			{
				lowestFScore = nodeList->at(i).FScore;

				nextNode = nodeList->at(i).node;
				listSlot = i;
			}
		}

		if(nextNode == -1) //No open nodes left, a path could not be found
		{
			//Removing garbage
			nodeList->clear();
			delete nodeList;
			return false;
		}

		PathNode* cNode = world->findNodeById(nextNode);
		//PathNode* cNode = goalNode;

		//Open the nodes linked to the current node
		for(unsigned int i = 0; i < cNode->getLinkAmount(); i++)
		{
			int node = cNode->getLinkID(i);

			//Checking if the node is already on the list
			bool onList = false;
			int listIndex;
			int listState = 0;

			for(unsigned int n = 0; n < nodeList->size(); n++)
			{
				if(nodeList->at(n).node == node) //If the node is on the list
				{
					onList = true;

					listIndex = n;
					listState = nodeList->at(n).state;
				}
			}
			
			//This is a new node or it is open
			//if(onList == false)
			//{
				//Calculating the walk cost between this and the next node
				PathNode* thisNode = world->findNodeById(node);

				float xDist = thisNode->getX() - cNode->getX();
				float yDist = thisNode->getY() - cNode->getY();
				float hScore = sqrt(pow(xDist, 2) + pow(yDist, 2)) + nodeList->at(listSlot).HScore;
				
				//If the node was on the list already
				if(onList == true && listState == openState)
				{
					//Checking if this has a better HScore
					if(hScore < nodeList->at(listIndex).HScore)
					{
						//Making this node the parent instead
						nodeList->at(listIndex).HScore = hScore;
						nodeList->at(listIndex).FScore = hScore + nodeList->at(listSlot).GScore;

						nodeList->at(listIndex).parent = listSlot;
					}
				}
				else if(listState == closedState) //Ignoring closed nodes
				{}
				else
				{
					thisNode = world->findNodeById(node);;
					//Calculating the score
					float xDist = thisNode->getX() - cNode->getX();
					float yDist = thisNode->getY() - cNode->getY();
					float hScore = sqrt(pow(xDist, 2) + pow(yDist, 2)) + nodeList->at(listSlot).HScore;

					float xLeft = thisNode->getX() - goalNode->getX();
					float yLeft = thisNode->getY() - goalNode->getY();
					float gScore = sqrt(pow(xLeft, 2) + pow(yLeft, 2));

					//Add it to the list
					listElement tempNode;
				
					tempNode.node = node;
					tempNode.parent = listSlot;
					
					tempNode.HScore = hScore;
					tempNode.FScore = hScore;

					//Opening the node
					tempNode.state = openState;

					//Add the new node to the list
					nodeList->push_back(tempNode);
				}
			//}
		}

		//Close the current node
		nodeList->at(listSlot).state = closedState;

		//Checking if the goal node has been added to the open list
		for(unsigned int i = 0; i < nodeList->size(); i++)
		{
			if(nodeList->at(i).node == goalNode->getID() && nodeList->at(i).state == closedState) //The goal has been added to the closed list
			{
				pathFound = true;

				finalNode = i; //SAving the location of the final node
			}
		}
	}
	
	if(finalNode == -1)
	{
		return false;
	}
	else
	{
		bool pathSaved = false;
		
		//Selecting the last node
		int thisNode = finalNode;

		//adding the node to the path deque
		path->push_front(nodeList->at(thisNode).node);
		while(pathSaved == false)
		{
			thisNode = nodeList->at(thisNode).parent;

			if(thisNode == -1) //If this is the final node
			{
				pathSaved = true;
			}
			else
			{
				//add the node to the path deque
				path->push_front(nodeList->at(thisNode).node);
			}
		}
	}
	//Everything is done, the path has been saved properly

	//Removing garbage
	nodeList->clear();
	delete nodeList;