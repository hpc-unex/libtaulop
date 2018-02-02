//
//  matrix_config.h
//  TauLopCost
//
//  Created by jarico on 22/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef matrix_config_h
#define matrix_config_h

#include "config.h"
#include <string>
using namespace std;

// BUG (16-10-05):
//    It is solved in the code (taulop_sequence file, substract() method)
//    but it is sorrounded by this macro for testing.
//    Although the bug fixed design & code seems correct, I am not completely sure.
//    (change BUG_T = 1 for activate new code).
#define BUG_T  0


#define TLOP_DEBUG  0

const int NUM_PROC = 1024; // Max. number of processes in the Matrix
const int COL_SIZE = 128;  // Max. number of rows in column
const int ROW_SIZE = 128;  // Max. number of processes in a row


// Synthetic matrix generation. Change the values ...
const int synt_N       = 256;
const int synt_KB      = 64;
const int synt_P       = 8;
const int synt_M       = 4;
const int synt_rowNr   = 4;
const int synt_colNr   = 4;
const int synt_ratio   = 3;
const string synt_file = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/squares/part.2dist";
const string synt_plot = "/Users/jarico/Google Drive/code_tau_lop/taulop_code/squares/part_2dist.gpl";


const int DEFAULT           = 0;
const int PT2PT             = 1;
const int RING              = 2;
const int BCAST_BIN_MPICH   = 3;
const int BCAST_BIN_OPENMPI = 4;

/* column_algorithm refers to the algorithm used to send/receive the PBR (vertical comm).
   row_algorithm referes to the algorithm used to send/receive the PBC (horizontal communication).
 */
//const int column_algorithm = PT2PT;
extern int row_algorithm;
extern int column_algorithm;



#endif /* matrix_config_h */


