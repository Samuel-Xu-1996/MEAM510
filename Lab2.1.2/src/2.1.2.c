/* Lab2.1.2.c
 * Author: <Jiacan Xu Penn ID 76254982>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h" // includes the resources included in the t_usb.h file

long double timeTotal = 0;

void onePress()
{
    while(bit_is_set(PINC, 7)){
            clear(PORTC, 6);
        }
    while(!bit_is_set(PINC, 7)){
            set(PORTC, 6);
        } 
}

unsigned long tperiod = 0;
unsigned long oldtime = 0;
long double real_time;

void period()
{
    while(!bit_is_set(TIFR3, ICF3));
    set(TIFR3, ICF3); // clear flag by writing 1 to flag
    tperiod = ICR3 - oldtime;
    oldtime = ICR3;
    real_time = (tperiod*1024.0/8000000.0)*1000.0;
    m_usb_tx_uint(real_time);
    timeTotal += real_time;
}

int main(void)
{
    teensy_clockdivide(1); // set the clock speed to 8MHz
    
    clear(DDRC, 7); // set B6 as input
    
    set(DDRC, 6);
    clear(PORTC, 6);
    
    set(TIFR3, ICF3);
    set(TCCR3B, CS32);
    set(TCCR3B, CS30);
    
    m_usb_init(); // set USB connection
    while(!m_usb_isconnected()); // wait for USB connection
    
    while(1)
    {
        teensy_wait(1000);

        m_usb_tx_string("Please start!\n");

        onePress();
        oldtime = ICR3;
        m_usb_tx_string("\tFirst hit");
        m_usb_tx_string("\n");
        
        onePress();
        period();
        m_usb_tx_string("\tSecond hit");
        m_usb_tx_string("\n");
        
        onePress();
        period();
        m_usb_tx_string("\tThird hit");
        m_usb_tx_string("\n");
        
        onePress();
        period();
        m_usb_tx_string("\tFourth hit");
        m_usb_tx_string("\n");
        
        onePress();
        period();
        m_usb_tx_string("\tFifth hit");
        m_usb_tx_string("\n");
        
        m_usb_tx_uint(timeTotal/4);
        m_usb_tx_string("\tAverage time");
        m_usb_tx_string("\n");
        
        

        TCNT3 = 0;
        ICR3 = 0;
        timeTotal = 0;
    }
    return 0;   /* never reached */
}
