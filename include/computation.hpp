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
#include "coll_params.hpp"

#include <iostream>



// Attributes represent a Computation in the format:  n x (tau || g(m))
// With:
//     n: number of transmissions (tau || g(m)).
//     tau: Number of concurrent transmissions g(m)
//     m: message size of the transmission

class Computation : public CostElement {
   
private:
   
   OpType  opType;   // Operation type
      
public:
   
          Computation   ();
   
          Computation   (const Process &p_src, int n, int m);
          Computation   (const Process &p_src, int n, int m, OpType opType);
   
          Computation   (const Process &p_src, int m);
          Computation   (const Process &p_src, int m, OpType opType);
          
   		 Computation   (int m);
          Computation   (int m, OpType opType);
   
          Computation   (const Computation *c);
   
         ~Computation   ()         override;
   
   Computation *clone   ()  const  override;
   CEType       getType ()  const  override;

   OpType getOpType     ()  const;
   
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
