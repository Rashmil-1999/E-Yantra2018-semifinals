/*
*
* Team Id: #5405
* Author List: <Name of the team memberswho worked on the functions are
* <Rashmil, Preet, Parag, Palash>
* Filename: Sandbox.cpp
* Theme: Nutty Squirrel
* Functions: int line_follow(void),void test_run(void), int bot_traversal(int from, int to, int par_node),
*			 void travel(int start, int end), void correct_turn(int from, int to), int nut_detector(void), void pick_nut(void),
			void port_init(),void init_devices,void led_port_config(),void servo1(),void servo2(),void servo_1_free(),void servo_2_free(),
			void pick(),void place()
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
*/#define F_CPU 14745600UL
//#define __OPTIMIZE__ -O0
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define INFINITY 9999
#define MAX 28
int Arena_Size = 28;//size
int obstacle_flag = 0, picked_flag = 0;//Flag variable to  determine the encounter of an obstacle
int currentNode = 23, Going_toNode = 23;//default working nodes will be 23 which is start position
int pickupPositions[] = { 7,8,9,11,12,13 };//the pick up zones
int S1_zone[] = { 0,2 };//red deposit nodes
int S2_zone[] = { 19,21 };//green deposit zones
int S3_zone[] = { 25,26 };//blue deposit zones
int RD_index = 0, GD_index = 0, BD_index = 0, D_Node, P_Node = 0, START = 23;
int par_node = 23;
int front =-1,rear=-1,top=-1;
int path[MAX];

int servo1_downlim = 0; //Pick servo 1 movement down
int servo1_uplim = 140;//Servo 1 neutral position
int servo2_openlim = 30;//Servo 2 neutral position
int servo2_closelim = 10;//Servo 2 grip value
//list<int> path;//list to hold path
// The arena is represented in form of a matrix of weights
//                     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
int arena[28][28] = { {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {1,0,1,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//                     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
					  {0,0,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0},
					  {0,0,0,2,0,0,1,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,8,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//                     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
					  {0,0,0,0,1,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,8,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,2,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//                     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
					  {0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0,0,1,0,1,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//                     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
					  {0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0},
				      {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,3,0,0,0,0,0,3,0,0,0,0,0,0,3,0,0,0,0,0,1,0,0,0,0},
//                     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
					  {0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,2,0,0,0,0,8,0,0,0,0,0,0,0},
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,1,0,0,2,0,0,0,0,0,0,0,0,0},
//                     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,1,0,0,3,0,0,0,0,0},
				      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0},
				      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,1,0,1,0,0,0,0,0,0},
//                     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
					  {0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,1,0,0,0},
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
//                     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
				      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
				      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
					  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0}, };


void port_init();
void timer5_init();
void velocity(unsigned char, unsigned char);
void motors_delay();

unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
unsigned char flag = 0;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;
/*
  ?  * Function Name: void lcd_port_config (void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:Function to configure LCD port
  ?  * Example Call: lcd_port_config()  
  */ 

void lcd_port_config (void)
{
 DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
 PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}
/*
  ?  * Function Name: adc_pin_config
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:ADC pin configuration
  ?  * Example Call:adc_pin_config()
  */ 

void adc_pin_config (void)
{
 DDRF = 0x00; 
 PORTF = 0x00;
 DDRK = 0x00;
 PORTK = 0x00;
}

/*
variable unsigned int blue stores value of blue pulses
variable unsigned int red stores value of red pulses
variable unsigned int green stores value of green pulses
variable unsigned int Pulse stores value of pulses received by color sensor
unsigned int rthreshold has a fixed value for red pulses
unsigned int bthreshold has a fixed value for blue pulses
unsigned int gthreshold has a fixed value for green pulses
*/

unsigned int PULSE = 0, blue, red, green;
unsigned int rthreshold = 5000,gthreshold = 2000,bthreshold = 2000;//set it to the value below which u want to turn the led off


 /*
  ?  * Function Name: void rgb_port_config (void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     Define DDR and PORT values for the port on which RGB LED is connected
  ?  * Example Call:  rgb_port_config();   
  */ 
 
void rgb_port_config (void)
{
	/*****************************************
	Define DDR and PORT values for the port on which RGB LED is connected
	******************************************/
	DDRA = DDRA | 0xE0; //port 27->blue_led 28->green_led 29->red_led of port A 
	PORTA = PORTA | 0xE0;
}
/* led terminals
+ve -> +5V
red terminal -> 29
blue terminal -> 26
green terminal -> 27
*/

/*
    * Function Name: led_color_set
    * Input:     color
    * Output:    void
    * Logic:    to set color of lcd
    * Example Call:  led_color_set();  
  */ 


void led_color_set(unsigned char color)
{
	unsigned char PortARestore = 0;

	color &= 0xF0; 		// removing upper ininibbel for the protection
	PortARestore = PORTA; 		// reading the PORTA original status
	PortARestore &= 0x0F; 		// making lower direction nibbel to 0
	PortARestore |= color; // adding lower nibbel for forward command and restoring the PORTA status
	PORTA = PortARestore; 		// executing the command
}

/*
  ?  * Function Name: void all_off (void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     set this for all off for common anode led and 0x00 for common cathode 
  ?  * Example Call:  all_off();   
  */ 
void all_off(void){
	PORTA = 0xFF;//set this for all off for common anode led and 0x00 for common cathode 
}

/*
  ?  * Function Name: lcd_reset(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     a function to reset the lcd screen to blank screen (wait for 1 second and then reset)
  ?  * Example Call:  lcd_reset();  
  */ 

void lcd_reset(void){
	_delay_ms(700);
	lcd_cursor(1,1);
	lcd_string("                ");
	lcd_cursor(2,1);
	lcd_string("                ");
}

/*
  ?  * Function Name: color_sesnor_pin_config(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:    Define DDR and PORT values for the port on which Color sensor is connected
  ?  * Example Call:  color_sesnor_pin_config();  
  */ 

void color_sensor_pin_config(void)
{
	/*****************************************
	Define DDR and PORT values for the port on which Color sensor is connected
	******************************************/
	DDRB = 0xF0;      //using pins 13->s3 12->s2 11->s1 10->s0
	PORTB = 0x00;
	DDRD = DDRD & 0xFE;//using pin 21-> output
	PORTD = PORTD & 0xFE;
}

/*
  ?  * Function Name: color_sesnor_pin_interrupt_init(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:    Initialization of pins when interrupt occurs
  ?  * Example Call:  color_sesnor_pin_interrupt_init();  
  */ 
void color_sensor_pin_interrupt_init(void) //Interrupt 0 enable
{
	cli(); //Clears the global interrupt
	EICRA = EICRA | 0x02; // INT0 is set to trigger with falling edge
	EIMSK = EIMSK | 0x01; // Enable Interrupt INT0 for color sensor
	sei(); // Enables the global interrupt
}

/*
  ?  * Function Name: ISR(INTn_vect)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     Triggered when interrupt occurs
  ?  * Example Call:  ISR(INT0_vect); 
  */ 
//ISR for color sensor
ISR(INT0_vect) // Pass the timer number in place of n in INTn_vect
{
	//increment on receiving pulse from the color sensor
	++PULSE;
}

/*
  ?  * Function Name: filter_red(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     Used to select red filter
  ?  * Example Call:  filter_red(); 
  */ 
//Filter Selection
void filter_red(void)    //Used to select red filter
{
	//Filter Select - red filter
	 //set S2 low
	 //set S3 low
	 PORTB = 0x10;
	 
}

/*
  ?  * Function Name: filter_green(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     Used to select green filter
  ?  * Example Call:  filter_green(); 
  */ 
void filter_green(void)	//Used to select green filter
{
	//Filter Select - green filter
	 //set S2 High
	//set S3 High
	PORTB = 0xD0;
	
}

/*
  ?  * Function Name: filter_blue(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     Used to select blue filter
  ?  * Example Call:  filter_blue(); 
  */ 
void filter_blue(void)	//Used to select blue filter
{
	//Filter Select - blue filter
	//set S2 low
	//set S3 High
	PORTB =0x90;
	
}

/*
  ?  * Function Name: filter_clear(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     Used to select no filter
  ?  * Example Call:  filter_clear(); 
  */ 
void filter_clear(void)	//select no filter
{
	//Filter Select - no filter
	//set S2 High
	//set S3 Low
	PORTB = PORTB | 0x80;
}

/*
  ?  * Function Name: color_sensor_scaling(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     This function is used to select the scaled down version of the original frequency of the output generated by the color sensor, generally 20% scaling is preferable, though you can change the values as per your application by referring datasheet
  ?  * Example Call:  color_sensor_scaling(); 
  */ 
void color_sensor_scaling(void)		
{
	//Output Scaling 20% from datasheet
	
	//set S0 high
	//set S1 low
	PORTB = 0x10;
}

/*
  ?  * Function Name: red_read(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     function to select red filter and display the count generated by the sensor on LCD. The count will be more if the color is red. The count will be very less if its blue or green.
  ?  * Example Call:  red_read(); 
  */ 
void red_read(void) // 
{
	//Red

	filter_red(); //select red filter
	PULSE=0; //reset the count to 0
	_delay_ms(400); //capture the pulses for 400 ms or 0.4 second to soften red pulse count
}

/*
  ?  * Function Name: green_read(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     function to select green filter and display the count generated by the sensor on LCD. The count will be more if the color is green. The count will be very less if its blue or red.
  ?  * Example Call:  green_read(); 
 */ 
void green_read(void) 
{
	//Green
	
	filter_green(); //select green filter
	PULSE=0; //reset the count to 0
	_delay_ms(450); //capture the pulses for 100 ms or 0.1 second
}

/*
  ?  * Function Name: blue_read(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     function to select blue filter and display the count generated by the sensor on LCD. The count will be more if the color is blue. The count will be very less if its green or red.
  ?  * Example Call:  blue_read(); 
 */ 
void blue_read(void) 
{
	//Blue
	
	filter_blue(); //select blue filter
	PULSE=0; //reset the count to 0
	_delay_ms(450); //capture the pulses for 100 ms or 0.1 second
}	

/*
  ?  * Function Name: servo1_pin_config
  ?  * Input:  void   
  ?  * Output:void     
  ?  * Logic: To configure servo1    
  ?  * Example Call:servo1_pin_config()
 */ 
void servo1_pin_config (void)
{
	DDRH  = DDRH | 0x08; //0x08 //making PORTH 3 pin output
	PORTH = PORTH | 0x08; //setting PORTB 5 pin to logic 1
}
/*
  ?  * Function Name: servo1_pin_config
  ?  * Input:  void   
  ?  * Output:void     
  ?  * Logic: To configure servo1    
  ?  * Example Call:servo1_pin_config()
 */
//Configure PORTH 4 pin for servo motor 2 operation
void servo2_pin_config (void)
{
	DDRH  = DDRH | 0x10; //0x10 //making PORTH 4 pin output
	PORTH = PORTH | 0x10; //setting PORTH 4 pin to logic 1
}

/*
  ?  * Function Name: motion_pin_config
  ?  * Input:void   
  ?  * Output:void     
  ?  * Logic: Function to configure ports to enable robot's motion  
  ?  * Example Call:motion_pin_config()
 */

void motion_pin_config (void) 
{
 DDRA = DDRA | 0x0F;
 PORTA = PORTA & 0xF0;
 DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
 PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}
/*
  ?  * Function Name: led_port_config
  ?  * Input:  void   
  ?  * Output:void     
  ?  * Logic: To configure led   
  ?  * Example Call:led_port_config()
 */
void led_port_config(void)
{	
	DDRA = DDRA | 0xF0;
	PORTA = PORTA & 0xF0;
}
/*
  ?  * Function Name: led_color_set
  ?  * Input:  color to which the led is to be set  
  ?  * Output:void     
  ?  * Logic: To set the color of led 
  ?  * Example Call:led_color_set(blue)
 */
void led_color_set(unsigned char color)
{
	unsigned char PortARestore = 0;

	color &= 0xF0; 		// removing upper ininibbel for the protection
	PortARestore = PORTA; 		// reading the PORTA original status
	PortARestore &= 0x0F; 		// making lower direction nibbel to 0
	PortARestore |= color; // adding lower nibbel for forward command and restoring the PORTA status
	PORTA = PortARestore; 		// executing the command
}
/*
  ?  * Function Name: 
  ?  * Input: unsigned char adc_reading is the analog value
  ?  * Output:void     
  ?  * Logic: To calculate distance from the analog reading 
  ?  * Example Call:Sharp_GP2D12_estimation(230)
 */
unsigned int Sharp_GP2D12_estimation(unsigned char adc_reading)
{
	float distance;
	unsigned int distanceInt;
	distance = (int)(10.00*(2799.6*(1.00/(pow(adc_reading,1.1546)))));
	distanceInt = (int)distance;
	if(distanceInt>800)
	{
		distanceInt=800;
	}
	return distanceInt;
}

/*
    * Function Name: timer5_init
    * Input:void
    * Output:void     
    * Logic:
   Timer 5 initialized in PWM mode for velocity control
   Prescale:256
   PWM 8bit fast, TOP=0x00FF
   Timer Frequency:225.000Hz
  * Example Call:timer5_init()
 */

void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/
	
	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}


/*
    * Function Name: timer4_init
    * Input:void
    * Output:void     
    * Logic:TIMER4 initialization in 10 bit fast PWM mode
    prescale:256
    WGM: 7) PWM 10bit fast, TOP=0x03FF
    actual value: 52.25Hz
  * Example Call:timer4_init()
 */

void timer4_init(void)
{
	TCCR4B = 0x00; //stop
	TCNT4H = 0xFC; //Counter high value to which OCR1xH value is to be compared with
	TCNT4L = 0x01;	//Counter low value to which OCR1xH value is to be compared with
	OCR4AH = 0x03;	//Output compare Register high value for servo 1
	OCR4AL = 0xFF;	//Output Compare Register low Value For servo 1
	OCR4BH = 0x03;	//Output compare Register high value for servo 2
	OCR4BL = 0xFF;	//Output Compare Register low Value For servo 2
	ICR4H  = 0x03;
	ICR4L  = 0xFF;
	TCCR4A = 0xA3;//0xAB /*{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
	//  					For Overriding normal port functionality to OCRnA outputs.
	// 				  {WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
	TCCR4C = 0x00;
	TCCR4B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
	}

/*
  ?  * Function Name: 
  ?  * Input: void
  ?  * Output:void     
  ?  * Logic: To initialize all the adc configuration
  ?  * Example Call:adc_init()
 */
void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

/*
  ?  * Function Name: port_init
  ?  * Input: void
  ?  * Output:void     
  ?  * Logic: To initialize all the ports 
  ?  * Example Call:port_init()
 */
void port_init()
{
	lcd_port_config();
	adc_pin_config();
	motion_pin_config();
	led_port_config();	
	servo1_pin_config();
	servo2_pin_config();
	timer5_init();
	timer4_init();
	adc_init();
}


/*
  ?  * Function Name:ADC_Conversion() 
  ?  * Input: unsigned char channel number required for the conversion
  ?  * Output:void     
  ?  * Logic: Function For ADC Conversion 
  ?  * Example Call:port_init()
 */

unsigned char ADC_Conversion(unsigned char Ch) 
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;  			
	ADMUX= 0x20| Ch;	   		
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}

/*
    * Function Name:print_sensor 
    * Input: char row,columns and channels to print on the led
    * Output:void     
    * Logic: Function To Print Sensor Values At Desired Row And Column Location on LCD
    * Example Call:print_sensor(2,10,5)
 */
void print_sensor(char row, char column,unsigned char channel)
{
	
	ADC_Value = ADC_Conversion(channel);
	lcd_print(row, column, ADC_Value, 3);
}

/*
  ?  * Function Name:velocity 
  ?  * Input: unsigned char left_motor and unsigned char right_motor to provide the velocity
  ?  * Output:void     
  ?  * Logic:To set the values of the motor
  ?  * Example Call:velocity(120,150)
 */
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}
/*
  ?  * Function Name:print_sensor 
  ?  * Input: unsigned char Direction to set motion
  ?  * Output:void     
  ?  * Logic: Function used for setting motor's direction
  ?  * Example Call:print_sensor(2,10,5)
 */

void motion_set (unsigned char Direction)
{
 unsigned char PortARestore = 0;

 Direction &= 0x0F; 		// removing upper nibbel for the protection
 PortARestore = PORTA; 		// reading the PORTA original status
 PortARestore &= 0xF0; 		// making lower direction nibbel to 0
 PortARestore |= Direction; // adding lower nibbel for forward command and restoring the PORTA status
 PORTA = PortARestore; 		// executing the command
}
/*
  ?  * Function Name:forward
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: Function used for moving in forward direction
  ?  * Example Call:forward()
 */
void forward (void) 
{
  motion_set (0x06);
}
/*
  ?  * Function Name:back
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: Function used for moving in backward direction
  ?  * Example Call:back()
 */
void back (void) //both wheels backward
{
	motion_set(0x09);
}

void left (void) //Left wheel backward, Right wheel forward
{
	motion_set(0x05);
}
/*
  ?  * Function Name:right
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: Left wheel forward, Right wheel backward
  ?  * Example Call:right()
 */
void right (void) 
{
	motion_set(0x0A);
}
/*
  ?  * Function Name:soft_left
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: Left wheel stationary, Right wheel forward
  ?  * Example Call:soft_left()
 */
void soft_left (void) 
{
	motion_set(0x04);
}
/*
  ?  * Function Name:soft_right
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: Left wheel forward, Right wheel is stationary
  ?  * Example Call:soft_right()
 */
void soft_right (void) 
{
	motion_set(0x02);
}
/*
  ?  * Function Name:stop
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: Left wheel and Right wheel is stationary
  ?  * Example Call:stop
 */
void stop (void)
{
  motion_set (0x00);
}
/*
  ?  * Function Name:push_front
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: to push node in the front of the path array
  ?  * Example Call:push_front()
 */
void push_front(int num){
	if (front == 0){
		//printf("\n OVERFLOW");
	}
	else if (front == -1 && rear == -1)
	{
		front = rear = MAX-1;
	}
	else{
		front--;
	}
	path[front] = num;
}

/*
  ?  * Function Name:pop_element_front
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: to pop the element from the front of the path
  ?  * Example Call:pop_element_front()
 */
int pop_element_front()
{
	int pop;
	if(front == -1 || front>rear)
	{
		//printf("\n UNDERFLOW");

	}
	else
	{
		pop = path[front];
		front++;
		if(front > rear)
		front = rear = -1;
	}
	return pop;
}
/*
  ?  * Function Name:path_clear
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: To clear the path after complete traversal along that path
  ?  * Example Call:
 */


void path_clear()
{
	front =-1;
	rear=-1;
}

/*
*
* Function Name: line_follow
* Input: void
* Output: returns an integer value -1 when a node is encountered
* Logic: the basic logic of the line following algo used here is to turn left as soon as the left sensor starts reading the black line  and turns right if the
* right sensor reads the black line and stop if all the sensors read black
* Example Call: line_follow()
*
*/
int line_follow(int curve){
	int track=0;
	forward();
	velocity(150,150);
	while(1){
		_delay_ms(1);
		Left_white_line = ADC_Conversion(1);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(3);	//Getting data of Right WL Sensor
		
		lcd_print(1, 1, Left_white_line, 3);
		lcd_print(1, 5, Center_white_line, 3);
		lcd_print(1, 9, Right_white_line, 3);
		
		
		if((ADC_Conversion(2)>135 ) || ( ADC_Conversion(1) > 135 ) || (ADC_Conversion(3) > 135) ) //Node  detection  // Threshold 51 set karke dekhte hai

		{
			//Prints Value of White Line Sensor
			lcd_print(1, 1, Left_white_line, 3);
			lcd_print(1, 5, Center_white_line, 3);
			lcd_print(1, 9, Right_white_line, 3);
			forward();
			velocity(130,130);
			_delay_ms(150);
			stop(); //Node detection
			return -1;
		}
		//soft left
		if(ADC_Conversion(1) > 100 && ADC_Conversion(1) <= 135 )
		{
			soft_left();
			velocity(140,140) ;//case1 soft right // 170,110 180,120
			track = 1;
		}
		//forward
		if(ADC_Conversion(2) > 100 && ADC_Conversion(2) <= 135 )
		{
			forward();
			velocity(150,150) ;//case1 soft right // 170,110 180,120
			track = 2;
		}
		//soft right
		if(ADC_Conversion(3) > 100 && ADC_Conversion(3) <= 135 )
		{
			soft_right();
			velocity(140,140) ;//case1 soft right // 170,110 180,120
			track = 3;
		}
		//drift left
		if(ADC_Conversion(1) > 30 && ADC_Conversion(1) <= 100 )
		{
			forward();
			velocity(120,160) ;//case1 soft right // 170,110 180,120
			track = 4;
		}
		//drift right
		if(ADC_Conversion(3) > 30 && ADC_Conversion(3) <= 100 )
		{
			forward();
			velocity(160,120) ;//case1 soft right // 170,110 180,120
			track = 5;
		}
	}
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
	_delay_ms(3);
	right();
	velocity(150,150);
	_delay_ms(300);
	while(ADC_Conversion(2) <= 30);
	//right();
	//_delay_ms(500);
	stop();
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
void forward_wls(unsigned char curve)
{
	int val,node=1;
	while(node != 0){
		val = line_follow(curve);
		if(val == -1)
		{
			node = node + val;
			
		}
		else
		{
			obstacle_flag = 1;
			right_turn_wls();
			forward_wls(curve);
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
	_delay_ms(3);
	left();
	velocity(150,150);
	_delay_ms(300);
	while(ADC_Conversion(2) <= 30);
	//left();
	//_delay_ms(300);
	stop();
}


/*
  ?  * Function Name:servo_1
  ?  * Input:unsigned char degree is input to rotate the servo
  ?  * Output:void     
  ?  * Logic: Function to rotate Servo 1 by a specified angle in the multiples of 1.86 degrees
  ?  * Example Call: servo_1(25)
 */
void servo_1(unsigned char degrees)
{
	float PositionPanServo = 0;
	PositionPanServo = ((float)degrees / 1.86) + 35.0;
	OCR4AH = 0x00;
	OCR4AL = (unsigned char) PositionPanServo;
}


/*
  ?  * Function Name:servo_2
  ?  * Input:unsigned char degree is input to rotate the servo
  ?  * Output:void     
  ?  * Logic: Function to rotate Servo 2 by a specified angle in the multiples of 1.86 degrees
  ?  * Example Call:servo_2(20)
 */
void servo_2(unsigned char degrees)
{
	float PositionTiltServo = 0;
	PositionTiltServo = ((float)degrees / 1.86)+ 35.0;
	OCR4BH = 0x00;
	OCR4BL = (unsigned char) PositionTiltServo;
}
/*
  ?  * Function Name:servo_1_free
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: To free servo 1
  ?  * Example Call:servo_1_free()
 */
void servo_1_free(void){
	OCR4AH = 0x03;	//Output compare Register high value for servo 1
	OCR4AL = 0xFF;// Servo 1 stops
}
/*
  ?  * Function Name:servo_2_free
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: to free  servo 2
  ?  * Example Call:servo_2_free()
 */
void servo_2_free(void){
	OCR4BH = 0x03;
	OCR4BL = 0xFF; //Servo 2 off
}
/*
  ?  * Function Name:orient_claws
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: To orient the claws 
  ?  * Example Call:orient_claws()
 */
void orient_claws(void){
	servo_1(servo1_uplim);
	_delay_ms(1000);
	servo_1_free();
	servo_2(servo2_openlim);
	_delay_ms(1000);
	servo_2_free();
}
/*
  ?  * Function Name:pick
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: To pick the nut the servo uses this 
  ?  * Example Call:pick()
 */
void pick(void){
	servo_1(servo1_downlim);
	_delay_ms(1500);
	servo_2(servo2_closelim);
	_delay_ms(1500);
	servo_2_free();
	servo_1(servo1_uplim);
	_delay_ms(1500);
}
/*
  ?  * Function Name:place
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: To place the nut the servo uses this 
  ?  * Example Call:place()
 */
void place(void){
	servo_1(servo1_downlim);
	_delay_ms(1500);
	servo_1_free();
	servo_2(servo2_openlim);
	_delay_ms(1000);
	servo_2_free();
	servo_1(servo1_uplim);
	_delay_ms(1500);
}
/*
    * Function Name:init_device
    * Input:void
    * Output:void     
    * Logic: To initialize all the timers and devices being used
    * Example Call:init_device()
 */
void init_devices (void)
{
	cli(); //Clears the global interrupts
	port_init();
	adc_init();
	timer5_init();
	timer4_init();
	color_sensor_pin_interrupt_init();
	color_sensor_pin_config(); //initialize color sensor pin configuration
	rgb_port_config();//rgb port config
	lcd_port_config();//lcd port config
	sei();   //Enables the global interrupts
}
/*
  ?  * Function Name:dijkstra
  ?  * Input:int G[][] for the graph , int n is for number of nodes, int start the start node and int end is the end node
  ?  * Output:void     
  ?  * Logic:To create the path for traversal from a start node to end node
  ?  * Example Call:dijkstra(G,28,10,25)
 */
void dijkstra(int G[MAX][MAX],int n,int start, int end)
{

	int cost[MAX][MAX],distance[MAX],pred[MAX];
	int visited[MAX],count,mindistance,nextnode,i,j;
	obstacle_flag=0;
	//pred[] stores the predecessor of each node
	//count gives the number of nodes seen so far
	//create the cost matrix
	for(i=0;i<n;i++)
	for(j=0;j<n;j++)
	if(G[i][j]==0)
	cost[i][j]=INFINITY;
	else
	cost[i][j]=G[i][j];

	//initialize pred[],distance[] and visited[]
	for(i=0;i<n;i++)
	{
		distance[i]=cost[start][i];
		pred[i]=start;
		visited[i]=0;
	}

	distance[start]=0;
	visited[start]=1;
	count=1;

	while(count<n-1)
	{
		mindistance=INFINITY;

		//nextnode gives the node at minimum distance
		for(i=0;i<n;i++)
		if(distance[i]<mindistance&&!visited[i])
		{
			mindistance=distance[i];
			nextnode=i;
		}

		//check if a better path exists through nextnode
		visited[nextnode] = 1;
		for(i=0;i<n;i++)
		if(!visited[i])
		if(mindistance+cost[nextnode][i]<distance[i])
		{
			distance[i]=mindistance+cost[nextnode][i];
			pred[i]=nextnode;
		}
		count++;
	}

	//print the path and distance of each node

	//printf("\nDistance of node%d=%d",end,distance[end]);
	//printf("\nPath=%d",endnode);

	j=end;
	push_front(j);
	do
	{
		j=pred[j];
		push_front(j);
		//printf("<-%d",j);
	}while(j!=start);

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
int bot_traversal(int from, int to, int par_node)
{

	if ((from == 23 && to == 14) || (from == 14 && to == 23)) {//conditions to fo from node 23 to 14 or coming from 10,4 or 17
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
		if (par_node == 23 || par_node == 6)
			left_turn_wls();
		else if (par_node == 10)
			right_turn_wls();
		forward_wls(2);
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
	/*
	else if ((from == 3 && to == 18) || (from == 18 && to == 3)) { //for traversals between node 3 and 18 from 2,4,17,19
		if (par_node == 19)
			left_turn_wls();
		else if (par_node == 2)
			right_turn_wls();
		forward_wls(1);
	}*/
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
		else if (par_node == 23|| par_node == 15)
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
	//TO FRO 3 & 22 Coming From 2,18,4,24
	else if ((from == 3 && to == 22) || (from == 22 && to == 3)) {
	if (par_node == 2)
		right_turn_wls();
	else if (par_node == 24)
		left_turn_wls();
	forward_wls(1);
	}

	//TO FRO 18 & 22 Coming From 3,17,19,24
	else if ((from == 18 && to == 22) || (from == 22 && to == 18)) {
	if (par_node == 24)
		right_turn_wls();
	else if (par_node == 19)
		left_turn_wls();
	forward_wls(1);
	}

	//LIFT SECTION
	else if (from == 22 && to == 24) {
	if (par_node == 3)
		right_turn_wls();
	else if (par_node == 18)
		left_turn_wls();
	forward_wls(1);
	stop();
	_delay_ms(8000);
	//Add a certain amount of stop delay for the lift to move
	}

	else if (from == 24 && to == 22) {
	//Add a certain amount of stop delay for the lift to move
	forward_wls(1);
	}

	//TO FRO 24 & 27 Coming From 22,25,26
	else if ((from == 27 && to == 24) || (from == 24 && to == 27)) {
	if (par_node == 26)
		{right_turn_wls();}
	else if (par_node == 25)
		{left_turn_wls();}
	forward_wls(1);
	}

	//TO FRO 26 & 27 Coming From 25,24
	else if ((from == 27 && to == 26) || (from == 26 && to == 27)) {
	if (par_node == 24)
		{left_turn_wls();}
	forward_wls(1);
	}

	//TO FRO 25 & 27 Coming From 26,24
	else if ((from == 27 && to == 25) || (from == 25 && to == 27)) {
	if (par_node == 24)
		{right_turn_wls();}
	forward_wls(1);
	}

	return from;
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
	if ((from == 8 && to == 7) || (from == 9 && to == 8) || (from == 10 && to == 9) || (from == 12 && to == 11) || (from == 13 && to == 12) || (from == 18 && to == 19) || (from == 1 && to == 2) || (from == 25 && to ==27))
	right_turn_wls();
	else if ((from == 7 && to == 8) || (from == 8 && to == 9) || (from == 10 && to == 11) || (from == 11 && to == 12) || (from == 12 && to == 13) || (from == 3 && to == 2) || (from == 20 && to == 19) || (from == 26 && to == 27))
	left_turn_wls();
	_delay_ms(15);
}
/*
  ?  * Function Name:node_traverse
  ?  * Input:void
  ?  * Output:void     
  ?  * Logic: To traverse the nodes 
  ?  * Example Call:node_traverse()
 */
void node_traverse(void) {
	int n = 0;
	while (n != 2) {
		n = rear-front+1;
		currentNode = pop_element_front();
		
		Going_toNode = path[front];
		//printf("Current node: %d Going to: %d \n", currentNode, Going_toNode);

		par_node = bot_traversal(currentNode, Going_toNode, par_node);

		if (obstacle_flag == 1) {
			par_node = Going_toNode;
			arena[currentNode][Going_toNode] = 0;
			arena[Going_toNode][currentNode] = 0;
			path_clear();
			break;
		}
	}
	if (n == 2)
		correct_turn(currentNode, Going_toNode);
	if (obstacle_flag == 0)
		currentNode = path[front];
	path_clear();
	//printf("parent :%d current :%d\n", par_node, currentNode);
}

/*
*
* Function Name: travel
* Input: int startnode and endnode 
* Output: void
* Logic: to initialize all the nodes and create path and guide the bot to traverse 
* Example Call: travel(0,7);
*
*/
void travel(int startnode, int endnode)
{
	dijkstra(arena,28,startnode,endnode);
	node_traverse();
 	if (obstacle_flag == 1) {
 		travel(currentNode, endnode);
 	}
	 par_node = endnode;
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
	int choice=0;
	red_read();
	red = PULSE;//store the count in variable called red
	green_read();
	green = PULSE;//store the count in variable called green

	blue_read();
	blue = PULSE;//store the count in variable called blue
	choice = (red > green)?((red > blue)?1:3):((green > blue)?2:3);
	if(choice == 1)
	{
		led_color_set(0x10);
		_delay_ms(1000);
		led_color_set(0x00);
	}
	else if (choice == 2)
	{
		led_color_set(0x40);
		_delay_ms(1000);
		led_color_set(0x00);
	}
	else if(choice == 3)
	{
		led_color_set(0x80);
	}
	return choice;

}

/*
* Function Name: pick_nut
* Input: void
* Output: void 
* Logic: Used to guide the bot to place the nut according to its color in the corresponding deposit zone
* Example Call: pick_nut();
*
*/

void pick_nut(void) {
	if (nut_detector() == 1) {
		D_Node = S1_zone[RD_index];
		RD_index++;
		//pick();
		picked_flag = 1;
		_delay_ms(10);
	}
	else if (nut_detector() == 2) {
		D_Node = S2_zone[GD_index];
		GD_index++;
		//pick();
		picked_flag = 1;
		_delay_ms(10);
	}
	else if (nut_detector() == 3) {
		D_Node = S3_zone[BD_index];
		BD_index++;
		//pick();
		picked_flag = 1;
		_delay_ms(10);
	}
	else {
		picked_flag = 0;
	}

}


