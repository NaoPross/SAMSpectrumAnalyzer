#!/bin/env python3

import sys
import time
import random

# VSERIAL_CMD = 'socat -d -d pty,raw,echo=0 pty,raw,echo=0'
# VSERIAL_DEVICE = '/dev/pts/2'

VSERIAL_DEVICE = sys.argv[1]

# number of samples
SAMPLES_SIZE = 128

# packet parts
pkt_header = "S\n\r"
pkt_data   = ""
pkt_footer = "E\n\r"

for i in range(SAMPLES_SIZE):
    # real value
    pkt_data += "%04di%04d\n\r" % (random.randint(0, 1024), random.randint(0,1024))

# build packet
pkt = bytes(pkt_header + pkt_data + pkt_footer, "ascii")

# show packet
print("packet:")
#print(':'.join("{:02x}".format(c) for c in pkt))
print(pkt_header + pkt_data + pkt_footer)

# send packet
f = open(VSERIAL_DEVICE, 'wb')
f.write(pkt)
f.close()
