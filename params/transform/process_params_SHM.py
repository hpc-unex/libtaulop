#!/usr/bin/env python
# coding: utf-8


import pandas            as pd
import numpy             as np

from params_operations   import *


# TODO: DOCUMENTACION
# Overhead is computed in shared memory using:

# For eager protocol (m < H), a ping pong algorithm.
# For rendezvous protocol (m >= H), a Round Trip Time (RTT).

# Overhead is computed in networks using:

# For eager protocol (m < H), a ping pong algorithm.
# For rendezvous protocol (m >= H), a Round Trip Time (RTT).


def processOverheadSHM_eager (overhead):
    return RTT(overhead)


def processOverheadSHM_rndv  (overhead):
    return Ping(overhead)


def processTm1_SHM_noseg (transfert, overhead):
    return PingPong_SHM(transfert, overhead)


def processTm1_SHM_seg (transfert, overhead, S):
    return PingPong_SHM(transfert, overhead)


def processTransferTimeSHM_noseg (transfert, overhead):
    return Ring_SHM(transfert, overhead)


def processTransferTimeSHM_seg (transfert, overhead, S):
    k = transfert.index // S
    # TODO: Esto tiene miga.
    # seg = transfert.subtract(overhead, axis=0) / (2.0 * k)

    # BY NOW: no segmented:
    return transfert.subtract(overhead, axis=0) / 2.0




def processChannelSHM (config, overhead_times, transfert_times):

    H   = config['H']
    S   = config['k']
    tau = config["tau"]

    # 1. Create Pandas in order to manipulate data
    cols = ['o(m)'] + ['L(m, {})'.format(i) for i in range(1, tau+1)]

    t_array = np.zeros((overhead_times.shape[0], tau+1))

    taulop_times = pd.DataFrame(t_array,
                                columns = cols,
                                index   = overhead_times.index)

    #num_m = o_array.shape[0]

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

    # 2. Overhead
    if H == 0:
        H = overhead_times.index[-1] + 1 # Last value (included)

    # 2a. overhead with eager protocol (m < H)
    taulop_times.loc[ :H-1, 'o(m)'] = processOverheadSHM_eager(overhead_times.loc[ :H-1, 'o(m)_eager'])
    # 2b. OVerhead with rendezvous protocol (m >= H)
    taulop_times.loc[H:   , 'o(m)'] = processOverheadSHM_rndv (overhead_times.loc[H:   , 'o(m)_rndv'])


    # 3. Transfer time:  T(m,1), tau=1 needs P=2 in a PingPong
    if S == 0:
        S = transfert_times.index[-1] + 1  # Last value (included)

    # 3a. No segmented messages: m < S
    transfert_noseg = transfert_times.loc[0:S-1, transfert_times.columns[0]]
    o_noseg  = taulop_times.loc[ :S-1, 'o(m)']  # taulop overhead
    # Usage of .values because columns names are not the same in mpiblib and taulop times DataFrames:
    taulop_times.loc[0:S-1, taulop_times.columns[1]] = processTm1_SHM_noseg(transfert_noseg, o_noseg).values

    # 3b. Segmented messages: m >= S
    transfert_seg   = transfert_times.loc[S: , transfert_times.columns[0]]
    o_seg           = taulop_times.loc[S: , 'o(m)']    # taulop overhead
    taulop_times.loc[S: , taulop_times.columns[1]]   = processTm1_SHM_seg(transfert_seg, o_seg, S).values


    # 4. Transfer time:  T(m,2) ... T(m, P), tau=2..P needs P=tau in a Ring
    #transfert = mpiblib_times.loc[:, mpiblib_times.columns[2:]] # T(m,2) ... T(m,tau)
    # 4a. No segmented messages: m < S
    transfert_noseg = transfert_times.loc[0:S-1, transfert_times.columns[1:]]
    o_noseg  = taulop_times.loc[ :S-1, 'o(m)']  # taulop overhead
    # Usage of .values because columns names are not the same in mpiblib and taulop times DataFrames:
    taulop_times.loc[0:S-1, taulop_times.columns[2:]] = processTransferTimeSHM_noseg(transfert_noseg, o_noseg).values

    # 4b. Segmented messages: m >= S
    transfert_seg   = transfert_times.loc[S: , transfert_times.columns[1:]]
    o_seg           = taulop_times.loc[S: , 'o(m)']    # taulop overhead
    taulop_times.loc[S: , taulop_times.columns[2:]]   = processTransferTimeSHM_seg(transfert_seg, o_seg, S).values

    # 5. Return a new DataFrame with taulop times processed.
    return taulop_times
