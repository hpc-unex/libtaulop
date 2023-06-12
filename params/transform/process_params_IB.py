#!/usr/bin/env python
# coding: utf-8


import pandas            as pd
import numpy             as np

from params_operations   import *


# TODO: DOCUMENTACION
#


def processOverheadNET_eager (overhead):
    return RTT(overhead)


def processOverheadNET_rndv  (overhead):
    return Ping(overhead)


def processTransferTimeRDMA (transfert, overhead):
    tt = transfert / 2.0
    tt.subtract(overhead, axis=0)
    return tt





def processChannelIB (config, mpiblib_times):

    H   = config['H']
    S   = config['S']
    tau = config["tau"]

    # 1. Create Pandas in order to manipulate data
    cols = ['o(m)'] + ['L(m, {})'.format(i) for i in range(1, tau+1)]

    t_array = np.zeros((mpiblib_times.shape[0], tau+1))

    taulop_times = pd.DataFrame(t_array,
                                columns = cols,
                                index   = mpiblib_times.index)

    #num_m = o_array.shape[0]

    # Now, we have a DataFram with the following shape (mpiblib_times):
    #
    #         o(m)   T(m,1)   T(m,2)   ...   T(m,tau)
    #     1    -       -        -              -
    #    64    -       -        -              -
    #   128    -       -        -              -
    #   ...
    #   MAX    -       -        -              -
    #
    #   Values are those from MPIBLib measurement. We have to transform into
    #    taulop parameters, using different methods that depends on the
    #    communication channel. The output wwill be a DataFrame (taulop_times):
    #
    #         o(m)   L(m,1)   L(m,2)   ...   L(m,tau)
    #     1    -       -        -              -
    #    64    -       -        -              -
    #   128    -       -        -              -
    #   ...
    #   MAX    -       -        -              -
    #

    # 2. Overhead: same as SHM and standard for all channels.
    overhead = mpiblib_times['o(m)']

    if H == 0:
        # 2a. Consider ONLY eager messages
        overhead.loc[:] = processOverheadNET_eager(overhead.loc[:])
    else:
        # 2a. overhead with eager protocol (m < H)
        overhead.loc[:H-1] = processOverheadNET_eager(overhead.loc[:H-1])
        # 2b. OVerhead with rendezvous protocol (m >= H)
        overhead.loc[H:]   = processOverheadNET_rndv(overhead.loc[H:])

    taulop_times['o(m)'] = overhead


    # 3. Transfer time
    transfert = mpiblib_times.loc[:, mpiblib_times.columns[1:]] # T(m,1) ... T(m,tau)

    # 3a. No segmented messages along all sizes
    transfert = mpiblib_times.loc[:, mpiblib_times.columns[1:]]
    overhead  = taulop_times.loc[:, 'o(m)']  # Already computed overhead
    transfert = processTransferTimeRDMA (transfert, overhead)
    taulop_times.loc[:, taulop_times.columns[1:]] = transfert.values

    # 4. Return transfer times (L) dataframe.
    return taulop_times
