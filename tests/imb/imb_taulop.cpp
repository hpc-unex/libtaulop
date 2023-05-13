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
                      MAIN
 **************************************************/

int main (int argc, char * argv[]) {
   
   Benchmark b (argc, argv);
   
   b.run();
   b.write();
      
   return 0;
}
