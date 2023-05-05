#!/usr/bin/env python
# coding: utf-8

# ### Script for calling transform_params with a set of files:

import sys
import argparse
from transform_params  import execute_transform

# configuration

path = "/Users/jarico/Documents/Investigacion/Software/taulop-tool/taulop_develop/parameters/Metropolis/02"
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
            prefix = path + "/" # + machine + "_"
            suffix = "_" + channel + ".txt"

            args["verbose"]   = False
            args["channel"]   = channel
            args["overhead"]  = prefix + "overhead" + suffix
            args["transfert"] = prefix + "transfertime" + suffix
            args["writefile"] = channel + ".txt"
            args["plot"]      = channel + ".png"
            # 1b. Convert to Namespace in ArgParse:
            main_args = argparse.Namespace(**args)

            # 2. Invoke transform parameters
            execute_transform (main_args)