//
//  heuristic_beaumont.hpp
//  TauLopCost
//
//  Created by jarico on 10/10/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//
//
// Implements the Beaumont algorithm in
//    [Beaumont et al "Matrix Multiplication on Heterogeneous Platforms", 2001]
// without "r" variable. That is, evaluates all the possible arrangements with P processes and
// get the best (not evaluates the arrangements with less than P processes).
// For evaluating the cost, the heuristic can call to an algorithm based on half-perimeters or
//   time (as tau-Lop).
// The order of the processes is based in their areas, as Beaumont algorithm establishes.



#ifndef heuristic_beaumont_mod_hpp
#define heuristic_beaumont_mod_hpp

#include "heuristic.hpp"
#include "algorithm.hpp"
#include "arrangement.hpp"

#include <iostream>
using namespace std;


class BeaumontHeuristicMod : public Heuristic {
    
private:
    
    Algorithm *alg;
    
    class Group {
    private:
        
        int  col;
        int *groups;
        int  r;
        
    public:
        
        Group (int col) {
            this->col     = col;
            this->r       = 0;
            this->groups  = new int [col];
        }
        
        ~Group () {
            delete [] this->groups;
        }
        
        void putGroup (int idx, int value) {
            this->groups[idx] = value;
        }
        
        int getGroup (int idx) {
            return this->groups[idx];
        }
        
        void putR (int col_nr) {
            this->r = col_nr;
        }
        
        int getCols() {
            return this->col;
        }
        
        void show () {
            cout << "Group: [";
            for (int i = 0; i < this->col; i++) {
                cout << this->groups[i];
                if (i < this->col - 1)
                    cout << " ";
            }
            cout << "], col = " << this->col << " r = " << this->r << endl;
        }
    };
    
    // Return the aggrupations possibilities for a vector and c columns.
    // Vector is of the form: [P-c, 1, 1, 1, ... ], that is, we depart from all processes in
    //    the first column and one in the rest.
    void agrupation_nr (int *v, int c, int C, long int &n);
    // Return also a vector with the possible groups.
    void agrupate      (int *v, int c, int C, long int &n, Group *agr[]);
    
    
public:
    
    BeaumontHeuristicMod  (Algorithm *alg);
    ~BeaumontHeuristicMod ();
    
    virtual  Arrangement ** apply        (Arrangement *r);
    virtual  long int       combinations (Arrangement *r);
    
};

#endif /* heuristic_beaumont_mod_hpp */
