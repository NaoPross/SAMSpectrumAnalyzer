/* 
 * File:    hwconfig.h
 * Author:  Naoki Pross
 * Project: SAM Spectrum Analyzer
 *
 * Created on 16.04.2018
 */

#ifndef HWCONFIG_H
#define	HWCONFIG_H

// PIC18F44K22 Configuration Bit Settings
// 'C' source line config statements

// CONFIG1H
// Oscillator Selection bits (Internal oscillator block)
#pragma config FOSC = INTIO67    
// 4X PLL Enable (Oscillator multiplied by 4)
#pragma config PLLCFG = ON      
// Primary clock enable bit (Primary clock is always enabled)
#pragma config PRICLKEN = ON    
// Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config FCMEN = OFF      
// Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
#pragma config IESO = OFF       

// CONFIG2L
// Power-up Timer Enable bit (Power up timer disabled)
#pragma config PWRTEN = OFF     
// Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BOREN = SBORDIS  
// Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)
#pragma config BORV = 190       

// CONFIG2H
// Watchdog Timer Enable bits (WDT is always enabled. SWDTEN bit has no effect)
#pragma config WDTEN = OFF      
// Watchdog Timer Postscale Select bits (1:32768)
#pragma config WDTPS = 32768    

// CONFIG3H
// CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config CCP2MX = PORTC1  
// PORTB A/D Enable bit (PORTB<5:0> pins are configured as analog input channels on Reset)
#pragma config PBADEN = ON      
// P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config CCP3MX = PORTB5  
// HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config HFOFST = ON      
// Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config T3CMX = PORTC0   
// ECCP2 B output mux bit (P2B is on RD2)
#pragma config P2BMX = PORTD2   
// MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)
#pragma config MCLRE = EXTMCLR  

// CONFIG4L
// Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config STVREN = ON      
// Single-Supply ICSP Enable bit (Single-Supply ICSP enabled if MCLRE is also 1)
#pragma config LVP = ON         
// Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))
#pragma config XINST = OFF      

// CONFIG5L
// Code Protection Block 0 (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP0 = OFF        
// Code Protection Block 1 (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP1 = OFF        
// Code Protection Block 2 (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP2 = OFF        
// Code Protection Block 3 (Block 3 (006000-007FFFh) not code-protected)
#pragma config CP3 = OFF        

// CONFIG5H
// Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPB = OFF        
// Data EEPROM Code Protection bit (Data EEPROM not code-protected)
#pragma config CPD = OFF        

// CONFIG6L
// Write Protection Block 0 (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT0 = OFF       
// Write Protection Block 1 (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT1 = OFF       
// Write Protection Block 2 (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT2 = OFF       
// Write Protection Block 3 (Block 3 (006000-007FFFh) not write-protected)
#pragma config WRT3 = OFF       

// CONFIG6H
// Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTC = OFF       
// Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTB = OFF       
// Data EEPROM Write Protection bit (Data EEPROM not write-protected)
#pragma config WRTD = OFF       

// CONFIG7L
// Table Read Protection Block 0 (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR0 = OFF      
// Table Read Protection Block 1 (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      
// Table Read Protection Block 2 (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      
// Table Read Protection Block 3 (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      

// CONFIG7H
// Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)
#pragma config EBTRB = OFF      

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 64000000

#endif	/* HWCONFIG_H */

