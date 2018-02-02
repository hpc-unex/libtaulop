//
//  heuristic.cpp
//  TauLopCost
//
//  Created by jarico on 26/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "heuristic.hpp"

#include "algorithm.hpp"
#include "arrangement.hpp"

#include <iostream>
using namespace std;



// UTILITY FUNCTIONS

/* Function to swap values at two pointers */
void Heuristic::swap(int *x, int *y) {
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}


long int Heuristic::factorial (int n) {
    
    if (n <= 1)
        return 1;
    else
        return (n * factorial(n - 1));
    
}


/* Function to print permutations of string
 This function takes three parameters:
 1. String
 2. Starting index of the string
 3. Ending index of the string. */
void Heuristic::permute (int *nodes, int l, int r, int *perm, int &idx) {
    
    if (l == r) {
        //cout << idx << " -> ";
        for (int j = 0; j <= r; j++) {
            perm[(idx * (r+1)) + j] = nodes[j];
            //cout << perm[(idx * (r+1)) + j] << " ";
        }
        idx++;
        //cout << endl;
    }
    else
    {
        for (int i = l; i <= r; i++)
        {
            swap(nodes + l, nodes + i);
            permute(nodes, l+1, r, perm, idx);
            swap(nodes + l, nodes + i); //backtrack
        }
    }

}


void Heuristic::generate_permutations (int *nodes, int r, int *perm) {
    
    int idx = 0;
    permute(nodes, 0, r-1, perm, idx);
}


