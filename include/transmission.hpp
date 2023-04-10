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

#include "cost_element.hpp"
#include "process.hpp"
//#include "taulop_params.hpp"

#include <iostream>

// Attributes represent a communication in the format:  n x (tau || T^{channel}(m))
// With:
//     n: number of transmissions (tau || T^{channel}(m)).
//     tau: Number of concurrent transmissions T^{channel}(m)
//     channel: Channel of communication
//     m: message size of the transmission

class Transmission : public CostElement {
   
private:
   
   Process p_dst;    // Destination process
   int  channel;     // Channels through which the transmission progress
      
public:
   
          Transmission   ();
   
          Transmission   (const Process &p_src, const Process &p_dst, int channel, int n, int m, int tau);
          Transmission   (const Process &p_src, const Process &p_dst, int n, int m, int tau);
          Transmission   (const Process &p_src, const Process &p_dst, int m, int tau);

          Transmission   (int channel, int n, int m, int tau);
          Transmission   (int channel, int m, int tau);

          Transmission   (const Transmission *c);
   
         ~Transmission   ()  override;
   
   Transmission *clone   ()             const  override;
   CEType getType        ()             const  override;

   void   putProcDst     (const Process &p);
   
   int    getMsgSize     ()             const;
   
   void   putChannel     (int channel);
   int    getChannel     ()             const;
   
   int    getRank        ()             const  override;  // Source rank
   int    getDstRank     ()             const;
   
   int    getDstNode     ()             const;
      
   void   incrTau        (int inc = 1);
   void   initTau        ();
   
   double getCost        ()                   const  override;  // Cost of the blocks in c
   long   getBytes       (double t, int tau)  const  override; // Inverse: bytes sent in time t when tau concurrent
   
   void   getOverlap     (const CostElement *c)      override;
   
   bool   areCompactable (const Transmission *c);
   void   compact        (const Transmission *c);

   bool   areSequential  (const Transmission *c);
   void   add            (const Transmission *c);
   
   void   show           ()  const  override;
   void   notate         ()  const  override;
   
   Transmission& operator=  (const Transmission &c);
};

#endif /* transmission_hpp */
