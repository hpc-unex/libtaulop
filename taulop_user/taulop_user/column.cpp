//
//  column.cpp
//  TauLopCost
//
//  Created by jarico on 18/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "column.hpp"

#include <iostream>
using namespace std;


Column::Column (int row_nr) {

    this->row_nr = row_nr;
    this->width  = 0;
    
    this->v_proc = new Rectangle * [row_nr];
    
    for (int i = 0; i < row_nr; i++) {
        this->v_proc[i] = nullptr;
    }
    
}


Column::Column (Column *c) {
    
    this->row_nr = c->row_nr;
    this->width  = c->width;
    
    this->v_proc = new Rectangle * [c->row_nr];
    
    for (int i = 0; i < c->row_nr; i++) {
        Rectangle *p = c->v_proc[i];
        this->v_proc[i] = new Rectangle(p);
    }

}

Column::~Column () {

    for (int i = 0; i < this->row_nr; i++) {
        delete this->v_proc[i];
    }

}


void Column::insert (Rectangle *p, int row) {
    this->v_proc[row] = p;
    
    if (row >= this->row_nr) {
        this->row_nr = row + 1;
    }
}


//void Column::replace (Rectangle *p, int row) {
//
//    if (row > this->row_nr) {
//        // Error
//        cerr << "No rectangle in row: " << row << endl;
//        return;
//    }
//
//    this->v_proc[row] = p;
//}



void Column::setWidth (int width) {
    this->width = width;
}


int Column::getWidth () {
    return this->width;
}


Rectangle * Column::get (int row) {
    return this->v_proc[row];
}


void Column::change (int r_orig, int r_dest) {
    
    Rectangle *p_orig = this->v_proc[r_orig];
    Rectangle *p_dest = this->v_proc[r_dest];
    
    this->v_proc[r_orig] = p_dest;
    this->v_proc[r_dest] = p_orig;
}


int Column::getRowNr () {
    return this->row_nr;
}


int Column::getNodeNr () {

    int nnodes[this->row_nr];
    for (int i = 0; i < this->row_nr; i++) nnodes[i] = -1;
    int j = 0;
    
    for (int i = 0; i < this->row_nr; i++) {
        
            bool found = false;
            Rectangle *p = this->v_proc[i];
            int node = p->getNode();
            
            int k;
            for (k = 0; nnodes[k] != -1; k++) {
                if (node == nnodes[k]) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                nnodes[k] = node;
                j = k + 1;
            }
    }
    return j;
}


void Column::show () {
    
    for (int i = 0; i < this->row_nr; i++) {
        if (this->v_proc[i] != nullptr) {
            this->v_proc[i]->show();
        }
    }
}

ostream& Column::operator<< (ostream& outs) {
    
    for (int i = 0; i < this->row_nr; i++) {
        outs << this->v_proc[i] << " " << endl;
    }
    return outs;
}



