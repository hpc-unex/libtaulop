//
//  taulop_operator.hpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef taulop_operator_hpp
#define taulop_operator_hpp

#include "ilist.h"
#include "transmission.hpp"

#include <iostream>


const int CHANNEL_NR = 2;

class TauLopOperator {
    
private:
    
    IList<Transmission *> * l_comm; // Initial communication starting at the same time (possibly concurrent)
    IList<Transmission *> * l_real_conc; // Real concurrent communication from l_comm
    
    void show (IList<Transmission *> *l);
    
public:
    
    TauLopOperator  ();
    ~TauLopOperator ();
    
    void   add            (Transmission *  c);
    void   evaluate       ();
    double getMinCost     (Transmission * &c);
    int    getConcurrency (Transmission *  c); // Get the concurrency (tau) of the comm. in the same channel
        
    void  show_init_comms ();
    void  show_concurrent ();
    void  show_channel    ();
};

#endif /* taulop_operator_hpp */
