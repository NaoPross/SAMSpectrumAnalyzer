#include "ht1632.h"

/* Funzioni interne alla libreria:*/
inline void pulse_clk() {
	//digitalWrite(_pinCLK, HIGH);
    _pinCLK = HIGH;
	NOP();
	//digitalWrite(_pinCLK, LOW);
    _pinCLK = LOW;
}

inline int max(int a, int b) {
    return (a > b) ? a : b;
}

inline int min(int a, int b) {
    return (a < b) ? a : b;
}

#if defined TYPE_3216_BICOLOR
// This is used to send initialization commands, and so selects all chips
// in the selected board.
void select(uint8_t mask) {
    
    uint8_t t=1;
            
    #if defined _pinCS1
        _pinCS1 = ((t & mask)?LOW:HIGH);
        t <<= 1;
    #endif
        
    #if defined _pinCS2
        _pinCS2 = ((t & mask)?LOW:HIGH);
        t <<= 1:
    #endif

    #if defined _pinCS3
        _pinCS3 = ((t & mask)?LOW:HIGH);
        t <<= 1;
    #endif

    #if defined _pinCS4
        _pinCS4 = ((t & mask)?LOW:HIGH);
    #endif
        
	for (uint8_t tmp = 0; tmp < NUM_ACTIVE_CHIPS; tmp++) {
		pulse_clk();
	}
}
#endif

// Write single bit to display, used as padding between commands.
// PRECONDITION: WR is LOW
void write_single_bit() {
	// Set the DATA pin to the correct state
    _pinDATA = LOW;
	NOP(); // Delay
	// Raise the WR momentarily to allow the device to capture the data
    _pinWR = HIGH;
	NOP(); // Delay
	// Lower it again, in preparation for the next cycle.
    _pinWR = LOW;
}

// Integer write to display. Used to write commands/addresses.
// PRECONDITION: WR is LOW
void write_data(byte data, uint8_t len) {
	for(int j = len - 1, t = 1 << (len - 1); j >= 0; --j, t >>= 1){
		// Set the DATA pin to the correct state
        _pinDATA =(((data & t) == 0)?LOW:HIGH);
		NOP(); // Delay 
		// Raise the WR momentarily to allow the device to capture the data
		_pinWR = HIGH;
        NOP(); // Delay
		// Lower it again, in preparation for the next cycle.
        _pinWR = LOW;
	}
}

void write_command(char data) {
	write_data(data, HT1632_CMD_LEN);
	write_single_bit();
}

int get_char_width(int font_end [], uint8_t font_height, uint8_t font_index) {
	uint8_t bytesPerColumn = (font_height >> 3) + ((font_height & 0b111)?1:0); // Assumes that PIXELS_PER_BYTE is 8

	if(font_index == 0) {
		return font_end[0];
	}
	// The width is the difference between the ending index of
	//  this and the previous characters:
	return (font_end[font_index] - font_end[font_index - 1])/bytesPerColumn;
}

int get_char_offset(int font_end [], uint8_t font_index) {
	if(font_index == 0) {
		return 0;
	}
	// The offset is in the ending index of the previous character:
	return font_end[font_index - 1];
}

void set_clk() {
    _pinCLK = LOW;
}

/* Funzioni esterne alla libreria:*/
void render_target(uint8_t target) {
	if(target < _numActivePins) {
		_tgtRender = target;
	}
}

void clear() {
	for(uint8_t c = 0; c < NUM_CHANNEL; ++c) {
		for(uint8_t i = 0; i < ADDR_SPACE_SIZE; ++i) {
			mem[c][i] = 0x00; // Needs to be redrawn
		}
	}
}

#if defined TYPE_3216_BICOLOR
// Draw the contents of mem
void render() {
	if(_tgtRender >= _numActivePins) {
		return;
	}
    
	// Write chip-by-chip:	
	for (uint8_t nChip = 0; nChip < NUM_ACTIVE_CHIPS; ++nChip) {
		// Select a single sub-chip:
       
        if(_tgtRender == 0){
            _pinCS1 = HIGH;
            
            for(uint8_t tmp = 0; tmp < NUM_ACTIVE_CHIPS; tmp++){
                if (tmp == nChip) {
                	//digitalWrite(_pinForCS, LOW);
                    _pinCS1 = LOW;
            		pulse_clk();
        			//digitalWrite(_pinForCS, HIGH);
                    _pinCS1 = HIGH;
    			} else {
                    pulse_clk();
                }
            }
        }
        
    #if defined _pinCS2
        else if(_tgtRender == 1){
            _pinCS2 = HIGH;
            
            for(uint8_t tmp = 0; tmp < NUM_ACTIVE_CHIPS; tmp++){
                if (tmp == nChip) {
                	//digitalWrite(_pinForCS, LOW);
                    _pinCS2 = LOW;
            		pulse_clk();
        			//digitalWrite(_pinForCS, HIGH);
                    _pinCS2 = HIGH;
    			} else {
                    pulse_clk();
                }
            }
        }
    #endif

    #if defined _pinCS2
        else if(_tgtRender == 2){
            _pinCS3 = HIGH;
            
            for(uint8_t tmp = 0; tmp < NUM_ACTIVE_CHIPS; tmp++){
                if (tmp == nChip) {
                	//digitalWrite(_pinForCS, LOW);
                    _pinCS3 = LOW;
            		pulse_clk();
        			//digitalWrite(_pinForCS, HIGH);
                    _pinCS3 = HIGH;
    			} else {
                    pulse_clk();
                }
            }
        }
    #endif
        
    #if defined _pinCS2
        else if(_tgtRender == 3){
            _pinCS4 = HIGH;
            
            for(uint8_t tmp = 0; tmp < NUM_ACTIVE_CHIPS; tmp++){
                if (tmp == nChip) {
                	//digitalWrite(_pinForCS, LOW);
                    _pinCS4 = LOW;
            		pulse_clk();
        			//digitalWrite(_pinForCS, HIGH);
                    _pinCS4 = HIGH;
    			} else {
                    pulse_clk();
                }
            }
        }
    #endif
        
		// Output data!
		write_data(HT1632_ID_WR, HT1632_ID_LEN);
		write_data(0, HT1632_ADDR_LEN); // Selecting the memory address

		// Write the channels in order
		for(uint8_t c = 0; c < NUM_CHANNEL; ++c) {
			//for(uint8_t i = (nChip & 0b1)?0:(ADDR_SPACE_SIZE >> 1); i < (nChip & 0b1)?(ADDR_SPACE_SIZE >> 1):ADDR_SPACE_SIZE; ++i) {
			uint8_t i, iMax;

			if(nChip & 0b1) { // If we're writing to the chips on the left
				i = 0; // Start from zero
				iMax = ADDR_SPACE_SIZE/2; // Stop at the halfway point.
			} else { // If we're writing to the chips on the right
				i = ADDR_SPACE_SIZE/2; // Start from the halfway point.
				iMax = ADDR_SPACE_SIZE; // Stop at the end of the buffer.
			}

			// If we are not (top-row chip)
			if(!(nChip & 0b10)) {
				++i; // Write only odd-numbered bytes.
			}

			for(; i < iMax; i+=2) { // Write every other byte.
				// Write the higher bits before the the lower bits.
				write_data(mem[c][i] >> HT1632_WORD_LEN, HT1632_WORD_LEN);
				write_data(mem[c][i], HT1632_WORD_LEN);
			}
		}
	}
}
#endif

int get_text_width(const char text [], int font_end [], uint8_t font_height, uint8_t gutter_space) {
	int wd = 0;
	char i = 0;
	char currchar;
	
	while(1){  
		if (text[i] == '\0') {
			return wd - gutter_space;
		}
			
		currchar = text[i] - 32;
		if (currchar >= 65 && currchar <= 90) { // If character is lower-case, automatically make it upper-case
			currchar -= 32;                     // Make this character uppercase.
		}

		if (currchar < 0 || currchar >= 64) {   // If out of bounds, skip
			++i;
			continue; // Skip this character.
		}

		wd += get_char_width(font_end, font_height, currchar) + gutter_space;
		++i;
	}
}

void draw_image(const byte * img, uint8_t width, uint8_t height, int8_t x, int8_t y, int img_offset, uint8_t offsetLeft, uint8_t offsetRight) {
	// Assuming that we are using 8 PIXELS_PER_BYTE, this does the equivalent of Math.ceil(height/PIXELS_PER_BYTE):
	uint8_t bytesPerColumn = (height >> 3) + ((height & 0b111)?1:0);

	// Sanity checks
	if(y + height < 0 || x + width < 0 || y > COM_SIZE || x > OUT_SIZE)
		return;
	// After looking at the rest of this function, you may need one.
	
	// Copying Engine.

	// Current off
	int8_t dst_x = x;
	int8_t src_x = 0;
	// Repeat until each column has been copied.
	while (src_x < width) {
		if(dst_x < 0 || dst_x < offsetLeft) {                           //offsetLeft
			// Skip this column if it is too far to the left.
			src_x++;
			dst_x++;
			continue;
		} else if (dst_x >= OUT_SIZE || dst_x >= offsetRight) {         //offsetRight
			// End the copy if it is too far to the right.
			break;
		}

		int8_t src_y = 0;
		int8_t dst_y = y;
		while (src_y < height) {
			if (dst_y < 0) {
				// Skip pixels if the starting point to too far up.
				src_y -= dst_y;
				dst_y = 0;
				continue;
			} else if (dst_y >= COM_SIZE) {
				// End copying this column if it is too far down
				break;
			}

			// The use of bitmasking here assumes that PIXELS_PER_BYTE == 8

			// Find out how many we can copy in the next step:
			//  as a minimum of the number of bits in the current byte of source
			//  and destination.
			uint8_t copyInNextStep = 8 - max((src_y & 0b111), (dst_y & 0b111));

			// Limit this by the height of the image:
			copyInNextStep = min(copyInNextStep, (height - src_y));

			// Prepare the bitmask with the number of bits that need to be copied.
			uint8_t dst_copyMask = (0b1 << copyInNextStep) - 1;
			
			// Shift the bitmasks to the correct position.
			dst_copyMask <<= (8 - (dst_y & 0b111) - copyInNextStep);

			// Shift the data to the bits of highest significance
            uint8_t copyData = pgm_read_byte(&img[img_offset + (bytesPerColumn * src_x) + (src_y >> 3)]) << (src_y & 0b111);
			// Shift data to match the destination place value.
			copyData >>= (dst_y & 0b111);

			// Perform the copy
			mem[_tgtChannel][GET_ADDR_FROM_X_Y(dst_x, dst_y)] =  // Put in destination
				(mem[_tgtChannel][GET_ADDR_FROM_X_Y(dst_x, dst_y)] & ~dst_copyMask) | // All bits not in the mask from destination
				(copyData & dst_copyMask); // All bits in the mask from source

			src_y += copyInNextStep;
			dst_y += copyInNextStep;
		}

		src_x++;
		dst_x++;
	}
}

void draw_text(const char text [], int x, int y, const byte font [], int font_end [], uint8_t font_height, uint8_t gutter_space, uint8_t offsetLeft, uint8_t offsetRight) {
	int curr_x = x;
	char i = 0;
	char currchar;
	
	// Check if string is within y-bounds
	if(y + font_height < 0 || y >= COM_SIZE)
	return;
	
	while(1){  
		if(text[i] == '\0')
			return;
		
		currchar = text[i] - 32;
		if(currchar >= 65 && currchar <= 90) // If character is lower-case, automatically make it upper-case
			currchar -= 32; // Make this character uppercase.

		if(currchar < 0 || currchar >= 64) { // If out of bounds, skip
			++i;
			continue; // Skip this character.
		}

		// Check to see if character is not too far right.
		if(curr_x >= OUT_SIZE)     //OUT_SIZE   
			break; // Stop rendering - all other characters are no longer within the screen 
        
        // Check to see if character is not too far left.
		int chr_width = get_char_width(font_end, font_height, currchar);
        
        if(curr_x + chr_width + gutter_space >= 0){
            //if(curr_x <= offsetLeft)
              //  break;
            
            draw_image(font, chr_width, font_height, curr_x, y,  get_char_offset(font_end, currchar), offsetLeft, offsetRight);
			
			// Draw the gutter space
			for(char j = 0; j < gutter_space; ++j)
			draw_image(font, 1, font_height, curr_x + chr_width + j, y, 0, 0, 0);
		}
		
		curr_x += chr_width + gutter_space;
		++i;
	}
}

// (Funzione interna)
void initialize() {

	select(0);
    
    // Clear all memory
	clear();

	// Send configuration to chip:
	// This configuration is from the HT1632 datasheet, with one modification:
	//   The RC_MASTER_MODE command is not sent to the master. Since acting as
	//   the RC Master is the default behaviour, this is not needed. Sending
	//   this command causes problems in HT1632C (note the C at the end) chips. 
	
	// Send Master commands
	
	select(0b1111); // Assume that board 1 is the master.
	write_data(HT1632_ID_CMD, HT1632_ID_LEN);    // Command mode
	
	write_command(HT1632_CMD_SYSDIS); // Turn off system oscillator
	
	// Custom initialization from each:
#if defined TYPE_3216_BICOLOR
	write_command(HT1632_CMD_COMS00);
	write_command(HT1632_CMD_RCCLK);  // Master Mode, external clock
#endif

	write_command(HT1632_CMD_SYSEN); //Turn on system
	write_command(HT1632_CMD_LEDON); // Turn on LED duty cycle generator
	write_command(HT1632_CMD_PWM(16)); // PWM 16/16 duty
	
	select(0);
	
	// Clear all screens by default:
	for(uint8_t i = 0; i < _numActivePins; ++i) {
		render_target(i);
		render();
	}
	// Set render_target to the first board.
	render_target(0);
}

void begin_cs(){    
    set_clk();
    
#if defined _pinCS4 
    _numActivePins = 4;
	initialize();


#elif defined _pinCS3
    _numActivePins = 3;
	initialize();    


#elif defined _pinCS2 
    _numActivePins = 2;
	initialize();

        
#elif defined _pinCS1
    _numActivePins = 1;
	initialize();    
#endif
}

void select_channel(uint8_t channel) {
	if(channel < NUM_CHANNEL) {
		_tgtChannel = channel;
	}
}

void set_pixel(uint8_t x, uint8_t y, uint8_t channel) {
	if( x < 0 || x > OUT_SIZE || y < 0 || y > COM_SIZE )
		return;
	mem[channel][GET_ADDR_FROM_X_Y(x, y)] |= GET_BIT_FROM_Y(y);
}

void clear_pixel(uint8_t x, uint8_t y, uint8_t channel) {
	if( x < 0 || x > OUT_SIZE || y < 0 || y > COM_SIZE )
		return;
	mem[channel][GET_ADDR_FROM_X_Y(x, y)] &= ~(GET_BIT_FROM_Y(y));
}

uint8_t get_pixel(uint8_t x, uint8_t y, uint8_t channel) {
	if( x < 0 || x > OUT_SIZE || y < 0 || y > COM_SIZE )
		return 0;
	return mem[channel][GET_ADDR_FROM_X_Y(x, y)] & GET_BIT_FROM_Y(y);
}

void fill() {
	for(uint8_t i = 0; i < ADDR_SPACE_SIZE; ++i) {
		mem[_tgtChannel][i] = 0xFF;
	}
}

void fill_all() {
	for(uint8_t c = 0; c < NUM_CHANNEL; ++c) {
		for(uint8_t i = 0; i < ADDR_SPACE_SIZE; ++i) {
			mem[c][i] = 0xFF; // Needs to be redrawn
		}
	}
}