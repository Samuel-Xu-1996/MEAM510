/* Lab3.2.c
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

void PWM_init(int Output_Channel)
{
    if (Output_Channel == 6)
    {
        set(DDRC, 6); // set PIN C6
    
        clear(TCCR3B, CS31);clear(TCCR3B, CS30);set(TCCR3B, CS32); // set prescaler to /256
    
        set(TCCR3A, COM3A1);
    
        set(TCCR3B, WGM33); // set to mode 14
        set(TCCR3B, WGM32); // set to mode 14
        set(TCCR3A, WGM31); // set to mode 14
        clear(TCCR3A, WGM30); // set to mode 14
    
        ICR3 = 1250; // 16000000/(256 * 50) = 1250

    }
    else if(Output_Channel == 5)
    {
        set(DDRB, 5); // set PIN B5
    
        clear(TCCR1B, CS11);clear(TCCR1B, CS10);set(TCCR1B, CS12); // set prescaler to /256
    
        set(TCCR1A, COM1A1);
    
        set(TCCR1B, WGM13); // set to mode 14
        set(TCCR1B, WGM12); // set to mode 14
        set(TCCR1A, WGM11); // set to mode 14
        clear(TCCR1A, WGM10); // set to mode 14
    
        ICR1 = 1250; // 16000000/(256 * 50) = 1250

    }
}

void set_PWM(int ADC_value, int Output_Channel)
{
    if (Output_Channel == 6)
    {
        float revised_value = (float)ADC_value + 814;
        float duty_cycle = (((1024.0 - revised_value) / 1024.0) + 1.0) / 20.0;
        OCR3A = duty_cycle * 1250;
    
        m_usb_tx_string("OCR3A   ");
        m_usb_tx_int(OCR3A);
        m_usb_tx_string("\n");
    }
    else if (Output_Channel == 5)
    {
        float revised_value = (float)ADC_value + 814;
        float duty_cycle = (((1024.0 - revised_value) / 1024.0) + 1.0) / 20.0;
        OCR1A = duty_cycle * 1250;
    
        m_usb_tx_string("OCR1A   ");
        m_usb_tx_int(OCR1A);
        m_usb_tx_string("\n");
    }
}

void main()
{
    m_usb_init();
    
    while(1)
    {
        
        //Set up of first servo

        adc_setup(1);
        PWM_init(5); // configure pin B5 as output

        int adc1 = adc_read();

        m_usb_tx_string("ADC1 = ");   
        m_usb_tx_uint(adc1);
        m_usb_tx_string("\n");
        
        set_PWM(adc1, 5);
        //teensy_wait(2);
 
        //Set up second servo
        adc_setup(4);
        PWM_init(6);//configure pin C6 as output
        
        int adc2 = adc_read();

        m_usb_tx_string("ADC2 = ");   
        m_usb_tx_uint(adc2);
        m_usb_tx_string("\n");

        set_PWM(adc2, 6);
        //teensy_wait(5);
    
        m_usb_tx_string("\n");
    
    }
    
    return 0;
}
