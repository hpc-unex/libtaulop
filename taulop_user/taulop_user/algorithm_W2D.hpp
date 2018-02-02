//
//  W2D_algorithm.hpp
//  TauLopCost
//
//  Created by jarico on 5/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//
//  Predicting the cost of a Finite Element communication
//

#ifndef FE_algorithm_hpp
#define FE_algorithm_hpp

#include "algorithm.hpp"

#include "taulop_sequence.hpp"
#include "taulop_concurrent.hpp"

#include <iostream>



class W2D_Algorithm : public Algorithm {
    
private:
    
    const int DEPTH = 1;
    
    // Add a column (Up or Down) communication. The overlap is the column size.
    bool addCol (Arrangement *r, Rectangle *p_src, int row_nr, int col_nr, TauLopSequence *seq);
    // Add a row (Left or Right) communication. See the overlap length.
    bool addRow (Arrangement *r, Rectangle *p_src, int row_nr, int col_nr, TauLopSequence *seq);

    // Add a column (Up or Down) communication. The overlap is the column size.
    bool addColConcurrent (Arrangement *r, Rectangle *p_src, int row_nr, int col_nr, TauLopConcurrent *conc);
    // Add a row (Left or Right) communication. See the overlap length.
    bool addRowConcurrent (Arrangement *r, Rectangle *p_src, int row_nr, int col_nr, TauLopConcurrent *conc);

public:
    
    W2D_Algorithm  ();
    ~W2D_Algorithm ();
    
    virtual  Cost * execute (Arrangement *r);
};


#endif /* W2D_algorithm_hpp */
