//
//  benchs.h
//  IMB-style taulop benchmarks.
//    Print similar output in order to compare results.
//
//  Created by jarico on 30/4/23.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"
#include "benchmark.h"

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <unistd.h>
#include <getopt.h>
#include <string>
#include <vector>

using namespace std;


// PRIVATE functions

int Benchmark::readTextFile(string filename, vector<string> &values) {
   
   ifstream  f;
   string    line;
   
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


void Benchmark::generateMsglen (string msglen_file, vector<int> &sizes) {
   
   if (msglen_file != "") {
      
      vector<string> v_str_msglen;
      
      readTextFile(this->msglen_file, v_str_msglen);
      
      for (auto const &v: v_str_msglen) {
         this->sizes.push_back(stoi(v));
      }
      
   } else { // Default sizes (power of to up to 1MB)
      
      for (int i = 1; i <= 1024*1024; i*=2) {
         this->sizes.push_back(i);
      }
      
   }
   
}


int Benchmark::generateMap(string filename, vector<int> &map) {
   
   ifstream  f;
   string    line;
      
   f.open(filename);
   
   if (! f.is_open()) {
      cerr << "ERROR: opening map file <" << filename << ">" << endl;
      return -1;
   }
   
   map.clear();
   int num = 0;
   while (f.good()) {
      f >> line;
      if (!f.eof()) {
         map.push_back(stoi(line));
         num++;
      }
   }
      
   f.close();
   
   if (num != this->P) {
      cerr << "ERROR: number of rank nodes in map file: "
           << num
           << " does not match P: "
           << this->P
           << endl;
      exit(-1);
   }
   return 0;
}


int Benchmark::parseArgs (int argc, char *argv[]) {
   
   int verbose = 0;
   int i;
   
   this->help = false;
   this->verbose = false;
   
   static struct option long_options[] =
   {
      {"msglen",         required_argument,  nullptr,  'm'},
      {"algorithm",      required_argument,  nullptr,  'a'},
      {"write_file",     required_argument,  nullptr,  'w'},
      {"operation",      required_argument,  nullptr,  'o'},
      
      {"mapping",        required_argument,  nullptr,  'd'},
      {"map_file",       required_argument,  nullptr,  'x'},
      {"num_procs",      required_argument,  nullptr,  'P'},
      {"num_nodes",      required_argument,  nullptr,  'M'},
      {"procs_per_node", required_argument,  nullptr,  'Q'},
      {"root",           required_argument,  nullptr,  'r'},
      
      {"img_file",       required_argument,  nullptr,  'i'},
      {"error_file",     required_argument,  nullptr,  'e'},
      
      {"verbose",        no_argument,        &verbose,  0 },
      {"help",           no_argument,        nullptr,  'h'},
      {nullptr,          0,                  nullptr,   0 }
   };
   
   int c;
   int option_index = 0;
   while ((c = getopt_long(argc, argv, "-m:a:o:d:x:P:M:Q:r:i:e:w:vh", long_options, &option_index)) != -1) {
      
      switch (c) {
            
         case 1:
            
            i = 0;
            for (const auto &value: s_benchmark) {
               if (value == optarg) break;
               i++;
            }
            if (i < 4) {
               this->type = (e_benchmark) i;
               this->name = s_benchmark[i];
            } else {
               cerr << "ERROR: benchmark does not exist: " << optarg << endl;
               return -1;
            }
            
            break;
            
         case 'a':
            i = 0;
            for (const auto &value: s_algorithm) {
               if (value == optarg) break;
               i++;
            }
            if (i < 10) {
               this->algorithm = (e_algorithm) i;
            } else {
               cerr << "ERROR: algorithm does not exist: " << optarg << endl;
               return -1;
            }
            break;
            
         case 'm':
            this->msglen_file = optarg;
            break;
            
         case 'w':
            this->write_file = optarg;
            break;
            
         case 'i':
            this->imb_file = optarg;
            break;
            
         case 'e':
            this->error_file = optarg;
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
            
         case 'r':
            this->root = stoi(optarg);
            break;
            
         case 'd':
            
            i = 0;
            for (const auto &value: mapping_s) {
               if (value == optarg) break;
               i++;
            }
            if (i < 5) {
               this->mapping = (Map) i;
            }
            break;
            
         case 'x':
            this->map_file = optarg;
            break;

         case 'v':
            this->verbose = 1;
            break;
            
         case 'h':
            this->help = true;
            break;
            
         case ':':
            cout << "Missing arg for " << optopt << endl;
            break;
            
         default:
            cout << "?? getopt returned character code " << c << endl;
      }
      
   }
   return 0;
}


int Benchmark::saveTextFile (string filename, vector<Results_t> results) {
   
   ofstream  f;
   
   f.open(filename);
   
   if (! f.is_open()) {
      cerr << "ERROR: opening file <" << filename << ">" << endl;
      return -1;
   }
   
   /* Write header */
   printHeader(f);
   
   /* Write data */
   for (auto const &r: results) {
      printData(f, r);
   }
   
   f.close();
   return 0;
}


void Benchmark::printHeader (ostream& outp) {
   
   auto time = std::time(nullptr);
   
   outp << "#---------------------------------------------------" << endl;
   outp << "#    tauLop estimations (IMB format)                " << endl;
   outp << "#---------------------------------------------------" << endl;
   outp << "# Date: " << put_time(std::gmtime(&time), "%F %T%z")  << endl;
   outp << "#                                                   " << endl;
   outp << "#---------------------------------------------------" << endl;
   outp << "# Benchmarking " << this->name                        << endl;
   outp << "# Algorithm    " << s_algorithm[(int)this->algorithm] << endl;
   outp << "# Processes    " << this->P                           << endl;
   outp << "# !!! operation, Q, M, etc.    "                                 << endl;
   outp << "#---------------------------------------------------" << endl;
   outp << "#bytes \t #repetitions \t t[usec] \t Mbytes/sec     " << endl;
   
}


void Benchmark::printData (ostream& outp, Results_t res) {
   
   /* Write data (IMB format) */
   outp << fixed << setprecision(6);  // Same precision as default IMB
   
   // IMB outputs: latency is in usec, bw in MBytes/s
   outp << res.m           << " \t "
        << res.repetitions << " \t "
        << res.latency     << " \t "
        << res.bandwidth   << endl;
   
}




// PUBLIC functions


Benchmark::Benchmark (int argc, char *argv[]) {
   
   /* 1. Set default values: */
   this->P    = 2;
   this->Q    = 2;
   this->M    = 1;
   this->root = 0;
   
   this->algorithm = e_algorithm::Default;
   this->optype    = OpType::DEFAULT;
   
   this->output_screen = true;
   
   this->mapping = Map::Sequential;
   //vector<int>  map_user;
   
   this->verbose = false;
   this->help    = false;
   
   
   /* 2. Override default values from arguments */
   if (0 > parseArgs(argc, argv)) {
      cerr << "ERROR: parsing arguments." << endl;
      exit(1);
   }
   
   if (this->help) {
      cout << "Options (similar to IMB):"                                         << endl;
      
      cout << "Flags:" << endl;
      cout << "\t-help,           -h:  show this help."                           << endl;
      cout << "\t-verbose,        -v:  verbose output."                           << endl;
      
      cout << "Arguments:" << endl;
      cout << "\t-msglen,         -m:  file name with message lengths."           << endl;
      
      cout << "\t-algorithm,      -a:  Algorithm for the collective to execute."           << endl;
      
      cout << "\t-write_file,     -w:  file name prefix for writing results."      << endl;
      cout << "\t-operation,      -o:  (NO ACTIVADO) Operation type for reduce collectives. Default: " << s_operations[(int)this->optype] << endl;
      cout << "\t-num_procs,      -P:  Number of processes to run the benchmark. Default " << this->P << endl;
      cout << "\t-num_nodes,      -M:  Number of nodes to run the benchmark. Default " << this->M << endl;
      cout << "\t-procs_per_node, -Q:  Number of processes per node. Default: " << this->Q << endl;
      cout << "\t-root,           -r:  Root process rank in case of rooted collective. Default: " << this->root << endl;
      
      cout << "\t-mapping,        -d:  Predefined mapping of processes [Default, Sequential, RoundRobin, Random, User]." << endl;
      cout << "\t-map_file,       -x:  Map file of ranks to nodes (one rank-node per line).  Overrides mapping option." << endl;
      cout << "\t-imb_file,       -i:  (NO ACTIVADO) IMB generated file as input to compare." << endl;
      cout << "\t-error_file,     -e:  (NO ACTIVADO) Error comparing IMB and imb_taulop results." << endl;
      
      exit(0);
   }
   
   
   if (this->verbose) {
      cout << "Benchmark:      " << this->name                         << endl;
      cout << "Algorithm:      " << s_algorithm[(int)this->algorithm]  << endl;

      cout << "P:              " << this->P                            << endl;
      cout << "Q:              " << this->Q                            << endl;
      cout << "M:              " << this->M                            << endl;
      cout << "Root:           " << this->root                         << endl;
      cout << "Operation:      " << s_operations[(int)this->optype]    << endl;
      
      cout << "Mapping:        " << mapping_s[(int)this->mapping]      << endl;
      cout << "Map file:       " << this->map_file                     << endl;
      
      cout << "Write to file:  " << this->write_file                   << endl;
      cout << "Msglen file:    " << this->msglen_file                  << endl;
      
      cout << "IMB input file: " << this->imb_file                     << endl;
      cout << "Error file:     " << this->error_file                   << endl;
   }
   
   /* 2. Read message lengths to estimate (-msglen, -m) */
   generateMsglen (this->msglen_file, this->sizes);
      
   /* 3. Read map file in case */
   if (this->map_file != "") {
      generateMap (this->map_file, this->map_user);
      this->mapping = Map::User;
   }
   
}


void Benchmark::run () {
   
   TauLopParam::setInstance({"SHM","IB"});
   
   if (this->verbose) {
      printHeader(cout);
   }
   
   this->results.clear();
   
   for (auto const &m: sizes) {
      
      Arguments_t args {m, this->P, this->Q, this->M, this->root, this->optype, this->mapping, this->map_user};
      
      this->benchmark = getBenchmark(this->type, this->algorithm);
      
      if (this->benchmark == nullptr) {
         break;
      }
      
      Results_t res = this->benchmark (args);

      if (this->verbose) {
         printData(cout, res);
      }
      
      this->results.push_back(res);
      
   }
   
}


void Benchmark::write () {

   if (this->write_file != "") {
      if (0 > saveTextFile (this->write_file, this->results)) {
         cerr << "ERROR: write file not open: " << this->write_file << endl;
      }
   }
   
}


void Benchmark::error () {
   
   cout << endl << "*** [Benchmark::error()] NOT implemented by now ***" << endl << endl;
   
   
   if (this->imb_file == "") {
      cerr << "ERROR: need an IMB file to compare: " << this->imb_file << endl;
      return;
   }
   
   if (this->error_file != "") {
      // 1. Open
      // 2. Write error in each m
      if (this->verbose) {
         
      }
      // 3. Close
   }
}

