//
//  algorithm_basic.cpp
//  TauLopCost
//
//  Created by jarico on 22/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "algorithm_basic.hpp"


BasicAlgorithm::BasicAlgorithm  () {
    
}


BasicAlgorithm::~BasicAlgorithm () {
    
}


Cost * BasicAlgorithm::execute (Arrangement *arrang) {
    
    Cost *cost = new Cost();

    int  t_vol_inter = 0;  // Total comm. volume
    int  t_vol_intra = 0;
    int  r_vol_intra = 0;  // Per Row comm. volume
    int  r_vol_inter = 0;

    int  p_vol_inter = 0;  // Per process comm. volume
    int  p_vol_intra = 0;

    
    // Horizontal cost
    //cout << "Horizontal" << endl << endl;
    
    for (int col_nr = 0; col_nr < arrang->getColNr(); col_nr++) {
        
        for (int n_proc = 0; n_proc < arrang->getRowNr(col_nr); n_proc++) {
            
            Rectangle *p_orig = arrang->get(n_proc, col_nr);
            
            p_vol_inter = 0;
            p_vol_intra = 0;
            
            for (int col_dest_nr = 0; col_dest_nr < arrang->getColNr(); col_dest_nr++) {
                
                if (col_nr != col_dest_nr) {
                    
                    for (int n_proc_dest = 0; n_proc_dest < arrang->getRowNr(col_dest_nr); n_proc_dest++) {
                        
                        Rectangle *p_dest = arrang->get(n_proc_dest, col_dest_nr);
                        
                        int pair_intra = h_intraLimits(p_orig, p_dest);
                        int pair_inter = h_interLimits(p_orig, p_dest);
                        
                        // cout << p_orig->getRank() << " -> " << p_dest->getRank()
                        // << "   " << pair_intra << " / " << pair_inter
                        // << " * " << p_orig->getWidth() << endl;
                        
                        p_vol_intra += (pair_intra * p_orig->getWidth());
                        p_vol_inter += (pair_inter * p_orig->getWidth());
                        
                    }
                }
            }
            
            r_vol_intra += p_vol_intra;
            r_vol_inter += p_vol_inter;
        }
    }
    
    t_vol_intra = r_vol_intra;
    t_vol_inter = r_vol_inter;
    
    //cout << "HORIZONTAL cost is: " << t_vol_intra << " / " << t_vol_inter << endl;
    
    cost->putVolIntraHoriz(t_vol_intra);
    cost->putVolInterHoriz(t_vol_inter);
    
    
    // Vertical cost
    //cout << endl << "Vertical" << endl << endl;

    int  c_vol_inter = 0;  // Per column comm. volume
    int  c_vol_intra = 0;
    
    t_vol_intra = 0;
    t_vol_inter = 0;
    
    for (int col_nr = 0; col_nr < arrang->getColNr(); col_nr++) {

        c_vol_inter = 0;
        c_vol_intra = 0;

        for (int n_proc = 0; n_proc < arrang->getRowNr(col_nr); n_proc++) {
            
            Rectangle *p_orig = arrang->get(n_proc, col_nr);
            
            p_vol_inter = 0;
            p_vol_intra = 0;
            
            for (int n_proc_dest = 0; n_proc_dest < arrang->getRowNr(col_nr); n_proc_dest++) {
                
                Rectangle *p_dest = arrang->get(n_proc_dest, col_nr);
                
                if (p_orig->getRank() != p_dest->getRank()) {

                    int pair_intra = v_intraLimits(p_orig, p_dest);
                    int pair_inter = v_interLimits(p_orig, p_dest);
                    
                    // cout << p_orig->getRank() << " -> " << p_dest->getRank()
                    // << "   " << pair_intra << " / " << pair_inter
                    // << " * " << p_orig->getWidth() << endl;

                    p_vol_intra += (pair_intra * p_orig->getHeight());
                    p_vol_inter += (pair_inter * p_orig->getHeight());

                }
                
            }
            
            c_vol_intra += p_vol_intra;
            c_vol_inter += p_vol_inter;
            
        }
        
        t_vol_intra += c_vol_intra;
        t_vol_inter += c_vol_inter;
        
    }

    cost->putVolIntraVert(t_vol_intra);
    cost->putVolInterVert(t_vol_inter);
    
    //cout << "VERTICAL cost is: " << t_vol_intra << " / " << t_vol_inter << endl;
    
    
    return cost;
}

