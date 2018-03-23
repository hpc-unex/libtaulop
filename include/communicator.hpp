//
//  communicator.hpp
//  taulop
//
//  Created by jarico on 17/11/16.
//  Copyright © 2016 University of Extremadura. All rights reserved.
//

#ifndef communicator_hpp
#define communicator_hpp

#include "mapping.hpp"

using namespace std;


// TBD: Comment attributes


class Communicator {
    
private:
    
    int *ranks;    
    int  P;
    Mapping *mapping;
    
public:
    
     Communicator (int P);
    ~Communicator ();
    
    Communicator *create (int P, int *ranks);
    
    void map (Mapping *map);
    void getMap(Mapping *map);
    
    int  getRank (int p);
    int  getSize ();
    int  getNode (int p);
    
    void show ();
};



#endif /* communicator_hpp */
