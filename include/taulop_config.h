//
//  matrix_config.h
//  TauLopCost
//
//  Created by jarico on 22/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef config_h
#define config_h

#include <string>
using namespace std;

// BUG (16-10-05):
//    It is solved in the code (taulop_sequence file, substract() method)
//    but it is sorrounded by this macro for testing.
//    Although the bug fixed design & code seems correct, I am not completely sure.
//    (change BUG_T = 1 for activate new code).
#define BUG_T  0


#define TLOP_DEBUG  0


const int DEFAULT_CHN_NR = 2; // IMP: More than 2 channels is not supported by now


// TBT
// There is a *first attempt* to erase this behavior, loading the parameter from a specific
// subfolder (IB/ or TCP/) of the params_folder.
// The specific folder is set in TauLopParams::setInstance(string network).
// Mybe the setInstance will need to be part of a more generic loadConfig function to initialize
//   the library at the beginning of an user program.

//const int TCP = 0;
//const int IB  = 1;
//const int NETWORK = IB;

// t-Lop code kernel will search for the parameter files under: ./params/<param_folder>/
//   The value is 0 for TCP and 1 for IB.
const string params_folder  = "/Users/jarico/Development/taulop_toolbox/params/ciemat/";
const string IB_NET  = "IB";
const string TCP_NET = "TCP";

//#define PARAMS_FOLDER  1
//#if PARAMS_FOLDER == 0
//const string params_folder  = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/params/";
//#elif PARAMS_FOLDER == 1
//const string params_folder  = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/params/";
//#else
////#error "ERROR: must specify a folder to search for the parameter files."
//const string params_folder  = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/params/";
//#endif

const int DATATYPE_SIZE  = sizeof(double);  // Type of each element in a block


// Default mappings constants
const int MAPPING_UNDEF = 0; // Undefined (random)
const int MAPPING_SEQ   = 1; // Sequential mapping  (p / Q, Q = #cores/node)
const int MAPPING_RR    = 2; // Round Robin mapping (p % Q, Q = #cores/node)


// Default rank and node for a process
const int RANK_UNDEFINED = -1;
const int NODE_UNDEFINED = -1;


#endif /* config_h */


