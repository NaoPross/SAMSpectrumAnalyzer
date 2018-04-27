#include "rs232.h"

#include <xc.h>

#ifdef EUSART_1_PRINTF
void putch(char c) {
#if EUSART_1_PRINTF
    eusart1_putch(c);
#else
    eusart2_putch(c);
#endif
}
#endif

void eusart1_init(void)
{
    // set Async and 8 bits frame
    TXSTA1bits.SYNC = 0;
    TXSTA1bits.TX9 = 0;
    
    // baud prescaler
    RCSTA1bits.SPEN = 1;
    SPBRG1 = 103;       // 9600 bps
    SPBRGH1 = 0;
    TXSTA1bits.BRGH = 0;
    BAUDCON1bits.BRG16 = 0;
    
    // set up TX / RX pins
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 1;
    // enable continuous reception
    RCSTA1bits.CREN = 1;
    TXSTA1bits.TXEN = 1;
}

void eusart2_init(void)
{
    // set Async and 8 bits frame
    TXSTA2bits.SYNC = 0;
    TXSTA2bits.TX9 = 0;
    
    // baud prescaler
    RCSTA2bits.SPEN = 1;
    SPBRG2 = 103;			// 9600 bps
    SPBRGH2 = 0;
    TXSTA2bits.BRGH = 0;
    BAUDCON2bits.BRG16 = 0;
    
    // set up TX / RX pins
    TRISDbits.TRISD7 = 1;
    TRISDbits.TRISD6 = 1;
    // enable continuous reception
    RCSTA2bits.CREN = 1;
    TXSTA2bits.TXEN = 1;
}

void eusart1_putch(char c)
{
    while (!TX1IF);
    TX1REG = c;
}

void eusart2_putch(char c)
{
    while (!TX2IF);
    TX2REG = c;
}

char eusart1_getch(void)
{
    while (!RC1IF);
    return RC1REG;
}

char eusart2_getch(void)
{
    while (!RC2IF);
    return RC2REG;
}

char eusart1_getche(void)
{
    char c = eusart1_getch();
    eusart1_putch(c); // echo
    
    return c;
}

char eusart2_getche(void)
{
    char c = eusart2_getch();
    eusart2_putch(c); // echo
    
    return c;
}

void eusart1_write(void *data, size_t len)
{
    char *dptr = (char *) data;
    
    while (len--) {
        eusart1_putch(*(dptr++));
    }
}