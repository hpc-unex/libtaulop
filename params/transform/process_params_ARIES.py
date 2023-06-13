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





def processChannelARIES (config, overhead_times, transfert_times):

    H   = config['H']
    S   = config['k']
    tau = config["tau"]

    # 1. Create Pandas in order to manipulate data
    cols = ['o(m)'] + ['L(m, {})'.format(i) for i in range(1, tau+1)]

    t_array = np.zeros((overhead_times.shape[0], tau+1))

    taulop_times = pd.DataFrame(t_array,
                                columns = cols,
                                index   = overhead_times.index)

    # Inputs are 2 dataframes with the following shapes:
    #
    #  Overhead (overhead_times):
    #
    #         o(m)_eager    o(m)_rndv
    #     1       -             -
    #    64       -             -
    #   128       -             -
    #   ...
    #   MAX       -             -
    #
    #  Transfer times (transfert_times):
    #
    #         T(m,1)   T(m,2)   ...   T(m,tau)
    #     1    -        -              -
    #    64    -        -              -
    #   128    -        -              -
    #   ...
    #   MAX    -        -              -
    #
    #   The goal is to transform previous data read from MPIBlib into
    #    taulop parameters, using different methods that depends on the
    #    communication channel. The output will be a DataFrame (taulop_times):
    #
    #         o(m)   L(m,1)   L(m,2)   ...   L(m,tau)
    #     1    -       -        -              -
    #    64    -       -        -              -
    #   128    -       -        -              -
    #   ...
    #   MAX    -       -        -              -
    #


    # 2. Overhead: same as SHM and standard for all channels.
    if H == 0:
        H = overhead_times.index[-1] + 1 # Last value (included)

    # 2a. overhead with eager protocol (m < H)
    taulop_times.loc[ :H-1, 'o(m)'] = processOverheadNET_eager(overhead_times.loc[ :H-1, 'o(m)_eager'])
    # 2b. OVerhead with rendezvous protocol (m >= H)
    taulop_times.loc[H:   , 'o(m)'] = processOverheadNET_rndv (overhead_times.loc[H:   , 'o(m)_rndv'])


    # 3. Transfer time: No segmented messages across all sizes. Not using S.
    #                   (segmentation is in TCP/IP protocol)
    transfert = transfert_times.loc[:, transfert_times.columns[0:]] # T(m,1) ... T(m,tau)
    overhead  = taulop_times.loc[:, 'o(m)']                         # Overhead already computed

    transfert = processTransferTimeRDMA (transfert, overhead)
    taulop_times.loc[:, taulop_times.columns[1:]] = transfert.values


    # 4. Return transfer time (L) dataframe.
    return taulop_times
