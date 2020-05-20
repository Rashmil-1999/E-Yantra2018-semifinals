/*
*
* Team Id: #5405
* Author List: <Name of the team memberswho worked on the functions are
* Rashmil, Preet, Parag, Palash>
* Filename: Sandbox.cpp
* Theme: Nutty Squirrel
* Functions:void sense(void) int line_follow(void)
* GlobalVariables:None
*
*/

#include "NS_Task_1_Sandbox.h"


/*
*
* Function Name: forward_wls
* Input: node
* Output: void
* Logic: Uses white line sensors to go forward by the number of nodes specified
* Example Call: forward_wls(2); //Goes forward by two nodes
*
*/
void forward_wls(unsigned char node)
{
	int val;//value to hold the return value from the line follow function which returns -1 when all the snesors detect black (node encountered)
	while (node != 0) {
		val = line_follow();
		node = node + val;//decrease the value of node after each node is encountered
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
	_delay_ms(165);
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
	_delay_ms(165);
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
	_delay_ms(136.13);
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
* Logic : Use this function to encapsulate your Task 1.2 logic
* Example Call : Task_1_2();
*/

void Task_1_2()
{

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
	int Flag = 0;//to disable other checking of if statements
	forward();
	velocity(180, 180);
	while (true)
	{
		_delay_ms(1);
		Flag = 0;//reset flag
		//condition to stop on encountering the node
		if ((ADC_Conversion(1) >= 241) && (ADC_Conversion(2) >= 241) && (ADC_Conversion(3) >= 241) && (Flag == 0)) {
			Flag++;
			stop();
			forward();
			velocity(142, 142);
			_delay_ms(600);
			stop();
			return -1;
		}
		//turn slight left
		if ((ADC_Conversion(1) >= 230) &&  (ADC_Conversion(2) >= 230) && (Flag == 0)){
			Flag++;
			forward();
			velocity(140, 180);//set the left motor's velocity less than the right so that bot drifts towards left
			_delay_ms(5);
		}
		//turn slight right
		else if((ADC_Conversion(3) >= 230) && (ADC_Conversion(2) >= 230) && (Flag == 0)){
			Flag++;
			forward();
			velocity(180, 140);//set the right  motor's velocity less than the left so that the bot drifts towards right
			_delay_ms(5);
		}
		//turn right
		else if ((ADC_Conversion(3) >= 230) && (ADC_Conversion(2) <= 230) && (Flag == 0)) {
			Flag++;
			soft_right();
			velocity(200, 160);//increase left motor's velocity for better right turn as forward velocity is being set to 150,150
			_delay_ms(10);
		}
		//turn left
		else if ((ADC_Conversion(1) >= 230) && (ADC_Conversion(2) <= 230) && (Flag == 0)) {
			Flag++;
			soft_left();
			velocity(160, 200);//increase right motor's velocity for better left turn as forward velocity is being set to 150,150
			_delay_ms(10);
		}
		//continue moving forward if only mid sensor is detecting the line
		else if ((ADC_Conversion(1) <= 100) && (ADC_Conversion(3) <= 100) && (ADC_Conversion(2) >= 230) && (Flag == 0)) {
			Flag++;
			forward();
			velocity(200, 200);
			_delay_ms(25);
		}
		//if all white keep going forward
		else {
			forward();
			velocity(130, 130);
			_delay_ms(25);
		}
	}
}

