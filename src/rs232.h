/* 
 * File:   rs232.h
 * Author: _prossn
 *
 * Created on 7. dicembre 2017, 15:07
 */

#ifndef RS232_H
#define	RS232_H

// defines which eusart device is used with prinf()
#define EUSART_1_PRINTF 1

extern void eusart1_init(void);
extern void eusart2_init(void);

extern void eusart1_putch(char c);
extern void eusart2_putch(char c);

extern char eusart1_getch(void);
extern char eusart2_getch(void);

extern char eusart1_getche(void);
extern char eusart2_getche(void);

#endif	/* RS232_H */

