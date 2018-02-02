//
//  arrangement.hpp
//  TauLopCost
//
//  Created by jarico on 18/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef arrangement_hpp
#define arrangement_hpp

#include "matrix_config.h"

#include "column.hpp"
#include "rectangle.hpp"

#include <iostream>

class Arrangement {

private:
    
    int  col_nr;  // Number of cols in this arrangement
    int  P;       // Number of processes
    int  M;       // Number of Nodes
    //int  N;       // Size (in blocks)
    int  H;       // Height size (in blocks)  H = W if square matrix.
    int  W;       // Width size (in blocks)
    int  B;       // Block size
    
    Column **v_cols; // Columns with equal-width rectangles
    
    
    int calculateProcNr ();
    int calculateNodeNr ();
    
public:
    
     Arrangement  ();
     Arrangement  (int col_nr, int row_nr[]);
     Arrangement  (const Arrangement *r);
    ~Arrangement  ();
    
    void        insert   (Rectangle *p, int row, int col);
    Rectangle * get      (int row, int col) const;
    
    void      setProcNr    (int P);
    void      setNodeNr    (int M);
    void      setWidth     (int W);
    void      setHeight    (int H);
    void      setBlockSize (int B);
    
//    int       getN      ();  // TEMP: eliminar
    int       getWidth  ();
    int       getHeight ();
    int       getB      ();
    int       getP      ();
    int       getNodeNr ();
    int       getNodeNr (int col) const;
    
    int       getRowNr  (int col) const;
    int       getColNr  ()        const;
    
    int       getWidth  (int col);
//    int       getSize   ();
    
    // Utility functions
    void      establish    (); // Set (and test) the coordinates of the rectangles after movements
    void      changeCols   (int c_orig, int c_dest); // Interchange columns
    void      changeCols   (int *c_vec);   // Change columns using a vector of new positions
                                           // Column in index i in vector is the new column c_vec[i]
    
    void      groupByNode  (); // Group rectangles depending on their node in each Column
    void      groupByNode  (int col); // Group rectangles depending on their node in Column col
    
    void      changeRows   (int col, int r_orig, int r_dest); // Change two rectangles in a column
    void      changeRows   (int col, int *rows); // Change rectangles in a column using a vector of positions
                                                 //  rows contains absolute rank numbers.
    
    Arrangement *createSubArrangement (int ncols, int *cols); // Create an arrangement using cols in the vector
    void      addColumn (Column *c);  // Add a column to the arrangement (at the end)
    void      delColumn (int col_nr); // Delete the column in position col_nr
    Column *  getColumn (int col_nr); // Return a pointer to the column col_nr

    
    
    Rectangle * getProcByRank (int rank);  // Inefficient, but necessary.
    
    void      getNextVInterval (int b_ini, int &b_size); // Get next interval starting at block b_ini
                                                        //  b_size is the size of the new interval
    void      getNextHInterval (int b_ini, int &b_size); // Get next horizontal interval (next column)
    
    void      show       ();
    ostream&  operator<< (ostream& outs);
    void      plot       (string file);
    void      part2D     (string file);
};

#endif /* arrangement_hpp */
