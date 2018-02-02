//
//  heuristic_combinatorial.cpp
//  TauLopCost
//
//  Created by jarico on 28/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "heuristic.hpp"
#include "heuristic_combinatorial.hpp"

#include "algorithm.hpp"
#include "arrangement.hpp"

#include <iostream>
#include <string>
#include <iomanip>


#define ALPHA 0
#define OMEGA 1

// PRIVATE FUNCTIONS


void CombinatorialHeuristic::createCombinationsCols (Arrangement *r, int col, Config_t *c[2], int &num_cfg) {
    
    // How many columns
    int col_nr = r->getColNr();
    int *cols = new int [col_nr];
    for (int c = 0; c < col_nr; c++) {
        cols[c] = c;
    }
    long int arr_perm_nr = factorial(col_nr);
    int *perm_col = new int [arr_perm_nr * col_nr];
    this->generate_permutations(cols, col_nr, perm_col);
    
    Arrangement *r2 = nullptr;
    for (int c_nr = 0; c_nr < arr_perm_nr; c_nr++) {
        
        r2 = new Arrangement (r);
        
        r2->changeCols(&perm_col[c_nr * col_nr]);
        
        // r2->show();

        this->createCombinations(r2, col, c, num_cfg);
        
        delete r2;
    }

    delete [] perm_col;
    delete [] cols;

}


void CombinatorialHeuristic::createCombinations (Arrangement *r, int col, Config_t *c[2], int &num_cfg) {
    
    // 1. Possible combinations:
    //     fact(C_0) * fact(C_1) * ... * fact(C_n) * fact(#cols)
    
    // (a) How many processes in this column (col)
    int p_nr_col = r->getRowNr(col);
    int *procs_col = new int [p_nr_col];
    
    // (b) Array with the ranks of the processes in column col.
    for (int row = 0; row < p_nr_col; row++) {
        Rectangle *p = r->get(row, col);
        procs_col[row] = p->getRank();
    }
    
    // (c) Given the number of processes, we create a vector of all possible permutations
    //  for this column. Output: perm
    long int col_perm_nr = factorial(p_nr_col);
    
    int *perm = new int [col_perm_nr * p_nr_col];
    this->generate_permutations(procs_col, p_nr_col, perm);
    
    
    // 2. Test all possible permutations C_i-1 <-> C_i
    Arrangement *r_curr = nullptr;
    
    for (int p_nr = 0; p_nr < col_perm_nr; p_nr++) {
        
        //cout << "Permutation: ";
        //for (int i = 0; i < groups_nr; i++) cout << perm[(p_nr * groups_nr) + i] << " ";
        //cout << endl;
        
        // (a) Create the arrangement for the permutation
        r_curr = new Arrangement (r);
        
        int row = 0;
        
        for (int g = 0; g < p_nr_col; g++) {
            
            for (int k = row; k < r_curr->getRowNr(col); k++) {
                
                Rectangle *p = r_curr->get(k, col);
                if (p->getRank() == perm[(p_nr * p_nr_col) + g]) {
                    
                    if (row != k) {
                        Rectangle *p_aux = r_curr->get(row, col);
                        
                        r_curr->insert(p, row, col);
                        r_curr->insert(p_aux, k, col);
                        
                        p->setPos(row, col);
                        p_aux->setPos(k, col);
                    }
                    
                    row++;
                }
            }
        }
        
        // (b) Establish and calculate the new cost.
        r_curr->establish();
        
        if (num_cfg % 10000 == 0)
            cout << "# config: " << num_cfg << endl;
        //r_curr->show();

        // 3. Is this the best/worst configuration?
        // (a) Estimate time
        Cost *t = new Cost();
        t = this->alg->execute(r_curr);
        double time = t->getTime();
        
        //cout << fixed << setprecision(6) << "Time: " << time << "  compared to:  " << c[ALPHA]->time << "  and  " << c[OMEGA]->time << endl;
        
        // (b) Test current alpha and omega times
        if (c[ALPHA]->time > time) {
            
            delete (c[ALPHA]->r);
            c[ALPHA]->time = time;
            c[ALPHA]->r    = new Arrangement(r_curr);
            
        } else if (c[OMEGA]->time < time) {
            
            delete (c[OMEGA]->r);
            c[OMEGA]->time = time;
            c[OMEGA]->r    = new Arrangement(r_curr);
            
        }
        
        delete t;
        
        num_cfg++;
        
        if ((col + 1) < r_curr->getColNr()) {
            this->createCombinations(r_curr, col + 1, c, num_cfg);
        }
        
        delete r_curr;
        
    }
    
    delete [] perm;
    delete [] procs_col;
        
}



// PUBLIC FUNCTIONS

//CombinatorialHeuristic::CombinatorialHeuristic (Algorithm *alg) : Heuristic(alg) {
CombinatorialHeuristic::CombinatorialHeuristic (Algorithm *alg, string comm_mode, string network) {
    this->alg       = alg;
    this->comm_mode = comm_mode;
    this->network   = network;
}


CombinatorialHeuristic::~CombinatorialHeuristic () {
    
}


Arrangement ** CombinatorialHeuristic::apply (Arrangement *r) {
    
    // First position is for BEST (ALPHA) arrangement, and second position for WORST (OMEGA)
    Arrangement **v_arrang = new Arrangement* [3];
    
    long n_comb = this->combinations(r);
    cout << "Number of combinations: " << n_comb << endl;
    
    int col = 0;
    Config_t *c[2];

    Cost *t = new Cost();
    t = this->alg->execute(r);
    double time = t->getTime();
    delete t;

    c[ALPHA]->r = new Arrangement (r);
    c[ALPHA]->time = time;
    c[OMEGA]->r = new Arrangement (r);
    c[OMEGA]->time = time;
    
    
    int num_cfg = 0;
    this->createCombinationsCols (r, col, c, num_cfg);
    
    /* TEMPORAL
    double mintime = 9999999999.00;
    double maxtime = 0.0;
    long int maxi = -1;
    long int mini = -1;
    for (int i = 0; i < num_cfg; i++) {
        
        if (c[i].r != nullptr) {
            double t = c[i].time;
            
            char buffer [20];
            sprintf (buffer, "%010d", i);
            string name ("/Users/jarico/Development/combinations/");
            name = name + buffer + ".gpl";
            
            c[i].r->plot(name);
            cout  << fixed << setprecision(6) << name << " -> " << t << endl;
            
            if (c[i].time > maxtime) {
                maxtime = c[i].time;
                maxi = i;
            }
            
            if (c[i].time < mintime) {
                mintime = c[i].time;
                mini = i;
            }
            
        }
    }
    cout  << fixed << setprecision(6) << "MINIMUM (alpha): " << mini << " -> " << mintime << endl;
    cout  << fixed << setprecision(6) << "MAXIMUM (omega): " << maxi << " -> " << maxtime << endl;
    */
    
    // Return the minimum cost data distribution
    
    v_arrang[0] = c[ALPHA]->r;
    v_arrang[1] = c[OMEGA]->r;
    v_arrang[2] = nullptr;
    
    return (Arrangement **)v_arrang;
}



long int CombinatorialHeuristic::combinations (Arrangement *r) {
    
    // Compute all permutations in each column and entire columns
    //    r_0 ! * r_1 ! * ... * r_c !  *  #cols !
    
    long int perm_nr = 1;
    
    for (int col = 0; col < r->getColNr(); col++) {
        
        perm_nr = perm_nr * factorial(r->getRowNr(col));
        
    }
                                      
    // Rearrange columns
    perm_nr = perm_nr * factorial(r->getColNr());
    
    return perm_nr;
}
