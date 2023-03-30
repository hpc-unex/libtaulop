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
   
   Process p_src;    // Source process
   Process p_dst;    // Destination process
   int  node_src;    // Source node
   int  node_dst;    // Destination node
   int  channel;     // Channels through which the transmission progress
   int  n;           // Number of transmission (if more than one)
   long m;           // Message size
   int  tau;         // Number of concurrent transmissions
   
   TauLopParam *params;
   
public:
   
      Transmission   ();
      //Transmission   (Process *p_src, Process *p_dst, int channel, int n, int m, int tau);
      Transmission   (const Process &p_src, const Process &p_dst, int channel, int n, int m, int tau);
      Transmission   (Process *p_src, Process *p_dst, int n, int m, int tau);
      Transmission   (Process *p_src, Process *p_dst, int m, int tau);
      Transmission   (int channel, int n, int m, int tau);
      Transmission   (int channel, int m, int tau);
      Transmission   (const Transmission *c);
      Transmission   (const Transmission &c);
     ~Transmission   ();
   
   void   putProcSrc (const Process &p);
   void   putProcDst (const Process &p);
   
   int    getMsgSize ()  const;
   
   void   putChannel (int channel);
   int    getChannel ()  const;
   
   int    getSrcRank ()  const;
   int    getDstRank ()  const;
   
   int    getSrcNode ()  const;
   int    getDstNode ()  const;
   
   void   putM       (long m);
   long   getM       ()  const;
   
   void   putN       (int n);
   int    getN       ()  const;
   
   void   incrTau    (int inc = 1);
   void   initTau    ();
   int    getTau     ()  const;
   
   double getCost    ()  const;  // Cost of the blocks in c
   long   getBytes   (double t, int tau)  const; // Inverse: bytes sent in time t when tau concurrent
   
   bool   areConcurrent  (const Transmission *c);
   bool   areConcurrent  (const Transmission &c);
   void   getOverlap     (const Transmission *c);
   void   getOverlap     (const Transmission &c);
   
   bool   areCompactable (const Transmission *c);
   void   compact        (const Transmission *c);

   bool   areCompactable (const Transmission &c);
   void   compact        (const Transmission &c);

   bool   areSequential  (const Transmission *c);
   void   add            (const Transmission *c);
   
   Transmission& operator=  (const Transmission &c);
   
   void   show           ()  const;
};

#endif /* transmission_hpp */
