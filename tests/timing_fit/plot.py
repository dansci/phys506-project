# imports
import numpy as np
import scipy.stats as st
import matplotlib.mlab as mlab
import matplotlib.pyplot as pt

# function definitions
def r(x):
    return np.sqrt(x[0]**2 + x[1]**2 + x[2]**2)

def iqr(x):
    return st.scoreatpercentile(x,75) - st.scoreatpercentile(x, 25)

# data loading
nhits = np.array([10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000])
# high tolerance
data_1 = [np.loadtxt("times_N" + str(i) + "_t.1.txt", unpack=True) for i in nhits]
# low tolerance
data_2 = [np.loadtxt("times_N" + str(i) + "_t.01.txt", unpack=True) for i in nhits]

# data splitting
rmeans_1 = np.array([np.mean(r(i)) for i in data_1])
rmedians_1 = np.array([np.median(r(i)) for i in data_1])
rstds_1 = np.array([np.std(r(i)) for i in data_1])
riqrs_1 = np.array([iqr(r(i)) for i in data_1])
rmeans_2 = np.array([np.mean(r(i)) for i in data_2])
rmedians_2 = np.array([np.median(r(i)) for i in data_2])
rstds_2 = np.array([np.std(r(i)) for i in data_2])
riqrs_2 = np.array([iqr(r(i)) for i in data_2])
tmeans_1 = np.array([np.mean(i[3]) for i in data_1])
tmedians_1 = np.array([np.median(i[3]) for i in data_1])
tstds_1 = np.array([np.std(i[3]) for i in data_1])
tiqrs_1 = np.array([iqr(i[3]) for i in data_1])
tmeans_2 = np.array([np.mean(i[3]) for i in data_2])
tmedians_2 = np.array([np.median(i[3]) for i in data_2])
tstds_2 = np.array([np.std(i[3]) for i in data_2])
tiqrs_2 = np.array([iqr(i[3]) for i in data_2])

# data plotting-- how how many histograms do we want for the nhits?
pt.hist(r(data_1[10]), bins=70, range=(0, 3), color=".6")
pt.hist(r(data_1[len(data_1)-1]), bins=70, color=".2", range=(0, 3))
pt.show()

pt.hist(r(data_2[0]), bins=70, range=(0, 3), color=".6")
pt.hist(r(data_2[len(data_2)-1]), bins=70, color=".2", range=(0, 3))
pt.show()

# statistics plotting
pt.plot(nhits, rmeans_1, color="blue", label=".1 tolerance")
pt.plot(nhits, rmeans_2, color="red", label=".01 tolerance")
pt.xlabel("number of photons")
pt.ylabel("mean(|r-r0|)")
pt.legend()
pt.show()

pt.plot(nhits, rmedians_1, color="blue", label=".1 tolerance")
pt.plot(nhits, rmedians_2, color="red", label=".01 tolerance")
pt.xlabel("number of photons")
pt.ylabel("median(|r-r0|)")
pt.legend()
pt.show()

pt.plot(nhits, rstds_1, color="blue", label=".1 tolerance")
pt.plot(nhits, rstds_2, color="red", label=".01 tolerance")
pt.xlabel("number of photons")
pt.ylabel("std(|r-r0|)")
pt.legend()
pt.show()

pt.plot(nhits, riqrs_1, color="blue", label=".1 tolerance")
pt.plot(nhits, riqrs_2, color="red", label=".01 tolerance")
pt.xlabel("number of photons")
pt.ylabel("iqr(|r-r0|)")
pt.legend()
pt.show()

pt.plot(nhits, tmeans_1, color="blue", label=".1 tolerance")
pt.plot(nhits, tmeans_2, color="red", label=".01 tolerance")
pt.xlabel("number of photons")
pt.ylabel("mean(|t-t0|)")
pt.legend()
pt.show()

pt.plot(nhits, tmedians_1, color="blue", label=".1 tolerance")
pt.plot(nhits, tmedians_2, color="red", label=".01 tolerance")
pt.xlabel("number of photons")
pt.ylabel("median(|t-t0|)")
pt.legend()
pt.show()

pt.plot(nhits, tstds_1, color="blue", label=".1 tolerance")
pt.plot(nhits, tstds_2, color="red", label=".01 tolerance")
pt.xlabel("number of photons")
pt.ylabel("std(|t-t0|)")
pt.legend()
pt.show()

pt.plot(nhits, tiqrs_1, color="blue", label=".1 tolerance")
pt.plot(nhits, tiqrs_2, color="red", label=".01 tolerance")
pt.xlabel("number of photons")
pt.ylabel("iqr(|t-t0|)")
pt.legend()
pt.show()
