/* Lab2.1.1.c
 * Author: <Jiacan Xu Penn ID 76254982>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h" // includes the resources included in the t_usb.h file

int main(void)
{
    teensy_clockdivide(0); //set the clock speed as 16MHz
    
    clear(DDRB, 6); // set B6 as input 
    
    set(DDRC, 6);
    
    m_usb_init(); // start USB connection
    while(!m_usb_isconnected()); // wait for USB connection
    
    while(1)
    {
        m_usb_tx_string("Switch Off\n");
        while(bit_is_set(PINB, 6)){
            clear(PORTC, 6);
        }
        m_usb_tx_string("Switch On\n");
        while(!bit_is_set(PINB, 6)){
            set(PORTC, 6);
        } 
    }
    
    return 0;   /* never reached */
}
