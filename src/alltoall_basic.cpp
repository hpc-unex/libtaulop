//
//  alltoall_basic.cpp
//  Basic IN PLACE algorithm for the Alltoall collective
//  AlltoAll basic IN_PLACE cost of the algorithm implemented in Open MPI 3.1 coll/tuned component
//  The algorithm uses a temporal buffer for copying data to send. The cost of such copy is not
//   included in the total cost in this implementation.
//
//  Created by jarico on 1/Dic/18.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "alltoall_basic.hpp"

#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <cmath>
#include <iostream>
using namespace std;




AlltoallBasic::AlltoallBasic () {
    
}

AlltoallBasic::~AlltoallBasic () {
    
    
}


TauLopCost * AlltoallBasic::evaluate (Communicator *comm, int *size, int root) {
    
    TauLopConcurrent *conc;
    TauLopSequence   *seq;
    Transmission     *T;
    Process          *p_src, *p_dst;
    
    
    TauLopCost *cost = new TauLopCost();
    
    int P = comm->getSize();
    
    conc = new TauLopConcurrent ();
    
    for (int p = 0; p < P; p++) {
        
        seq = new TauLopSequence ();
        
        for (int i = 0; i < P; i++) {
            
            for (int j = i+1; j < P; j++) {
                
                if (i == p) { /* Exchange message */
                    
                    int src = j;
                    int dst = p;
                    
                    int node_src = comm->getNode(src);
                    int node_dst = comm->getNode(dst);
                    
                    p_src = new Process (src, node_src);
                    p_dst = new Process (dst, node_dst);
                    
                    int channel = (node_src == node_dst) ? 0 : 1;
                    int n = 1;
                    int tau = 2;
                    
                    T = new Transmission(p_src, p_dst, channel, n, (*size), tau);
                    
                    seq->add(T);
                    
                } else if (j == p) { /* Exchange message */
                    
                    int src = i;
                    int dst = p;
                    
                    int node_src = comm->getNode(src);
                    int node_dst = comm->getNode(dst);
                    
                    p_src = new Process (src, node_src);
                    p_dst = new Process (dst, node_dst);
                    
                    int channel = (node_src == node_dst) ? 0 : 1;
                    int n = 1;
                    int tau = 2;
                    
                    T = new Transmission(p_src, p_dst, channel, n, (*size), tau);
                    
                    seq->add(T);
                    
                } else {
                    
                    continue;
                    
                }
            }
            
        }
        
        conc->add(seq);
    
#if TLOP_DEBUG == 1
        cout << "P = " << P << endl;
        conc->show();
#endif
    }
    
    conc->evaluate(cost);
#if TLOP_DEBUG == 1
    cout << " -- Cost: " << endl;
    cost->show();
#endif
        
    delete conc;
    
    return cost;
}
