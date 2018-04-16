/*
 * File:    main.c
 * Author:  Naoki Pross
 * Project: SAM Spectrum Analyzer
 * 
 * Created on 12.04.2018, 11:57
 */

#define DEBUG

#include "hwconfig.h"
#include "rs232.h"
#include "ht1632.h"

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// number of samples to collect
#define SAMPLES_SIZE 64



volatile uint8_t sample_count = 0;
volatile uint16_t samples[SAMPLES_SIZE] = {0};


interrupt void isr(void)
{
#ifdef DEBUG
    PORTDbits.RD1 = 0;
#endif
    
    if (PIR1bits.TMR2IF) {    
        // reset interrupt flag
        PIR1bits.TMR2IF = 0;
    }
    
#ifdef DEBUG
    PORTDbits.RD1 = 1;
#endif
}


/* hardware initialization routine */
void init_hw()
{
    // disable interrupts
    di();
            
    /* PLL / FOSC configuration */
    // enable PLL
    OSCTUNEbits.PLLEN = 1;
    // set FOSC to HFINTOSC (max frequency)
    OSCTUNEbits.TUN = 0b011111;
    // set 16 MHz oscillator, datasheet p.30
    OSCCONbits.IRCF = 0b111;
    // select primary clock (with PLL)
    OSCCONbits.SCS = 0b00;
    
    /* initialize digital i/o */
    // signal input
    TRISAbits.RA0 = 1;
    ANSELAbits.ANSA0 = 1;
    
    // input selection button
    TRISBbits.TRISB0 = 1;
    ANSELBbits.ANSB0 = 0;
    // eusart and mssp port
    ANSELC = 0x00;
    TRISC = 0x00;
    // mux selection port
    TRISD = 0x00; // 0xF8;
    ANSELD = 0x00; // 0xF8;

    /* intialize ADC i/o */
    // set ADC clock period to Fosc/64 = 1us
    // (maximum without violating Tadmin)
    ADCON2bits.ADCS = 0b110;
    // set ADC acquisition time to 2*Tad)
    ADCON2bits.ACQT = 0b001;
    // set result format 2 bits on ADREDH and 8 bits on ADRESL
    ADCON2bits.ADFM = 1;
    // set internal reference
    ADCON1bits.PVCFG = 0b00;
    // set internal negative reference
    ADCON1bits.NVCFG = 0b00;
    // select channel 0 (AN0)
    ADCON0bits.CHS = 0b00000;
    // enable ADC
    ADCON0bits.ADON = 1;
    
    /* interrupts initialization */
    // timer 2 comp value
    PR2 = 128;
    // postscaler 1:2
    T2CONbits.T2OUTPS = 0b0001;
    // prescaler 1:2
    T2CONbits.T2CKPS = 0b01;
    // start timer
    T2CONbits.TMR2ON = 1;
    // timer 2 interrupts
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
    // enable peripheral interrupts
    INTCONbits.PEIE = 1;
    
    /* initialize serial devices */
    eusart1_init();
    eusart2_init();
    
    // enable interrupts
    ei(); 
}


void main(void)
{        
    init_hw();
    
    while (true) {
        
    }
    
    return;
}
