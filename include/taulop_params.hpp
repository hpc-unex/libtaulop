//
//  taulop_params.hpp
//
//  Created by jarico on 15/5/16.
//   Modified by jarico on 09/04/23
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef taulop_params_hpp
#define taulop_params_hpp

#include "taulop_config.h"
#include "taulop_params_channel.hpp"

#include <iostream>
#include <vector>
using namespace std;


enum gammaOp {def, max, min, sum, prod, land, band, lor, bor, lxor, bxor, maxloc, minloc};


// TBD: Comment the attributes and methods.


// Singleton pattern
class TauLopParam {
   
private:
   
   vector<TaulopParamChannel *> channel;
   
   // P2P cost in both channels. (TBD: maybe it must be a class for any alg.)
   vector<double **> p2p;
   
   vector<long> sizes;
   
   double ** gamma;  // Computation values (m rows x ops cols)
   
   int  max_idx;
   int  max_tau;
   int  max_ops;
   int  num_m;
   
   static bool instanceFlag;
   static TauLopParam *single;
   
   static vector<string> channel_names;
   
   
   TauLopParam(); //private constructor
   
   void  setP2P ();
   void  loadGamma  ();
   
public:
   
   static void setInstance(vector<string> channel_names);
   static TauLopParam* getInstance();
   
   ~TauLopParam();
   
   // For Transmissions
   double  getTime  (long   m, int tau, int chn);
   long    getBytes (double t, int tau, int chn);
   
   // For computations
   double  getTime  (long n, int op);
   long    getBytes (double t, int op);
   
   void show () const;
};



#endif /* taulop_params_hpp */
