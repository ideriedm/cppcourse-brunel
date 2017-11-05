import numpy as np
import matplotlib.pyplot as pl

data = np.genfromtxt('./result/spikes')
#random between 0 to 12500 excluded, and loaded them in an array selected_indexes (length =50)
selected_indexes = np.random.randint(0, high=12500, size=50)

select = np.array([d for d in data if d[1] in selected_indexes])
#Changes column into line
data1 = select.transpose()

#axs = array of the 3 subplots. param = Number of plots on the figure, number of columns , same x axis
fig, axs = pl.subplots(nrows=2, ncols=1, sharex=True)
# 50 random neurons in a table
ax = axs[0]
ax.scatter(0.1*data1[0],data1[1],alpha=0.8, edgecolors='none')
ax.set_title("50 randomly chosen neurons")
ax.set_ylabel("ID of the neuron")
ax.set_ylim(0,12500)
ax.set_yticks([], [])

#The graph with the firing frequency
ax = axs[1]
n, bins, patches = ax.hist(0.1*data.transpose()[0], 2000, normed=0, alpha=0.75)
ax.set_xlabel("Time [ms]", fontsize=12, fontweight ="medium")
ax.set_ylabel("Firing frequency in bins of 0.1 [ms]", fontsize=12, fontweight ="medium")
ax.set_title("Firing frequency vs time")
pl.show();


