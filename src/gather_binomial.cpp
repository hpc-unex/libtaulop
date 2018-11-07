//
//  gather_binomial.cpp
//  TauLopCost
//
//  Created by jarico on 20/Apr/17
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "gather_binomial.hpp"

#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <cmath>
#include <iostream>
using namespace std;




GatherBinomial::GatherBinomial () {
    
}

GatherBinomial::~GatherBinomial () {
    
}


TauLopCost * GatherBinomial::evaluate (Communicator *comm, int *size, int root) {
    
    TauLopConcurrent *conc;
    TauLopSequence   *seq;
    Transmission     *c;
    Process          *p_src, *p_dst;
    
    TauLopCost *cost = new TauLopCost();
    
    int P = comm->getSize();
        
    int mask  = 1;
    int stage = 0;
    
    while (mask < P) {
        
        conc = new TauLopConcurrent ();
        
        //cout << "Stage: " << stage << "  mask = " << mask << endl;
        
        for (int r = 0; r < P; r += pow(2, stage)) {
            
            if ((mask & r) == 0) {
                
                seq = new TauLopSequence ();
                
                // r is the relative rank. We need to get the real rank from it:
                //   rank = (r + root) % P
                int src = r | mask;
                if (src >=  P) continue;
                src = (src + root) % P;

                int dst = r;
                dst = (dst + root) % P;
                
                p_src = new Process (src, comm->getNode(src));
                p_dst = new Process (dst, comm->getNode(dst));
                
                int channel = (p_src->getNode() == p_dst->getNode()) ? 0 : 1;
                
                // How many bytes to send? (needed if P is not power of 2)
                int recvblks = mask;
                if (2 * recvblks > P) {
                    recvblks = P - mask;
                }
                int m = (*size) * recvblks;
                
                int n    = 1;
                int tau  = 1;
                
                c = new Transmission(p_src, p_dst, channel, n, m, tau);
                seq->add(c);
                
                conc->add(seq);
                
                //cout << "  " << src << " -> " << dst << " >> " << m << endl;
            }
            
        }
        mask <<= 1;
        stage++;
        
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

