#!/usr/bin/env python
# coding: utf-8


import pandas              as pd
import numpy               as np

from process_params_SHM    import processChannelSHM
from process_params_TCP    import processChannelTCP
from process_params_IB     import processChannelIB
from process_params_ARIES  import processChannelARIES



def processChannel (channel, config, overhead_times, transfert_times):

    if channel == "SHM":
        taulop_times = processChannelSHM   (config, overhead_times, transfert_times)

    if channel == "TCP":
        taulop_times = processChannelTCP   (config, overhead_times, transfert_times)

    if channel == "IB":
        taulop_times = processChannelIB    (config, overhead_times, transfert_times)

    if channel == "ARIES":
        taulop_times = processChannelARIES (config, overhead_times, transfert_times)

    return taulop_times
