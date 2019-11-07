/* Lab3.1.1.c
 * Author: <Jiacan Xu Penn ID 76254982>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include "teensy_general.h"  // includes the resources included in the teensy_general.h file
#include "t_usb.h" // includes the resources included in the t_usb.h file

void adc_setup(int ADCchannel)
    {
        set(ADMUX, REFS0); // set 2.56V as reference voltage
        set(ADMUX, REFS1);
        
        set(ADCSRA, ADPS2); // set 128 prescaler
        set(ADCSRA, ADPS1);
        set(ADCSRA, ADPS0);
        
        if(ADCchannel == 1)
        {
            set(DIDR0, ADC1D);  // disable digital input

            set(ADMUX, MUX0);   // pin f1
            clear(ADMUX, MUX1);
            clear(ADMUX, MUX2);
            clear(ADCSRB, MUX5);
            
        }
        if(ADCchannel == 4)
        {
            set(DIDR0, ADC4D);  // disable digital input
    
            clear(ADMUX, MUX0); // pin f4
            clear(ADMUX, MUX1);
            set(ADMUX, MUX2);
            clear(ADCSRB, MUX5);
            
        }
    }

int adc_read()
{
    int adc_value;

    set(ADCSRA, ADEN);

    for(;;){
        
        set(ADCSRA, ADSC);
        
        //read the ADC
        if (bit_is_set(ADCSRA, ADIF)) { 
            // if flag is set (conversion complete) update  
            set(ADCSRA, ADIF); // reset the flag, see page 316 

            adc_value = ADC;
            return adc_value;
            break;                      
                                                       
        }
        
        
    }
}

int main(void)
{    
    m_usb_init(); //To initialize USB communication

    while(1)
    {
    adc_setup(1);

    int adc1 = adc_read();

    m_usb_tx_string("\rADC1 = ");  // \r is for a carriage return, ascii 13   
    m_usb_tx_uint(adc1);
    m_usb_tx_string("    "); // Clears any overflow chars for \r.

    teensy_wait(5);

    adc_setup(4);

    int adc2 = adc_read();

    m_usb_tx_string("ADC2 = ");  // \r is for a carriage return, ascii 13   
    m_usb_tx_uint(adc2);
    m_usb_tx_string("\n"); // Clears any overflow chars for \r.

    teensy_wait(10);
   }
    
    return 0;   
    /* never reached */
    
}
    
