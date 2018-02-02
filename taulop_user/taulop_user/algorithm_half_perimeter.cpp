//
//  algorithm_half_perimeter.cpp
//  TauLopCost
//
//  Created by jarico on 10/10/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "algorithm_half_perimeter.hpp"

#include <limits>
#include <iomanip>

HalfPerimeterAlgorithm::HalfPerimeterAlgorithm  () {
    
}


HalfPerimeterAlgorithm::~HalfPerimeterAlgorithm () {
    
}


Cost * HalfPerimeterAlgorithm::execute (Arrangement *arrang) {
    
    Cost   *cost   = new Cost();
    int     col_nr = arrang->getColNr();
    double  b_cost = 0.0;
    
    
    
    for (int c = 0; c < col_nr; c++) {
        
        int num_procs_r = arrang->getRowNr(c);
        
        for (int r = 0; r < num_procs_r; r++) {
        
            Rectangle *p  = arrang->get (r, c);
            double     hp = p->getHeight() + p->getWidth();
            
            //cout << "Rectangle: " << p->getRank() << " with: " << fixed << std::setprecision (2) << hp << endl;
            
            // Normalize value or not ?
            int norm = arrang->getWidth(); // Suppose square matrix
            //int norm = 1;
            b_cost += (hp / norm);
            
        }
    }

    cost->putTime(b_cost);
    
    return cost;
}

