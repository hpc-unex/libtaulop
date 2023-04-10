//
//  computation.hpp
//
//  Created by jarico on 7/4/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//
//

#ifndef computation_hpp
#define computation_hpp

#include "cost_element.hpp"
#include "process.hpp"
//#include "taulop_params.hpp"

#include <iostream>


// MPI operation types
// DEFAULT = SUM, arbitrary.
enum class OpType {DEFAULT, MAX, MIN, SUM, PROD, LAND, BAND, LOR, BOR, LXOR, BXOR, MAXLOC, MINLOC};


// Attributes represent a Computation in the format:  n x (tau || g(m))
// With:
//     n: number of transmissions (tau || g(m)).
//     tau: Number of concurrent transmissions g(m)
//     m: message size of the transmission

class Computation : public CostElement {
   
private:
   
   Process p_dst;    // Destination process
   OpType  opType;   // Operation type
      
public:
   
          Computation   ();
   
          Computation   (const Process &p_src, int n, int m, int tau);
          Computation   (const Process &p_src, int n, int m, int tau, OpType opType);
   
          Computation   (const Process &p_src, int m, int tau);
          Computation   (const Process &p_src, int m, int tau, OpType opType);
          
   		 Computation   (int m, int tau);
          Computation   (int m, int tau, OpType opType);
   
          Computation   (const Computation *c);
   
         ~Computation   ()         override;
   
   Computation *clone   ()  const  override;
   CEType       getType ()  const  override;

   OpType getOpType     ()  const;
   
   void   putProcDst    (const Process &p);
   
   int    getMsgSize    ()  const;
   
   int    getRank       ()  const  override;
      
   void   incrTau       (int inc = 1);
   void   initTau       ();
      
   double getCost       ()                   const  override;  // Cost of the blocks in c
   long   getBytes      (double t, int tau)  const  override; // Inverse: bytes sent in time t when tau concurrent
   
   void   getOverlap     (const CostElement *c)  override;
   
   bool   areCompactable (const Computation *c);
   void   compact        (const Computation *c);

   bool   areSequential  (const Computation *c);
   void   add            (const Computation *c);
      
   void   show           ()  const  override;
   void   notate         ()  const  override;
   
   Computation& operator=  (const Computation &c);
};

#endif /* computation_hpp */
