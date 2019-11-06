/* Name: Lab2.2.1.c
 * Author: <Jiacan Xu Penn ID 76254982>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file

int main(void)
{
    teensy_clockdivide(0); //set the clock speed as 16MHz
    
    clear(DDRB, 6);  

    
    set(TCCR3B, CS32); // prescaler is 1024 for timer 3
    clear(TCCR3B, CS31); // prescaler is 1024 for timer 3
    set(TCCR3B, CS30); // prescaler is 1024 for timer 3
    
    set(DDRC,6); // set C6 as output

    set(TCCR3B,WGM33); // choose mode 14
    clear(TCCR3A, WGM30); // choose mode 14
    set(TCCR3A, WGM31); // choose mode 14
    set(TCCR3B, WGM32); // choose mode 14

    clear(TCCR3A, COM3A0); // clear at OCR3A, set at rollover
    set(TCCR3A, COM3A1); // clear at OCR3A, set at rollover

    float f = 50; // set the frequency of LED to be 50Hz
    float dc1 = 0.5; // set the duty cycle of LED to be 50%
    float dc2 = 0.0; // set the duty cycle of LED to be 0%


    while(1){
        if (bit_is_clear(PINB, 6)){
		ICR3 = 16000000/(1024*f);
    		OCR3A = ICR3 * dc2;
    	}

        if (bit_is_set(PINB, 6)){
		ICR3 = 16000000/(1024*f);
    		OCR3A = ICR3 * dc1;
        }
    }

    return 0;   /* never reached */
}
