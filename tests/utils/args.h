//
//  args.cpp
//   Read arguments to execute tests (util functions)
//
//  Created by jarico on 09/05/23.
//  Copyright © 2017 jarico. All rights reserved.
//

#ifndef taulop_args_h_
#define taulop_args_h_

#include "taulop_kernel.hpp"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <getopt.h>
#include <unistd.h>

using namespace std;



/* Class to store arguments to main for execute tests */

class TaulopArgs {
   
private:
   // Units to show results
   enum class units {s, ms, us, ns, fs};
   
   // Singleton pattern: private constructor.
   TaulopArgs (int arg, char **argv);
   
   // Instance to get
   static TaulopArgs *taulop_args;
   
   static bool initialized;
      
public:
   
   // Public attributes options.
   //  (OOP warning !)
   int     P;
   int     Q;
   int     M;
   int     m;
   int     root;
   OpType  op;
   units   unit;
   Map     mapping;
   bool    verbose;
   
   // Print time in specified units
   void printTime (double t, string legend = {});
   
   // Get arguments
   static TaulopArgs& GetArgs (int argc, char **argv);
};

#endif

