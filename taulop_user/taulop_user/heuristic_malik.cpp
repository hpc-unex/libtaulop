//
//  heuristic_malik.cpp
//  TauLopCost
//
//  Created by jarico on 26/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "heuristic.hpp"
#include "heuristic_malik.hpp"

#include "algorithm.hpp"
#include "rectangle.hpp"
#include "arrangement.hpp"

#include <iostream>
#include <string>
#include <iomanip>





void MalikHeuristic::createCombinations (Arrangement *r, Config_t *c, int &num_cfg) {
    
    // Possible combinations:
    //     fact(g_0) + fact(g_1) + ... + fact(g_n) + fact(#cols)
    
    // 1. Compact procs in groups of equal node
    Arrangement *r_curr = new Arrangement(r);
    r_curr->groupByNode();
    
    
    // 3. Is this the best/worst configuration?
    // (a) Estimate time
    Cost *total_t = new Cost();
    total_t = this->alg->execute(r_curr);
    double total_time = total_t->getTime();
    
    // (b) Test current alpha and omega times
    c->time = total_time;
    c->r    = new Arrangement(r_curr);
    
    Arrangement *s = nullptr;
    
    // 3. Rest of columns: permute by column and find better arrangement incrementally
    for (int col = 0; col < r_curr->getColNr(); col++) {
        
        if (col == 0) {
            
            // For the first column, create the arrangement
            int cols[1] = {0};
            s = r_curr->createSubArrangement(1, cols);
            
        } else {
            
            // 3.a) Add column to s
            Column *cm = r_curr->getColumn(col);
            s->addColumn(cm);
        }
        
        // 2.b) Permutations between groups in the first column
        int  groups_nr = 0;
        int  curr_node = -1;
        int *groups = new int [s->getNodeNr(col)];
        
        for (int row = 0; row < s->getRowNr(col); row++) {
            
            Rectangle *p = s->get(row, col);
            if (curr_node != p->getNode()) {
                curr_node = p->getNode();
                groups[groups_nr] = curr_node;
                groups_nr++;
            }
            
        }
        
        // Given the number of groups, we create a vector of all possible permutations
        long int tot_perm_nr = factorial(groups_nr);
        
        int *perm = new int [tot_perm_nr * groups_nr];
        this->generate_permutations(groups, groups_nr, perm);
        
        // Test all possible permutations
        for (int perm_nr = 0; perm_nr < tot_perm_nr; perm_nr++) {
            
            // Create a new arrangement from the best one
            Arrangement *s_curr = new Arrangement (s);
            
            int rows[s->getRowNr(col)];
            int row_nr = 0;
            
            for (int g = 0; g < groups_nr; g++) {
                
                for (int k = 0; k < s->getRowNr(col); k++) {
                    
                    Rectangle *p = s->get(k, col);
                    if (p->getNode() == perm[(perm_nr * groups_nr) + g]) {
                        
                        rows[row_nr] = p->getRank();
                        row_nr++;
                    }
                }
            }
            
            s_curr->changeRows(col, rows);
            
            Cost *t = this->alg->execute(s_curr);
            double time = t->getTime();
            
            if (perm_nr == 0) {
                
                c->time = time;
                c->r    = new Arrangement(s_curr);
                
            } else {
                // Test current alpha and omega times
                if (c->time > time) {
                    
                    delete (c->r);
                    c->time = time;
                    c->r    = new Arrangement(s_curr);
                }
            }
            
            delete s_curr;
            
            num_cfg++;
            delete t;
        }
        
        delete [] perm;
        
        delete s;
        s = c->r;
        
#if TLOP_DEBUG == 1
        cout << fixed << setprecision(6) << "Time (col = " << col << "): " << c->time  / 1000000.0 << endl;
        s->show();
        s->plot("/Users/jarico/Development/combinations/s.gpl");
#endif
    }
    
    
    // 4. Test permutations of columns
    int col_nr = s->getColNr();
    long int tot_col_permnr = factorial(col_nr);
    int *columns = new int [col_nr];
    for (int i = 0; i < col_nr; i++) columns[i] = i;
    
    int new_cols[col_nr]; // New positions of the columns
    
    // Generate all possible permutations of entire columns
    int *perm_col = new int [tot_col_permnr * col_nr];
    this->generate_permutations(columns, col_nr, perm_col);
    
    // Test all permutations
    for (int perm_nr = 0; perm_nr < tot_col_permnr; perm_nr++) {
        
        Arrangement *s_curr = new Arrangement (s);
        
        // Change columns according to the permutation
        //cout << "Changing to ... [";
        for (int c = 0; c < col_nr; c++) {
            new_cols[c] = perm_col[(perm_nr * col_nr) + c];
            //cout << new_cols[c]; if (c != col_nr-1) cout << ",";
        }
        //cout << "]" << endl;
        s_curr->changeCols(new_cols);
        
        // Get the cost of this permutation  (in volume ??)
        Cost *t = new Cost();
        t = this->alg->execute(s_curr);
        double time = t-> getTime();
        
        if (perm_nr == 0) {
            
            c->time = time;
            c->r    = new Arrangement(s_curr);
            
        } else {
            // Test current alpha and omega times
            if (c->time > time) {
                
                delete (c->r);
                c->time = time;
                c->r    = new Arrangement(s_curr);
                
            }
        }
        
        delete s_curr;
    }
    
    delete [] perm_col;
    delete [] columns;
    
#if TLOP_DEBUG == 1
    cout << fixed << setprecision(6) << "Time (columns): " << c->time  / 1000000.0 << endl;
    c[OMEGA]->r->show();
    c[OMEGA]->r->plot("/Users/jarico/Development/combinations/s.gpl");
#endif
    
    // Free memory and return
    return;
}





// PUBLIC interface

MalikHeuristic::MalikHeuristic (Algorithm *alg, string comm_mode, string network) {
    this->alg       = alg;
    this->comm_mode = comm_mode;
    this->network   = network;
    
}


MalikHeuristic::~MalikHeuristic () {
    
}


Arrangement ** MalikHeuristic::apply (Arrangement *r) {
    
    
    // First position is for BEST (ALPHA) arrangement, and second position for WORST (OMEGA)
    Arrangement **v_arrang = new Arrangement* [3];
    
    //long n_comb = this->combinations(r);
    //cout << "Number of combinations: " << n_comb << endl;
    
    Config_t *c = new Config_t;
    
    Cost *t = new Cost();
    t = this->alg->execute(r);
    double time = t->getTime();
    delete t;
    
    c->r    = new Arrangement (r);
    c->time = time;
    
    
    int num_cfg = 0;
    this->createCombinations (r, c, num_cfg);
    
    
    v_arrang[0] = c->r;
    v_arrang[1] = nullptr;
    
    return (Arrangement **)v_arrang;
}


long int MalikHeuristic::combinations (Arrangement *r) {
    
    
    // Compute all permutations in each column and entire columns
    //    r_0 ! + r_1 ! + ... + r_c !  +  #cols !
    
    Arrangement *r_curr = new Arrangement(r);
    long int perm_nr = 0;
    int      groups;
    int      curr_node;
    
    // 1. How many groups per column.
    r_curr->groupByNode();
    
    for (int col = 0; col < r_curr->getColNr(); col++) {
        
        curr_node = -1;
        groups = 0;
        
        for (int row = 0; row < r_curr->getRowNr(col); row++) {
            
            Rectangle *p = r_curr->get(row, col);
            if (curr_node != p->getNode()) {
                groups++;
                curr_node = p->getNode();
            }
        }
        
        perm_nr = perm_nr + factorial(groups);
    }
    // Rearrange columns
    perm_nr = perm_nr + factorial(r_curr->getColNr());
    
    delete r_curr;
    return perm_nr;
}

