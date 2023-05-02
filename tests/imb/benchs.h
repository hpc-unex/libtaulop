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

using namespace std;


/* Global verbose */
static int verbose = 0;


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
 *  IMB benchmarks function pointers
 */
struct Benchmarks_t {
   vector<Results_t> (*PingPong)  (vector<int> msglen);
   vector<Results_t> (*Broadcast) (vector<int> msglen);
};
typedef struct Benchmarks_t Benchmarks_t;

/*
 *  Taulop estimation function prototypes
 */

vector<Results_t> pingpong (vector<int> msglen);


#endif

