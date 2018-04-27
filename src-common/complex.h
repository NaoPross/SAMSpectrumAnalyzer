/* 
 * File:   complex.h
 * Author: Naoki Pross
 *
 * Created on 27.04.2018, 14:22
 */

#ifndef COMPLEX_H
#define	COMPLEX_H

#include <stdint.h>

typedef struct {
    int8_t real;
    int8_t imag;
} complex_int8_t;

typedef struct {
    uint8_t real;
    uint8_t imag;
} complex_uint8_t;

typedef struct {
    int16_t real;
    int16_t imag;
} complex_int16_t;

typedef struct {
    uint16_t real;
    uint16_t imag;
} complex_uint16_t;

// TODO: operations
// extern void complex_add()
// extern void complex_sub()
// extern void complex_mul()
// extern void complex_div()
// extern void complex_conj()

#endif	/* COMPLEX_H */

