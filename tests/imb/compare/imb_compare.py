#!/usr/bin/env python
# coding: utf-8

# Compare a IMB measurement of a benchmark to a tauLop estimation


import sys
import argparse

import pandas            as pd
import numpy             as np 
import matplotlib        as mpl
import matplotlib.pyplot as plt



# Get data from a IMB benchmark output file
def getIMBTimes (imb_filename):

    try:
        with open(imb_filename) as f_bench:

            imb_lines = f_bench.readlines()

            f_bench.close()
    except IOError as e:
        print ("ERROR: IMB file not found: ", imb_filename)
        return {}


    cols = ["m", "latency", "bandwidth"]

    num_line = 0
    for line in imb_lines: 
        
        line = line.lstrip()
        if line[0:6] == '#bytes':
            break
            
        num_line += 1
        
            
    t = dict()
    for i in range(num_line + 1, len(imb_lines)):
        
        if imb_lines[i][0] == '#':
            continue
            
        value = imb_lines[i][0:-1]   # Avoid last '\n'
        v = value.split("\t")    # Convert into a list
        
        if (len(v) < 5):
            continue
                    
        # Avoid first element (it is a trailing \t introduced by IMB)
        m   = int(v[1])
        lat = float(v[3]) / 1000000. # IMB returns usec, hence, to secs.
        bw  = float(v[4])
        
        t[m] = [lat, bw]
        
            
    times = pd.DataFrame(t.values(), index = t.keys(), columns = ["Latency", "Bandwidth"])
    return times


# Get data from a Taulop (imb_taulop) generated file with estimations
def getTaulopTimes (taulop_filename):

    try:
        with open(taulop_filename) as f_bench:

            tau_lines = f_bench.readlines()

            f_bench.close()
    except IOError as e:
        print ("ERROR: Taulop file not found: ", taulop_filename)
        return {}


    cols = ["m", "latency", "bandwidth"]
        
    num_line = 0
    for line in tau_lines: 
        
        line = line.lstrip()
        if line[0:6] == '#bytes':
            break
            
        num_line += 1
    
    
    t = dict()
    for i in range(num_line + 1, len(tau_lines)):
        
        if tau_lines[i][0] == '#':
            continue
            
        value = tau_lines[i][0:-1]   # Avoid last '\n'
        v = value.split("\t")    # Convert into a list
                
        if (len(v) < 4):
            continue
                    
        # Avoid first element (it is a trailing \t introduced by IMB)
        m   = int(v[0])
        lat = float(v[2]) / 1000000. # IMB returns usec, hence, to secs.
        bw  = float(v[3])
        
        t[m] = [lat, bw]
        
            
    times = pd.DataFrame(t.values(), index = t.keys(), columns = ["Latency", "Bandwidth"])
    return times


# Plot/Save latency and bandwidth
def plotLatencyAndBandwidth(imb_times, tlop_times, plot_file=None):

    fig, ax = plt.subplots(1, 2, figsize=(16,8))

    x = imb_times.index

    ax[0].plot(x, imb_times['Latency'], label="IMB Measurement")
    ax[0].plot(x, tlop_times['Latency'], label="t-Lop Estimation")
    ax[0].set_title("Latency")
    ax[0].xaxis.set_ticks(x)
    ax[0].set_xticklabels(imb_times.index, rotation=45)
    ax[0].set_xlabel("Size (bytes)")
    ax[0].set_ylabel("Time (secs)")
    ax[0].legend()

    ax[1].plot(x, imb_times['Bandwidth'], label="IMB Measurement")
    ax[1].plot(x, tlop_times['Bandwidth'], label="t-Lop Estimation")
    ax[1].set_title("Bandwidth")
    ax[1].xaxis.set_ticks(x)
    ax[1].set_xticklabels(imb_times.index, rotation=45)
    ax[1].set_xlabel("Size (bytes)")
    ax[1].set_ylabel("MBytes/sec")
    ax[1].legend()

    fig.suptitle(args.label)

    if plot_file:
        plt.savefig(plot_file)
    else:
        plt.show()
       
    return


# Plot/Save proportional and relative errors
def plotError (imb_times, tlop_times, error_file=None):

    # Mean proportional error. According to:
    #   Juan-Antonio Rico-Gallego, Juan-Carlos Díaz-Martín, Alexey L. Lastovetsky.
    #   Extending τ-Lop to model concurrent MPI communications in multicore clusters. 
    #   Future Generation Computer Systems, Volume 61, August 2016, Pages 66-82, 
    #   ISSN 0167-739X, http://dx.doi.org/10.1016/j.future.2016.02.021.
    #
    #  Proportional error P_i = max (R_i, E_i) / min (R_i, E_i)
    #  Mean Proporcional Error: mpe = (SUM_i^n P_i) / n

    mpe = 0.0

    R = imb_times['Latency']
    E = tlop_times['Latency']

    P = np.maximum(R, E) / np.minimum(R, E)
    mpe = np.mean(P)

    # (Standard) Relative Error
    Rel = np.abs(E - R) / R
    mre = np.mean(Rel)

    # Plot figure
    fig, ax = plt.subplots(figsize=(8,8))

    x = imb_times.index

    #ax.set_xscale('log')
    ax.plot(x, P, label="Proportional Error")
    ax.plot(x, Rel, label="Relative Error")
    ax.set_title(args.label)
    ax.xaxis.set_ticks(x)
    ax.set_xticklabels(imb_times.index, rotation=45)
    ax.set_xlabel("Size (bytes)")
    ax.set_ylabel("Error")
    ax.legend()

    if error_file:
        plt.savefig(error_file)
    else:
        plt.show()

    return mpe, mre


# Main function: compare measurements and estimations
def compare (args):
    
    # 0. Configuration
    pd.set_option('display.float_format', '{:.6f}'.format)

    # 1. Get data from IMB file
    imb_times = getIMBTimes(args.imb_file)
    if (args.verbose):
        print("IMB measured times:")
        print(imb_times)

    # 2. Get data from Taulop file
    tau_times = getTaulopTimes(args.taulop_file)
    if (args.verbose):
        print("Taulop estimated times:")
        print(tau_times)

    # 3. Comparison of times:
    plotLatencyAndBandwidth(imb_times, tau_times, args.plot_file)

    # 4. Error
    mpe, mre = plotError(imb_times, tau_times, args.error_file)

    return mpe, mre
	


# main

if __name__ == "__main__":

    # 1. Parse arguments
    parser = argparse.ArgumentParser(
                    prog        ='imb_compare.py',
                    description ='Comparison of IMB actual measurements and taulop estimations.',
                    epilog      = "--")

    parser.add_argument('-v', '--verbose',
                        action = 'store_true', help = "Verbose mode.")                 # on/off verbose mode
    
    parser.add_argument('-t', '--taulop_file', default="taulop.txt",
                        help = "Taulop estimations of a benchmark (default <taulop.txt>).")
                        
    parser.add_argument('-i', '--imb_file', default="imb.txt",
                        help = "IMB actual measurements of a benchmark (default <imb.txt>)")
                        
    parser.add_argument('-e', '--error_file', default=None,
                        help = "Write an error text file (default: no error file).")
                        
    parser.add_argument('-p', '--plot_file', default=None,
                        help = "Output plot png file (default: no png file).")          # Plot PNG output file

    parser.add_argument('-l', '--label', default="Benchmark",
                        help = "Title of the plots.")         

    args = parser.parse_args()


    # 2. Execute transform
    mpe, mre = compare(args)   
    if (args.verbose): 
        print("Mean Proportional Error: {0:6.3f}".format(mpe))
        print("Mean Relative Error:     {0:6.3f}".format(mre))




