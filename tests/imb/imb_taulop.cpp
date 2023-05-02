//
//  imb_taulop.cpp
//  IMB-style taulop benchmarks.
//    Print similar output in order to compare results.
//
//  Created by jarico on 30/4/23.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "benchs.h"

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
using namespace std;





/**************************************************
                  HELP FUNCTIONS
 **************************************************/


void printHelp () {

   cout << "Options (similar to IMB):"                                     << endl;
   cout << "\t-help,       -h:  show this help."                           << endl;
   cout << "\t-verbose,    -v:  verbose output."                           << endl;
   cout << "\t-npmin,      -n:  minimum number of processes."              << endl;
   cout << "\t-input,      -i:  file name with IMB benchmarks to execute." << endl;
   cout << "\t-msglen,     -m:  file name with message lengths."           << endl;

   cout << "Options (additional):"                                         << endl;
   cout << "\t-output,     -o:  file name prefix for output results."      << endl;
   cout << "\t-channels,   -c:  file name with channels to use."           << endl;
   cout << "\t-machines,   -M:  machines file name."                       << endl;

}



int readTextFile(string filename, vector<string> &values) {
   
   ifstream  f;
   string    line;
   
   if (verbose) {
      cout << "Reading file: " << filename << endl;
   }
   
   f.open(filename);
   
   if (! f.is_open()) {
      cerr << "ERROR: opening file <" << filename << ">" << endl;
      return -1;
   }
   
   while (f.good()) {
      f >> line;
      if (!f.eof()) {
         values.push_back(line);
      }
   }
   
   f.close();
   return 0;
}


template<typename TYPE>
void printVector(vector<TYPE> values) {
   
   for (auto const &value: values) {
      cout << value << endl;
   }
   
}



int saveTextFile (string filename, vector<Results_t> results) {
   
   ofstream  f;
   string    line;
   
   if (verbose) {
      cout << "Writting file: " << filename << endl;
   }
   
   f.open(filename);
   
   if (! f.is_open()) {
      cerr << "ERROR: opening file <" << filename << ">" << endl;
      return -1;
   }
   
	/* Write header */
   auto time = std::time(nullptr);
   
   f << "#---------------------------------------------------" << endl;
   f << "#    tauLop estimations (IMB format)                " << endl;
   f << "#---------------------------------------------------" << endl;
   f << "# Date: " << put_time(std::gmtime(&time), "%F %T%z")  << endl;
   f << "# List of Benchmarks to run:                        " << endl;
   f << "#                                                   " << endl;
   f << "#  AQUI EL BENCHMARK !!!                            " << endl;
   f << "#                                                   " << endl;
   f << "#---------------------------------------------------" << endl;
   f << "# Benchmarking  AQUI EL BENCHMARK !!!               " << endl;
   f << "# #processes =  AQUI NUM PROCESSES !!!              " << endl;
   f << "#---------------------------------------------------" << endl;
   f << "#bytes \t #repetitions \t t[usec] \t Mbytes/sec     " << endl;

   
   /* Write data (IMB format) */
   f << fixed << setprecision(6);  // Same precision as default IMB
   
   // Remember: IMB latency comes in usec.
   for (auto const &r: results) {
      
      f << r.m           << " \t "
        << r.repetitions << " \t "
        << r.latency     << " \t "
        << r.bandwidth   << endl;
      
      if (verbose) {
         cout << r.m << " \t " << r.repetitions << " \t " << r.latency << " \t " << r.bandwidth << endl;
      }
   }
   
   f.close();
   return 0;
}










/**************************************************
                      MAIN
 **************************************************/

int main (int argc, char * argv[]) {
   
   
   // Params
   string  machines;
   string  msglen;
   string  output_prefix = "taulop_";
   string  benchmarks;
   string  channels;
   int     min_P = 1;
   bool    help = false;
   
   
   static struct option long_options[] =
    {
       {"msglen",         required_argument,  nullptr,  'm'},
       {"npmin",          required_argument,  nullptr,  'n'},
       {"input",          required_argument,  nullptr,  'i'},

       {"output",         required_argument,  nullptr,  'o'},
       {"channels",       required_argument,  nullptr,  'c'},
       {"machines",       required_argument,  nullptr,  'M'},

       {"verbose",        no_argument,        &verbose,  0 },
       {"help",           no_argument,        nullptr,  'h'},
       {nullptr,          0,                  nullptr,   0 }
    };

   int c;
   int option_index = 0;
   while ((c = getopt_long(argc, argv, "-m:n:i:o:c:M:vh", long_options, &option_index)) != -1) {
      
      switch (c) {
            
         case 0:
            cout << "long option " << long_options[option_index].name << endl;
            if (optarg)
               cout << " with arg " << optarg << endl << endl;
            break;
            
         case 1:
            cout << "regular argument " << optarg << endl;
            break;
            
         case 'm':
            msglen = optarg;
            break;
            
         case 'n':
            min_P = stoi(optarg);
            break;

         case 'i': // Benchmarks
            benchmarks = optarg;
            break;

         case 'o':
            output_prefix = optarg;
            break;

         case 'c':
            channels = optarg;
            break;

         case 'M':
            machines = optarg;
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
      printHelp();
      return 0;
   }
   
   /* 1. Read configuration data */
   
   /* 1a. Read benchmarks to execute  (-input, -i) */
   vector<string> v_benchmarks;
   readTextFile(benchmarks, v_benchmarks);
   if (verbose)  printVector(v_benchmarks);

   /* 1b. Read machines to evaluate  (-machines, -M) */
   vector<string> v_machines;
   readTextFile(machines, v_machines);
   if (verbose)  printVector(v_machines);

   /* 1c. Read message lengths to estimate (-msglen, -m) */
   vector<string> v_str_msglen;
   vector<int>    v_msglen;
   readTextFile(msglen, v_str_msglen);
   for (auto const &v: v_str_msglen) {
      v_msglen.push_back(stoi(v));
   }
   if (verbose)  printVector(v_msglen);

   /* 1d. Read channels (-channels, -c) */
   vector<string> v_channels;
   readTextFile(channels, v_channels);
   if (verbose)  printVector(v_channels);

   if (verbose) {
      cout << "Min number of processes: " << min_P         << endl;
      cout << "Output prefix for files: " << output_prefix << endl;
   }

   
   
   /* 2. Set functions */
   Benchmarks_t imb_taulop;
   imb_taulop.PingPong  = pingpong;
   imb_taulop.Broadcast = nullptr;
   
   
   
   /* 3. Call benchmarks */
   vector<Results_t> results = imb_taulop.PingPong(v_msglen);
   
   
   
   /* 4. Generate output */
   string output_file = output_prefix + "metropolis" + "_" + "pingpong" + "_" + "SHM" + ".txt";
   saveTextFile(output_file, results);
   
   
   
   return 0;
}
