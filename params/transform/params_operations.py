#!/usr/bin/env python
# coding: utf-8


import pandas            as pd
import numpy             as np



# Operations implemented to compute parameters in different machines, channels, protocols, ...
# Defined in
#   Juan-Antonio Rico-Gallego, Juan-Carlos Díaz-Martín, Alexey L. Lastovetsky,
#   "Extending τ-Lop to model concurrent MPI communications in multicore clusters",
#   Future Generation Computer Systems, Volume 61, 2016, Pages 66-82, ISSN 0167-739X,
#   https://doi.org/10.1016/j.future.2016.02.021.
#
# Furthermore, some operations are added, as modifications to the previous paper reference:
#   PingPong.


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
    L = ring.subtract(o, axis=0) / 2.0
    return L


# Measuring transfer time in shared memory, with m < S and t concurrent processes.
# This operation is applied to T(m,1) usually, in which tau=1 case is covered by P=2 processes.
# A rank r sends a message to rank r+1 and receives from r+1 using MPI_Send and MPI_Recv.
#
#    PingPong^0_t(m) = 2 x [o^0(m) + 2 x L^0(m, t)] => L^0(m, t) = [(PingPong^0_t (m) / 2.0) - o^c(m)] / 2.0
#
def PingPong_SHM (pong, o):
    #pong.divide(pong, 2.0)
    tmp = pong / 2
    tmp.subtract(o, axis=0)
    L = tmp / 2.0
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


