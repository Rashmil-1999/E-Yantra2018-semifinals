/*
 * Color_Sensor.c
 *
 * Created: 01-12-2018 14:45:11
 *  Author: e-Yantra Team
 
This experiment demonstrates the use of Color Sensor.

Micro-controller: atmega2560
Optimization: -O0  (For more information read section: Selecting proper optimization
options below figure 2.22 in the Software Manual)
*/

/*
-- Team Id:  NS#5405
-- Author List: Rashmil, Palash, Parag, Preet
-- Filename: Color Sensor Demo.c
-- Theme: Nutty Squirrel
-- Functions: void red_on(void), void blue_on(void), void green_on(void), void rgb_port_config(void), void all_off(void), void lcd_port_config (void), void lcd_reset(void)
			  void blue_read(void), void green_read(void), void red_read(void), void init_devices(void), void color_sensor_pin_interrupt_init(void), void red_filter(void),
			  void blue_filter(void), void green_filter(void), void color_sensor_scaling(void), ISR, void filter_clear(void), void main(void)
-- Variable:  PULSE, blue, red, green, rthreshold, gthreshold, bthreshold
			  
			  
*/

#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h> //included to support power function
#include "lcd.c"

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

//utility functions of rgb led on off

/*
  ?  * Function Name: void red_on (void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     set red led to ground(low) for common anode led else set ~0x60 for common cathode
  ?  * Example Call:  red_on();   
  */ 

void red_on(void){
	PORTA =  0x60;//set red led to ground(low) for common anode led else set ~0x60 for common cathode
}

/*
  ?  * Function Name: void green_on (void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     set green led to ground(low) for common anode led else set ~0xA0 for common cathode
  ?  * Example Call:  green_on();   
  */ 
void green_on(void){
	PORTA = 0xA0;//set green led to ground(low) for common anode led else set ~0xA0 for common cathode
}

/*
  ?  * Function Name: void blue_on (void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     set blue led to ground(low) for common anode led else set ~0xC0 for common cathode
  ?  * Example Call:  blue_on();   
  */ 
void blue_on(void){
	PORTA = 0xC0;//set blue led to ground(low) for common anode led else set ~0xC0 for common cathode
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
  ?  * Function Name: lcd_port_config (void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     * all the LCD pin's direction set as output
				  * all the LCD pins are set to logic 0 except PORTC 7
  ?  * Example Call:  lcd_port_config ();  
  */ 
void lcd_port_config (void)
{
	DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
	PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
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
  ?  * Function Name: init_devices(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic:     Initialize all port configurations and for clearing and enabling global interrupt
  ?  * Example Call:  init_devices; 
  */ 
void init_devices(void)
{
	cli(); //Clears the global interrupt
	//Initialize all the ports here
	color_sensor_pin_interrupt_init();
	color_sensor_pin_config(); //initialize color sensor pin configuration
	rgb_port_config();//rgb port config
	lcd_port_config();//lcd port config
	sei();   // Enables the global interrupt
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
  ?  * Function Name: main(void)
  ?  * Input:     void
  ?  * Output:    void 
  ?  * Logic: Systematic Execution of all functions to obtain desired output     
  ?  * Example Call:  ---- 
 */ 
void main(void)
{
	int choice;
    init_devices();
	color_sensor_scaling();
	lcd_set_4bit();
	lcd_init();
	
	
	while(1)
    {
      red_read();
	  red = PULSE;//store the count in variable called red
	  lcd_cursor(1,1);
	  lcd_print(1,1,1,1);
	  lcd_print(2,1,red,5);
	  lcd_reset();
	  green_read();
	  green = PULSE;//store the count in variable called green
	  lcd_print(1,1,2,1);
	  lcd_print(2,1,green,5);
	  lcd_reset();
	  blue_read();
	  blue = PULSE;//store the count in variable called blue
	  lcd_print(1,1,3,1);
	  lcd_print(2,1,blue,5);
	  lcd_reset();
	 if ((red < rthreshold) && (green < gthreshold) && (blue < bthreshold))
	  {
		  all_off();
		  lcd_cursor(1,1);
		  lcd_string("No Nut");
		  lcd_reset();
	  }
	  else
	  {
			  choice = (red > green)?((red > blue)?1:3):((green > blue)?2:3);
			  switch(choice)
			  {
				  case 1:red_on();
						lcd_cursor(1,1);
						lcd_string("RED Nut");
						lcd_reset();
						break;
				  case 2:green_on();
						lcd_cursor(1,1);
						lcd_string("GREEN Nut");
						lcd_reset();
						break;
				  case 3:blue_on();
						lcd_cursor(1,1);
						lcd_string("blue Nut");
						lcd_reset();
						break;
			  }
	  }	
    }
}