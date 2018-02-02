//
//  rectangle.hpp
//  TauLopCost
//
//  Created by jarico on 04/11/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef rectangle_hpp
#define rectangle_hpp

#include "process.hpp"

#include <iostream>
#include <string>
using namespace std;

class Rectangle: public Process {
    
private:
    
    string name;      // Identifier
    
    int    row;       // Row position (i)
    int    col;       // Column position (j)
    
    int    coord_x;   // Block row coord
    int    coord_y;   // Block column coord
    
    int    width;     // Width of the rectange
    int    height;    // Height of the rectangle
    
public:
    
    Rectangle  ();
    Rectangle  (const Rectangle *p);
    ~Rectangle ();
    
    void   setName       (string name);
    
    void   setPos        (int i, int j);
    void   setCoords     (int x, int y);
    void   setSize       (int w, int h);

    string getName       ();

    int    getRow        ();
    int    getCol        ();
    void   getPos        (int &row, int &col);
    int    getCoordX     ();
    int    getCoordY     ();
    int    getHeight     ();
    int    getWidth      ();
    
    int    getArea       ();
    int    getPerimeter  ();
        
    void show  ();
    ostream& operator<< (ostream& outs);
    
};

#endif /* rectangle_hpp */
