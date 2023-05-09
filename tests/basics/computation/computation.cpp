//
//  computations.cpp
//   Performs some computations
//
//  Created by jarico on 9/5/23.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"
#include "args.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
using namespace std;






// T^0(m) + g^{suma}(m)]
double suma_seq (const TaulopArgs &args) {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq = nullptr;
   
   int m = args.m;

   seq = new TauLopSequence ();
   seq->add(new Transmission {0, m, 1});
   seq->add(new Computation  {m, OpType::SUM});
   concurrent->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
      
   if (args.verbose) {
      tc->show();
   }

   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}


// T^0(m) + g^{prod}(m)]
double prod_seq (const TaulopArgs &args) {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq = nullptr;
   
   int m = args.m;
   
   seq = new TauLopSequence ();
   seq->add(new Transmission {0, m, 1});
   seq->add(new Computation  {m, OpType::PROD});
   concurrent->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   if (args.verbose) {
      tc->show();
   }

   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}


// T^0(m) + g^{suma}(m) + T^0(m)
double suma_p2p (const TaulopArgs &args) {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq = nullptr;
   
   int m = args.m;
   
   seq = new TauLopSequence ();
   seq->add(new Transmission {0, m, 1});
   seq->add(new Computation  {m, OpType::SUM});
   seq->add(new Transmission {0, m, 1});
   concurrent->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   if (args.verbose) {
      tc->show();
   }
   
   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}


// T^0(m) || g^{suma}(m)]
double suma_conc (const TaulopArgs &args) {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq = nullptr;
   
   int m = args.m;
   
   seq = new TauLopSequence ();
   seq->add(new Transmission {0, m, 1});
   concurrent->add(seq);
   
   seq = new TauLopSequence ();
   seq->add(new Computation  {args.m, OpType::SUM});
   concurrent->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   if (args.verbose) {
      tc->show();
   }
   
   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}


// (P-1) * [T^0(m) + g^{suma}(m)]
double suma_coll (const TaulopArgs &args) {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq = nullptr;
   
   int m = args.m;
   
   seq = new TauLopSequence ();
   for (int i = 0; i < args.P-1; i++) {
      seq->add(new Transmission {0, m, 1});
      seq->add(new Computation  {m, OpType::SUM});
   }
   concurrent->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   if (args.verbose) {
      tc->show();
   }
   
   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}



// g^{max}(m1) || g^{sum}(m2) || g^{band}(m3)
double max_conc (const TaulopArgs &args) {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq = nullptr;
      
   seq = new TauLopSequence ();
   seq->add(new Computation  {1024, OpType::MAX});
   concurrent->add(seq);
   
   seq = new TauLopSequence ();
   seq->add(new Computation  {2000, OpType::SUM});
   concurrent->add(seq);

   seq = new TauLopSequence ();
   seq->add(new Computation  {500, OpType::BAND});
   concurrent->add(seq);

   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   if (args.verbose) {
      tc->show();
   }

   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}


// n || g^{max}(m)
double gamma_xN (int n, const TaulopArgs &args) {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq = nullptr;
      
   srand(42);
   for (int i = 0; i < n; i++) {
      seq = new TauLopSequence ();
      int m = rand() / (1024 * 1024);
      seq->add(new Computation {m, OpType::MAX});
      concurrent->add(seq);
   }

   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   if (args.verbose) {
      tc->show();
   }

   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}



/****************************
 *           MAIN
 ****************************
 */
int main (int argc, char *argv[]) {
   
   double t = 0.0;
   
   srand(time(NULL));
   
   // Network parameters
   TauLopParam::setInstance({"SHM","IB"});
   
   // Params
   TaulopArgs &args = TaulopArgs::GetArgs(argc, argv);

   
   // Example:  T^0(m) + g^{suma}(m)
   t = suma_seq (args);
   args.printTime(t, "T^0(m) + g^{suma}(m)");
   
   // Example:  T^0(m) + g^{prod}(m)
   t = prod_seq (args);
   args.printTime(t, "T^0(m) + g^{prod}(m)");
   
   // Example:  T^0(m) + g^{suma}(m) + T^c0(m)
   t = suma_p2p (args);
   args.printTime(t, "T^0(m) + g^{suma}(m) + T^c0(m)");

   // Example:  T^0(m) || g^{suma}(m)
   t = suma_conc (args);
   args.printTime(t, "T^0(m) || g^{suma}(m)");

   // Example:  T^0(m) + g^{suma}(m) + T^c0(m)
   t = suma_coll (args);
   args.printTime(t, "(P-1) * [T^0(m) + g^{suma}(m)]");

   // g^{max}(m1) || g^{sum}(m2) || g^{band}(m3)
   t = max_conc (args);
   args.printTime(t, "g^{max}(m1) || g^{sum}(m2) || g^{band}(m3)");
   
   // n || g^{max}(m)
   t = gamma_xN (4, args);
   args.printTime(t, "n || g^{max}(m)");
      
   return 0;
}
