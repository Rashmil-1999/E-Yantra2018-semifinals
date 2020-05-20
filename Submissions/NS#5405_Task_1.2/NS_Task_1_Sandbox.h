/*
*
* Team Id: #5405
* Author List: <Name of the team memberswho worked on this function
* Rashmil, Preet, Parag, Palash>
* Filename: Sandbox.cpp
* Theme: Nutty Squirrel
* Functions: void Square(void), void sense(void), int line_follow(void), bool Solve_AStar(void), void Create_path(void), void display_path(void),
*			 void nodes_init(void), void cleanSlate_and_Reconnect(void), void test_run(void), int bot_traversal(int from, int to, int par_node),
*			 void travel(int start, int end), void correct_turn(int from, int to), int nut_detector(void), void pick_nut(void),
*			 void Task_1_1(void), void Task_1_2(void).
* GlobalVariables:None
*
*/
#include "NS_Task_1_Predef.h"

#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <iterator>

using std::array;
using std::vector;
using std::string;
using std::list;


extern unsigned int color_sensor_pulse_count;
/*
* Function Name: forward_wls
* Input: node
* Output: void
* Logic: Uses white line sensors to go forward by the nodes specified
* Example Call: forward_wls(2) //Goes forward by two nodes
*
*/
void forward_wls(unsigned char node);

/*
*
* Function Name: left_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered
* Example Call: left_turn_wls(); //Turns right until black line is encountered
*
*/

void left_turn_wls(void);

/*
*
* Function Name: right_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn right until black line is encountered
* Example Call: right_turn_wls(); //Turns right until black line is encountered
*/

void right_turn_wls(void);

/*
*
* Function Name: Task_1_1
* Input: void
* Output: void
* Logic: this function uses the functions defined in this file to move the bot from node to node depending upon specific turn conditions and makes turns
due to the software rendering we need to run  the stimulations several times if the output is not as desired but majority of times the bot reaches the goal position
* Example Call: Task_1_1();
*/

void Task_1_1(void);

void Task_1_2(void);

void Square(void);

/*
*
* Function Name: line_follow
* Input: void
* Output: returns an integer value -1 when a node is encountered
* Logic: the basic logic of the line following algo used here is to turn left as soon as the left sensor starts reading the black line  and turns right if the
* right sensor reads the black line ,move slight left if left and mid both read black and move slight right if both right and mid read black and stop if all the sensors read black
* Example Call: line_follow()
*
*/

int line_follow(void);

struct sNode
{
	char node_name;
	int node_num;
	bool bObstacle = false;			// Is the node an obstruction?
	bool bVisited = false;			// Have we searched this node before?
	float fGlobalGoal;				// Distance to goal so far
	float fLocalGoal;				// Distance to goal if we took the alternative route
	float x;							// Nodes position in 2D space
	float y;
	vector<sNode*> vecNeighbours;	// Connections to neighbours
	sNode* parent;					// Node connecting to this node that offers shortest parent
};


/*
*
* Function Name: Solve_AStar
* Input: void
* Output: bool value : true
* Logic: Uses A-Star algorithm to find the shortest path between two nodes
* Example Call: Solve_AStar();
*
*/
bool Solve_AStar(void);

/*
*
* Function Name: Create_path
* Input: void
* Output: void
* Logic: Used to make a link between all the nodes that form a part of the shortest path between two nodes
* Example Call: Create_path();
*
*/
void Create_path(void);

/*
*
* Function Name: display_path
* Input: void
* Output: void
* Logic: Used to display all the nodes that form a part of the shortest distance between two nodes.Used for testing A-Star algorithm
* Example Call: display_name();
*
*/
void display_path(void);

/*
*
* Function Name: nodes_init
* Input: void
* Output: void
* Logic: Used for initialization of nodes by assigning co-ordinates and providing value to be used in heuristics
* Example Call: nodes_init(); //Initialization
*
*/
void nodes_init(void);

/*
*
* Function Name: connections
* Input: void
* Output: void
* Logic: Used to make connections after initializing the nodes
* Example Call: connections();
*
*/
void connections(void);

/*
*
* Function Name: cleanSlate_and_Reconnect
* Input: void
* Output: void
* Logic: Used to establish new connections as soon as an obstacle is encountered to provide alternate path using A* path planning algorithm
* Example Call: cleanSlate_and_Reconnect();
*
*/
void cleanSlate_and_Reconnect(void);

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
void test_run(void);

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
int bot_traversal(int from, int to, int par_node);

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
void travel(int start, int end);

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
void correct_turn(int from, int to);

/*
*
* Function Name: nut_detector
* Input: void
* Output: returns a value corresponding to the color of nut detected
* Logic: Used to detect nuts of different colors
* Example Call: nut_detector(void);
*
*/
int nut_detector(void);

/*
*
* Function Name: pick_nut
* Input: void
* Output: void
* Logic: Used to guide the bot to place the nut according to its color in the corresponding deposit zone
* Example Call: pick_nut();
*
*/
void pick_nut(void);

