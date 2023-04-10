//
//  taulop_params.cpp
//
//  Created by jarico on 15/5/16.
//   Modified by jarico on 09/04/23

//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_params.hpp"
#include "config.h"

#include <cmath>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;


// Initialization of static variables
TauLopParam* TauLopParam::single = nullptr;
vector<string> TauLopParam::channel_names;

// In taulop_param_channel.cpp:
extern vector<string> split(string strToSplit, char delimeter);


// PRIVATE methods

//private constructor
TauLopParam::TauLopParam() {
   
   // Create communication channels information
   int num = 0;
   for (auto it = TauLopParam::channel_names.begin(); it != TauLopParam::channel_names.end(); ++it, num++) {
      this->channel.push_back(new TaulopParamChannel (*it, num));
   }
   
   // Take data from the first channel (we assume that all channels have the next same values)
   this->max_tau = this->channel[0]->getNumTau(); // TBD
   this->max_idx = this->channel[0]->getNumM();   // TBD
   this->sizes   = this->channel[0]->getSizes();
   
   // Create the matrices to store the data of P2P transmissions
   for (int channel_nr = 0; channel_nr < this->channel.size(); channel_nr++) {
      double **chn = new double * [this->channel[channel_nr]->getNumTau()];
      for (int i = 0; i < this->max_tau; i++) {
         chn[i] = new double [this->max_idx];
      }
      this->p2p.push_back(chn);
   }
   
   // Set P2P transmission times values
   this->setP2P();

   // Load Gamma values
   this->loadGamma();
   
   
#if TLOP_DEBUG == 1
   this->show();
#endif
}



void TauLopParam::setP2P  () {
   
   int chn_nr = 0;
   
   for (auto it = TauLopParam::channel_names.begin(); it != TauLopParam::channel_names.end(); ++it, chn_nr++) {
      
      double **P2P = this->p2p[chn_nr];
      
      for (int tau = 0; tau < this->max_tau; tau++) {
         
         for (int idx = 0; idx < this->max_idx; idx++) {
            
            // Cost depends on the channel type (name)
            // TBD: this cost formulation should be provided by the user.
            
            double T = 0.0;
            string chn_type = *it;
            
            if (chn_type == SHM_NET) {
               
               double o0 = this->channel[chn_nr]->getO(idx);
               double L0 = this->channel[chn_nr]->getL(idx, tau);
               
               T = o0 + (2 * L0);
               
            } else if (chn_type == IB_NET) {
               
               double o1 = this->channel[chn_nr]->getO(idx);
               double L1 = this->channel[chn_nr]->getL(idx, tau);
               
               T = o1 + L1;
               //T = L1;  // RICO: PROB. MENSAJES PEQUEÑOS
               
            } else if (chn_type == TCP_NET) {
               
               double o1 = this->channel[chn_nr]->getO(idx);
               double L0 = this->channel[0]->getL(idx, tau);
               double L1 = this->channel[chn_nr]->getL(idx, tau);
               
               T = o1 + (2 * L0) + L1;  // RICO: PROB. MENSAJES PEQUEÑOS
                                        //T = (2 * L0) + L1;
               
            } else {
               cerr << "ERROR: channel name not known: " << chn_type << endl;
            }
            
            P2P[tau][idx] = T;
         }
      }
      
   }
}


void TauLopParam::loadGamma () {
   
   ifstream       ifs;
   vector<string> v;
   
   char delimiter = '\t';
   
   string params_folder (PARAMS_FOLDER);
   string name = params_folder + "/gamma.txt";
   
   ifs.open(name);
   if (!ifs.is_open()) {
      cout << "ERROR: unable to open file containing parameters: " << name << endl;
      return;
   }
   
   string line;
   
   // Read number of m    (first line)
   getline(ifs, line);
   v = split(line, delimiter);
   this->num_m = stoi(v[1]);
   
   // Read number of ops  (second line)
   getline(ifs, line);
   v = split(line, delimiter);
   this->max_ops = stoi(v[1]) + 1;  // Default operation is index 0
   
   
   // 1. Read parameters
   //    File must include an entry for m=1 (first line)

   // 1a. Make room for gamma matrix
   // Operations: Default, MAX, MIN, SUM, PROD, LAND, BAND, LOR, BOR, LXOR, BXOR, MAXLOC, MINLOC
   this->gamma = new double * [this->max_ops];
   for (int op = 0; op < this->max_ops; op++) {
      this->gamma[op] = new double [this->max_idx];
   }
   
   
   int idx = 0;
   while (!ifs.eof()) {
      
      getline(ifs, line);
      if (line[0] == '#') continue;
      
      v = split(line, delimiter);
      
      // Sizes is in position 0. It should match the P2P vectors.
      //this->sizes.push_back(stof(v[0]));
      
      // gamma per ops
      for (int op = 0; op < this->max_ops; op++) {  // First value is the size
         
         string token = v[op];
         
         if (op == 0) { // Check size
            
            if (stof(token) != this->sizes[idx]) {
               cerr << "ERROR: Sizes in p2p and gamma does not match." << endl;
               return;
            }
            
         } else {
            
            this->gamma[op][idx] = stof(token); // First position is for default operation
            
            if (op == sum) {
               this->gamma[0][idx] = this->gamma[sum][idx]; // hardcoded default = sum.
            }
            
         }
      }
      
      idx++;
      if (idx == this->num_m) break;
   }
   
   if (idx != this->num_m) {
      cout << "ERROR: file corrupt reading parameters: " << name << endl;
   }
   
   ifs.close();
}


 

// PUBLIC interface

TauLopParam::~TauLopParam () {
   
   TauLopParam::single = nullptr;
   
   for (int chn_nr; chn_nr < TauLopParam::channel_names.size(); chn_nr++) {
      delete this->channel[chn_nr];
   }
   
   for (auto it = this->p2p.begin(); it != this->p2p.end(); ++it) {
      
      double **p2p;
      for (int i = 0; i < this->max_tau; i++) {
         delete [] p2p[i];
      }
      delete [] p2p;
   }
   
   for (int op = 0; op < this->max_ops; op++) {
      delete [] this->gamma[op];
   }
   delete [] this->gamma;

}


void TauLopParam::setInstance(vector<string> networks) {
   
   if (!TauLopParam::single) {
      
      TauLopParam::channel_names = networks;
      TauLopParam::single        = new TauLopParam();
      
   } else {
      
      cerr << "ERROR: network parameters already loaded from: " << endl;
      
      for (auto i = TauLopParam::channel_names.begin(); i != TauLopParam::channel_names.end(); ++i) {
         cout << *i << endl;
      }
      
   }
}


TauLopParam* TauLopParam::getInstance() {
   
   if (!TauLopParam::single) {
      TauLopParam::single = new TauLopParam();
      cerr << "ERROR: no instance has been created by now." << endl;
      return nullptr;
   }
   
   return single;
}


double TauLopParam::getTime (long m, int tau, int chn) {
   
   double   t = 0.0;
   double **p2p;
   int      idx;
   

#if TLOP_DEBUG == 1
   if (m <= 0) {
      cerr << "ERROR: message size must be greater than 0: " << m << endl;
      return 0.0;
   }
   
   if (chn >= TauLopParam::channel_names.size()) {
      cerr << "ERROR: unknown channel: " << chn << endl;
      return -1;
   }
#endif
   
   // TODO: This error is common. Do not know why yet.
   if (tau > this->max_tau) {
#if TLOP_DEBUG == 1
      cerr << "ERROR: value of tau is too high: " << tau << " (" << this->max_tau << ")" << endl;
#endif
      tau = max_tau;
      //return -1;
   }

   
   p2p = this->p2p[chn];
   
   for (idx = 0; idx < this->max_idx; idx++) {
      if (this->sizes[idx] >= m) break;
   }
   
   if (this->sizes[idx] == m) {
      t = p2p[tau-1][idx];
   }
   
   else if (idx == 0) {
      t = p2p[tau-1][0];
   }
   
   else if (idx == this->max_idx) {
      t = p2p[tau-1][idx-1] + getTime(m - this->sizes[this->max_idx-1], tau, chn);
   }
   
   else {
      
      int idx_up = idx;
      int idx_dw = idx - 1;
      
      double t_up = p2p[tau-1][idx_up];
      double t_dw = p2p[tau-1][idx_dw];
      
      long n_up = this->sizes[idx_up];
      long n_dw = this->sizes[idx_dw];
      
      t = t_dw + ((m - n_dw) * (t_up - t_dw)) / (n_up - n_dw);
      
      // Security rule: if T < 0, t_up < t_dw, then use t_dw.
      if (t < 0) {
         t = t_dw;
      }
   }
   
   return t;
}


double TauLopParam::getTime  (long n, int op) {
   
   double g = 0.0;
   int idx;
   
#if TLOP_DEBUG == 1
   if (n <= 0) {
      cerr << "ERROR: message size must be greater than 0: " << n << endl;
      return 0.0;
   }
      
   if (op >= this->max_ops) {
      cerr << "ERROR: value of operation is too high: " << op << " (" << this->max_ops << ")" << endl;
      op = 0;
   }
#endif
   
   for (idx = 0; idx < this->max_idx; idx++) {
      if (this->sizes[idx] >= n) break;
   }
   
   if (this->sizes[idx] == n) {
      g = this->gamma[op][idx];
   }
   
   else if (idx == 0) {
      g = this->gamma[op][0];
   }
   
   else if (idx == this->max_idx) {
      g = this->gamma[op][idx-1] + getTime(n - this->sizes[this->max_idx-1], op);
   }
   
   else {
      
      int idx_up = idx;
      int idx_dw = idx - 1;
      
      double t_up = this->gamma[op][idx_up];
      double t_dw = this->gamma[op][idx_dw];
      
      long n_up = this->sizes[idx_up];
      long n_dw = this->sizes[idx_dw];
      
      g = t_dw + ((n - n_dw) * (t_up - t_dw)) / (n_up - n_dw);
      
      // Security rule: if T < 0, t_up < t_dw, then use t_dw.
      if (g < 0) {
         g = t_dw;
      }
   }
   
   return g;
}


long TauLopParam::getBytes (double t, int tau, int chn) {
   
   long     m = 0;
   int      idx;
   double **p2p;
   
#if TLOP_DEBUG == 1
   if (t <= 0.0) {
      cerr << "ERROR: time must be greater than 0.0: " << t << endl;
      return 0;
   }
   
   if (chn >= TauLopParam::channel_names.size()) {
      cerr << "ERROR: unknown channel: " << chn << endl;
      return -1;
   }
#endif
   
   // TODO: This error is common. Do not know why yet.
   if (tau > this->max_tau) {
#if TLOP_DEBUG == 1
      cerr << "ERROR: value of tau is too high: " << tau << " (" << this->max_tau << ")" << endl;
#endif
      tau = max_tau;
      //return -1;
   }
   
   p2p = this->p2p[chn];
   
   for (idx = 0; idx < this->max_idx; idx++) {
      if (p2p[tau-1][idx] > t) break;
   }
   
   if (idx == 0) {
      if (p2p[tau-1][idx] > t * 100) {  // If not significant, return 0.
         return 0;
      }
      return 1; // Time short. Only 1 block.
   }
   
   if (idx == this->max_idx) {
      
      if (t == p2p[tau-1][this->max_idx-1]) {
         
         m = this->sizes[this->max_idx-1];
         
      } else {
         
         double t_max = p2p[tau-1][this->max_idx-1];
         m = this->sizes[this->max_idx-1] + getBytes(t - t_max, tau, chn);
         
      }
      
   } else { // Time is in an interval
      
      long b_up = this->sizes[idx];
      double t_up = p2p[tau-1][idx];
      
      long b_dw = this->sizes[idx-1];
      double t_dw = p2p[tau-1][idx-1];
      
      m = b_dw + ceil((t - t_dw) * double(b_up - b_dw) / (t_up - t_dw));
      
      if (m < 1) {
         m = 1;
      }
   }
   
   return m;
}


long TauLopParam::getBytes (double t, int op) {
   
   long  b = 0;
   int   idx;
   
#if TLOP_DEBUG == 1
   if (t <= 0.0) {
      cerr << "ERROR: time must be greater than 0.0: " << t << endl;
      return 0;
   }

   // TODO: what if raise an error here?
   if (op >= this->max_ops) {
      cerr << "ERROR: value of operation is too high: " << op << " (" << this->max_ops << ")" << endl;
      op = 0;
   }
#endif
   
   for (idx = 0; idx < this->max_idx; idx++) {
      if (this->gamma[op][idx] > t) break;
   }
   
   if (idx == 0) {
      if (this->gamma[op][idx] > t * 100) {  // If not significant, return 0.
         return 0;
      }
      return 1; // Time short. Only 1 byte.
   }
   
   if (idx == this->max_idx) {
      
      double t_max = this->gamma[op][this->max_idx-1];
      b = this->sizes[this->max_idx-1] + getBytes(t - t_max, op);
      
   } else { // Time is in an interval
      
      long b_up = this->sizes[idx];
      double t_up = this->gamma[op][idx];
      
      long b_dw = this->sizes[idx-1];
      double t_dw = this->gamma[op][idx-1];
      
      b = b_dw + ceil((t - t_dw) * (b_up - b_dw)) / (t_up - t_dw);
      
      if (b < 1) {
         b = 1;
      }
   }
   
   return b;
}


void TauLopParam::show () const {
   
   int chn_nr = 0;
   
   for (auto it = TauLopParam::channel_names.begin(); it != TauLopParam::channel_names.end(); ++it, chn_nr++) {
      
      cout << "Channel name:  " << *it << endl;
      double **P2P = this->p2p[chn_nr];
      
      for (int i = 0; i < this->max_idx; i++) {
         
         cout << this->sizes[i] << ")  ";
         
         for (int j = 0; j < this->max_tau; j++) {
            cout << fixed << setprecision(9) << P2P[j][i] << " \t ";
         }
         cout << endl;
      }
      cout << endl;
      
   }
   
   // Gamma:
   cout << "Gamma: " << endl;
   for (int i = 0; i < this->max_idx; i++) {
      
      cout << this->sizes[i] << ")  ";
      
      for (int j = 0; j < this->max_ops; j++) {
         cout << fixed << setprecision(9) << this->gamma[j][i] << " \t ";
      }
      cout << endl;
   }
   cout << endl;

}




