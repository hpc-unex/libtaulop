//
//  mapping.cpp
//  taulop
//
//  Created by jarico on 17/11/16.
//  Copyright © 2016 University of Extremadura. All rights reserved.
//

#include "mapping.hpp"
#include "taulop_config.h"
#include <cstdlib>
#include <iostream>
using namespace std;


Mapping::Mapping  (int P) {
   this->P = P;
   this->nodes = new int [P];
   
   for (int p = 0; p < P; p++) {
      this->nodes[p] = 0;
   }
   
   this->mapping = Map::Default;
}


Mapping::Mapping  (int P, int *nodes) {
   
   this->P = P;
   this->nodes = new int [P];
   
   for (int p = 0; p < P; p++) {
      this->nodes[p] = nodes[p];
   }
   
   this->mapping = Map::User;
}



Mapping::Mapping (int P, int Q, Map map) {
   
   this->P = P;
   this->nodes = new int [P];
   
   int M = P / Q;
   if (M == 0) M = 1;
   
   int num[M]; // Only for Random mapping: control the number of processes per node.
   
   for (int p = 0; p < P; p++) {
      
      switch (map) {
            
         case Map::Default:
            this->nodes[p] = 0;
            break;
            
         case Map::Sequential: //MAPPING_SEQ:
            this->nodes[p] = p / Q;
            break;
            
         case Map::RoundRobin: //MAPPING_RR:
            this->nodes[p] = p % M;
            break;
            
         case Map::Random: // With max of Q processes per node.
            
            for (int i = 0; i < M; i++) num[i] = 0;
            
            for (int p = 0; p < P; p++) {
               
               int node = rand() % M;
               
               for (int n = 0; n < M; n++) {
                  
                  if (num[node] < Q) { // empty space in this node
                     
                     this->nodes[p] = node;
                     num[node] += 1;
                     break;
                     
                  } else { // try next node
                     node = (node + 1) % M;
                  }
                  
               }
               
            }
            break;
            
         default:
            this->nodes[p] = 0;
      }
   }
   
   this->mapping = map;
}



Mapping::~Mapping () {
   delete this->nodes;
}



Mapping& Mapping::operator = (const Mapping &m) {
   
   if (this != &m) {
      
      if (m.P != this->getP()) { // new size
         delete nodes;
         this->P = m.P;
         nodes = new int[this->P];
      }
      
      for (int i=0; i< this->P; i++) {
         this->nodes[i] = m.nodes[i];
      }
      
      this->mapping = m.mapping;
      
   }
   
   return *this;
}





int Mapping::getNode (int p) {
   return this->nodes[p];
}


int Mapping::getP() {
   return this->P;
}

void Mapping::show () {
   
   static string mapping_s [5] = {"Default", "Sequential", "RoundRobin", "Random", "User"};
   
   cout << "Mapping: " << mapping_s[(int)this->mapping] << endl;
   
   cout << "Nodes: [";
   for (int p = 0; p < this->P; p++) {
      cout << this->nodes[p];
      if (p != P-1) cout << ", ";
   }
   cout << "]" << endl;
   
}
