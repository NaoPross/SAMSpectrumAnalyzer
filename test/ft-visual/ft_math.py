import numpy as np
import cmath as cm

def transform(func, freq, x):
    return cm.exp(-2j * np.pi * x * freq) * func(x) 

def harmonic(freq, x, ampl=1, phase=0):
    return ampl * np.sin(2 * np.pi * freq * x + phase)

def f(x):
    return harmonic(3/3, x, 2) \
         + harmonic(1/3, x, .5, -np.pi/3) \
         + harmonic(5/3, x, .1, np.pi/2)
    # return harmonic(.3, x)
