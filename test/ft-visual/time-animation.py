import matplotlib.pyplot as plt
import matplotlib.animation as anim

import numpy as np
import math as m
import cmath as cm

# setup analysis data
samples_count = 1024 * 2
obs_periods = 1
obs_harmonic_freq = .1

def harmonic(freq, x, ampl=1, phase=0):
    return ampl * np.sin(2 * np.pi * freq * x + phase)

def f(x):
    return harmonic(.3, x) + harmonic(.1, x, ampl=.5, phase=-np.pi/3)
    # return harmonic(.3, x)

def transform(func, freq, x):
    return cm.exp(-2j * np.pi * x * freq) * func(x) 


# setup video
framerate = 60 # in fps
duration = 5 # in seconds
frames_count = framerate * duration

# setup plots
fig = plt.figure(1)

## time domain plot axis
sig_axis = fig.add_subplot(1, 2, 1)
sig_axis.set_title('Signal')
sig_axis.set_xlim(0, 2 * np.pi)
sig_axis.set_ylim(-2,2)

obs_line, = sig_axis.plot([], [], 'g', lw=1)
sig_line, = sig_axis.plot([], [], 'b', lw=2)

## complex (transform) axis
ft_polar_axis = fig.add_subplot(1, 2, 2, projection='polar')
ft_polar_axis.set_title('Fourier Transform')
ft_polar_axis.set_xlim(0, 2 * np.pi)
ft_polar_axis.set_ylim(0,2)

ft_polar, = ft_polar_axis.plot([], [], 'r', lw=1)

# animation
animation_running = False

anim_sig_x = []
anim_obs_y = []
anim_sig_y = []

anim_ft_x = []
anim_ft_y = []

def init_animation():
    global anim_sig_x
    global anim_obs_y
    global anim_sig_y

    global anim_ft_x
    global anim_ft_y

    global obs_line 
    global sig_line
    global ft_polar

    global animation_running

    anim_sig_x = []
    anim_obs_y = []
    anim_sig_y = []

    anim_ft_x = []
    anim_ft_y = []

    obs_line.set_data([], [])
    sig_line.set_data([], [])
    ft_polar.set_data([], [])

    animation_running = True
    return obs_line, sig_line, ft_polar


def animation(i):
    global frames_count
    global obs_periods
    global obs_harmonic_freq

    global anim_sig_x
    global anim_obs_y
    global anim_sig_y

    global anim_ft_x
    global anim_ft_y

    global obs_line
    global sig_line
    global ft_polar

    x = 2 * np.pi * i / frames_count * obs_periods

    anim_sig_x.append(x)
    anim_obs_y.append(harmonic(obs_harmonic_freq, x, 1, 0))
    anim_sig_y.append(f(x))

    t = transform(f, obs_harmonic_freq, x)
    anim_ft_x.append(cm.phase(t))
    anim_ft_y.append(abs(t))

    # print(str(obs_harmonic_freq))

    obs_line.set_data(anim_sig_x, anim_obs_y)
    sig_line.set_data(anim_sig_x, anim_sig_y)
    ft_polar.set_data(anim_ft_x, anim_ft_y)

    return obs_line, sig_line, ft_polar

def reset_animation():
    global six_axis
    global ft_polar_axis

    sig_axis.clear()
    ft_polar_axis.clear()

    a.frame_seq = a.new_frame_seq() 

    init_animation()
    

# handle gui events
def on_key_event(event):
    global animation_running
    global obs_harmonic_freq

    #if event.key == 'P':
        # if animation_running:
        #     a.event_source.stop()
        #     animation_running = False
        # else:
        #     a.event_source.start()
        #     animation_running = True

    if event.key == 'r':
        a.event_source.stop()
        reset_animation()
        a.event_source.start()

    elif event.key == '-':
        obs_harmonic_freq -= .1
        if obs_harmonic_freq <= 0:
            obs_harmonic_freq = .1

        reset_animation()
        print('observing harmonic of frequency: ', str(obs_harmonic_freq))

    elif event.key == '+' or event.key == '=':
        obs_harmonic_freq += .1
        reset_animation()
        print('observing harmonic of frequency: ', str(obs_harmonic_freq))


fig.canvas.mpl_connect('key_press_event', on_key_event)
a = anim.FuncAnimation(fig, animation, \
        init_func=init_animation, frames=frames_count, \
        interval=int(1/framerate*1000), repeat=False, blit=True)

plt.show()
