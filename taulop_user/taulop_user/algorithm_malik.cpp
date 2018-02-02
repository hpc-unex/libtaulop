//
//  algorithm_malik.cpp
//  TauLopCost
//
//  Created by jarico on 22/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "algorithm_malik.hpp"


MalikAlgorithm::MalikAlgorithm  () {
    
}


MalikAlgorithm::~MalikAlgorithm () {
    
}


Cost * MalikAlgorithm::execute (Arrangement *arrang) {
    
    Cost *cost = new Cost();
    int   col_nr = arrang->getColNr();
    int   pivot_row = 0;
    int   v_procnr [NUM_PROC];
    int   N = arrang->getWidth();
    int   hops;
    int   t_vol = 0; // Total volume:  SUM_c (o_vol) in H and V
    
    
    // Horizontal cost
    //cout << "Horizontal" << endl << endl;
    
    for (int i = 0; i < col_nr; i++)  v_procnr[i] = 0;
    pivot_row = 0;
    
    while (pivot_row < N) {
        
        int min = N + 1; // Higher than the maximum value
        int p_nr_col = -1;
        int o_vol = 0;   // This hop cost
        
        for (int i = 0; i < col_nr; i++) {
            
            Rectangle *p_tmp = arrang->get (v_procnr[i], i);
            
            // Process hop
            int hop = (p_tmp->getCoordY() + p_tmp->getHeight()) - pivot_row;
            
            if (hop < min) { // We take the minimum
                min = hop;
                p_nr_col = i;
            }
        }
        
        Rectangle *p = arrang->get(v_procnr[p_nr_col], p_nr_col);
        hops = 0;
        o_vol = 0;
        
        for (int j = 0; j < col_nr; j++) {
            
            int tmp_hops = 0;
            for (int i = 0; i < (col_nr - 1); i++) {
                
                Rectangle *p_orig = arrang->get (v_procnr[j], j);
                Rectangle *p_dest = arrang->get (v_procnr[(j + 1) % col_nr], (j + 1) % col_nr);
                
                if (p_orig->getNode() != p_dest->getNode())  tmp_hops++;
            }
            
            // Maximum of all hops between processes
            if (tmp_hops > hops)   hops = tmp_hops;
        }
        
        // Next process (row) in the column selected
        v_procnr[p_nr_col] = v_procnr[p_nr_col] + 1;
        if (v_procnr[p_nr_col] == arrang->getRowNr(p_nr_col))
            v_procnr[p_nr_col] = arrang->getRowNr(p_nr_col) - 1;
        
        // Calculate and accumulate
        o_vol = (hops * min) * p->getWidth();
        pivot_row = pivot_row + min;
        t_vol = t_vol + o_vol;
        
        //cout << "O: " << p->getRank() << ": " << min << " / " << hops << " -> " << o_vol << endl;
        
    }
    
    cost->putVolIntraHoriz(0);  // Malik doe not consider the intranode cost
    cost->putVolInterHoriz(t_vol);
    
    
    // Vertical cost
    //cout << endl << "Vertical" << endl << endl;

    int  c_hops = 0; // Per column hops
    int  c_vol  = 0; // Per column volume
    t_vol  = 0; // Total volume:  SUM_c (c_vol)
    
    for (int col_nr = 0; col_nr < arrang->getColNr(); col_nr++) {
        
        c_hops = 0;
        
        int row_nr  = arrang->getRowNr(col_nr);
        
        for (int n_row = 0; n_row < row_nr; n_row++) { // Counting hops departing from process n_row
            
            int n_proc = n_row;
            int hops = 0;
            int i = 0;
            
            while (i++ < (row_nr - 1)) {
                
                Rectangle *p_orig = arrang->get (n_proc, col_nr);
                Rectangle *p_dest = arrang->get ((n_proc + 1) % row_nr, col_nr);
                
                if (p_orig->getNode() != p_dest->getNode())  hops++;
                
                n_proc = (n_proc + 1) % row_nr;
                
                // cout << p_orig->getRank() << " -> " << p_dest->getRank()
                // << "   " << hops
                // << " * " << p_orig->getWidth() << endl;
            }
            
            if (hops > c_hops)  // Get the maximum
                c_hops = hops;
        }
        
        c_vol = c_hops * arrang->getWidth(col_nr);
        
        t_vol += c_vol;
    }
    
    t_vol = t_vol * arrang->getWidth();
    
    cost->putVolIntraVert(0);
    cost->putVolInterVert(t_vol);

    //cout << "VERTICAL cost is: " << t_vol << endl;
    
    return cost;
}

