//
//  transmission.hpp
//  TauLopCost
//
//  Created by jarico on 3/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//
//   Change the nemae Comm -> Transmission
//

#ifndef transmission_hpp
#define transmission_hpp

#include "process.hpp"
#include "taulop_params.hpp"

#include <iostream>

// Attributes represent a communication in the format:  n x (tau || T^{channel}(m))
// With:
//     n: number of transmissions (tau || T^{channel}(m)).
//     tau: Number of concurrent transmissions T^{channel}(m)
//     channel: Channel of communication
//     m: message size of the transmission

class Transmission {
   
private:
   
   Process * p_src;  // Source process
   Process * p_dst;  // Destination process
   int  node_src;    // Source node
   int  node_dst;    // Destination node
   int  channel;     // Channels through which the transmission progress
   int  n;           // Number of transmission (if more than one)
   long m;           // Message size
   int  tau;         // Number of concurrent transmissions
   
   TauLopParam *params;
   
public:
   
      Transmission   ();
      Transmission   (Process *p_src, Process *p_dst, int channel, int n, int m, int tau);
      Transmission   (Process *p_src, Process *p_dst, int n, int m, int tau);
      Transmission   (Process *p_src, Process *p_dst, int m, int tau);
      Transmission   (int channel, int n, int m, int tau);
      Transmission   (int channel, int m, int tau);
      Transmission   (const Transmission *c);
      Transmission   (const Transmission &c);
     ~Transmission   ();
   
   void   putProcSrc (Process *p);
   void   putProcDst (Process *p);
   
   int    getMsgSize ();
   
   void   putChannel (int channel);
   int    getChannel ();
   
   int    getSrcRank ();
   int    getDstRank ();
   
   int    getSrcNode ();
   int    getDstNode ();
   
   void   putM       (long m);
   long   getM       ();
   
   void   putN       (int n);
   int    getN       ();
   
   void   incrTau    (int inc = 1);
   void   initTau    ();
   int    getTau     ();
   
   double getCost    ();  // Cost of the blocks in c
   long   getBytes   (double t, int tau); // Inverse: bytes sent in time t when tau concurrent
   
   bool   areConcurrent  (const Transmission *c);
   void   getOverlap     (const Transmission *c);
   
   bool   areCompactable (const Transmission *c);
   void   compact        (const Transmission *c);
   
   bool   areSequential  (const Transmission *c);
   void   add            (const Transmission *c);
   
   void   show ();
   
   Transmission & operator=  (const Transmission &c) {
      
      this->p_src = c.p_src;
      this->p_dst = c.p_dst;
      this->node_src = c.node_src;
      this->node_dst = c.node_dst;
      this->channel  = c.channel;
      this->n        = c.n;
      this->m        = c.m;
      this->tau      = c.tau;
      this->params   = TauLopParam::getInstance();

      return *this;
   }
};

#endif /* transmission_hpp */
