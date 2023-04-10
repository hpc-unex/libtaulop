//
//  alltoall_linear.cpp
//  Linear algorithms for the Alltoall collective implemented in Open MPI 3.1
//
//  Created by jarico on 1/Dic/18.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "alltoall_linear.hpp"

#include "transmission.hpp"
#include "collective.hpp"
#include "communicator.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <cmath>
#include <iostream>
using namespace std;




AlltoallLinear::AlltoallLinear () {
    
}

AlltoallLinear::~AlltoallLinear () {
    
}


TauLopCost * AlltoallLinear::evaluate (Communicator *comm, int *size, int root) {
    
    TauLopConcurrent *conc;
    TauLopSequence   *seq;
    Transmission     *T;
    Process          *p_src, *p_dst;
    
    // This algorithm uses a sendrecv in each process for the local data
    // Then post a receive (using non-blocking communication) from the rest, with the order:
    //   p -> p + 1 % P
    // Finally, a point-to-point send message post to every process, in (reverse) order:
    //   p <- p - 1 % P
    
    
    TauLopCost *cost = new TauLopCost();
    
    int P = comm->getSize();
    
    conc = new TauLopConcurrent ();
    
    for (int p = 0; p < P; p++) {
        
        seq = new TauLopSequence ();
        
        for (int i = p; i != (p + P - 1) % P; i = (i + 1) % P) {
                
            int src = p;
            int dst = i;
            
            int node_src = comm->getNode(src);
            int node_dst = comm->getNode(dst);
            
            p_src = new Process (src, node_src);
            p_dst = new Process (dst, node_dst);
            
            int channel = (node_src == node_dst) ? 0 : 1;
            int n = 1;
            int tau = 1;
            
            T = new Transmission(p_src, p_dst, channel, n, (*size), tau);
            
            seq->add(T);
            
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
