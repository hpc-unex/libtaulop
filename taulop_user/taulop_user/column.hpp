//
//  column.hpp
//  TauLopCost
//
//  Created by jarico on 18/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef column_hpp
#define column_hpp

#include "matrix_config.h"

#include "rectangle.hpp"

#include <iostream>
#include <vector>
using namespace std;


class Column {
    
private:
    
    int  width;
    int  row_nr;  // Number of rows in this column (rectangles)
                  //  Calculated on insertion of rectangles.
    Rectangle **v_proc;
    
public:
    
//    Column  ();
     Column  (int row_nr);
     Column  (Column *c);
    ~Column  ();
    
    void     insert      (Rectangle *p, int row);
//    void     replace     (Rectangle *p, int row);
    Rectangle *get         (int row);
    
    void     change (int r_orig, int r_dest);
    
    void     setWidth    (int width);
    int      getWidth    ();
    
    int      getRowNr    ();
    
    int      getNodeNr   ();
    
    void show ();
    ostream& operator<< (ostream& outs);
};

#endif /* column_hpp */
