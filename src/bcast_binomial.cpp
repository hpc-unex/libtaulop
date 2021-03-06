//
//  bcast_binomial.cpp
//  TauLopCost
//
//  Created by jarico on 17/Nov/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "bcast_binomial.hpp"

#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <math.h>
#include <iostream>
using namespace std;




BcastBinomial::BcastBinomial () {
    
}

BcastBinomial::~BcastBinomial () {
    
    
}


TauLopCost * BcastBinomial::evaluate (Communicator *comm, int *size, int root) {
        
    TauLopConcurrent *conc;
    TauLopSequence   *seq;
    Transmission     *c;
    Process          *p_src, *p_dst;
    
    TauLopCost *cost = new TauLopCost();
    
    int P = comm->getSize();
            
    for (int stage = 0; pow(2, stage) < P; stage++) {
        
        
        conc = new TauLopConcurrent ();

        int p = root;
        
        for (int t = 0; t < pow(2, stage); t++) {
            
            seq = new TauLopSequence ();
            
            int src = p;
            int dst = src + P / pow(2, stage + 1);
            dst = dst % P;
            
            int node_src = comm->getNode(src);
            int node_dst = comm->getNode(dst);
            
            p_src = new Process (src, node_src);
            p_dst = new Process (dst, node_dst);
            
            int channel = (node_src == node_dst) ? 0 : 1;
            
            int n   = 1;
            int tau = 1;

            c = new Transmission(p_src, p_dst, channel, n, *size, tau);
            seq->add(c);
            
            conc->add(seq);
            
            p = p + (P / pow(2, stage));
            p = p % P;
            
        }
        
#if TLOP_DEBUG == 1
        cout << " ----  Stage " << stage << endl;
        conc->show();
#endif
        
        conc->evaluate(cost);
        
#if TLOP_DEBUG == 1
        cout << "  --------  Cost:  " << endl;
        cost->show();
#endif
        
        delete conc;
    }
    
    return cost;
}

