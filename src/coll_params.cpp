//
//  coll_params.cpp
//  Implementation of collective parameters
//
//  Created by jarico on 20/04/23.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "coll_params.hpp"


CollParams::CollParams  () {
   this->root = RANK_UNDEFINED;
   this->op   = OpType::DEFAULT;
   //this->m.push_back(-1);
   this->m    = {-1};
}


CollParams::CollParams (int m, int root) {
   this->root = root;
   this->op   = OpType::DEFAULT;
   this->m    = {m};
}


CollParams::CollParams (int m) {
   this->root = RANK_UNDEFINED;
   this->op   = OpType::DEFAULT;
   this->m    = {m};
}


CollParams::CollParams (int m, int root, OpType op) {
   this->root = root;
   this->op   = op;
   this->m    = {m};
}


CollParams::CollParams (int m, OpType op) {
   this->root = RANK_UNDEFINED;
   this->op   = op;
   this->m    = {m};
}


CollParams::CollParams (vector<int> m, int root) {
   this->root = root;
   this->op   = OpType::DEFAULT;
   this->m    = {m};
}


CollParams::~CollParams () {
    
}


int CollParams::getRoot () const {
   return this->root;
}


int CollParams::getM () const {
   return this->m[0];
}


int CollParams::getM (int p) const {
   // No test p < P
   return this->m[p];
}


OpType CollParams::getOp () const {
   return this->op;
}


void CollParams::show () const {

   string op_types [] = {"def", "max", "min", "sum", "prod", "land", "band", "lor", "bor", "lxor", "bxor", "maxloc", "minloc"};
   string op_type = op_types[(int)this->op];

   if (this->m.size() == 1) {
      cout << "m:    " << this->m[0] << endl;
   } else {
      cout << "m (*):" << endl;
      for (int i; i < this->m.size(); i++) {
         cout << this->m[i] << "  " << endl;
      }
   }
   cout << "root: " << this->root << endl;
   cout << "op:   " << op_type    << endl;
}
