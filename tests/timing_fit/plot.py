import numpy as np
import scipy.stats as st
import matplotlib.mlab as mlab
import matplotlib.pyplot as pt

def r(x):
    return np.sqrt(x[0]**2 + x[1]**2 + x[2]**2)

def iqr(x):
    return st.scoreatpercentile(x,75) - st.scoreatpercentile(x, 25)

nbins = 70
x50 = np.loadtxt('times_N50_t.1.txt', unpack=True)
r50 = r(x50)
x100 = np.loadtxt('times_N100_t.1.txt', unpack=True)
r100 = r(x100)
x500 = np.loadtxt('times_N500_t.1.txt', unpack=True)
r500 = r(x500)
x1000 = np.loadtxt('times_N1000_t.1.txt', unpack=True)
r1000 = r(x1000)
pt.hist(x50[0], range=(-1, 1), bins=nbins, color="black")
pt.hist(x100[0], range=(-1, 1), bins=nbins, color="green")
pt.hist(x500[0], range=(-1, 1), bins=nbins, color="blue")
pt.hist(x1000[0], range=(-1, 1), bins=nbins, color="red")
pt.show()

pt.hist(x50[3], bins=nbins, color="black")
pt.hist(x100[3], bins=nbins, color="green")
pt.hist(x500[3], bins=nbins, color="blue")
pt.hist(x1000[3], bins=nbins, color="red")
pt.show()

pt.hist(r50, range=(-1, 1), bins=nbins, color="black")
pt.hist(r100, range=(-1, 1), bins=nbins, color="green")
pt.hist(r500, range=(-1, 1), bins=nbins, color="blue")
pt.hist(r1000, range=(-1, 1), bins=nbins, color="red")
pt.show()

nhits = np.array([50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000])
data = np.array([np.loadtxt("times_N" + str(i) + "_t.1.txt", unpack=True) for i in nhits])
xmeans = np.array([np.mean(i[0]) for i in data])
xmedians = np.array([np.median(i[0]) for i in data])
xstds = np.array([np.std(i[0]) for i in data])
xiqrs = np.array([iqr(i[0]) for i in data])

tmeans = np.array([np.mean(i[3]) for i in data])
tmedians = np.array([np.median(i[3]) for i in data])
tstds = np.array([np.std(i[3]) for i in data])
tiqrs = np.array([iqr(i[3]) for i in data])
