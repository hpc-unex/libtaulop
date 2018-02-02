//
//  algorithm.cpp
//  TauLopCost
//
//  Created by jarico on 21/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "algorithm.hpp"


Algorithm::Algorithm  () {
    
}


Algorithm::~Algorithm () {
    
}



int Algorithm::h_intraLimits (Rectangle *p_orig, Rectangle *p_dest) {
    
    if (p_orig->getNode() != p_dest->getNode())
        return 0;
    
    int start = p_orig->getCoordY();
    int end   = p_orig->getCoordY() + p_orig->getHeight();
    
    int p_start = p_dest->getCoordY();
    int p_end   = p_dest->getCoordY() + p_dest->getHeight();
    
    // Case 1
    if (end <= p_start)
        return 0;
    
    // Case 2
    if (start >= p_end)
        return 0;
    
    // Case 3
    if ((start <= p_start) && (end <= p_end))
        return (end - p_start);
    
    // Case 4
    if ((start >= p_start) && (end >= p_end))
        return(p_end - start);
    
    // Case 5
    if ((start <= p_start) && (end >= p_end))
        return(p_end - p_start);
    
    // Case 6
    if ((start >= p_start) && (end <= p_end))
        return (end - start);
    
    return 0;
}


int Algorithm::h_interLimits (Rectangle *p_orig, Rectangle *p_dest) {
    
    if (p_orig->getNode() == p_dest->getNode())
        return 0;
    
    int start = p_orig->getCoordY();
    int end   = p_orig->getCoordY() + p_orig->getHeight();
    
    int p_start = p_dest->getCoordY();
    int p_end   = p_dest->getCoordY() + p_dest->getHeight();
    
    // Case 1
    if (end <= p_start)
        return 0;
    
    // Case 2
    if (start >= p_end)
        return 0;
    
    // Case 3
    if ((start <= p_start) && (end <= p_end))
        return (end - p_start);
    
    // Case 4
    if ((start >= p_start) && (end >= p_end))
        return(p_end - start);
    
    // Case 5
    if ((start <= p_start) && (end >= p_end))
        return(p_end - p_start);
    
    // Case 6
    if ((start >= p_start) && (end <= p_end))
        return (end - start);
    
    return 0;
}


int Algorithm::v_intraLimits (Rectangle *p_orig, Rectangle *p_dest) {
    
    if (p_orig->getNode() != p_dest->getNode())
        return 0;
    
    if (p_orig->getCol() != p_dest->getCol())
        return 0;
    
    return p_orig->getWidth();
}


int Algorithm::v_interLimits (Rectangle *p_orig, Rectangle *p_dest) {
    
    if (p_orig->getNode() == p_dest->getNode())
        return 0;
    
    if (p_orig->getCol() != p_dest->getCol())
        return 0;
    
    return p_orig->getWidth();
}

