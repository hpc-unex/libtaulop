//
//  scatter_binomial.cpp
//  TauLopCost
//
//  Created by jarico on 21/Apr/17
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "scatter_binomial.hpp"

#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <cmath>
#include <iostream>
using namespace std;




ScatterBinomial::ScatterBinomial () {
    
}

ScatterBinomial::~ScatterBinomial () {
    
}


TauLopCost * ScatterBinomial::evaluate (Communicator *comm, int n, int root) {
        
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
            
            
            p_src = new Process (src, comm->getNode(src));
            p_dst = new Process (dst, comm->getNode(dst));
            
            int channel = (p_src->getNode() == p_dst->getNode()) ? 0 : 1;
            
            int size = n * pow(2, stage);
            int m    = 1;
            int tau  = 1;
            
            c = new Transmission(p_src, p_dst, channel, m, size, tau);
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

