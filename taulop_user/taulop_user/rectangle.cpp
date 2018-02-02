//
//  rectangle.cpp
//  TauLopCost
//
//  Created by jarico on 04/Nov/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "rectangle.hpp"
#include <iostream>
using namespace std;


Rectangle::Rectangle () {
}

Rectangle::Rectangle (const Rectangle *p) {

    this->name      = p->name;
    this->rank      = p->rank;
    this->rankIntra = p->rankIntra;
    this->node      = p->node;
    this->row       = p->row;
    this->col       = p->col;
    this->coord_x   = p->coord_x;
    this->coord_y   = p->coord_y;
    this->width     = p->width;
    this->height    = p->height;
}


Rectangle::~Rectangle () {
}


void Rectangle::setName (string name) {
    this->name = name;
}


void Rectangle::setPos (int i, int j) {
    this->row = i;
    this->col = j;
}


void Rectangle::setCoords (int x, int y) {
    this->coord_x = x;
    this->coord_y = y;
}


void Rectangle::setSize (int w, int h) {
    this->width  = w;
    this->height = h;
}



int Rectangle::getRow () {
    return this->row;
}


int Rectangle::getCol () {
    return this->col;
}


string Rectangle::getName () {
    return this->name;
}


void Rectangle::getPos (int &row, int &col) {
    row = this->row;
    col = this->col;
}



int Rectangle::getCoordX () {
    return this->coord_x;
}


int Rectangle::getCoordY () {
    return this->coord_y;
}


int Rectangle::getHeight () {
    return this->height;
}


int Rectangle::getWidth () {
    return this->width;
}


int Rectangle::getArea () {
    return (this->width * this->height);
}


int Rectangle::getPerimeter () {
    return (2 * (this->width + this->height));
}


void Rectangle::show () {
    cout << this->name << " (" << this->rank << "," << this->rankIntra << " running on: " << this->node<< ") " << endl;
}

ostream& Rectangle::operator<< (ostream& outs) {
    outs << this->name << " (" << this->rank << "): ";
    return outs;
}
