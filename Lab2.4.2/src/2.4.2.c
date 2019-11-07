/* Lab2.4.2.c
 * Author: <Jiacan Xu Penn ID 76254982>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h"
#define PRINTNUM(x) m_usb_tx_uint(x); m_usb_tx_char(10); m_usb_tx_char(13)

int main(void){ // the main function

    unsigned long oldtime;
    unsigned long tperiod;
    unsigned long a;
    int boo;
    int sync_first;
    
    teensy_clockdivide(0); //set the clock speed to 16MHz
    
    m_usb_init(); //initialize USB connection
    while (!m_usb_isconnected()); //wait for USB connection to be estabilished
    set(TCCR3B,CS30); set(TCCR3B,CS32); //precaler 1/1024
    
    boo = 0;
    sync_first = 0;
    
    while(1){
        while(!bit_is_set(TIFR3,ICF3)); //check input capture flag
        set(TIFR3,ICF3); //clear flag by writing 1 to flag
        tperiod = ICR3-oldtime; //time between hits
        oldtime = ICR3;
        if(tperiod<100 && tperiod > 2 && sync_first == 1){
            boo = boo + 1;
            a=tperiod;
            if (boo%4 == 1){
                m_usb_tx_string("x pulse");
                m_usb_tx_char(10); m_usb_tx_char(13);
                PRINTNUM(a);
            }
            else if(boo%4 == 3){
                m_usb_tx_string("y pulse");
                m_usb_tx_char(10); m_usb_tx_char(13);
                PRINTNUM(a);
            }
        }
        else if(tperiod>120){ //print "sync" for sync signal, easier to read for x and y
            sync_first = 1;
            boo = 0;
            m_usb_tx_string("sync");
            m_usb_tx_char(10); m_usb_tx_char(13);
        }
        else{
            //
        }
        
    }
    return 0;   /* never reached */
}
