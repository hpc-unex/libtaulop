#!/usr/bin/env python
# coding: utf-8

# Compare a IMB measurement of a benchmark to a tauLop estimation


import sys
import argparse

import pandas            as pd
import numpy             as np 
import matplotlib        as mpl
import matplotlib.pyplot as plt

from imb_compare  import compare



# Write here all benchmarks to compare (by hand)
def getBenchmarkList ():

    bench_list = list()

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "pt2pt",
         "benchmark":  "PingPong",
         "algorithm":  "Lineal",
         "channel":    "SHM",
         "P":          "2", 
         "M":          "1", 
         "Q":          "2",
         "mapping":    "Sequential",
         "label":      "Metropolis PingPong P=2 M=1 Q=2 Shared Memory"
        }
    )

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "pt2pt",
         "benchmark":  "PingPong",
         "algorithm":  "Lineal",
         "channel":    "TCP",
         "P":          "2", 
         "M":          "2", 
         "Q":          "1",
         "mapping":    "Sequential",
         "label":      "Metropolis PingPong P=2 M=2 Q=1 Ethernet/TCP"
        }
    )

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "pt2pt",
         "benchmark":  "PingPong",
         "algorithm":  "Lineal",
         "channel":    "IB",
         "P":          "2", 
         "M":          "2", 
         "Q":          "1",
         "mapping":    "Sequential",
         "label":      "Metropolis PingPong P=2 M=2 Q=1 Infiniband"
        }
    )

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "collective",
         "benchmark":  "Broadcast",
         "algorithm":  "BinomialOMPI",
         "channel":    "IB",
         "P":          "8", 
         "M":          "4", 
         "Q":          "2",
         "mapping":    "RoundRobin",
         "label":      "Metropolis Broadcast Binomial Open MPI P=8 M=4 Q=2 Infiniband RoundRobin"
        }
    )

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "collective",
         "benchmark":  "Broadcast",
         "algorithm":  "BinomialOMPI",
         "channel":    "IB",
         "P":          "16", 
         "M":          "4", 
         "Q":          "4",
         "mapping":    "RoundRobin",
         "label":      "Metropolis Broadcast Binomial Open MPI P=16 M=4 Q=4 Infiniband RoundRobin"
        }
    )

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "collective",
         "benchmark":  "Broadcast",
         "algorithm":  "BinomialOMPI",
         "channel":    "IB",
         "P":          "16", 
         "M":          "2", 
         "Q":          "8",
         "mapping":    "Sequential",
         "label":      "Metropolis Broadcast Binomial Open MPI P=16 M=2 Q=8 Infiniband Sequential"
        }
    )

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "collective",
         "benchmark":  "Broadcast",
         "algorithm":  "BinomialOMPI",
         "channel":    "IB",
         "P":          "32", 
         "M":          "4", 
         "Q":          "8",
         "mapping":    "RoundRobin",
         "label":      "Metropolis Broadcast Binomial Open MPI P=32 M=4 Q=8 Infiniband RoundRobin"
        }
    )

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "collective",
         "benchmark":  "Broadcast",
         "algorithm":  "BinomialOMPI",
         "channel":    "IB",
         "P":          "32", 
         "M":          "4", 
         "Q":          "8",
         "mapping":    "Sequential",
         "label":      "Metropolis Broadcast Binomial Open MPI P=32 M=4 Q=8 Infiniband Sequential"
        }
    )

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "collective",
         "benchmark":  "Broadcast",
         "algorithm":  "BinomialOMPI",
         "channel":    "SHM",
         "P":          "8", 
         "M":          "1", 
         "Q":          "8",
         "mapping":    "Sequential",
         "label":      "Metropolis Broadcast Binomial Open MPI P=8 M=1 Q=8 Shared Memory"
        }
    )

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "collective",
         "benchmark":  "Broadcast",
         "algorithm":  "BinomialOMPI",
         "channel":    "TCP",
         "P":          "8", 
         "M":          "4", 
         "Q":          "2",
         "mapping":    "RoundRobin",
         "label":      "Metropolis Broadcast Binomial Open MPI P=8 M=4 Q=2 Ethernet/TCP RoundRobin"
        }
    )

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "collective",
         "benchmark":  "Broadcast",
         "algorithm":  "BinomialOMPI",
         "channel":    "TCP",
         "P":          "16", 
         "M":          "4", 
         "Q":          "4",
         "mapping":    "RoundRobin",
         "label":      "Metropolis Broadcast Binomial Open MPI P=16 M=4 Q=4 Ethernet/TCP RoundRobin"
        }
    )

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "collective",
         "benchmark":  "Broadcast",
         "algorithm":  "BinomialOMPI",
         "channel":    "TCP",
         "P":          "16", 
         "M":          "2", 
         "Q":          "8",
         "mapping":    "Sequential",
         "label":      "Metropolis Broadcast Binomial Open MPI P=16 M=2 Q=8 Ethernet/TCP Sequential"
        }
    )

    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "collective",
         "benchmark":  "Broadcast",
         "algorithm":  "BinomialOMPI",
         "channel":    "TCP",
         "P":          "32", 
         "M":          "4", 
         "Q":          "8",
         "mapping":    "RoundRobin",
         "label":      "Metropolis Broadcast Binomial Open MPI P=32 M=4 Q=8 Ethernet/TCP RoundRobin"
        }
    )


    bench_list.append(
        {"machine":    "metropolis",   
         "type":       "collective",
         "benchmark":  "Broadcast",
         "algorithm":  "BinomialOMPI",
         "channel":    "TCP",
         "P":          "32", 
         "M":          "4", 
         "Q":          "8",
         "mapping":    "Sequential",
         "label":      "Metropolis Broadcast Binomial Open MPI P=32 M=4 Q=8 Ethernet/TCP Sequential"
        }
    )

    return bench_list



def plotErrors (errors, filename = None):
    
    fig, ax = plt.subplots(1, 2, figsize=(16,8), sharey=True)

    # errors = errors.sort_values(by=['Tiempo minimo'])
    errors = errors.sort_index()    

    x = errors.index 
    mpe_v = errors["Proportional Error"] 
    mre_v = errors["Relative Error"] 

    y_pos = np.arange(len(x))

    ax[0].barh(y_pos, mpe_v, align='center')
    ax[0].set_yticks(y_pos, labels=x)
    ax[0].invert_yaxis()  # labels read top-to-bottom
    ax[0].set_ylabel("Benchmark")
    ax[0].set_title("Proportional Error")
    ax[0].grid(which='major', alpha=0.3)
    ax[0].axvline(x = 1, color = 'b')

    ax[1].barh(y_pos, mre_v, align='center', color='red')
    ax[1].set_yticks(y_pos, labels=x)
    ax[1].invert_yaxis()  # labels read top-to-bottom
    ax[1].set_title("Relative Error")
    ax[1].grid(which='major', alpha=0.3)

    fig.set_figwidth(12)
    fig.subplots_adjust(left=0.5)

    if filename != None:
        plt.savefig(filename)
        plt.close()
    else:
        plt.show()
       
    return




# main

if __name__ == "__main__":

    # 1. Parse arguments
    parser = argparse.ArgumentParser(
                    prog        ='imb_compare_script.py',
                    description ='Multiple Comparisons of IMB actual measurements and taulop estimations.',
                    epilog      = "--")

    parser.add_argument('-v', '--verbose',
                        action = 'store_true', help = "Verbose mode.")                 # on/off verbose mode
    
    parser.add_argument('-i', '--input_imb_path', default=".",
                        help = "Input path for IMB measurement files (default <.>).")
                        
    parser.add_argument('-t', '--input_taulop_path', default=".",
                        help = "Input path for Taulop estimation files (default <.>).")
                        
    parser.add_argument('-o', '--output_path', default=".",
                        help = "Output path for resulting PNG files (default <.>)")

    parser.add_argument('-w', '--write_file', default=None,
                        help = "Output summary of errors in a PNG file (default: None)")

    args = parser.parse_args()


    # 1. Fill paths
    imb_prefix    = args.input_imb_path + "/"      # Input IMB files
    taulop_prefix = args.input_taulop_path + "/"   # Input taulop files
    o_prefix      = args.output_path + "/"         # Error and plot output files

    # 2. Benchmarks to compare
    bench_list = getBenchmarkList ()


    error_list = list()
    labels = list()

    for bench in bench_list:
            
        # 2a. Files composed from benchmarks properties. Format: machine_[taulop]_coll_alg_channel_P_M_Q_mapping.txt
        imb_file     = imb_prefix    + bench["machine"] + "_"        + bench["benchmark"] + "_" + bench["algorithm"] + "_" + bench["channel"] + "_" + bench["P"] + "_" + bench["M"] + "_" + bench["Q"] + "_" + bench["mapping"] + ".txt"
        taulop_file  = taulop_prefix + bench["machine"] + "_taulop_" + bench["benchmark"] + "_" + bench["algorithm"] + "_" + bench["channel"] + "_" + bench["P"] + "_" + bench["M"] + "_" + bench["Q"] + "_" + bench["mapping"] + ".txt"
        results_file = o_prefix      + bench["machine"] + "_"        + bench["benchmark"] + "_" + bench["algorithm"] + "_" + bench["channel"] + "_" + bench["P"] + "_" + bench["M"] + "_" + bench["Q"] + "_" + bench["mapping"] + ".png"
        error_file   = o_prefix      + bench["machine"] + "_"        + bench["benchmark"] + "_" + bench["algorithm"] + "_" + bench["channel"] + "_" + bench["P"] + "_" + bench["M"] + "_" + bench["Q"] + "_" + bench["mapping"] + "_error.png"

        main_args = {"verbose":      False, 
                     "imb_file":     imb_file,
                     "taulop_file":  taulop_file,
                     "error_file":   error_file,
                     "plot_file":    results_file,
                     "label":        bench["label"]
                    }  

        # 2b. Convert to Namespace in ArgParse:
        main_args = argparse.Namespace(**main_args)

        # 2c. Compare
        mpe, mre = compare(main_args)
        if (args.verbose):
            print(bench["label"])
            print("Mean Proportional Error: {0:6.3f}".format(mpe))
            print("Mean Relative Error:     {0:6.3f}".format(mre))

        # 2d. Aggregate errors
        labels.append(bench["label"])
        error_list.append([mpe, mre])

    # 3. Plot errors
    errors = pd.DataFrame(error_list, index = labels, columns = ["Proportional Error", "Relative Error"])
    plotErrors(errors, args.write_file)




