import numpy as np
import scipy.stats as st
import matplotlib.mlab as mlab
import matplotlib.pyplot as pt

def r(x):
    return np.sqrt(x[0]**2 + x[1]**2 + x[2]**2)

def iqr(x):
    return st.scoreatpercentile(x,75) - st.scoreatpercentile(x, 25)

nhits = np.array([50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000])
data = np.array([np.loadtxt("comb_N" + str(i) + "_t.1.txt", unpack=True) for i in nhits])
xmeans = np.array([np.mean(i[0]) for i in data])
xmedians = np.array([np.median(i[0]) for i in data])
xstds = np.array([np.std(i[0]) for i in data])
xiqrs = np.array([iqr(i[0]) for i in data])

rmeans = np.array([np.mean(r(i)) for i in data])
rmedians = np.array([np.median(r(i)) for i in data])
rstds = np.array([np.std(r(i)) for i in data])
riqrs = np.array([iqr(r(i)) for i in data])

tmeans = np.array([np.mean(i[3]) for i in data])
tmedians = np.array([np.median(i[3]) for i in data])
tstds = np.array([np.std(i[3]) for i in data])
tiqrs = np.array([iqr(i[3]) for i in data])
