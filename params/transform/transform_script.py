#!/usr/bin/env python
# coding: utf-8

# ### Script for calling transform_params with a set of files:

import sys
import argparse
from transform_params  import execute_transform

# configuration

i_path = "/Users/jarico/Documents/Investigacion/Software/taulop-tool/taulop_develop/parameters/Metropolis/08"
o_path = "/Users/jarico/Documents/Investigacion/Software/taulop-tool/taulop_develop/parameters/Metropolis/08"

channels = ["SHM", "TCP", "IB"]
machines = ["Metropolis"]


# main

if __name__ == "__main__":

    for machine in machines:

        print("Generating for machine: ", machine)

        for channel in channels:

            print("  Communication channel: ", channel)

            # 1. Fill params
            args = dict()

            # 1a. Read file names
            i_prefix = i_path + "/" # + machine + "_"
            i_suffix = "_" + channel + ".txt"

            o_prefix = o_path + "/" 

            args["verbose"]   = False
            args["channel"]   = channel
            args["overhead"]  = i_prefix + "overhead" + i_suffix
            args["transfert"] = i_prefix + "transfertime" + i_suffix
            args["writefile"] = o_prefix + channel + ".txt"
            args["plot"]      = o_prefix + channel + ".png"
            # 1b. Convert to Namespace in ArgParse:
            main_args = argparse.Namespace(**args)

            # 2. Invoke transform parameters
            execute_transform (main_args)
