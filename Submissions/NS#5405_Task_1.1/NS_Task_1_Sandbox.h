/*
*
* Team Id: #5405
* Author List: <Name of the team memberswho worked on this function
* Rashmil, Preet, Parag, Palash>
* Filename: Sandbox.cpp
* Theme: Nutty Squirrel
* Functions: void sense(void), int line_follow(void)
* GlobalVariables:None
*
*/
#include "NS_Task_1_Predef.h"
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


