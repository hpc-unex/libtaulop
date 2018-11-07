//
//  taulop_params.hpp
//  TauLopCost
//
//  Created by jarico on 15/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef taulop_params_hpp
#define taulop_params_hpp

#include "taulop_config.h"
#include "taulop_params_channel.hpp"

#include <iostream>


// TBD: Comment the attributes and methods.


// Singleton pattern
class TauLopParam {
    
private:
    
    TaulopParamChannel *v_chn [DEFAULT_CHN_NR];
    
    // P2P cost in both channels. (TBD: maybe it must be a class for any alg.)
    double **p2p_T0;
    double **p2p_T1;
    
    long int *sizes; // Array of message sizes
    
    int  max_idx;
    int  max_tau;
    long max_m;
    
    static bool instanceFlag;
    static TauLopParam *single;
    
    static string network;
        
    TauLopParam(); //private constructor
    
    void  loadSizes ();
    void  setP2P    ();
    
public:
    
    static void setInstance(string network);
    static TauLopParam* getInstance();
    
    ~TauLopParam();
    
    double  getTime  (long n, int tau, int chn);
    long    getBytes (double t, int tau, int chn);
    
    void show ();
};



#endif /* taulop_params_hpp */
