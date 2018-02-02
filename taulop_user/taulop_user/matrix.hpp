//
//  matrix.hpp
//  TauLopCost
//
//  Created by jarico on 18/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef matrix_hpp
#define matrix_hpp

#include "matrix_config.h"

#include "arrangement.hpp"
#include "column.hpp"
#include "arrangement.hpp"
#include "algorithm.hpp"
#include "cost.hpp"
#include "rectangle.hpp"

#include "ilist.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;


class Matrix {
    
private:
    
    //int  N;
    int  W;
    int  H;
    int  n;
    int  B;
    int  P;
    int  M;
    
    int  col_nr;

    //Arrangement *arrang_init;
    IList<Arrangement *> *l_arrang;
    int  arrang_nr;
    
    // Rectangles in the matrix
    Rectangle * v_proc[NUM_PROC];
    
    
    void         setInitialArrangement ();
    Arrangement *getInitialArrangement ();

    void         generateSynthetic ();
    
    // Index in the following list is the node number
    IList<string> *nodes;
    int find_node_nr (string name);
    
public:
    
    Matrix  ();
    ~Matrix ();
    
    void  load              (string name = string()); // Load default matrix if empty name
    int   addArrangement    (int P, int W, int H, int B, int col_nr, Rectangle *p[]);
    
    int   newArrangement    (Arrangement *r);
    void  removeArrangement ();
    void  getArrangement    (Arrangement *& r, int index);
    Arrangement
         *getArrangement    (int index);
    
    void toString ();
};

#endif /* matrix_hpp */
