import matplotlib.pyplot as plt
import matplotlib.animation as anim

import numpy as np

# user_on_key_event = (lambda *args: None)
# user_update = (lambda *args: None)

def setup(update, on_key_event_=None, dotted=False):
    # setup figure
    fig = plt.figure(1)

    # handle ui events 
    global user_on_key_event
    global user_update

    user_update = update

    if (on_key_event_ != None):
        user_on_key_event = on_key_event_

    fig.canvas.mpl_connect('key_press_event', on_key_event)

    # setup axis and lines
    global sig_axis
    global ft_polar_axis
    global ft_axis

    global obs_line
    global sig_line 

    global ft_polar_line
    global ft_polar_point
    global ft_axis

    global ft_line

    fmt_opts = 'o' if dotted else ''

    # time domain plot axis
    sig_axis = fig.add_subplot(1, 2, 1)
    sig_axis.set_title('Signal')
    sig_axis.set_xlim(0, 2 * np.pi)
    sig_axis.set_ylim(-4,4)

    obs_line, = sig_axis.plot([], [], 'g' + fmt_opts, lw=1)
    sig_line, = sig_axis.plot([], [], 'b' + fmt_opts, lw=2)

    # complex (transform) axis
    ft_polar_axis = fig.add_subplot(1, 2, 2, projection='polar')
    ft_polar_axis.set_title('Fourier Transform')
    ft_polar_axis.set_xlim(0, 2 * np.pi)
    ft_polar_axis.set_ylim(0,4)

    ft_polar_line, = ft_polar_axis.plot([], [], 'r' + fmt_opts, lw=1)
    ft_polar_point, = ft_polar_axis.plot([], [], 'mo')

    # frequency domain axis
    # ft_axis = fig.add_subplot(2, 2, 4)
    # ft_axis.set_title('Fourier Trasform')
    # ft_axis.set_xlim(0, 2 * np.pi)
    # ft_axis.set_ylim(0, 20)

    # ft_line, = ft_axis.plot([], [], 'm' + fmt_opts, lw=2)

def on_key_event(event):
    global user_update
    global user_on_key_event

    global sig_axis
    global ft_polar_axis

    # update plot
    if event.key == 'r':
        sig_axis.clear()
        ft_polar_axis.clear()
        user_update()
        return

    # close window
    elif event.key == 'escape':
        plt.close()
        return

    user_on_key_event(event)

