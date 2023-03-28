//
//  computation.hpp
//  TauLopCost
//
//  Created by jarico on 27/3/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//
//   Represents a computation cost.
//

#ifndef computation_hpp
#define computation_hpp

#include "process.hpp"
#include "taulop_params.hpp"

#include <iostream>

// Attributes represents a computation: gamma(m, op)
//  It does not take into account concurrency.
//  Its impemented as a linear function, that is:  A x gamma(m, op) = gamma(m x A, op)
// With:
//     m:  message size to compute in bytes.
//     op: operation to apply.

class Computation {
   
private:
   
   Process      proc;   // Process
   int          node;   // Node
   long         m;      // Message size
   int          op;     // Operation to apply  (TODO)
   
   TauLopParam *params;
   
public:
   
             Computation ();
             Computation (const Process &proc, int m, int op);
             Computation (const Computation &c);
            ~Computation ();
   
   void      putProcess  (const Process &p);
   Process&  getProcess  ();
         
   int       getRank     () const;
   
   int       getNode     () const;
   
   void      putM        (long m);
   long      getM        () const;
   
   
   double    getCost     ();  // Cost of the blocks in computation
   long      getBytes    (double t);
   
   void      add         (const Computation &c);
   
   void      compact     (const Computation &c);
   
   void      show        ();
};

#endif /* computation_hpp */
