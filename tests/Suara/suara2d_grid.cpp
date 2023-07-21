
//
//  suara2d_grid.cpp
//  Find optimal SUARA2D grid and algorithm numerically
//
//  Created by jarico on 07/07/23.
//  Copyright © 2023 jarico. All rights reserved.
//


#include "taulop_kernel.hpp"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <unistd.h>
#include <getopt.h>
#include <string>
#include <vector>
#include <fstream>
#include <list>
#include <vector>
#include <map>
#include <limits>
using namespace std;


/* Alreduce algorithms */
extern double allreduce_linear_ompi       (int P, int Q, int M, int m, int ms, Map mapping, OpType op);
extern double allreduce_nonoverlap_ompi   (int P, int Q, int M, int m, int ms, Map mapping, OpType op);
extern double allreduce_rda_ompi          (int P, int Q, int M, int m, int ms, Map mapping, OpType op);
extern double allreduce_ring_ompi         (int P, int Q, int M, int m, int ms, Map mapping, OpType op);
extern double allreduce_ringsegm_ompi     (int P, int Q, int M, int m, int ms, Map mapping, OpType op);
extern double allreduce_rabenseifner_ompi (int P, int Q, int M, int m, int ms, Map mapping, OpType op);


/* taulop mapping types */
static string mapping_s  [5] = {"Default", "Sequential", "RoundRobin", "Random", "User"};


/* taulop operation strings */
static string operation_s [] = {"default", "max", "min", "sum", "prod", "land", "band", "lor", "bor", "lxor", "bxor", "maxloc", "minloc"};


/* Algorithms in allreduce collective */
const int NUM_ALGS = 6;
static string algorithms [NUM_ALGS] = {"lin", "nono", "rd", "rnos", "rs", "rab"};


/* Channels */
static string channels_s[4] = { "SHM", "TCP", "IB", "ARIES"};


/* Pointers to functions for algorithms */
double (*fxn_algs [6]) (int P, int Q, int M, int m, int ms, Map mapping, OpType op) = {
   allreduce_linear_ompi,
   allreduce_nonoverlap_ompi,
   allreduce_rda_ompi,
   allreduce_ring_ompi,
   allreduce_ringsegm_ompi,
   allreduce_rabenseifner_ompi
};


/* 2D grid dimensions */
struct grid_2d_t {
   int x;
   int y;
};


/* Return values from find optimals */
struct Optimal_t {
   int     Pr;       /* Number of process in rows              */
   int     Pc;       /* Number of process in columns           */
   int     ar;       /* Algorithm to execute in rows           */
   int     ac;       /* Algorithm to execute in columns        */
   string  ar_name;  /* Name of the rows algorithm             */
   string  ac_name;  /* Name of the columns algorithm          */
   double  t;        /* Total estimated time for the algorithm */
};


/* Some global variables */
int  verbose = 0;
bool help    = false;



/**************************************************
         helper functions
 **************************************************/

void generate_2Ddims_combinations (int P, list<grid_2d_t> &dims_arrangements) {

   for (int x = 1; x <= P; x++) {
      
      if (P % x == 0) {
         int y = int (P / x);
         
         if (x * y == P) {
            dims_arrangements.push_back({x, y});
         }
      }
   }
      
}


void generate_procs_to_test (list<grid_2d_t> &dims_combination, list<int> &procs_to_test) {
   
   for (const auto& d : dims_combination) {
      auto it = std::find(procs_to_test.begin(), procs_to_test.end(), d.x);
      
      if (it == procs_to_test.end()) { /* Not yet in the list. Only need to test d.x */
         procs_to_test.push_back(d.x);
      }
      
   }
}


double exec_algorithm (int a, int P, int Q, int M, int m, int ms, Map mapping, OpType op) {
   
   double t = 0.0;

   t = fxn_algs[a] (P, Q, M, m, ms, mapping, op);
   
   return t;
}


/**************************************************
        SUARA2D_find_optimals_numerically
 **************************************************/

/*
    The function works as follows:
 
 1. Find all possible arrangements of processes in a 2D grid (dims_arrangements).
 2. From the previous possible arrangements, obtain the number of processes we have to
    execute the allreduce algorithms on (procs_to_test).
 3. Execute all the allreduce algorithms for all the number of processes, and save the estimated
    times in a matrix Axp (times_grid).
 4. For each number of processes, take the minimum time and algorithm (t_algs_map).
 5. Check all possible combinations os processes in the grid (dims_arrangements) to
    obtain the minimum (PrxPc) (ar,ac) and t.
 
 The cost of the function is in the order: O(A x P),
     with A the number of algorithms to evaluate and P the number of processes.
     Actually, is clearly lower because the number of processes to test is quite lower than P.
 
 Example: P = 12
 
 dims_arrangements = { (1,12), (2,6), (3,4), (4,3), (6,2), (12,1) }
 procs_to_test = { 1, 2, 3, 4, 6, 12 }
 times_grid:
               lin    nono   rd     rnos   rs     rab
         1:    0.0    0.0    0.0    0.0    0.0    0.0
         2:    0.12   0.08   0.07   0.08   0.12   0.08
         3:    0.15   0.10   0.09   0.11   0.12   0.10
         4:    0.21   0.12   0.14   0.12   0.13   0.15
         6:    0.28   0.19   0.17   0.16   0.18   0.20
        12:    0.45   0.29   0.24   0.27   0.25   0.23
 
 t_algs_map (t, alg, a, p):
    1: { 0.0,   -   }
    2: { 0.07, rd   }
    3: { 0.09, rd   }
    4: { 0.12, nono }
    6: { 0.16, rnos }
   12: { 0.23, rab  }
 
 Times per arrangements: (t, PrxPc, ar,ac)
   (1,12):  0.0  + 0.23 = 0.23  (   -, rab)
   (2,6):   0.07 + 0.16 = 0.23  (  rd, rnos)
   (3,4):   0.09 + 0.12 = 0.21  (  rd, nono)
   (4,3):   0.12 + 0.09 = 0.21  (nono, rd)
   (6,2):   0.16 + 0.07 = 0.23  (rnos, rd)
   (12,1):  0.23 + 0.0  = 0.23  ( rab, - )
 
 Minimum cost: (3 x 4) (rd, nono) 0.21
 
 */

Optimal_t SUARA2D_find_optimals_numerically (int P, int m, int ms, string algorithms[] /* , H */, int Q, int M, Map mapping, OpType op) {
      
   /* 1. Generate grid2D posible dimensions arrangements */
   list<grid_2d_t> dims_arrangements;
   generate_2Ddims_combinations(P, dims_arrangements);
   
   /* 2. Generate a list of number of processes to test */
   list<int> procs_to_test;
   generate_procs_to_test(dims_arrangements, procs_to_test);
   
   if (verbose) {
      cout << "Posible 2D grid dimensions arrangements: " << endl;
      for (const auto& d : dims_arrangements) {
         cout << "(" << d.x << "," << d.y << ") ";
      }
      cout << endl;
      
      cout << "Processes to test (" << procs_to_test.size() << "): " << endl;
      for (const auto& p : procs_to_test) {
         cout << p << " ";
      }
      cout << endl;
   }
   
   int procs_to_test_nr = procs_to_test.size();
      
   
   /* 3. Execute estimations for all cobinations of algorithm and process number */
   
   /* 3.1. Create a matrix of times:  (procs_to_test_nr x NUM_ALGS) */
	double **times_grid = new double * [procs_to_test_nr];
   for (int i = 0; i < procs_to_test_nr; i++) {
      times_grid[i] = new double [NUM_ALGS];
   }
   
   
   /* 3.2. Invoke algorithms for every process number */
   for (int a = 0; a < NUM_ALGS; a++) {
      
      int i = 0;
      for (const auto& p : procs_to_test) {
         
         double t = exec_algorithm (a, p, Q, M, m, ms, mapping, op);
         if (t == 0.0 && p != 1) t = numeric_limits<double>::max();
         times_grid[i][a] = t;
         i++;
         
      }
      
   }
   
   /* 3.3. Print times */
   if (verbose) {
      int i = 0;
      
      cout << "   \t   lin    \t   nono   \t    rd    \t   rnos   \t    rs    \t   rab" << endl;
      for (const auto& p : procs_to_test) {
                  
         cout << p;
         for (int j = 0; j < NUM_ALGS; j++) {
            cout << "\t " << std::fixed << std::setprecision(12) << times_grid[i][j];
         }
         cout << endl;
         i++;
      }
   }

   
   /* 4. Minimum times and algorithm for each process number */
   struct t_algs {
      double t;
      int    a;
   };
   map<int, t_algs> t_algs_map;

   int i = 0;
   for (const auto& p : procs_to_test) {
      
      double t_min = times_grid[i][0]; /* Assume the first element is the minimum */
      int    a_min = 0;
      
      for (int a = 1; a < NUM_ALGS; a++) {
         if (times_grid[i][a] < t_min) {
            t_min = times_grid[i][a];
            a_min = a;
         }
      }
      i++;
      
      /* (t_min, a_min, p_r) */
      t_algs_map[p] = {t_min, a_min};
      
   }
   
   if (verbose) {
      for (const auto& pair : t_algs_map) {
         cout << "p = " << pair.first << "> (" << algorithms[pair.second.a] << "): " << pair.second.t << endl;
      }
   }
   

	/* 5. Compute final: (Pr, Pc), (ar, ac), time */
   double t_min = numeric_limits<double>::max();
   Optimal_t opt_grid;
   
   for (const auto& d : dims_arrangements) {

      t_algs t_algs_x = t_algs_map[d.x];
      t_algs t_algs_y = t_algs_map[d.y];
            
      if (verbose) {
         cout << "(" << d.x << "," << d.y << "):\t ";
         cout << std::fixed << std::setprecision(12) << t_algs_x.t + t_algs_y.t;
         string x_name = (d.x != 1) ? algorithms[t_algs_x.a] : " - ";
         string y_name = (d.y != 1) ? algorithms[t_algs_y.a] : " - ";
         cout << "(" << x_name << "," << y_name << ")" << endl;
      }

      if (t_algs_x.t + t_algs_y.t < t_min) {
         opt_grid.t  = t_algs_x.t + t_algs_y.t;
         opt_grid.ar = t_algs_x.a;
         opt_grid.ac = t_algs_y.a;
         opt_grid.Pr = d.x;
         opt_grid.Pc = d.y;
         
         opt_grid.ar_name = (d.x != 1) ? algorithms[opt_grid.ar] : " - ";
         opt_grid.ac_name = (d.y != 1) ? algorithms[opt_grid.ac] : " - ";
         
         t_min = opt_grid.t;
      }
      
   }
   

   /* Free memory */
   for (int i = 0; i < procs_to_test_nr; i++) {
       delete[] times_grid[i];
   }
   delete[] times_grid;

   
   return opt_grid;
}



/**************************************************
                      MAIN
 **************************************************/

int main (int argc, char * argv[]) {
         
   int i = 0;
      
   vector<string> channels;
   Map            mapping;
   OpType         operation;
   
   /* Default values */
   int m  = 256 * 1024;
   int ms =   4 * 1024;
   int P  = 32;
   int Q  =  8;
   int M  =  4;
   
   channels.push_back("SHM");
   mapping   = Map::Default;
   operation = OpType::DEFAULT;
   
   help    = false;
   verbose = false;
   
   /* Parse arguments */
   static struct option long_options[] =
   {
      {"msglen",         required_argument,  nullptr,  'm'},
      {"seglen",         required_argument,  nullptr,  's'},
      {"num_procs",      required_argument,  nullptr,  'P'},
      {"num_nodes",      required_argument,  nullptr,  'M'},
      {"procs_per_node", required_argument,  nullptr,  'Q'},
          
      {"channel",        required_argument,  nullptr,  'c'},
      {"mapping",        required_argument,  nullptr,  'd'},
      {"operation",      required_argument,  nullptr,  'o'},
      
      {"verbose",        no_argument,        &verbose,  0 },
      {"help",           no_argument,        nullptr,  'h'},
      {nullptr,          0,                  nullptr,   0 }
   };
   
   int c;
   int option_index = 0;
   while ((c = getopt_long(argc, argv, "m:s:P:M:Q:c:d:o:vh", long_options, &option_index)) != -1) {
      
      switch (c) {
                        
         case 'm':
            m = stoi(optarg);
            break;
            
         case 's':
            ms = stoi(optarg);
            break;
                        
         case 'P':
            P = stoi(optarg);
            break;
            
         case 'M':
            M = stoi(optarg);
            break;
            
         case 'Q':
            Q = stoi(optarg);
            break;
            
         case 'c': // Additional channels
            for (const auto &value: channels_s) {
               if (value == optarg) {
                  channels.push_back(optarg);
                  break;
               }
            }
            break;
            
         case 'd':
            i = 0;
            for (const auto &value: mapping_s) {
               if (value == optarg) {
                  mapping = (Map) i;
                  break;
               }
               i++;
            }
            break;
            
         case 'o':
            i = 0;
            for (const auto &value: operation_s) {
               if (value == optarg) {
                  operation = (OpType) i;
                  break;
               }
               i++;
            }
            break;

         case 'v':
            verbose = 1;
            break;
            
         case 'h':
            help = true;
            break;
            
         case ':':
            cout << "Missing arg for " << optopt << endl;
            break;
            
         default:
            cout << "?? getopt returned character code " << c << endl;
      }
      
   }
      
   if (help) {
      cout << "Options:"                                                               << endl;
      
      cout << "Flags:"                                                                 << endl;
      cout << "\t--help,           -h:  show this help."                               << endl;
      cout << "\t--verbose,        -v:  verbose output."                               << endl;
      
      cout << "Arguments:"                                                             << endl;
      cout << "\t--msglen,         -m:  message length. Default: "               << m  << endl;
      cout << "\t--seglen,         -s:  segment length. Default: "               << ms << endl;
      cout << "\t--num_procs,      -P:  Number of processes. Default: "          << P  << endl;
      cout << "\t--num_nodes,      -M:  Number of nodes. Default: "              << M  << endl;
      cout << "\t--procs_per_node, -Q:  Number of processes per node. Default: " << Q  << endl;
      
      cout << "\t--channel,        -c:  Network channel to use in addition to Shared Memory (SHM) [TCP, IB, ARIES]. Default: none."                                                       << endl;
      cout << "\t--mapping,        -d:  Predefined mapping of processes [Default, Sequential, RoundRobin, Random, User]."                                                               << endl;
      cout << "\t--operation,      -o:  Operation type for allreduce collective. Default: " << operation_s[(int)operation]                                                   << endl;

      exit(0);
   }

   if (verbose) {
      cout << "m  = " << m  << endl;
      cout << "ms = " << ms << endl;
      cout << "P  = " << P  << endl;
      cout << "Q  = " << Q  << endl;
      cout << "M  = " << M  << endl;
      
      cout << "Mapping:   " << mapping_s[(int)mapping]     << endl;
      cout << "Channels:  ";
      for (const auto &value: channels) {
         cout << value << " ";
      }
      cout << endl;
      cout << "Operation: " << operation_s[(int)operation] << endl;
   }
   
   
   /* Execute algorithm: find optimals */
   TauLopParam::setInstance(channels);
   
   Optimal_t opt = SUARA2D_find_optimals_numerically (P,
                                                      m,
                                                      ms,
                                                      algorithms,
                                                      /*, H */
                                                      Q,
                                                      M,
                                                      mapping,
                                                      operation);

   /* Print result */
   cout << endl;
   cout << "(Pr x Pc):  (" << opt.Pr      << " x " << opt.Pc      << ")" << endl;
   cout << "(ar x ac):  (" << opt.ar_name << ", "  << opt.ac_name << ")" << endl;
   cout << "time:       "  << std::fixed << std::setprecision(12) << opt.t << " s." << endl;
   
   return 0;
}
