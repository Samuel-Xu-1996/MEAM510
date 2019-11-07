/* Lab2.3.2.c
 * Author: <Jiacan Xu Penn ID 76254982>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file

unsigned int oldtime = 0;
unsigned int tperiod = 0;

#define T_200 40000

void wait_for_rising()
{
    while(!bit_is_set(TIFR3, ICF3));
    set(TIFR3, ICF3);
    
    tperiod = ICR3 - oldtime;
    oldtime = ICR3;
}

int main()
{
    teensy_clockdivide(1);
    set(TCCR3B, ICES3);
    
    set(DDRD, 6);
    clear(PORTD, 6);
    
    set(DDRD, 7);
    clear(PORTD, 7);
    
    clear(TCCR3B, CS32);
    clear(TCCR3B, CS31);
    set(TCCR3B, CS30);
    
    while(1)
    {
        wait_for_rising();
        
        if(tperiod >= T_200)
        {
            clear(PORTD, 7);
            set(PORTD, 6);

        }
        
        else if(tperiod < T_200)
        {
            clear(PORTD, 6);
            set(PORTD, 7);

        }
    }
}