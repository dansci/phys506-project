import numpy as np
import scipy.stats as st
import matplotlib.mlab as mlab
import matplotlib.pyplot as pt

def r(x):
    return np.sqrt(x[0]**2 + x[1]**2 + x[2]**2)

def iqr(x):
    return st.scoreatpercentile(x,75) - st.scoreatpercentile(x, 25)

nhits = [50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
data_p = [np.loadtxt("../position_fit/pos_N" + str(i) + "_t.1.txt", unpack=True) for i in nhits]
data_t = [np.loadtxt("../timing_fit/times_N" + str(i) + "_t.1.txt", unpack=True) for i in nhits]
data_c = [np.loadtxt("../combined_fit/comb_N" + str(i) + "_t.1.txt", unpack=True) for i in nhits]

rmeans_p = np.array([np.mean(r(i)) for i in data_p])
rmedians_p = np.array([np.median(r(i)) for i in data_p])
rstds_p = np.array([np.std(r(i)) for i in data_p])
riqrs_p = np.array([iqr(r(i)) for i in data_p])

rmeans_t = np.array([np.mean(r(i)) for i in data_t])
rmedians_t = np.array([np.median(r(i)) for i in data_t])
rstds_t = np.array([np.std(r(i)) for i in data_t])
riqrs_t = np.array([iqr(r(i)) for i in data_t])

rmeans_c = np.array([np.mean(r(i)) for i in data_c])
rmedians_c = np.array([np.median(r(i)) for i in data_c])
rstds_c = np.array([np.std(r(i)) for i in data_c])
riqrs_c = np.array([iqr(r(i)) for i in data_c])

tmeans_c = np.array([np.mean(i[3]) for i in data_c])
tmedians_c = np.array([np.median(i[3]) for i in data_c])
tstds_c = np.array([np.std(i[3]) for i in data_c])
tiqrs_c = np.array([iqr(i[3]) for i in data_c])

tmeans_t = np.array([np.mean(i[3]) for i in data_t])
tmedians_t = np.array([np.median(i[3]) for i in data_t])
tstds_t = np.array([np.std(i[3]) for i in data_t])
tiqrs_t = np.array([iqr(i[3]) for i in data_t])

pt.plot(nhits, rmeans_p, color="red", label="pos dist")
pt.plot(nhits, rmeans_t, color="blue", label="time dist")
pt.plot(nhits, rmeans_c, color="green", label="combined")
pt.xlabel("number of photons")
pt.ylabel("mean(r-r0)")
pt.legend()
pt.show()

pt.plot(nhits, rmedians_p, color="red", label="pos dist")
pt.plot(nhits, rmedians_t, color="blue", label="time dist")
pt.plot(nhits, rmedians_c, color="green", label="combined")
pt.xlabel("number of photons")
pt.ylabel("median(r-r0)")
pt.legend()
pt.show()

pt.plot(nhits, rstds_p, color="red", label="pos dist")
pt.plot(nhits, rstds_t, color="blue", label="time dist")
pt.plot(nhits, rstds_c, color="green", label="combined")
pt.xlabel("number of photons")
pt.ylabel("sd(r-r0)")
pt.legend()
pt.show()

pt.plot(nhits, riqrs_p, color="red", label="pos dist")
pt.plot(nhits, riqrs_t, color="blue", label="time dist")
pt.plot(nhits, riqrs_c, color="green", label="combined")
pt.xlabel("number of photons")
pt.ylabel("iqr(r-r0)")
pt.legend()
pt.show()

pt.plot(nhits, tmeans_t, color="blue", label="time dist")
pt.plot(nhits, tmeans_c, color="green", label="combined")
pt.xlabel("number of photons")
pt.ylabel("mean(t-t0)")
pt.legend()
pt.show()

pt.plot(nhits, tmedians_t, color="blue", label="time dist")
pt.plot(nhits, tmedians_c, color="green", label="combined")
pt.xlabel("number of photons")
pt.ylabel("median(t-t0)")
pt.legend()
pt.show()

pt.plot(nhits, tstds_t, color="blue", label="time dist")
pt.plot(nhits, tstds_c, color="green", label="combined")
pt.xlabel("number of photons")
pt.ylabel("std(t-t0)")
pt.legend()
pt.show()

pt.plot(nhits, tiqrs_t, color="blue", label="time dist")
pt.plot(nhits, tiqrs_c, color="green", label="combined")
pt.xlabel("number of photons")
pt.ylabel("iqr(t-t0)")
pt.legend()
pt.show()



