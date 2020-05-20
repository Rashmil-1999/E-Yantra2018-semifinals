/*
*
* Team Id: #5405
* Author List: <Name of the team memberswho worked on the functions are
* Rashmil, Preet, Parag, Palash>
* Filename: Sandbox.cpp
* Theme: Nutty Squirrel
* Functions: void Square(void), void sense(void), int line_follow(void), bool Solve_AStar(void), void Create_path(void), void display_path(void),
*			 void nodes_init(void), void cleanSlate_and_Reconnect(void), void test_run(void), int bot_traversal(int from, int to, int par_node),
*			 void travel(int start, int end), void correct_turn(int from, int to), int nut_detector(void), void pick_nut(void),
*			 void Task_1_1(void), void Task_1_2(void).
* GlobalVariables: Arena_Size-> Total number of nodes on Arena and size for the Arena matrix
*				   obstacle_flag -> integer to determine encounter of obstacle
*				   currentNode, Going_toNode -> default working node will be start position i.e node - 23
*				   pickupPosition -> Array of nodes marking the pick up zones
*				   RD_zone -> Array of nodes marking Red Nut deposit zone
*				   GD_zone -> Array of nodes marking Green Nut deposit zone
*				   par_zone -> integer corresponding to the node before the currentNode
*				   path -> List to hold nodes corresponding to the path
*
*
*/
#include "NS_Task_1_Sandbox.h"

sNode *nodes = nullptr;
sNode *InitialPos = &nodes[23];//at the start the initial position is the startPosition 
sNode *nodeStart = InitialPos;//default start position is start position
sNode *nodeEnd = nullptr;//destination pointer
sNode *nodeFinalPos = InitialPos;//Finally after completing the task go to initial position

int Arena_Size = 24;//size 
int obstacle_flag = 0, picked_flag = 0;//Flag variable to  determine the encounter of an obstacle
int currentNode = 23, Going_toNode = 23;//default working nodes will be 23 which is start position 
int pickupPositions[] = { 7,8,9,11,12,13 };//the pick up zones
int RD_zone[] = { 0,2 };//red deposit nodes
int RD_index = 0, GD_index = 0, D_Node, P_Node = 0, START = 23;
int GD_zone[] = { 19,21 };//green deposit zones
int par_node = 22;
list<int> path;//list to hold path


int arena[24][24] = { {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
					  {0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
					  {0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},
					  {0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0},
					  {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0},
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0},
					  {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0},
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0},
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0},
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0} };

/*
*
* Function Name: nodes_init
* Input: void
* Output: void
* Logic: Used for initialization of nodes by assigning co-ordinates and providing value to be used in heuristics
* Example Call: nodes_init(); //Initialization
*
*/
void nodes_init(void) {
	nodes = new sNode[24];
	nodes[0].x = 0;
	nodes[0].y = 0;
	nodes[0].node_num = 0;
	nodes[0].node_name = 'A';

	nodes[1].x = 0;
	nodes[1].y = 1;
	nodes[1].node_num = 1;
	nodes[1].node_name = 'B';


	nodes[2].x = 1;
	nodes[2].y = 1;
	nodes[2].node_num = 2;
	nodes[2].node_name = 'C';

	nodes[3].x = 2;
	nodes[3].y = 1;
	nodes[3].node_num = 3;
	nodes[3].node_name = 'D';

	nodes[4].x = 3;
	nodes[4].y = 4;
	nodes[4].node_num = 4;
	nodes[4].node_name = 'E';

	nodes[5].x = 0;
	nodes[5].y = 4;
	nodes[5].node_num = 5;
	nodes[5].node_name = 'F';

	nodes[6].x = 1;
	nodes[6].y = 6;
	nodes[6].node_num = 6;
	nodes[6].node_name = 'G';

	nodes[7].x = 0;
	nodes[7].y = 8;
	nodes[7].node_num = 7;
	nodes[7].node_name = 'H';

	nodes[8].x = 1;
	nodes[8].y = 8;
	nodes[8].node_num = 8;
	nodes[8].node_name = 'I';

	nodes[9].x = 3;
	nodes[9].y = 8;
	nodes[9].node_num = 9;
	nodes[9].node_name = 'J';

	nodes[10].x = 4;
	nodes[10].y = 8;
	nodes[10].node_num = 10;
	nodes[10].node_name = 'K';

	nodes[11].x = 5;
	nodes[11].y = 8;
	nodes[11].node_num = 11;
	nodes[11].node_name = 'L';

	nodes[12].x = 7;
	nodes[12].y = 8;
	nodes[12].node_num = 12;
	nodes[12].node_name = 'M';

	nodes[13].x = 8;
	nodes[13].y = 8;
	nodes[13].node_num = 13;
	nodes[13].node_name = 'N';

	nodes[14].x = 4;
	nodes[14].y = 3;
	nodes[14].node_num = 14;
	nodes[14].node_name = 'O';

	nodes[15].x = 6;
	nodes[15].y = 7;
	nodes[15].node_num = 15;
	nodes[15].node_name = 'P';

	nodes[16].x = 8;
	nodes[16].y = 4;
	nodes[16].node_num = 16;
	nodes[16].node_name = 'Q';

	nodes[17].x = 5;
	nodes[17].y = 2;
	nodes[17].node_num = 17;
	nodes[17].node_name = 'R';

	nodes[18].x = 6;
	nodes[18].y = 1;
	nodes[18].node_num = 18;
	nodes[18].node_name = 'S';

	nodes[19].x = 7;
	nodes[19].y = 1;
	nodes[19].node_num = 19;
	nodes[19].node_name = 'T';

	nodes[20].x = 8;
	nodes[20].y = 1;
	nodes[20].node_num = 20;
	nodes[20].node_name = 'U';

	nodes[21].x = 8;
	nodes[21].y = 0;
	nodes[21].node_num = 21;
	nodes[21].node_name = 'V';

	nodes[22].x = 4;
	nodes[22].y = 1.5;
	nodes[22].node_num = 22;
	nodes[22].node_name = 'W';

	nodes[23].x = 4;
	nodes[23].y = 1;
	nodes[23].node_num = 23;
	nodes[23].node_name = 'X';

	connections();//make connections after initializing the node
}

/*
*
* Function Name: connections
* Input: void
* Output: void
* Logic: Used to make connections after initializing the nodes
* Example Call: connections();
*
*/
void connections(void) {
	for (auto i = 0; i < 24; i++) {
		for (auto j = 0; j < 24; j++) {
			if (arena[i][j] == 1)
			{
				nodes[i].vecNeighbours.push_back(&nodes[j]);
			}
		}
	}
}

/*
*
* Function Name: cleanSlate_and_Reconnect
* Input: void
* Output: void
* Logic: Used to establish new connections as soon as an obstacle is encountered to provide alternate path using A* path planning algorithm
* Example Call: cleanSlate_and_Reconnect();
*
*/
void cleanSlate_and_Reconnect(void) {
	for (int i = 0; i < Arena_Size; i++) {
		nodes[i].vecNeighbours.clear();
	}
	connections();
}

/*
*
* Function Name: forward_wls
* Input: node
* Output: void
* Logic: Uses white line sensors to go forward by the number of nodes specified and also take care of obstacle encountered
* Example Call: forward_wls(2); //Goes forward by two nodes
*
*/
void forward_wls(unsigned char node)
{
	int val;//value to hold the return value from the line follow function which returns -1 when all the snesors detect black (node encountered)
	while (node != 0) {
		val = line_follow();
		if (val == -1)
			node = node + val;//decrease the value of node after each node is encountered
		else {
			obstacle_flag = 1;
			right_turn_wls();
			forward_wls(1);
			break;
		}
	}
}

/*
*
* Function Name: left_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered
* Example Call: left_turn_wls(); //Turns right until black line is encountered
*
*/
void left_turn_wls(void)
{
	_delay_ms(1);
	left();
	velocity(190, 190);
	_delay_ms(230);//200
	while (ADC_Conversion(1) <= 200);
	stop();
}

/*
*
* Function Name: right_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn right until black line is encountered
* Example Call: right_turn_wls(); //Turns right until black line is encountered
*/
void right_turn_wls(void)
{
	_delay_ms(1);
	right();
	velocity(190, 190);
	_delay_ms(230);//200
	while (ADC_Conversion(3) <= 200);
	stop();
}

/*
*
* Function Name: Square
* Input: void
* Output: void
* Logic: Use this function to make the robot trace a square path on the arena
* Example Call: Square();
*/
void Square(void)
{

	forward();
	_delay_ms(1000);
	left();
	_delay_ms(136);
	forward();
	_delay_ms(1000);
	left();
	_delay_ms(150);
	forward();
	_delay_ms(1000);
	left();
	_delay_ms(150);
	forward();
	_delay_ms(1000);
	left();
	_delay_ms(160);
	stop();
	_delay_ms(5000);
}

/*
*
* Function Name: Task_1_1
* Input: void
* Output: void
* Logic: this function uses the functions defined in this file to move the bot from node to node depending upon specific turn conditions and makes turns
due to the software rendering we need to run  the stimulations several times if the output is not as desired but majority of times the bot reaches the goal position
* Example Call: Task_1_1();
*/
void Task_1_1(void)
{
	//go to node 1
	forward_wls(1);
	right_turn_wls();
	right_turn_wls();
	//right_turn_wls();    this function might be needed at the first node due to 2 encounters of the black line in some cases

	//go to node 2
	forward_wls(1);
	left_turn_wls();

	//go to node 3
	forward_wls(1);
	left_turn_wls();

	//go to node 4
	forward_wls(1);
	left_turn_wls();

	//go to node 5
	forward_wls(1);
	right_turn_wls();

	//go to node 6,7
	forward_wls(3);
	right_turn_wls();

	//go to node 9
	forward_wls(1);
	left_turn_wls();

	//go to node 10
	forward_wls(1);
	left_turn_wls();

	//go to node 11
	forward_wls(1);
	left_turn_wls();

	//go to node 12
	forward_wls(1);
	right_turn_wls();

	//go towards goal
	forward_wls(1);

	//make a u turn
	right_turn_wls();
	stop();
	_delay_ms(5000);
}

/*
*
* Function Name : Task_1_2
* Input : void
* Output : void
* Logic : Used this function to encapsulate your Task 1.2 logic
* Example Call : Task_1_2();
*/
void Task_1_2()
{
	nodes_init();
	while (P_Node != 6)
	{
		travel(currentNode, pickupPositions[P_Node]);
		forward();
		velocity(20, 20);
		_delay_ms(2);
		pick_nut();
		if (picked_flag == 1) {
			travel(currentNode, D_Node);
			printf("\n\nPLACED\n\n");
			place();
		}
		P_Node++;
	}
	travel(currentNode, START);
}

/*
*
* Function Name: line_follow
* Input: void
* Output: returns an integer value -1 when a node is encountered
* Logic: the basic logic of the line following algo used here is to turn left as soon as the left sensor starts reading the black line  and turns right if the
* right sensor readsthe black line and stop if all the sensors read black
* Example Call: line_follow()
*
*/
int line_follow(void) {
	int Flag = 0, prox_sen = 100, left_sen, right_sen, mid_sen;//to disable other checking of if statements
	forward();
	velocity(150, 150);
	while (true)
	{
		_delay_ms(1);
		Flag = 0;//reset flag
		left_sen = ADC_Conversion(1);
		mid_sen = ADC_Conversion(2);
		right_sen = ADC_Conversion(3);
		prox_sen = ADC_Conversion(4);
		//condition to stop on encountering the node
		if ((left_sen >= 230) && (mid_sen >= 230) && (right_sen >= 230) && (Flag == 0)) {
			Flag++;
			forward();
			velocity(150, 150);//142,142
			printf("Complete Forward\n\n");
			_delay_ms(575);//600
			stop();
			return -1;
		}
		//turn slight left
		if ((left_sen >= 230) && (mid_sen >= 230) && (Flag == 0)) {
			forward();
			velocity(175, 225);//140,180
			//velocity(165, 200);//set the left motor's velocity less than the right so that bot drifts towards left
			_delay_ms(4);//5
			Flag++;
		}
		//turn slight right
		else if ((right_sen >= 230) && (mid_sen >= 230) && (Flag == 0)) {
			forward();
			velocity(225, 175);//180,140
			//velocity(200, 165);//set the right  motor's velocity less than the left so that the bot drifts towards right
			_delay_ms(4);//5
			Flag++;
		}
		//turn right
		else if ((right_sen >= 230) && (mid_sen <= 230) && (Flag == 0)) {
			soft_right();
			velocity(255, 175);
			//velocity(210, 160);//increase left motor's velocity for better right turn as forward velocity is being set to 150,150
			_delay_ms(8);//10
			Flag++;
		}
		//turn left
		else if ((left_sen >= 230) && (mid_sen <= 230) && (Flag == 0)) {
			soft_left();
			velocity(175, 255);//160,230
			//velocity(160, 210);//increase right motor's velocity for better left turn as forward velocity is being set to 150,150
			_delay_ms(8);//10
			Flag++;
		}
		//continue moving forward if only mid sensor is detecting the line
		else if ((left_sen <= 100) && (right_sen <= 100) && (mid_sen >= 230) && (Flag == 0)) {

			forward();
			velocity(230, 230);
			_delay_ms(20);//25
			Flag++;
		}
		//if all white keep going forward
		else {
			back();
			velocity(145, 145);
			_delay_ms(25);
		}

		if (prox_sen <= 25 && prox_sen > 0) {
			stop();
			printf("OH my my an obstacle gotta go back to the cave where i crept from \n");
			return 0;
		}


	}
}


/*
*
* Function Name: Solve_AStar
* Input: void
* Output: bool value : true
* Logic: Uses A-Star algorithm to find the shortest path between two nodes
* Example Call: Solve_AStar();
*
*/
bool Solve_AStar(void)
{
	obstacle_flag = 0;
	// Reset Navigation Graph - default all node states
	for (int i = 0; i < Arena_Size; i++)
	{
		nodes[i].bVisited = false;
		nodes[i].fGlobalGoal = INFINITY;
		nodes[i].fLocalGoal = INFINITY;
		nodes[i].parent = nullptr;	// No parents
	}

	auto distance = [](sNode* a, sNode* b) // For convenience
	{
		return sqrtf((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
	};

	auto heuristic = [distance](sNode* a, sNode* b) // So we can experiment with heuristic
	{
		return distance(a, b);
	};

	// Setup starting conditions
	sNode *nodeCurrent = nodeStart;
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

	// Add start node to not tested list - this will ensure it gets tested.
	// As the algorithm progresses, newly discovered nodes get added to this
	// list, and will themselves be tested later
	list<sNode*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);

	// if the not tested list contains nodes, there may be better paths
	// which have not yet been explored. However, we will also stop 
	// searching when we reach the target - there may well be better
	// paths but this one will do - it wont be the longest.
	while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
	{
		// Sort Untested nodes by global goal, so lowest is first
		listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		// Front of listNotTestedNodes is potentially the lowest distance node. Our
		// list may also contain nodes that have been visited, so ditch these...
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
			listNotTestedNodes.pop_front();

		// ...or abort because there are no valid nodes left to test
		if (listNotTestedNodes.empty())
			break;

		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->bVisited = true; // We only explore a node once


		// Check each of this node's neighbours...
		for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
		{
			// ... and only if the neighbour is not visited and is 
			// not an obstacle, add it to NotTested List
			if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
				listNotTestedNodes.push_back(nodeNeighbour);

			// Calculate the neighbours potential lowest parent distance
			float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

			// If choosing to path through this node is a lower distance than what 
			// the neighbour currently has set, update the neighbour to use this node
			// as the path source, and set its distance scores as necessary
			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = nodeCurrent;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

				// The best path length to the neighbour being tested has changed, so
				// update the neighbour's score. The heuristic is used to globally bias
				// the path algorithm, so it knows if its getting better or worse. At some
				// point the algo will realise this path is worse and abandon it, and then go
				// and search along the next best path.
				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
			}
		}
	}
	Create_path();
	return true;
}

/*
*
* Function Name: Create_path
* Input: void
* Output: void
* Logic: Used to make a link between all the nodes that form a part of the shortest path between two nodes
* Example Call: Create_path();
*
*/
void Create_path(void) {
	if (nodeEnd->parent != nullptr)
	{
		sNode *p = nodeEnd;
		while (p->parent != nullptr)
		{
			path.push_front(p->node_num);

			// Set next node to this node's parent
			p = p->parent;
		}
		path.push_front(p->node_num);

	}
	else
	{
		printf("no path found\n");
	}
}

/*
*
* Function Name: display_path
* Input: void
* Output: void
* Logic: Used to display all the nodes that form a part of the shortest distance between two nodes.Used for testing A-Star algorithm
* Example Call: display_name();
*
*/
void display_path(void) {
	for (auto i = path.begin(); i != path.end(); ++i)
	{
		printf("node-> %d \n", *i);
	}
}

/*
*
* Function Name: bot_traversal
* Input: from -> integer corresponding to current position node of bot
*		 to -> integer corresponding to next position node of bot
*		 par_node -> integer corresponding to previous position  node of bot
* Output: from -> integer corresponding to current position node of bot
* Logic: Used to guide the bot to traverse the path between two nodes depending on it's orientation i.e direction from which it is coming towards node.
* Example Call: bot_traversal(int from, int to, int par_node);
*
*/
int bot_traversal(int from, int to, int par_node) {

	if ((from == 23 && to == 22) || (from == 22 && to == 23)) {//for traversals between node 23 and 22 from 14 and 23 itself
		if (par_node == 14) {
			forward();
			velocity(200, 200);
			_delay_ms(800);
			left_turn_wls();
		}
		else
			forward_wls(1);
	}
	else if ((from == 22 && to == 14) || (from == 14 && to == 22)) {//conditions to fo from node 22 to 14 or coming from 10,4 or 17 
		if (par_node == 4) {
			right_turn_wls();
			//right_turn_wls();
		}
		else if (par_node == 17) {
			left_turn_wls();
			//left_turn_wls();
		}
		forward_wls(1);
	}
	else if ((from == 14 && to == 4) || (from == 4 && to == 14)) {//for traversals between node 14 and 4 from 3,6,10,22,17
		if (par_node == 22 || par_node == 6)
			left_turn_wls();
		else if (par_node == 10)
			right_turn_wls();
		forward_wls(1);
	}
	else if ((from == 4 && to == 6) || (from == 6 && to == 4)) {//for traversals between node 4 and 6 from 5,8,3,14
		if (par_node == 3)
			left_turn_wls();
		else if (par_node == 14 || par_node == 5)
			right_turn_wls();
		forward_wls(1);
	}
	else if ((from == 6 && to == 8) || (from == 8 && to == 6)) {//for traversals between node 8 and 6 from 5,4,7,9 and 8 itself
		if (par_node == 5 || par_node == 9)
			left_turn_wls();
		else if (par_node == 7)
			right_turn_wls();
		else if (par_node == 8) {
			right_turn_wls();
			right_turn_wls();
		}
		forward_wls(1);
	}
	else if ((from == 8 && to == 7) || (from == 7 && to == 8)) {//for traversals between node 7 and 8 from 5,6,9 and 7 and 8 itself
		if (par_node == 6 || par_node == 8)
			left_turn_wls();
		else if (par_node == 5 || par_node == 7)
			right_turn_wls();
		forward_wls(1);
	}
	else if ((from == 7 && to == 5) || (from == 5 && to == 7)) {//for traversals between node 5 and 7 from 1,6,8 and 7 itself
		if (par_node == 8 || par_node == 7)
			left_turn_wls();
		else if (par_node == 6)
			right_turn_wls();
		forward_wls(1);
	}
	else if ((from == 5 && to == 1) || (from == 1 && to == 5)) {//for traversals between node 5 and 1 from 6,7,0,2
		if (par_node == 6)
			left_turn_wls();
		else if (par_node == 2)
			right_turn_wls();
		forward_wls(1);
	}
	else if ((from == 1 && to == 2) || (from == 2 && to == 1)) {//for traversals between node 1 and 2 from 3,5,0 and 2 itself
		if (par_node == 5)
			left_turn_wls();
		else if (par_node == 0 || par_node == 2)
			right_turn_wls();
		forward_wls(1);
	}
	else if ((from == 2 && to == 3) || (from == 3 && to == 2)) {//for traversals between node 2 and 3 from 1,18,4 and 2 itself
		if (par_node == 4)
			right_turn_wls();
		else if (par_node == 18 || par_node == 2)
			left_turn_wls();
		forward_wls(1);
	}
	else if ((from == 3 && to == 4) || (from == 4 && to == 3)) {//for traversals between node 3 and 4 from 2,18,14,6
		if (par_node == 2)
			left_turn_wls();
		else if (par_node == 6)
			right_turn_wls();
		forward_wls(1);
	}
	else if ((from == 0 && to == 1) || (from == 1 && to == 0)) {//for traversals between node 1 and 0 from 5,2 and 0 itself
		if (par_node == 2 || par_node == 0)
			left_turn_wls();
		forward_wls(1);
	}
	else if ((from == 8 && to == 9) || (from == 9 && to == 8)) {//for traversals between node 9 and 8 from 7,6,10 and 9 and 8 itself
		if (par_node == 6 || par_node == 8)
			right_turn_wls();
		else if (par_node == 9)
			left_turn_wls();
		forward_wls(1);
	}
	else if ((from == 10 && to == 9) || (from == 9 && to == 10)) { //for traversals between node 9 and 10 from 8,11,14 and 9 itself
		if (par_node == 14)
			left_turn_wls();
		else if (par_node == 9)
			right_turn_wls();
		forward_wls(1);
	}
	else if ((from == 3 && to == 18) || (from == 18 && to == 3)) { //for traversals between node 3 and 18 from 2,4,17,19 
		if (par_node == 19)
			left_turn_wls();
		else if (par_node == 2)
			right_turn_wls();
		forward_wls(1);
	}
	else if ((from == 5 && to == 6) || (from == 6 && to == 5)) { //for traversals between node 5 and 6 from 1,7,4,8 
		if (par_node == 7 || par_node == 4)
			left_turn_wls();
		else if (par_node == 1 || par_node == 8)
			right_turn_wls();
		forward_wls(1);
	}
	//TO FRO 10 & 14 Coming From 4,11,17,9
	else if ((from == 14 && to == 10) || (from == 10 && to == 14)) {
		if (par_node == 4 || par_node == 11)
			left_turn_wls();
		else if (par_node == 17 || par_node == 9)
			right_turn_wls();
		forward_wls(1);
	}
	//TO FRO 14 & 17 Coming From 10,22,15,18,4
	else if ((from == 14 && to == 17) || (from == 17 && to == 14)) {
		if (par_node == 10)
			left_turn_wls();
		else if (par_node == 22 || par_node == 15)
			right_turn_wls();
		forward_wls(1);
	}
	//TO FRO 17 & 18 Coming From 15,19,14,3
	else if ((from == 17 && to == 18) || (from == 18 && to == 17)) {
		if (par_node == 15)
			left_turn_wls();
		else if (par_node == 19)
			right_turn_wls();
		forward_wls(1);
	}
	//TO FRO 17 & 15 Coming From 14,16,18,12
	else if ((from == 17 && to == 15) || (from == 15 && to == 17)) {
		if (par_node == 18)
			right_turn_wls();
		else if (par_node == 14 || par_node == 16)
			left_turn_wls();
		forward_wls(1);
	}
	//TO FRO 15 & 16 Coming From 13,12,17,20
	else if ((from == 15 && to == 16) || (from == 16 && to == 15)) {
		if (par_node == 17 || par_node == 13)
			right_turn_wls();
		else if (par_node == 12 || par_node == 20)
			left_turn_wls();
		forward_wls(1);
	}
	//TO FRO 16 & 13 Coming From 12,13,15,20
	else if ((from == 16 && to == 13) || (from == 13 && to == 16)) {
		if (par_node == 15)
			left_turn_wls();
		else if (par_node == 12 || par_node == 13)
			right_turn_wls();
		forward_wls(1);
	}
	//TO FRO 12 & 15 Coming From 14 16,11,13,12,17
	else if ((from == 12 && to == 15) || (from == 15 && to == 12)) {
		if (par_node == 16 || par_node == 11)
			right_turn_wls();
		else if (par_node == 13)
			left_turn_wls();
		else if (par_node == 12) {
			right_turn_wls();
			right_turn_wls();
		}
		forward_wls(1);
	}
	//TO FRO 12 & 11 Coming From 15,12,11,10,13
	else if ((from == 11 && to == 12) || (from == 12 && to == 11)) {
		if (par_node == 15 || par_node == 12)
			left_turn_wls();
		else if (par_node == 11)
			right_turn_wls();
		forward_wls(1);
	}
	//TO FRO 11 & 10 Coming From 14,11,9,12
	else if ((from == 11 && to == 10) || (from == 10 && to == 11)) {
		if (par_node == 14)
			right_turn_wls();
		else if (par_node == 11)
			left_turn_wls();
		forward_wls(1);
	}
	//TO FRO 12 & 13 Coming From 12,15,16,13,11
	else if ((from == 12 && to == 13) || (from == 13 && to == 12)) {
		if (par_node == 15 || par_node == 12)
			right_turn_wls();
		else if (par_node == 13 || par_node == 16)
			left_turn_wls();
		forward_wls(1);
	}
	//TO FRO 20 & 16 Coming From 19,15,21,13
	else if ((from == 16 && to == 20) || (from == 20 && to == 16)) {
		if (par_node == 15)
			right_turn_wls();
		else if (par_node == 19)
			left_turn_wls();
		forward_wls(1);
	}
	//TO FRO 19 & 18 Coming From 19,3,17,20
	else if ((from == 18 && to == 19) || (from == 19 && to == 18)) {
		if (par_node == 17)
			left_turn_wls();
		else if (par_node == 3 || par_node == 19)
			right_turn_wls();
		forward_wls(1);
	}
	//TO FRO 20 & 19 Coming From 19,16,21,18
	else if ((from == 20 && to == 19) || (from == 19 && to == 20)) {
		if (par_node == 19 || par_node == 21)
			left_turn_wls();
		else if (par_node == 16)
			right_turn_wls();
		forward_wls(1);
	}
	//TO FRO 20 & 21 Coming From 21,19,16
	else if ((from == 21 && to == 20) || (from == 20 && to == 21)) {
		if (par_node == 19 || par_node == 21)
			right_turn_wls();
		forward_wls(1);
	}
	return from;
}

/*
*
* Function Name: test_run
* Input: void
* Output: void
* Logic: Used to call bot_traversal(int from, int to, int par_node) function corresponding to nodes obtained from path(list) and make changes
*		 if obstacle encountered
* Example Call: test_run();
*
*/

void test_run(void) {
	int n = 0;

	while (n != 2) {
		n = path.size();
		currentNode = path.front();
		path.pop_front();
		Going_toNode = path.front();
		printf("Current node: %d Going to: %d \n", currentNode, Going_toNode);

		par_node = bot_traversal(currentNode, Going_toNode, par_node);
		if (obstacle_flag == 1) {
			par_node = Going_toNode;
			arena[currentNode][Going_toNode] = 0;
			arena[Going_toNode][currentNode] = 0;
			cleanSlate_and_Reconnect();
			path.clear();
			nodeStart = &nodes[currentNode];
			break;
		}
	}
	if (n == 2)
		correct_turn(currentNode, Going_toNode);
	if (obstacle_flag == 0)
		currentNode = path.front();
	path.clear();
	printf("parent :%d current :%d\n", par_node, currentNode);
}

/*
*
* Function Name: travel
* Input: start-> integer
*		 end -> integer
* Output: void
* Logic: Calls Solve_AStar() function to find the shortest path and than calls test run to guide the bot on the path and resolves obstacle problem
*		 by maintaining the node position and calling for a new path.
* Example Call: nut_detector(void);
*
*/

void travel(int start, int end)
{
	nodeStart = &nodes[start];
	nodeEnd = &nodes[end];
	Solve_AStar();
	test_run();
	if (obstacle_flag == 1) {
		travel(currentNode, end);
	}
	par_node = end;
}

/*
*
* Function Name: nut_detector
* Input: void
* Output: returns a value corresponding to the color of nut detected
* Logic: Used to detect nuts of different colors
* Example Call: nut_detector(void);
*
*/

int nut_detector(void) {
	unsigned int red_pulse_count, green_pulse_count;
	filter_red();
	_delay_ms(5);
	red_pulse_count = color_sensor_pulse_count;
	printf("\n%d\n", red_pulse_count);
	filter_green();

	_delay_ms(5);
	green_pulse_count = color_sensor_pulse_count;
	printf("\n%d\n", green_pulse_count);
	filter_clear();
	if (red_pulse_count > 2000)
		return 1;
	else if (green_pulse_count > 2000)
		return 2;
	else
		return 3;

}

/*
*
* Function Name: pick_nut
* Input: void
* Output: void
* Logic: Used to guide the bot to place the nut according to its color in the corresponding deposit zone
* Example Call: pick_nut();
*
*/

void pick_nut(void) {
	if (nut_detector() == 1) {
		D_Node = RD_zone[RD_index];
		RD_index++;
		pick();
		picked_flag = 1;
		_delay_ms(10);
	}
	else if (nut_detector() == 2) {
		D_Node = GD_zone[GD_index];
		GD_index++;
		pick();
		picked_flag = 1;
		_delay_ms(10);
	}
	else {
		picked_flag = 0;
	}

}

/*
*
* Function Name: correct_turn
* Input: from->integer corresponding to the current position node of the bot
*        to ->integer corresponding to next position node of the bot
* Output: void
* Logic: Used to guide the bot which direction to turn to pick nuts depending on the orientation of bot.
* Example Call: correct_turn
*
*/
void correct_turn(int from, int to) {
	if ((from == 8 && to == 7) || (from == 9 && to == 8) || (from == 10 && to == 9) || (from == 12 && to == 11) || (from == 13 && to == 12) || (from == 18 && to == 19) || (from == 1 && to == 2))
		right_turn_wls();
	else if ((from == 7 && to == 8) || (from == 8 && to == 9) || (from == 10 && to == 11) || (from == 11 && to == 12) || (from == 12 && to == 13) || (from == 3 && to == 2) || (from == 20 && to == 19))
		left_turn_wls();
	_delay_ms(15);
}