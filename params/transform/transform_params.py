#!/usr/bin/env python
# coding: utf-8

# ### Transform mpiblib output parameter measurements to taulop-Lib parameter format.

import sys
#import getopt
import argparse

import pandas            as pd
import numpy             as np
import matplotlib        as mpl
import matplotlib.pyplot as plt

from read_params         import *
from process_params      import *
from plot_params         import *


# Print some values
def print_data(msg, data):
    print(msg)
    print(data)


# Execute transform of parameters
def execute_transform (args):

    # 1. Generate dictionaries with the information from the MPIBLib file outputs.
    mapping, config, overhead_times, transfert_times = generateChannel(args)

    # 2. Process previous data to get a Pandas dataframe with the times (taulop parameter values)
    #     The times from MPIBLib are processed according to their communication channel.
    taulop_times = processChannel(args.channel, config, overhead_times, transfert_times)

    # 3. Print values
    if args.verbose:
        print_data("Configuration data: ", config)
        print_data("Mapping data: ", mapping)
        print_data("Overhead times: ", overhead_times)
        print_data("Transfer times: ", transfert_times)
        print_data("Taulop times: ", taulop_times)

    # 4. Generate plot (png) file
    if args.plot:
        plot_params(taulop_times, args)

    # 5. Write output file with parameters of a channel ready to be used by taulop-library.
    if args.writefile:
        saveOutputFiles(taulop_times, args)

    return





# main

if __name__ == "__main__":

    # 1. Parse arguments
    parser = argparse.ArgumentParser(
                    prog        ='transform_params.py',
                    description ='Transforms MPIBLib output files into taulop-library parameters file format.',
                    epilog      = "--")

    parser.add_argument('channel',             help = "Communication Channel.")  # Positional argument
    parser.add_argument('-v', '--verbose',
                        action = 'store_true', help = "Verbose mode.")                 # on/off verbose mode
    parser.add_argument('-o', '--overhead', default="overhead.txt",
                        help = "Overhead input file (default <overhead.txt>).")        # overhead input file
    parser.add_argument('-t', '--transfert', default="transfert.txt",
                        help = "Transfer tiimie input file (default <transfert.txt>)") # transfer time input file
    parser.add_argument('-w', '--writefile', default="parameters.txt",
                        help = "Parameters output file (default <parameters.txt>).")   # Parameters output file
    parser.add_argument('-p', '--plot', default=None,
                        help = "Output plot png file (default: no png file.")          # Plot PNG output file

    args = parser.parse_args()


    # 2. Execute transform
    execute_transform(args)
