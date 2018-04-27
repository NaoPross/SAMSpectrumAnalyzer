/* 
 * File:         complex.h
 * Author:       Naoki Pross
 * Description:  Signal Data Transfer (SDT) protocol definition
 * Created on:   27.04.2018, 14:22
 */

#ifndef SDTPROTO_H
#define SDTPROTO_H

#include "complex.h"

const uint16_t SDT_HEADER = 0xF00D;

struct sdt_frame
{
    uint16_t header;
    uint16_t length;
    complex_uint16_t *samples;
};

#endif
