/* 
 * File:   rs232.h
 * Author: Naoki Pross
 *
 * Created on 7.12.2017
 */

#ifndef RS232_H
#define	RS232_H

#include <stddef.h>

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

extern void eusart1_write(void *data, size_t len);
extern void eusart2_write(void *data, size_t len);


#endif	/* RS232_H */

