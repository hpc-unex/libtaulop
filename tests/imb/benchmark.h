//
//  benchs.h
//  IMB-style taulop benchmarks.
//    Print similar output in order to compare results.
//
//  Created by jarico on 30/4/23.
//  Copyright © 2017 jarico. All rights reserved.
//

#ifndef benchs_h_
#define benchs_h_

#include "taulop_kernel.hpp"

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


/*
 *  Arguments to benchmarks
 */
struct Arguments_t {
   int         m;
   int         P;
   int         Q;
   int         M;
   int         root;
   OpType      optype;
   Map         mapping;
   vector<int> map_user;
};


/*
 *  Results of a benchmark
 */
struct Results_t {
   int     m;
   int     repetitions;
   double  latency;
   double  bandwidth;
};
typedef struct Results_t Results_t;


/*
 *  Taulop estimation function prototypes
 */
Results_t  pingpong                     (const Arguments_t &args);
Results_t  bcast_linear                 (const Arguments_t &args);
Results_t  bcast_binomial               (const Arguments_t &args);
Results_t  bcast_binomial_ompi          (const Arguments_t &args);
Results_t  reduce_linear                (const Arguments_t &args);
Results_t  reduce_binomial_ompi         (const Arguments_t &args);
Results_t  allreduce_linear_ompi        (const Arguments_t &args);
Results_t  allreduce_nonoverlap_ompi    (const Arguments_t &args);
Results_t  allreduce_rda_ompi           (const Arguments_t &args);
Results_t  allreduce_ring_ompi          (const Arguments_t &args);
Results_t  allreduce_ringsegm_ompi      (const Arguments_t &args);
Results_t  allreduce_rabenseifner_ompi  (const Arguments_t &args);

//Results_t (*fxn [3]) (const Arguments_t &args) = {pingpong, bcast_linear, reduce_linear};


/* taulop operation strings */
static string s_operations [] = {"default", "max", "min", "sum", "prod", "land", "band", "lor", "bor", "lxor", "bxor", "maxloc", "minloc"};

/* taulop mapping types */
static string mapping_s [5] = {"Default", "Sequential", "RoundRobin", "Random", "User"};

/* Benchmarks supported */
enum class    e_benchmark       { PingPong,   Broadcast,   Reduce,   Allreduce };
static string s_benchmark [4] = {"PingPong", "Broadcast", "Reduce", "Allreduce"};

/* Algorithms supported (depends on benchmarks */
enum class    e_algorithm        { Default,   Linear,   Binomial,   BinomialOMPI,   LinearOMPI,    NonoverlapOMPI,   RecDoublingOMPI,   RingOMPI,   RingSegmOMPI,   RabenseifnerOMPI };
static string s_algorithm [10] = {"Default", "Linear", "Binomial", "BinomialOMPI", "LinearOMPI", "NonoverlapOMPI", "RecDoublingOMPI", "RingOMPI", "RingSegmOMPI", "RabenseifnerOMPI"};


/* Networks supported, in addition to SHM */
enum class    Network_t       { NONET,  TCP,   IB,   ARIES };
static string channels_s[4] = { "SHM", "TCP", "IB", "ARIES"};




// Benchmark to execute: sequence of if-else
static Results_t (*getBenchmark(e_benchmark bench, e_algorithm alg))(const Arguments_t &) {
   
   if (bench == e_benchmark::PingPong) {
      
      return &pingpong;
      
   } else if (bench == e_benchmark::Broadcast) {
      
      if (alg == e_algorithm::Linear) {
         return &bcast_linear;
      }

      if (alg == e_algorithm::Binomial) {
         return &bcast_binomial;
      }

      if (alg == e_algorithm::BinomialOMPI) {
         return &bcast_binomial_ompi;
      }

   } else if (bench == e_benchmark::Reduce) {
      
      if (alg == e_algorithm::Linear) {
         return &reduce_linear;
      }

      if (alg == e_algorithm::BinomialOMPI) {
         return &reduce_binomial_ompi;
      }

   } else if (bench == e_benchmark::Allreduce) {
      
      if (alg == e_algorithm::LinearOMPI) {
         return &allreduce_linear_ompi;
      }

      if (alg == e_algorithm::NonoverlapOMPI) {
         return &allreduce_nonoverlap_ompi;
      }

      if (alg == e_algorithm::RecDoublingOMPI) {
         return &allreduce_rda_ompi;
      }

      if (alg == e_algorithm::RingOMPI) {
         return &allreduce_ring_ompi;
      }

      if (alg == e_algorithm::RingSegmOMPI) {
         return &allreduce_ringsegm_ompi;
      }

      if (alg == e_algorithm::RabenseifnerOMPI) {
         return &allreduce_rabenseifner_ompi;
      }

   }
   
   cout << endl;
   cout << "Benchmarks / Algorithms supported:" << endl;
   cout << "\tPingPong"                         << endl;
   cout << "\tBroadcast   - Linear"             << endl;
   cout << "\tBroadcast   - Binomial"           << endl;
   cout << "\tBroadcast   - BinomialOMPI"       << endl;
   cout << "\tReduce      - Linear"             << endl;
   cout << "\tReduce      - BinomialOMPI"       << endl;
   cout << "\tAllreduce   - LinearOMPI"         << endl;
   cout << "\tAllreduce   - NonoverlapOMPI"     << endl;
   cout << "\tAllreduce   - RecDoublingOMPI"    << endl;
   cout << "\tAllreduce   - RingOMPI"           << endl;
   cout << "\tAllreduce   - RingSegmOMPI"       << endl;
   cout << "\tAllreduce   - RabenseifnerOMPI"   << endl;
   cout << endl;
   
   return nullptr;
}



/*
 *  Class representing a benchmark
 */
class Benchmark {

private:
   
   int               P;
   int               Q;
   int               M;
   
   int               root;
   OpType            optype;
   
   string            name;
   e_benchmark       type;
      
   Results_t (*benchmark) (const Arguments_t &args);
   
   bool              output_screen;
   string            write_file;
   
   e_algorithm       algorithm;
   
   Map               mapping;
   string            map_file;
   vector<int>       map_user;
   
   string            msglen_file;
   vector<int>       sizes;
   
   Network_t         network;
   vector<string>    channels;
   
   string            imb_file;
   string            error_file;
   
   vector<Results_t> results;
   
   bool              verbose;
   bool              help;
 
   
   // Methods:
   
   int   readTextFile   (string filename, vector<string> &values);
   void  generateMsglen (string msglen_file, vector<int> &sizes);
   int   generateMap    (string filename, vector<int> &map);
   int   parseArgs      (int argc, char *argv[]);
   int   saveTextFile   (string filename, vector<Results_t> results);
   void  printHeader    (ostream& outp);
   void  printData      (ostream& outp, Results_t res);
   void  printMapping   (ostream& outp);
   
public:
   
        Benchmark (int argc, char *argv[]);
   void run       ();
   void write     ();
   
};

#endif
