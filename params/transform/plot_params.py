#!/usr/bin/env python
# coding: utf-8


import pandas            as pd
import numpy             as np
import matplotlib        as mpl
import matplotlib.pyplot as plt


# Plot values: times dataframe
def plot_params(times, args):

    fig, ax = plt.subplots(1, 2, figsize=(16,6), dpi=80, sharex=True)

    num_m = times.index.shape[0]
    P     = times.columns.shape[0] - 1

    x = np.arange (1, num_m+1)

    ax[0].plot(x, times['o(m)'])
    ax[0].set_title("Overhead - o(m)")
    ax[0].xaxis.set_ticks(x)
    ax[0].set_xticklabels(times.index, rotation=45)
    ax[0].set_xlabel("m (bytes)")
    ax[0].set_ylabel("time (secs)")

    for p in range(1, P+1):
        y = times.iloc[:, p] # BANDWIDTH??: / times.index
        ax[1].plot(x, y)
    ax[1].set_title("Transfer time - L(m,t)")
    ax[1].set_xticklabels(times.index, rotation=45)
    ax[1].set_ylabel("time (secs)")

    fig.suptitle("Times of " + args.channel)
    
    #plt.show()
    plt.savefig(args.plot)


    return
