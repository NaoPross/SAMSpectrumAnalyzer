#!/bin/env python3

import sys
import time
import random

VSERIAL_CMD = 'socat -d -d pty,raw,echo=0 pty,raw,echo=0'
VSERIAL_DEVICE = '/dev/pts/2'

# number of samples
SAMPLES_SIZE = 64

# size of a complex value in bytes
COMPLEX_VALUE_SIZE = 2 

# packet parts
pkt_header = bytes(b'\xf0\x0d')
pkt_length = (SAMPLES_SIZE * COMPLEX_VALUE_SIZE).to_bytes(2, 'little') # little endian
pkt_data   = b''

for i in range(SAMPLES_SIZE):
    # real value
    pkt_data += random.randint(0,1024).to_bytes(2, 'little')
    # complex value
    pkt_data += random.randint(0,1024).to_bytes(2, 'little')

# build packet
pkt = pkt_header + pkt_length + pkt_data

# show packet
print("packet:")
print(':'.join("{:02x}".format(c) for c in pkt))

f = open(VSERIAL_DEVICE, 'wb')
# send packet
f.write(pkt)
time.sleep(1)
f.close()
