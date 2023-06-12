#!/usr/bin/env python
# coding: utf-8


import pandas            as pd
import numpy             as np
import matplotlib        as mpl
import matplotlib.pyplot as plt



def getOverheadMapping(lines):

    mapping = dict()
    P = 0

    for i in range(0, len(lines)):

        value = lines[i][1:-1]  # Avoid first character (#) and last "\n"

        v = value.split("\t")

        if v[0] == "P":
            P = int(v[1])
            continue

        if v[0] == "rank":
            continue

        if i > P+1:
            break

        rank = int(v[0])
        node = v[1]

        mapping[rank] = node

    return mapping



def getValue (value):

    try:
        return int(value)
    except ValueError:
        None

    try:
        return float(value)
    except ValueError:
        None

    return str(value)




def getOverheadConfig(lines):

    config = dict()
    #P = config["P"]

    sizes = 0
    for i in range(0, len(lines)):     # Starting after mapping lines

        value = lines[i][0:-1]         # Avoid last "\n"

        v = value.split("\t")

        sizes += 1

        if v[0] == "#P":
            config["P"] = int(v[1])
            continue

        if (v[0] == "#"):
            break

    for i in range(sizes, len(lines)): # Starting after mapping lines

        value = lines[i][1:-1]       # Avoid first character (#) and last "\n"

        v = value.split("\t")

        if v[0] == "size":
            break

        # Read key and value:
        key   = v[0]
        value = getValue(v[1])       # Depends on the type of the value

        # Set entry
        config[key] = value

    return config





def getOverheadTimes(lines):

    times = dict()

    sizes = 0
    for i in range(0, len(lines)):     # Starting after mapping lines

        value = lines[i][0:-1]         # Avoid last "\n"

        v = value.split("\t")

        sizes += 1

        if (v[0] == "#size"):
            break


    for i in range(sizes, len(lines)): # Starting after mapping lines

        value = lines[i][0:-1]         # Avoid last "\n"
        v = value.split("\t")

        # Read key and value:
        key   = int(v[0])
        value = float(v[1])            # Depends on the type of the value

        # Set entry
        times[key] = value

    return times




def getTransferTimes(lines, max_tau):

    times = dict()

    sizes = 0
    for i in range(0, len(lines)):     # Starting after mapping lines

        value = lines[i][0:-1]         # Avoid last "\n"

        v = value.split("\t")

        sizes += 1

        if (v[0] == "#size"):
            break


    for i in range(sizes, len(lines)): # Starting after mapping lines

        value = lines[i][0:-1]         # Avoid last "\n"
        v = value.split("\t")

        # Read key:
        key   = int(v[0])

        # Read values
        values = []
        for t in range (0, max_tau):
            values.append(float(v[t+1]))

        # Set entry
        times[key] = values

    return times



def saveOutputFiles (times, args):

    cols = ['m'] + list(times.columns)

    num_m = times.index.shape[0]
    P     = times.columns.shape[0] - 1

    filename = args.writefile

    with open(filename, 'w') as f:
        f.write('#m\t'   + str(num_m) + "\n")
        f.write('#tau\t' + str(P)     + "\n")
        f.write("#" + '\t'.join(cols) + "\n")

        for m, t in times.iterrows():
            t_str = '\t'.join(str("{:.12f}".format(i)) for i in t.values)
            f.write(str(m) + '\t' + t_str + "\n")

    f.close()
    return




def generateChannel(args):

    overhead_name  = args.overhead
    transfert_name = args.transfert

    # 1. Read lines of the overhead_XXX and transfertime_XXX.txt files:
    with open(overhead_name) as f_overhead:
        o_lines = f_overhead.readlines()
        f_overhead.close()

    with open(transfert_name) as f_transfert:
        tt_lines = f_transfert.readlines()
        f_transfert.close()


    # 2. Get data from file "overhead_xxx.txt"

    # 2a. Get data -> Mapping
    mapping = getOverheadMapping(o_lines)

    # 2b. Get data -> Config
    config = getOverheadConfig(o_lines)
    P   = config["P"]
    tau = config["tau"]

    # 2c. Get data -> times
    o_times = getOverheadTimes(o_lines)


    # 3. Get data from file "transfertime_xxx.txt"

    # 3a & 3b: assuming same config and mapping than in overhead.

    # 3c. Get data -> times
    tt_times = getTransferTimes(tt_lines, tau)


    # 4. Create a Pandas DataFrame with all the data read from MPIBLib output files
    #
    #         o(m)   T(m,1)   T(m,2)   ...   T(m,tau)
    #     1    -       -        -              -
    #    64    -       -        -              -
    #   128    -       -        -              -
    #   ...
    #   MAX    -       -        -              -
    #
    #   Values are those read from MPIBLib output files. Next step is to transform
    #    values into actual taulop parameters.

    cols = ['o(m)'] + ['T(m, {})'.format(i) for i in range(1, tau+1)]

    o_array = np.array([list(o_times.values())]).T  # Rows = m, Cols = o(m)
    t_array = np.array(list(tt_times.values()))     # Rows = m, Cols = T(m,1) ... T(m, tau)

    mpiblib_times = pd.DataFrame(np.concatenate((o_array, t_array), axis=1),
                                 columns = cols,
                                 index   = o_times.keys())

    #num_m = o_array.shape[0]

    return mapping, config, mpiblib_times #o_times, tt_times
