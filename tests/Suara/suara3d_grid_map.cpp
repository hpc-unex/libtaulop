
//
//  suara3d_grid_map.cpp
//  Find optimal SUARA3D grid and algorithm numerically (considering mapping)
//
//  Created by jarico on 01/08/23.
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
extern double allreduce_linear_ompi       (Communicator *comm, int P, int m, int ms, OpType op);
extern double allreduce_nonoverlap_ompi   (Communicator *comm, int P, int m, int ms, OpType op);
extern double allreduce_rda_ompi          (Communicator *comm, int P, int m, int ms, OpType op);
extern double allreduce_ring_ompi         (Communicator *comm, int P, int m, int ms, OpType op);
extern double allreduce_ringsegm_ompi     (Communicator *comm, int P, int m, int ms, OpType op);
extern double allreduce_rabenseifner_ompi (Communicator *comm, int P, int m, int ms, OpType op);


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
double (*fxn_algs [6]) (Communicator *comm, int P, int m, int ms, OpType op) = {
   allreduce_linear_ompi,
   allreduce_nonoverlap_ompi,
   allreduce_rda_ompi,
   allreduce_ring_ompi,
   allreduce_ringsegm_ompi,
   allreduce_rabenseifner_ompi
};


/* 3D grid dimensions (avoid the use of x,y,z) */
struct grid_3d_t {
   int r;   // number of rows
   int c;   // number of cols
   int d;   // number of depths
   
   // Define a comparison operator for the struct (in order to be used in a map)
   bool operator<(const grid_3d_t& g) const {
      
      if (r < g.r) {
         return true;
      } else if (r == g.r) {
         if (c < g.c) {
            return true;
         } else {
            return d < g.d;
         }
      }
      
      return false;
   }

};

const int D = 2;  // Number of dimensions
const int R = 0;  // Index to rows
const int C = 1;  // Index to columns
const int K = 2;  // Index to depth


/* Return values from find optimals */
struct Optimal_t {
   int     Pr;       /* Number of process in rows              */
   int     Pc;       /* Number of process in columns           */
   int     Pd;       /* Number of process in depth             */
   int     ar;       /* Algorithm to execute in rows           */
   int     ac;       /* Algorithm to execute in columns        */
   int     ad;       /* Algorithm to execute in depth          */
   string  ar_name;  /* Name of the rows algorithm             */
   string  ac_name;  /* Name of the columns algorithm          */
   string  ad_name;  /* Name of the depth algorithm            */
   double  t;        /* Total estimated time for the algorithm */
};


/* Some global variables */
int  verbose = 0;
bool help    = false;



/**************************************************
         helper functions
 **************************************************/

void generate_3Ddims_combinations (int P, list<grid_3d_t> &dims_arrangements) {

   for (int x = 1; x <= P; x++) {
      
      if (P % x == 0) {
         int xy = int (P / x);
         
         for (int y = 1; y <= xy; y++) {
      
            if (xy % y == 0) {
               
               int z = int (xy / y);
               
               if (x * y * z == P) {
                  dims_arrangements.push_back({x, y, z});
               }
            }
            
         }
      }
   }
}


void generate_procs_to_test (list<grid_3d_t> &dims_combination, list<int> &procs_to_test) {
   
   for (const auto& d : dims_combination) {
      auto it = std::find(procs_to_test.begin(), procs_to_test.end(), d.r);
      
      if (it == procs_to_test.end()) { /* Not yet in the list. Only need to test d.r */
         procs_to_test.push_back(d.r);
      }
      
   }
}


double exec_algorithm (Communicator *comm, int a, int P, int m, int ms, OpType op) {
   
   double t = 0.0;

   t = fxn_algs[a] (comm, P, m, ms, op);
   
   return t;
}


void grid_to_comm (grid_3d_t d, int dim, int n, int p, int *ranks) {
   /*
    * INPUT
    *   d: grid (e.g. (3,4,2)
    *   dim: dimension number (0, for rows, 1 for cols and 2 for depth)
    *   n: number of row, col or depth to get ranks.
    *   p: number of processes
    * OUTPUT
    *   ranks: ranks to build a new comm.
    *          The ranks are those in the column "n", row "n" and depth "n" depending on the "dim".
    *          TODO: n = 0 is required.
    */
      
   for (int i = 0; i < p; i++) {
      
      if (dim == 0) { // Rows
         ranks[i] = i;
      }
      
      if (dim == 1) { // Columns
         ranks[i] = i * d.c;
      }
      
      if (dim == 2) { // Depth
         ranks[i] = i * d.r * d.c;
      }
      
   }
}


/**************************************************
        SUARA3D_find_optimals_numerically
 **************************************************/

/*
    The algorithm works as follows:
 
 1. Find all possible arrangements of processes in a 3D grid (dims_arrangements).
 2. Apply actual mapping of the processes on the platform. A "world" communicator is created to represent all processes.
 3. Main loop of the algorithm:
      for every valid process grid arrangement d=(dr,dc,dd) in dims_arrangement:
      3.1. Just initialize values.
      3.2. Obtain the optimum algorithm for a row of processes
 		   a) Create a communicator representing a row (with dc processes)
         b) Invoke every algorithm and get that of minimum cost.
      3.3. Obtain the optimum algorithm for a column of processes
         a) Create a communicator representing a column (with dr processes)
         b) Invoke every algorithm and get that of minimum cost.
      3.4. Obtain the optimum algorithm for a depth dim of processes
         a) Create a communicator representing a depth dim (with dd processes)
         b) Invoke every algorithm and get that of minimum cost.
      3.5. Utilize a map to store the minimum values for rows, columns and depth.
         Key:    d=(dr,dc,dd)
         Value:  t_row+t_col+t_depth, alg_row, alg_col, alg_depth (sum of times, algorithm for rows, for columns and for depth)
 4. Compare times for each valid process grid arrangment "d" in "map" and obtain the minimum.
 
 The cost of the function is in the order: O(A x D x d), with:
     A: the number of algorithms to evaluate.
     D: number of dimensions.
     d: number of valid process grid arrangments for P.
 
 Example: P = 12
 
 dims_arrangements = { (1,1,12), (1,2,6), (1,3,4), (1,4,3), . . ., (4,3,1), (6,2,1), (12,1,1) }
 
 for d in dims_arrangments:
 
   d=(1,1,12)
   ROWS: 1 row with 1 process
         Create communicator with P=1 (map is inherited from world)
         Execute algorithms and get that of minimum cost.
   COLS: 1 cols with 1 processes
         Create a communicator with P=1 (map is inherited from world)
         Execute algorithms and get that of minimum cost.
   DEPTH: 1 depth dims with 12 processes
       Create a communicator with P=12 (map is inherited from world)
       Execute algorithms and get that of minimum cost.

   map[(1,12)] = {t_r + t_c + t_d, a_r, a_c, a_d}
 
   d=(1,2,6)
     < repeat previous procedure >
   
   . . .
 
 t_algs_map[d] = {t, alg_r, alg_c, alg_d):
    (1, 1,12): { 0.28, lin,   lin,   rnos}
    (1, 2, 6): { 0.18, lin,    rd,   rnos}
    (1, 3, 4): { 0.24, lin,    rd,   rs  }
    (1, 4, 3): { 0.16, lin,  nono,   rab }
    . . .
    (6, 2, 1): { 0.17, rnos, rnos,   lin }
   (12, 1, 1): { 0.23, rab,  lin,    lin }
 
 Result:
    (1, 4, 3): {0.16, lin, nono, rab}

 */

Optimal_t SUARA3D_find_optimals_numerically (int P, int m, int ms, string algorithms[] /* , H */, int Q, int M, Map mapping, OpType op) {
      
   /* 1. Generate grid2D posible dimensions arrangements */
   list<grid_3d_t> dims_arrangements;
   generate_3Ddims_combinations(P, dims_arrangements);

   if (verbose) {
      cout << "Posible 3D grid dimensions arrangements: " << endl;
      for (const auto& d : dims_arrangements) {
         cout << "(" << d.r << "," << d.c << "," << d.d << ") ";
      }
      cout << endl;
   }
   
   /* 2. Apply mapping to the world communicator:
    *
    *   Currently supported: Sequential, RoundRobin and Random.
    *   Unsupported: user mapping (requires a nodes array).
    */
   Communicator *world = new Communicator (P);
   Mapping *w_map = new Mapping (P, M, Q, mapping);
   world->map(w_map);
   
   if (verbose) {
      cout << "Main Communicator: " << endl;
      world->show();
   }
      
   Communicator *comm = nullptr;
   int *ranks = nullptr;


	/* 3. Main loop over valid arrangements:
    *
    *   3.1. Initialize
    *   3.2. For rows, execute algorithms in A and obtains the minimum
    *   3.3. For columns, execute algorithms in A and obtain the minimum
    *   3.4. Save current min row and column minimum time and algorithm
    */
   struct t_algs {
      double t;
      int    a_r;
      int    a_c;
      int    a_d;
   };
   map<grid_3d_t, t_algs> t_algs_map;

   Optimal_t opt_grid;
      
   for (const auto& d : dims_arrangements) {
        
      /* 3.1. Initialize */
      if (verbose) {
         cout << "(" << d.r << "," << d.c << "," << d.d << ") " << endl;
      }
      
      double t_r = 0.0;
      double t_c = 0.0;
      double t_d = 0.0;
      
      double t_r_min = numeric_limits<double>::max();
      double t_c_min = numeric_limits<double>::max();
      double t_d_min = numeric_limits<double>::max();
      
      int a_r_min = 0;
      int a_c_min = 0;
      int a_d_min = 0;
      
      
      /* 3.2. Get optimum algorithm for rows */
      ranks = new int[d.c];
      grid_to_comm (d, R, 0, d.c, ranks);
      comm = world->create(d.c, ranks);

      if (verbose) {
         comm->show();
      }
      
         
      for (int a = 0; a < NUM_ALGS; a++) {
         
         t_r = exec_algorithm(comm, a, d.c, m, ms, op);
         
         if (verbose) {
            cout << "Rows:     " << setw(5) << algorithms[a] << ") t_r = " << std::fixed << std::setprecision(12) << t_r << endl;
         }
         
         if (t_r < t_r_min) {
            t_r_min = t_r;
            a_r_min = a;
         }
         
      }

      if (verbose) {
         cout << "*MIN Rows:     " << algorithms[a_r_min] << ") t_r_min = " << std::fixed << std::setprecision(12) << t_r_min << endl;
      }

      delete comm;
      delete [] ranks;
      

      /* 3.3. Get optimum algorithm for columns */
      ranks = new int[d.r];
      grid_to_comm (d, C, 0, d.r, ranks);
      comm = world->create(d.r, ranks);
      
      if (verbose) {
         comm->show();
      }


      for (int a = 0; a < NUM_ALGS; a++) {
         
         t_c = exec_algorithm(comm, a, d.r, m, ms, op);
         
         if (verbose) {
            cout << "Columns:  " << setw(5) << algorithms[a] << ") t_c = " << std::fixed << std::setprecision(12) << t_c << endl;
         }
         
         if (t_c < t_c_min) {
            t_c_min = t_c;
            a_c_min = a;
         }
         
      }
      
      if (verbose) {
         cout << "*MIN Column:   " << algorithms[a_c_min] << ") t_c_min = " << std::fixed << std::setprecision(12) << t_c_min << endl;
      }
               
      delete comm;
      delete [] ranks;


      /* 3.4. Get optimum algorithm for depth dimension */
      ranks = new int[d.d];
      grid_to_comm (d, K, 0, d.d, ranks);
      comm = world->create(d.d, ranks);
      
      if (verbose) {
         comm->show();
      }


      for (int a = 0; a < NUM_ALGS; a++) {
         
         t_d = exec_algorithm(comm, a, d.d, m, ms, op);
         
         if (verbose) {
            cout << "Depth:   " << setw(5) << algorithms[a] << ") t_d = " << std::fixed << std::setprecision(12) << t_d << endl;
         }
         
         if (t_d < t_d_min) {
            t_d_min = t_d;
            a_d_min = a;
         }
         
      }
      
      if (verbose) {
         cout << "*MIN Depth:    " << algorithms[a_d_min] << ") t_d_min = " << std::fixed << std::setprecision(12) << t_d_min << endl;
      }
               
      delete comm;
      delete [] ranks;


      /* 3.5. Store values for the current valid arrangement:
       *
       *    - Sum of the rows and columns times
       *    - a_r_min: optimum algorithm for rows
       *    - a_c_min: optimum algorithm for columns
       *
       */
      t_algs_map[d] = {t_r_min + t_c_min + t_d_min, a_r_min, a_c_min, a_d_min};
      
   }
   
   
   /* 4. Find the minimum time across valid arrangements (together with algorithms) */
   double t_min = numeric_limits<double>::max();
   grid_3d_t key_min;
   for (const auto& pair : t_algs_map) {
      
      if (verbose) {
         cout << "d = (" << pair.first.r << "," << pair.first.c << "," << pair.first.d << ") = {" << algorithms[pair.second.a_r] << "," << algorithms[pair.second.a_c] << "," << algorithms[pair.second.a_d] << ") -> " << pair.second.t << "}" << endl;
      }
      
      if (pair.second.t < t_min) {
         t_min = pair.second.t;
         key_min = {pair.first.r, pair.first.c, pair.first.d};
      }
   }

   
   /* 5. Return optimum */
   t_algs grid_min = t_algs_map[key_min];
   
   opt_grid.t  = grid_min.t;
   opt_grid.ar = grid_min.a_r;
   opt_grid.ac = grid_min.a_c;
   opt_grid.ad = grid_min.a_d;
   opt_grid.Pr = key_min.r;
   opt_grid.Pc = key_min.c;
   opt_grid.Pd = key_min.d;
   
   opt_grid.ar_name = (key_min.c != 1) ? algorithms[grid_min.a_r] : " - ";
   opt_grid.ac_name = (key_min.r != 1) ? algorithms[grid_min.a_c] : " - ";
   opt_grid.ad_name = (key_min.d != 1) ? algorithms[grid_min.a_d] : " - ";

   if (verbose) {
      cout << endl << "** Optimum 3D process grid arrangement: " << endl;
      cout << "d = (" << key_min.r << "," << key_min.c << "," << key_min.d << ") = {(" << algorithms[grid_min.a_r] << "," << algorithms[grid_min.a_c] << algorithms[grid_min.a_d] << "," << ") -> " << grid_min.t << "}" << endl;
   }
   
   delete w_map;
   delete world;
   
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
   
   srand((unsigned)time (NULL));  // In case of Map::Random
   
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
   
   Optimal_t opt = SUARA3D_find_optimals_numerically (P,
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
   cout << "(Pr x Pc x Pd):  (" << opt.Pr      << " x " << opt.Pc      << " x " << opt.Pd      << ")" << endl;
   cout << "(ar, ac, ad):    (" << opt.ar_name << ", "  << opt.ac_name << ", "  << opt.ad_name << ")" << endl;
   cout << "time:            "  << std::fixed << std::setprecision(12) << opt.t << " s." << endl;

   return 0;
}
