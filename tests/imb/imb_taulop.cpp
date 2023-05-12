//
//  imb_taulop.cpp
//  IMB-style taulop benchmarks.
//    Print similar output in order to compare results.
//
//  Created by jarico on 30/4/23.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "benchmark.h"
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

/*
void printHelp () {

   cout << "Options (similar to IMB):"                                     << endl;
   cout << "\t-help,       -h:  show this help."                           << endl;
   cout << "\t-verbose,    -v:  verbose output."                           << endl;
   cout << "\t-msglen,     -m:  file name with message lengths."           << endl;
   cout << "\t-output,     -o:  file name prefix for output results."      << endl;

}
*/

/*
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
*/

/*
template<typename TYPE>
void printVector(vector<TYPE> values) {
   
   for (auto const &value: values) {
      cout << value << endl;
   }
   
}
*/


/*
int saveTextFile (string filename, vector<Results_t> results, e_benchmark benchmark) {
   
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
   
	/* Write header *
   auto time = std::time(nullptr);
   
   f << "#---------------------------------------------------" << endl;
   f << "#    tauLop estimations (IMB format)                " << endl;
   f << "#---------------------------------------------------" << endl;
   f << "# Date: " << put_time(std::gmtime(&time), "%F %T%z")  << endl;
   f << "# Benchmarks to run:                                " << endl;
   f << "#                                                   " << endl;
   f << "#  AQUI EL BENCHMARK !!!                            " << endl;
   f << "#                                                   " << endl;
   f << "#---------------------------------------------------" << endl;
   f << "# Benchmarking = " << s_benchmark[(int)benchmark]     << endl;
   f << "# processes =                " << endl;
   f << "#---------------------------------------------------" << endl;
   f << "#bytes \t #repetitions \t t[usec] \t Mbytes/sec     " << endl;

   
   /* Write data (IMB format) *
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
*/




/**************************************************
                      MAIN
 **************************************************/

int main (int argc, char * argv[]) {
   
   
   // Params
   //string  machines;
   //string  msglen_file = "msglen.txt";
   //string  output_prefix = "taulop_";
   
   //const Args_t &arguments = getArguments(argc, argv);
   
   //Benchmarks_t  benchmark = Benchmark_t::PingPong;
   //string  output_file = "output.txt";
   
   //Map     mapping     = Map::Default;

   
   /* 1. Build a benchmark object */
   //Benchmark b {arguments};
   Benchmark b (argc, argv);
   
   b.run();
   b.write();
   b.error();
   
   
   
   /* . Read configuration data */
   
   /* 1a. Read message lengths to estimate (-msglen, -m) *
   vector<string> v_str_msglen;
   vector<int>    v_msglen;
   readTextFile(msglen_file, v_str_msglen);
   for (auto const &v: v_str_msglen) {
      v_msglen.push_back(stoi(v));
   }
   if (verbose)  printVector(v_msglen);

      
   /* 2. Call benchmarks *
   vector<Results_t> results = fxn[(int)benchmark](v_msglen);
   
   /* 3. Generate output *
   saveTextFile(output_file, results, benchmark);
   
   */
   
   return 0;
}
