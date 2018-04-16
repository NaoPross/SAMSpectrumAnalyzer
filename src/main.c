/*
 * File:    main.c
 * Author:  Naoki Pross
 * Project: SAM Spectrum Analyzer
 * 
 * Created on 12.04.2018, 11:57
 */



#include "hwconfig.h"
#include "rs232.h"
#include "ht1632.h"

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>



interrupt void isr(void)
{

}


/* hardware initialization routine */
void init_hw()
{
    /* PLL / FOSC configuration */
    // enable PLL
    OSCTUNEbits.PLLEN = 1;
    // set FOSC to HFINTOSC (max frequency)
    OSCTUNEbits.TUN = 0b011111;
    // set 16 MHz oscillator, datasheet p.30
    OSCCONbits.IRCF = 0b111;
    // select primary clock (with PLL)
    OSCCONbits.SCS = 0b00;
    
    /* intialize ADC i/o */
    // signal input
    ANSELAbits.ANSA0 = 1;
    // input selection button
    ANSELBbits.ANSB0 = 0;
    
    // other ports
    ANSELC = 0x00;
    ANSELD = 0x00;
    
    /* initialize digital i/o */

    // input selection button
    TRISBbits.TRISB0 = 1;
    // eusart and mssp
    TRISC = 0x00;
    // mux selection ouput
    TRISD = 0x00; // 0xF8;
    
    /* initialize serial devices */
    eusart1_init();
    eusart2_init();
    
}


void main(void)
{        
    init_hw();
    
    while (true) {
        printf("hello world!\n\r");
        // putch('A');
        
        PORTDbits.RD1 = 0;
        __delay_ms(200);
        PORTDbits.RD1 = 1;
        __delay_ms(200);
    }
    
    return;
}
