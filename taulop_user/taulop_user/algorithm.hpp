//
//  algorithm.hpp
//  TauLopCost
//
//  Created by jarico on 21/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef algorithm_hpp
#define algorithm_hpp

#include "arrangement.hpp"
#include "cost.hpp"
#include "rectangle.hpp"

#include <iostream>
using namespace std;


class Algorithm {
    
private:
    
public:
    
    Algorithm  ();
    ~Algorithm ();
    
    // Return the limits between two rectangles in inter and intra
    //   communications in horizontal and vertical.
    int  h_intraLimits (Rectangle *p_orig, Rectangle *p_dest);
    int  h_interLimits (Rectangle *p_orig, Rectangle *p_dest);
    int  v_intraLimits (Rectangle *p_orig, Rectangle *p_dest);
    int  v_interLimits (Rectangle *p_orig, Rectangle *p_dest);
    
    //virtual void  execute (Arrangement *arrang, Cost *&cost) = 0;
    virtual Cost *execute (Arrangement *arrang) = 0;
};

#endif /* algorithm_hpp */
