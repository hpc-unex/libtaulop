//
//  taulop_params_channel.hpp
//  TauLopCost
//
//  Created by jarico on 18/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef taulop_params_channel_hpp
#define taulop_params_channel_hpp

#include <iostream>
#include <string>
using namespace std;

class TaulopParamChannel {
    
private:
    
    double   *o;     // Array of overhead values per message size
    double  **L;     // Matrix of transfer times per message size and tau
    int       num_sizes;    
    int       channel_nr; // Channel number
    
    // Load data from files
    void loadOs    (string network); // From overhead file
    void loadLs    (string network); // From tranfers time file
        
public:
    
    TaulopParamChannel  (string network, int channel_nr, int num_sizes);
    ~TaulopParamChannel ();
    
    double getO (int idx);
    double getL (int idx, int tau);

    void   show ();
};

#endif /* taulop_params_channel_hpp */
