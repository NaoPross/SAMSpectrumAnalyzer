#!/bin/env python3

from functools import reduce

import ft_plot 
import ft_args 
from ft_math import *

# parse argumtents
args = ft_args.parse_args()

# setup analysis data
samples_count = args.samples

# data
def update():
    global args

    sig_x = [2 * np.pi * t / samples_count * args.periods for t in range(samples_count)]

    obs_y = [harmonic(args.harmonic, x, 1, 0) for x in sig_x] 
    sig_y = [f(x) for x in sig_x]

    ft_plot.obs_line.set_data(sig_x, obs_y)
    ft_plot.sig_line.set_data(sig_x, sig_y)

    ft_cpx = [transform(f, args.harmonic, x) for x in sig_x]
    ft_polar = reduce((lambda x,y: x+y), ft_cpx) / len(ft_cpx)

    # ft_even = [ft_cpx[2 * i] for i in range(int(len(ft_cpx)/2))]
    # ft_odd  = [ft_cpx[2 * i + 1] for i in range(int(len(ft_cpx)/2))]

    ft_plot.ft_polar_line.set_data([cm.phase(z) for z in ft_cpx], [abs(z) for z in ft_cpx])
    ft_plot.ft_polar_point.set_data([cm.phase(ft_polar)], [abs(ft_polar)])

    # ft_polar_even.set_data([cm.phase(z) for z in ft_even], [abs(z) for z in ft_even])
    # ft_polar_odd.set_data([cm.phase(z) for z in ft_odd], [abs(z) for z in ft_odd])

    # ft_plot.ft_line.set_data(sig_x, [abs(z) for z in ft_y])

    ft_plot.plt.draw()

def on_key_event(event):
    global args

    # manipulate observed harmonic
    if event.key == '-':
        args.harmonic -= args.step
        if args.harmonic <= 0:
            args.harmonic = args.step

    elif event.key == '+' or event.key == '=':
        args.harmonic += args.step

    elif event.key == '>':
        args.harmonic /= 2

    elif event.key == '<':
        args.harmonic *= 2

    elif event.key == 't':
        args.harmonic += 2 * np.pi / samples_count

    update()
    print('observing harmonic of frequency: ', str(args.harmonic))

ft_plot.setup(update, on_key_event, args.dots)
ft_plot.plt.show()
