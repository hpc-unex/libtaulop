//
//  args.cpp
//   Read arguments to execute tests
//
//  Created by jarico on 20/4/17.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"
#include "args.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <getopt.h>
#include <unistd.h>

using namespace std;

// Initialize static variables
TaulopArgs* TaulopArgs::taulop_args = nullptr;
bool TaulopArgs::initialized = false;

// Help enums
static string operation_s[13] = {"def", "max", "min", "sum", "prod", "land", "band", "lor", "bor", "lxor", "bxor", "maxloc", "minloc"};

static string units_s [5] = {"s", "ms", "us", "ns", "fs"};

static string mapping_s [4] = {"Default", "Sequential", "RoundRobin", "Random"};



// Util function to print times in specifics units
void TaulopArgs::printTime (double t, string legend) {
   
   int p = 6 + int(this->unit) * 3;
   t = t * pow(10, int(this->unit) * 3);
   
   int d = 12 - int(this->unit) * 3;
      
   //cout << legend << left << setw(20) << " -> Time: " << fixed << std::setprecision (d) << t << " " << units_s[int(this->unit)] << endl;
   cout << legend << fixed << std::setprecision (d) << t << " " << units_s[int(this->unit)] << endl;
}


// Initialize
TaulopArgs::TaulopArgs (int argc, char **argv) {
   
   this->P       = 8;
   this->Q       = P;
   this->M       = 1;
   this->m       = 1024;
   this->root    = 0;
   this->op      = OpType::DEFAULT;
   this->unit    = units::s;
   this->verbose = false;
   this->mapping = Map::Default;
   
   
   int verbose = 0;
   int i;
   
   static struct option long_options[] =
    {
       {"msg_size",       required_argument,  nullptr,       'm'},
       {"num_procs",      required_argument,  nullptr,       'P'},
       {"num_nodes",      required_argument,  nullptr,       'M'},
       {"procs_per_node", required_argument,  nullptr,       'Q'},
       {"operation",      required_argument,  nullptr,       'o'},
       {"mapping",        required_argument,  nullptr,       'd'},
       {"units",          required_argument,  nullptr,       'u'},
       {"verbose",        no_argument,        &verbose,       1 },
       {"help",           no_argument,        nullptr,       'h'},
       {nullptr,          0,                  nullptr,        0 }
    };

   int c;
   int option_index = 0;
   while ((c = getopt_long(argc, argv, "-m:P:M:Q:M:o:d:u:vh", long_options, &option_index)) != -1) {
            
      switch (c) {
                        
         case 'm':
            this->m = stoi(optarg);
            break;
            
         case 'P':
            this->P = stoi(optarg);
            break;

         case 'M':
            this->M = stoi(optarg);
            break;

         case 'Q':
            this->Q = stoi(optarg);
            break;

         case 'o':
            i = 0;
            for (const auto &value: operation_s) {
               if (value == optarg) break;
               i++;
            }
            if (i < 13) {
               this->op = (OpType) i;
            }
            break;

         case 'd':
            i = 0;
            for (const auto &value: mapping_s) {
               if (value == optarg) break;
               i++;
            }
            if (i < 4) {
               this->mapping = (Map)i;
            }
            break;

         case 'u':
            i = 0;
            for (const auto &value: units_s) {
               if (value == optarg) break;
               i++;
            }
            if (i < 5) {
               this->unit = (units)i;
            }

            break;

         case 'v':
            this->verbose = true;
            break;

         case 'h':
            cout << argv[0] << " <args>" << endl;
            cout << "\t -P, --num_procs      <num>: Process number to execute test. Default: " << this->P << endl;
            cout << "\t -m, --msg_size       <num>: Size of message in bytes. Default: " << this->m << endl;
            cout << "\t -M, --num_nodes      <num>: Number of nodes in the computer. Default: " << this->M << endl;
            cout << "\t -Q, --procs_per_node <num>: Number of processes in each node of the computer. Default: " << this->Q << endl;
            cout << "\t -o, --operation      <str>: Type of operation in case of reduction. Default: " << operation_s [(int) this->op] << endl;
            cout << "\t -d, --mapping        <str>: Mapping of ranks to nodess. Default: " << mapping_s [(int) this->mapping] << endl;
            cout << "\t -u, --units          <str>: Units to show times [s|ms|us|ns|fs]. Default: " << units_s[(int) this->unit] << endl;
            cout << "\t -v, --verbose: verbose output. Default: " << this->verbose << endl;
            cout << "\t -h, --help:    show this help and exit." << endl << endl;
            
            exit(0);

         case ':':
            cerr << "Missing argument for " << optopt << endl;
            break;
            
         default:
            cerr << " getopt returned character code 0%o" << c << endl;
      }
      
   }
   
   
   if (this->verbose) {
      cout << "Parameters: "          << endl;
      cout << "  P:  "    << this->P  << endl;
      cout << "  Q:  "    << this->Q  << endl;
      cout << "  M:  "    << this->M  << endl;
      cout << "  m:  "    << this->m  << endl;
      
      cout << "  mapping: " << mapping_s   [(int) this->mapping] << endl;      
      cout << "  units:   " << units_s     [(int) this->unit]    << endl;
      cout << "  Optype:  " << operation_s [(int) this->op]      << endl;
   }
   
}




TaulopArgs& TaulopArgs::GetArgs (int argc, char **argv) {

   if (initialized == false) {
      taulop_args = new TaulopArgs(argc, argv);
      initialized = true;
   }
   
   return *taulop_args;
}



