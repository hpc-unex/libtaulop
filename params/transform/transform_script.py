#!/usr/bin/env python
# coding: utf-8

# ### Script for calling transform_params with a set of files in a folder:

import sys
import argparse
from transform_params  import execute_transform

# Configuration parameters: communication channels to generate and machine
channels = ["SHM", "TCP", "IB"]
machines = ["Metropolis"]


# main

if __name__ == "__main__":

    # 1. Parse arguments
    parser = argparse.ArgumentParser(
                    prog        ='transform_script.py',
                    description ='Transforms MPIBLib output files [SHM, TCP, IB] into taulop-library parameters file format. \nChange configuration parameters (communication channel and machine) in the script code.',
                    epilog      = "--")

    parser.add_argument('-v', '--verbose',
                        action = 'store_true', help = "Verbose mode.")                 # on/off verbose mode
    parser.add_argument('-i', '--input_path', default=".",
                        help = "Input path for mpiblib files (default <.>).")          # input path files
    parser.add_argument('-o', '--output_path', default=".",
                        help = "Output path for generated files (default <.>).")       # output path files

    margs = parser.parse_args()

    for machine in machines:

        if margs.verbose:
            print("Generating for machine: ", machine)

        for channel in channels:

            if margs.verbose:
                print("  Communication channel: ", channel)

            # 1. Fill params
            args = dict()

            # 1a. Read file names
            i_prefix = margs.input_path + "/" # + machine + "_"
            i_suffix = "_" + channel + ".txt"

            o_prefix = margs.output_path + "/"

            args["verbose"]   = margs.verbose
            args["channel"]   = channel
            args["overhead"]  = i_prefix + "overhead" + i_suffix
            args["transfert"] = i_prefix + "transfertime" + i_suffix
            args["writefile"] = o_prefix + channel + ".txt"
            args["plot"]      = o_prefix + channel + ".png"

            # 1b. Convert to Namespace in ArgParse:
            main_args = argparse.Namespace(**args)

            # 2. Invoke transform parameters
            execute_transform (main_args)
