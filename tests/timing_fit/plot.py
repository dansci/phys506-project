import numpy as np
import scipy.stats as spst
import matplotlib.mlab as mlab
import matplotlib.pyplot as pt

nbins = 70
x100 = np.loadtxt('times_N100_t.1.txt', unpack=True)
x500 = np.loadtxt('times_N500_t.1.txt', unpack=True)
x1000 = np.loadtxt('times_N1000_t.1.txt', unpack=True)
pt.hist(x100[0], range=(-1, 1), bins=nbins, color="green")
pt.hist(x500[0], range=(-1, 1), bins=nbins, color="blue")
pt.hist(x1000[0], range=(-1, 1), bins=nbins, color="red")
pt.show()
