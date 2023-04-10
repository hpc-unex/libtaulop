//
//  cost_element.hpp
//
//  Created by jarico on 31/3/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//
//   Represents a Cost imputable element (Transmission or Computation).
//

#ifndef cost_element_hpp
#define cost_element_hpp

#include "process.hpp"
#include "taulop_params.hpp"

#include <iostream>
using namespace std;


// TODO: Suppose that this is not an OOP great idea:
enum class CEType {Transmission, Computation};


// Class represents a cost imputable element (Transmission or Computation)

class CostElement {
      
protected:
   
   TauLopParam *params;
   CEType  ceType;
   
   long    m;       // Message size (to operate or transmit)
   int     n;       // Number of transmissions or computations (if more than one)
   int     tau;     // Number of concurrent transmissions or computations
   Process process; // Process that computes or initiates the transmission (source).
   
public:
   
                   CostElement    ();
   virtual        ~CostElement    ();

   long    getM           ()        const;
   void    putM           (long m);

   int     getN           ()        const;
   void    putN           (int n);
   
   int     getTau         ()        const;
   
   void    putProcess     (const Process &p);
   
   int     getNode        ()        const;

   
   virtual double  getCost        ()                      const = 0;
   virtual long    getBytes       (double t, int tau)     const = 0;
   
   virtual int     getRank     ()                      const = 0;

   virtual void    getOverlap     (const CostElement *c) = 0;
      
   virtual CostElement *clone() const = 0;

   virtual CEType getType        ()  const = 0;
   virtual void   show           ()  const = 0;
   virtual void   notate         ()  const = 0;
};

#endif /* cost_element_hpp */
