import argparse

# parse arguments
def parse_args():
    parser = argparse.ArgumentParser( \
            description='Graphical visualization of the Fourier Transform')

    parser.add_argument('-n', '--samples', type=int, \
            action='store', default=1024, \
            help='number of points used to calculate the FT')

    parser.add_argument('-p', '--periods', type=float, \
            action='store', default=1, \
            help='number of periods observed by the FT')

    parser.add_argument('-H', '--harmonic', type=float, \
            action='store', default=.1, \
            help='frequency (in Hz) of the harmonic to observe')

    parser.add_argument('-s', '--step', type=float, \
            action='store', default=.1, \
            help='frequency step (in Hz) used when increasing or decreasing the \
            observed frequency')

    parser.add_argument('-d', '--dots', \
            action='store_true', \
            help='plot dots instead of continuous lines')

    return parser.parse_args()
