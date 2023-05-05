#!/usr/bin/env python
# coding: utf-8


import pandas            as pd
import numpy             as np
import matplotlib        as mpl
import matplotlib.pyplot as plt



# Operations implemented to compute parameters in different machines, channels, protocols, ...
# Defined in
#   Juan-Antonio Rico-Gallego, Juan-Carlos Díaz-Martín, Alexey L. Lastovetsky,
#   "Extending τ-Lop to model concurrent MPI communications in multicore clusters",
#   Future Generation Computer Systems, Volume 61, 2016, Pages 66-82, ISSN 0167-739X,
#   https://doi.org/10.1016/j.future.2016.02.021.


# Measuring overhead for eager protocol in all channels.
# It sends messages of size 0 using MPI_Send, hence:
#
#    RTT^c(m=0) = 2 x [o^c(m) + SUM L^c(0, 1)] => o^c(m) = RTT^c(0) / 2
#
def RTT (rtt):
    o = rtt / 2.0
    return o



# Measuring overhead for rendezvous protocol in all channels.
# It sends messages of size 0 using MPI_Ssend, hence
#
#    Ping^c(m=0) = o^c(m) + SUM L^c(0, 1) => o^c(m) = Ping^c(0)
#
def Ping (ping):
    o = ping
    return o



# Measuring transfer time in shared memory, with m < S and t concurrent processes.
# A rank r sends a message to rank r+1 and receives from r-1 using MPI_Sendrecv.
#
#    Ring^0_t(m) = o^0(m) + 2 x L^0(m, t) => L^0(m, t) = [Ring^0_t (m) - o^c(m)] / 2
#
def Ring_SHM (ring, o):
    L = (ring - o) / 2.0
    return L



# Measuring transfer time in shared memory, with m >= S and t concurrent processes.
# A rank r sends a message to rank r+1 and receives from r-1 using MPI_Sendrecv.
#
#    Ring^0_t(m) = o^0(m) + 2 x k x L^0(S, t) =>
#       L^0(m, t) = [Ring^0_t (m) - o^c(m)] / (2 k) + <rest of last segment>
#
# TODO: last segment.
def Ring_SHM_segm (ring, o, S, m):
    k      = m / S
    last_s = 0
    last_L = 0
    L = (ring - o) / (2 * k) + last_L
    return L



# Measuring transfer time in TCP, with t concurrent processes.
# A rank r sends a message to rank r+1 and receives from r-1 using MPI_Sendrecv.
#
#    Ring^TCP_t(m) = 2 x [o^TCP(m) + 2 x L^0(m, t) + L^TCP(m, t)] =>
#       L^TCP(m, t) = [Ring^TCP_t (m) / 2] - o^TCP(m) - 2 x L^0(m, t)
#
def Ring_TCP (ring, o, L0):
    L_tcp = (ring / 2.0) - o - (2 * L0)
    return L_tcp



# Measuring transfer time in channels with RDMA, with t concurrent processes.
# A rank r sends a message to rank r+1 and receives from r-1 using MPI_Sendrecv.
#
#    Ring^RDMA_t(m) = 2 x [o^NET(m) + L^RDMA(m, t)] =>
#       L^RDMA(m, t) = [Ring^TCP_t (m) / 2] - o^NET(m)
#
def Ring_RDMA (ring, o):
    L = (ring / 2.0) - o
    return L


# In[19]:


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


def processTransferTimeSHM_noseg (transfert, overhead):
    return transfert.subtract(overhead, axis=0) / 2.0


def processTransferTimeSHM_seg (transfert, overhead, S):

    k = transfert.index // S
    # TODO: Esto tiene miga.
    # seg = transfert.subtract(overhead, axis=0) / (2.0 * k)

    # BY NOW: no segmented:
    return transfert.subtract(overhead, axis=0) / 2.0


def processOverheadNET_eager (overhead):
    return RTT(overhead)


def processOverheadNET_rndv  (overhead):
    return Ping(overhead)


def processTransferTimeTCP (transfert, overhead, L0):
    transfert = transfert / 2.0
    transfert.subtract(overhead, axis=0)
    transfert.subtract(2 * L0, axis=0)
    return transfert


def processTransferTimeRDMA (transfert, overhead):
    transfert = transfert / 2.0
    transfert.subtract(overhead, axis=0)
    return transfert




def process(channel, config, o_times, tt_times):

    H = config['H']
    P = config['P']
    S = config['S']



    # 4a. Create Pandas in order to manipulate data
    cols = ['o(m)'] + ['T(m, {})'.format(i) for i in range(1, P+1)]

    o_array = np.array([list(o_times.values())]).T  # Rows = m, Cols = o(m)
    t_array = np.array(list(tt_times.values()))     # Rows = m, Cols = T(m,1) ... T(m, P)

    times = pd.DataFrame(np.concatenate((o_array, t_array), axis=1),
                         columns = cols,
                         index   = o_times.keys())

    num_m = o_array.shape[0]



    if channel == "SHM":

        # 1. Overhead
        overhead = times['o(m)']

        # 1a) overhead with eager protocol (m < H)
        overhead.loc[:H-1] = processOverheadSHM_eager(overhead.loc[:H-1])
        # 1b OVerhead with rendezvous protocol (m >= H)
        overhead.loc[H:]   = processOverheadSHM_rndv(overhead.loc[H:])

        times['o(m)'] = overhead


        # 2. Transfer time
        transfert = times.loc[:, times.columns[1:]] # T(m,1) ... T(m,P)

        # 2a. No segmented messages: m < S
        transfert_noseg = times.loc[0:S-1, times.columns[1:]]
        overhead_noseg  = times.loc[ :S-1, 'o(m)']
        transfert_noseg = processTransferTimeSHM_noseg(transfert_noseg, overhead_noseg)
        times.loc[0:S-1, times.columns[1:]] = transfert_noseg

        # 2b. Segmented messages: m >= S
        transfert_seg   = times.loc[S: , times.columns[1:]]
        overhead_seg    = times.loc[S: , 'o(m)']
        transfert_seg   = processTransferTimeSHM_seg(transfert_seg, overhead_seg, S)
        times.loc[S: , times.columns[1:]] = transfert_seg




    if channel == "TCP":

        # 1. Overhead: same as SHM and standard for all channels.
        overhead = times['o(m)']

        # 1a) overhead with eager protocol (m < H)
        overhead.loc[:H-1] = processOverheadNET_eager(overhead.loc[:H-1])
        # 1b OVerhead with rendezvous protocol (m >= H)
        overhead.loc[H:]   = processOverheadNET_rndv(overhead.loc[H:])

        times['o(m)'] = overhead


        # 2. Transfer time
        transfert = times.loc[:, times.columns[1:]] # T(m,1) ... T(m,P)

        # 2a. No segmented messages along all sizes (segmentation is in TCP/IP protocol)
        transfert = times.loc[:, times.columns[1:]]
        overhead  = times.loc[:, 'o(m)']
        
        # TODO: L0 should take a value to invoke:
        transfert = processTransferTimeTCP (transfert, overhead, 0.0)
        times.loc[:, times.columns[1:]] = transfert



    if channel == "IB":

        # 1. Overhead: same as SHM and standard for all channels.
        overhead = times['o(m)']

        # 1a) overhead with eager protocol (m < H)
        overhead.loc[:H-1] = processOverheadNET_eager(overhead.loc[:H-1])
        # 1b OVerhead with rendezvous protocol (m >= H)
        overhead.loc[H:]   = processOverheadNET_rndv(overhead.loc[H:])

        times['o(m)'] = overhead


        # 2. Transfer time
        transfert = times.loc[:, times.columns[1:]] # T(m,1) ... T(m,P)

        # 2a. No segmented messages along all sizes
        transfert = times.loc[:, times.columns[1:]]
        overhead  = times.loc[:, 'o(m)']
        transfert = processTransferTimeRDMA (transfert, overhead)
        times.loc[:, times.columns[1:]] = transfert




    if channel == "ARIES":
        # 1. Overhead: same as SHM and standard for all channels.
        overhead = times['o(m)']

        # 1a) overhead with eager protocol (m < H)
        overhead.loc[:H-1] = processOverheadNET_eager(overhead.loc[:H-1])
        # 1b OVerhead with rendezvous protocol (m >= H)
        overhead.loc[H:]   = processOverheadNET_rndv(overhead.loc[H:])

        times['o(m)'] = overhead


        # 2. Transfer time
        transfert = times.loc[:, times.columns[1:]] # T(m,1) ... T(m,P)

        # 2a. No segmented messages along all sizes
        transfert = times.loc[:, times.columns[1:]]
        overhead  = times.loc[:, 'o(m)']
        transfert = processTransferTimeRDMA (transfert, overhead)
        times.loc[:, times.columns[1:]] = transfert



    cols = ['o(m)'] + ['L(m, {})'.format(i) for i in range(1, P+1)]
    times.set_axis(cols, axis=1, inplace=True)


    return times
