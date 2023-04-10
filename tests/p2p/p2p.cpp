//
//  main.cpp
//  test_p2p
//
//  Created by jarico on 25/4/17.
//  Copyright © 2017 jarico. All rights reserved.
//

#include "taulop_kernel.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
using namespace std;


//  T^c(m)
double ex1_p2p () {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq        = new TauLopSequence   ();
   
   int chn = 1;
   int m   = 128 * 1024;
   
   Transmission *c = new Transmission(chn, m, 1);
   seq->add(c);
   concurrent->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   tc->show();
   
   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}


//  T^c0(m1) + T^c1(m2)
double ex2_p2p () {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq        = new TauLopSequence   ();
   
   int m1  = 64  * 1024;
   int m2  = 256 * 1024;
   
   seq->add(new Transmission(0, m1, 1));
   seq->add(new Transmission(1, m2, 1));
   
   concurrent->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   tc->show();
   
   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}



//  T^c(m) || T^c(m)
double ex3_p2p () {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq;
   
   int m   = 128 * 1024;
   int chn = 1;
   
   seq = new TauLopSequence ();
   seq->add(new Transmission(chn, m, 1));
   concurrent->add(seq);
   
   seq = new TauLopSequence ();
   seq->add(new Transmission(chn, m, 1));
   concurrent->add(seq);
   
   
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   tc->show();
   
   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}


//  T^c(m1) || T^c(m2)
double ex4_p2p () {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq        = new TauLopSequence   ();
   
   Process *src = new Process(0);
   src->setNode(0);
   Process *dst = new Process(1);
   dst->setNode(1);
   
   int chn = (src->getNode() == dst->getNode()) ? 0 : 1;
   int n = 1; // Number of messages
   int m = 128 * 1024; // Message size in bytes
   
   Transmission *c = new Transmission(src, dst, chn, n, m, 2);
   seq->add(c);
   concurrent->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   tc->show();
   
   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}




//  T^c(m1) || T^c(m2)
double ex5_p2p () {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq;
   
   int m1 = 128;
   int m2 = 192;
   
   seq = new TauLopSequence ();
   seq->add(new Transmission(1, m1, 1));
   concurrent->add(seq);
   
   seq = new TauLopSequence ();
   seq->add(new Transmission(1, m2, 1));
   concurrent->add(seq);
   
   
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   tc->show();
   
   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}




//  T^c0(m1) || T^c1(m2)
double ex6_p2p () {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq;
   
   int m1 = 128;
   int m2 = 192;
   
   seq = new TauLopSequence ();
   seq->add(new Transmission(0, m1, 1));
   concurrent->add(seq);
   
   seq = new TauLopSequence ();
   seq->add(new Transmission(1, m2, 1));
   concurrent->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   tc->show();
   
   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}


// [T^c0(m1) + T^c1(m2)] || [T^c0(m3) + T^c1(m4)]
double ex7_p2p () {
   
   TauLopConcurrent *concurrent = new TauLopConcurrent ();
   TauLopSequence   *seq;
   
   int m1 = 128;
   int m2 = 192;
   int m3 = 256;
   int m4 = 64;
   
   seq = new TauLopSequence ();
   seq->add(new Transmission(0, m1, 1));
   seq->add(new Transmission(1, m2, 1));
   concurrent->add(seq);
   
   seq = new TauLopSequence ();
   seq->add(new Transmission(0, m3, 1));
   seq->add(new Transmission(1, m4, 1));
   concurrent->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   concurrent->evaluate(tc);
   
   tc->show();
   
   double t = tc->getTime();
   
   delete tc;
   delete concurrent;
   
   return t;
}




// (T^1(m_a) + T^0(m_b) + T^1(m_b)) || (T^0(m_b) + T^1(m_a) || (T^1(m_a) + T^0(m_b))
double ex8_p2p () {
   
   TauLopConcurrent *conc = new TauLopConcurrent ();
   TauLopSequence   *seq;
   
   int ma = 128 * 1024;
   int mb =  64 * 1024;
   
   seq = new TauLopSequence ();
   seq->add(new Transmission(1, ma, 1));
   seq->add(new Transmission(0, mb, 1));
   seq->add(new Transmission(1, mb, 1));
   conc->add(seq);
   
   seq = new TauLopSequence ();
   seq->add(new Transmission(0, mb, 1));
   seq->add(new Transmission(1, ma, 1));
   conc->add(seq);
   
   seq = new TauLopSequence ();
   seq->add(new Transmission(1, ma, 1));
   seq->add(new Transmission(0, mb, 1));
   conc->add(seq);
   
   TauLopCost *tc = new TauLopCost();
   conc->evaluate(tc);
   
   tc->show();
   
   double t = tc->getTime();
   
   delete tc;
   delete conc;
   
   return t;
}







int main (int argc, const char * argv[]) {
   
   TauLopParam::setInstance({"SHM","IB"});
   
   double t = 0.0;
   
   // Example 1:  T^c(m)
   t = ex1_p2p ();
   cout << "T^c(m) - Time:               " << fixed << std::setprecision (9) << t << " usec." << endl << endl;
   
   // Example 2:  T^c0(m1) + T^c1(m2)
   t = ex2_p2p ();
   cout << "T^c0(m1) + T^c1(m2) - Time:  " << fixed << std::setprecision (9) << t << " usec." << endl << endl;
   
   // Example 3:  T^c(m) || T^c(m)
   t = ex3_p2p ();
   cout << "T^c(m) || T^c(m) - Time:     " << fixed << std::setprecision (9) << t << " usec." << endl << endl;
   
   // Example 4:  2 || T^c(m)  (same cost as example 3)
   t = ex4_p2p ();
   cout << "2 || T^c(m) - Time:          " << fixed << std::setprecision (9) << t << " usec." << endl << endl;
   
   // Example 5:  T^c(m1) || T^c(m2)
   t = ex5_p2p ();
   cout << "T^c(m1) || T^c(m2) - Time:   " << fixed << std::setprecision (9) << t << " usec." << endl << endl;
   
   // Example 6:  T^c0(m1) || T^c1(m2)
   t = ex6_p2p ();
   cout << "T^c0(m1) || T^c1(m2) - Time: " << fixed << std::setprecision (9) << t << " usec." << endl << endl;
   
   // Example 7:  [T^c0(m1) + T^c1(m2)] || [T^c0(m3) + T^c1(m4)]
   t = ex7_p2p ();
   cout << "[T^c0(m1) + T^c1(m2)] || [T^c0(m3) + T^c1(m4)] - Time: " << fixed << std::setprecision (9) << t << " usec." << endl << endl;
   
   // Example 8:  (T^1(m_a) + T^0(m_b) + T^1(m_b)) || (T^0(m_b) + T^1(m_a) || (T^1(m_a) + T^0(m_b))
   t = ex8_p2p ();
   cout << "(T^1(m_a) + T^0(m_b) + T^1(m_b)) || (T^0(m_b) + T^1(m_a) || (T^1(m_a) + T^0(m_b)) - Time: " << fixed << std::setprecision (9) << t << " usec." << endl << endl;
   
   
   return 0;
}
