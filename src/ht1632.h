/* 
 * File:   ht1632.h
 * Autore: Naoki Pross
 *
 * Creato il 16 aprile 2018.
 * 
 * Adatatato dal lavoro di:
 * Randjelovic Patrik del 6 aprile 2017.
 * 
 * Originariamente creato da:
 * Gaurav Manek l'8 aprile 2011 per Arduino.
 * 
 * Copyright:
 * Released into the public domain.
 */




#ifndef HT1632_H
#define	HT1632_H

#include <xc.h>
#include <stdint.h>
#define PROGMEM
#define pgm_read_byte(x) (*(x))

//Definizione dei nomi delle entrate/uscite:
#define _pinCS1     LATCbits.LATC2  //Pin chip_1 select 
//#define _pinCS2                   //Pin chip_2 select 
//#define _pinCS3                   //Pin chip_3 select 
//#define _pinCS4                   //Pin chip_4 select 
#define _pinWR      LATCbits.LATC4  //Pin write
#define _pinCLK     LATCbits.LATC3  //Pin clock
#define _pinDATA    LATCbits.LATC5  //Pin data 

//Le definizioni input/output sono eseguite nel tool MCC

// Custom typedefs
typedef unsigned char uint8_t;
typedef unsigned char byte;
typedef char int8_t;

/*
 * USER OPTIONS
 * Change these options
 */

// Uncomment the line that matches the board you have, or edit the 
//  settings in the else block:

// SureElectronics 32X16 Bicolor LED Dot Matrix Unit Board
#define TYPE_3216_BICOLOR 1

#if defined TYPE_3216_BICOLOR
  #define COM_SIZE 16
  #define OUT_SIZE 32
  #define NUM_CHANNEL 2
  #define USE_NMOS 1
  // Number of chips in a single Bicolor board:
  #define NUM_ACTIVE_CHIPS 4
#endif

/*
 * END USER OPTIONS
 * Don't edit anything below unless you know what you are doing!
 */


 // Pixels in a single byte of the internal image representation:
#define PIXELS_PER_BYTE 8

// Address space size (number of 4-bit words in HT1632 memory)
// Exactly equal to the number of 4-bit address spaces available.
#define ADDR_SPACE_SIZE (COM_SIZE * OUT_SIZE / PIXELS_PER_BYTE)
#define GET_ADDR_FROM_X_Y(_x,_y) ((_x)*((COM_SIZE)/(PIXELS_PER_BYTE))+(_y)/(PIXELS_PER_BYTE))
#define GET_BIT_FROM_Y(_y) ( (0b1 << PIXELS_PER_BYTE-1) >> (y % PIXELS_PER_BYTE) )

// NO-OP Definition
//#define NOP(); __asm__("nop\n\t");
// #define NOP() asm("nop")
// The HT1632 requires at least 50 ns between the change in data and the rising
// edge of the WR signal. On a 16MHz processor, this provides 62.5ns per NOP. 

// Standard command list.
// This list is modified from original code by Bill Westfield

#define HT1632_ID_CMD 0b100	/* ID = 100 - Commands */
#define HT1632_ID_RD  0b110	/* ID = 110 - Read RAM */
#define HT1632_ID_WR  0b101	/* ID = 101 - Write RAM */
#define HT1632_ID_LEN 3         /* IDs are 3 bits */

#define HT1632_CMD_SYSDIS 0x00	/* CMD= 0000-0000-x Turn off oscil */
#define HT1632_CMD_SYSEN  0x01	/* CMD= 0000-0001-x Enable system oscil */
#define HT1632_CMD_LEDOFF 0x02	/* CMD= 0000-0010-x LED duty cycle gen off */
#define HT1632_CMD_LEDON  0x03	/* CMD= 0000-0011-x LEDs ON */
#define HT1632_CMD_BLOFF  0x08	/* CMD= 0000-1000-x Blink ON */
#define HT1632_CMD_BLON   0x09	/* CMD= 0000-1001-x Blink Off */
#define HT1632_CMD_SLVMD  0x10	/* CMD= 0001-00xx-x Slave Mode */
#define HT1632_CMD_MSTMD  0x14 /* CMD= 0001-01xx-x Master Mode, on-chip clock */
#define HT1632_CMD_RCCLK  0x18  /* CMD= 0001-10xx-x Master Mode, external clock */
#define HT1632_CMD_EXTCLK 0x1C	/* CMD= 0001-11xx-x Use external clock */
#define HT1632_CMD_COMS00 0x20	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS01 0x24	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS10 0x28	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS11 0x2C	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_PWM_T  0xA0	/* CMD= 101x-PPPP-x PWM duty cycle - template*/
#define HT1632_CMD_PWM(lvl) (HT1632_CMD_PWM_T | (lvl-1))
  /* Produces the correct command from the given value of lvl. lvl = [0..15] */
#define HT1632_CMD_LEN    8	/* Commands are 8 bits long, excluding the trailing bit */
#define HT1632_ADDR_LEN   7	/* Addresses are 7 bits long */
#define HT1632_WORD_LEN   4     /* Words are 4 bits long */


#if PIXELS_PER_BYTE != 8
	#error "The current drawImage, drawText and getTextWidth implementation requires PIXELS_PER_BYTE == 8"
#endif

/* Definizioni logiche */
#define HIGH 1
#define LOW 0
#define INPUT 1
#define OUTPUT 0

/* Variabili globali */
uint8_t _numActivePins;
uint8_t _currSelectionMask;
uint8_t _tgtRender;
uint8_t _tgtChannel;
byte mem[NUM_CHANNEL][ADDR_SPACE_SIZE];

extern void write_single_bit();
extern void write_data(byte data, uint8_t len);
extern void write_command(char data);
extern int get_char_width(int font_end [], uint8_t font_height, uint8_t font_index);
extern int get_chat_offset(int font_end[], uint8_t font_index);
extern void set_clk();
extern void render_target(uint8_t target);
extern void clear();

#if defined TYPE_3216_BICOLOR
extern void select(uint8_t mask);
extern void render();
#endif

extern int get_text_width(const char text [], int font_end [], uint8_t font_height, uint8_t gutter_space);
extern void draw_image(const byte * img, uint8_t width, uint8_t height, int8_t x, int8_t y, int img_offset, uint8_t offsetLeft, uint8_t offsetRight);
extern void draw_text(const char text [], int x, int y, const byte font [], int font_end [], uint8_t font_height, uint8_t gutter_space, uint8_t offsetLeft, uint8_t offsetRight);

// function for internal use
// extern void initialize();
extern void begin_cs();
extern void select_channel(uint8_t channel);


extern void set_pixel(uint8_t x, uint8_t y, uint8_t channel);
extern void clear_pixel(uint8_t x, uint8_t y, uint8_t channel);
extern uint8_t get_pixel(uint8_t x, uint8_t y, uint8_t channel);
extern void fill();
extern void fill_all();

#else
//#error "HT1632.h" already defined!

#endif	/* HT1632_H */

