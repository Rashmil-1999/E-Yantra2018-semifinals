/*
*
* Team Id: #5405
* Author List: <Name of the team memberswho worked on the functions are
* Rashmil, Preet, Parag, Palash>
* Filename: Sandbox.cpp
* Theme: Nutty Squirrel
*/

#define F_CPU 14745600UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.c"
#include "logic.c"
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

while (P_Node != 6)
{
travel(currentNode, pickupPositions[P_Node]);
// // 		forward();
// // 		velocity(20, 20);
// // 		_delay_ms(2);
pick_nut();
if (picked_flag == 1) {
travel(currentNode, D_Node);
// 			printf("\n\nPLACED\n\n");
place();
}
P_Node++;
}
travel(currentNode, START);

}
/*
    * Function Name:main
    * Input:void
    * Output:void     
    * Logic: To call all the functions for completing the whole task
    * Example Call:---
 */


int main(void)
{
    /* Replace with your application code */
   init_devices();
   lcd_set_4bit();
   lcd_init();
   while(1){
   _delay_ms(5000);
   orient_claws();
   forward_wls(1);
   break;
   }
   return 0;
}

