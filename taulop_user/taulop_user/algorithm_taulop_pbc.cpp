//
//  algorithm_taulop_pbc.cpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "algorithm_taulop_pbc.hpp"

#include "matrix_config.h"
#include "taulop_cost.hpp"
#include "taulop_operator.hpp"
#include "arrangement.hpp"
#include "taulop_concurrent.hpp"
#include "taulop_sequence.hpp"

#include <iostream>
using namespace std;



// PRIVATE Methods

int TauLopPBC::overlap (Arrangement *r, Rectangle *p_src, Rectangle *p_dst, Rectangle *p_orig) {
    
    int length = 0;
    
    // Do they Overlap ??
    int y_src = p_src->getCoordY();
    int h_src = p_src->getHeight();
    int y_dst = p_dst->getCoordY();
    int h_dst = p_dst->getHeight();
    if ((y_src < y_dst + h_dst) && (y_src + h_src > y_dst)) {
        
        int start = y_dst - y_src;
        if (start < 0) start = 0;
        
        if (y_dst + h_dst > y_src + h_src) {
            length = h_src - start;
        } else {
            length = y_dst + h_dst - y_src - start;
        }
        
        // DBG:        cout << "Overlap: " << p_src->getRank() << "|" << p_dst->getRank()
        // << " from: " << start << " in: " << length << endl;
        
        int b = r->getB();
        
        // Blocks to bytes
        length = length * b * b * DATATYPE_SIZE;
    }
    
    return length;
}

void TauLopPBC::fillRow (TauLopSequence *seq, Arrangement *r, int b_ini, int col, int row) {
    
    Transmission *c;
    
    Rectangle *p_src = nullptr;
    Rectangle *p_dst = nullptr;
    
    
    // 1. Depending on the algorithm, create communications per row.
    
    if (row_algorithm == PT2PT) {
        
        p_src = r->get(row, col);
        for (int col_nr = 0; col_nr < r->getColNr(); col_nr++) {
            
            if (col_nr == col) continue;
            
            for (int row_nr = 0; row_nr < r->getRowNr(col_nr); row_nr++) {
                
                p_dst = r->get(row_nr, col_nr);
                
                // Do they Overlap ??
                int y_src = p_src->getCoordY();
                int h_src = p_src->getHeight();
                int y_dst = p_dst->getCoordY();
                int h_dst = p_dst->getHeight();
                if ((y_src < y_dst + h_dst) && (y_src + h_src > y_dst)) {
                    
                    int start = y_dst - y_src;
                    if (start < 0) start = 0;
                    int length;
                    if (y_dst + h_dst > y_src + h_src) {
                        length = h_src - start;
                    } else {
                        length = y_dst + h_dst - y_src - start;
                    }
                    
                    // DBG:                    cout << "Overlap: " << p_src->getRank() << "|" << p_dst->getRank()
                    // << " from: " << start << " in: " << length << endl;
                    
                    int m = 1;
                    int chn = (p_src->getNode() == p_dst->getNode()) ? 0 : 1;
                    int b = r->getB();
                    
                    // Blocks to bytes
                    length = length * b * b * DATATYPE_SIZE;
                    
                    c = new Transmission(p_src, p_dst, chn, m, length, 1);
                    seq->add(c);
                    
                }
                
            }
            
        }
        
    } else if (row_algorithm == RING) {
        
        p_src = r->get(row, col);
        
        int col_dst = (col + 1) % r->getColNr();
        
        for (int row_nr = 0; row_nr < r->getRowNr(col_dst); row_nr++) {
            
            p_dst = r->get(row_nr, col_dst);
            
            // Do they Overlap ??
            int y_src = p_src->getCoordY();
            int h_src = p_src->getHeight();
            int y_dst = p_dst->getCoordY();
            int h_dst = p_dst->getHeight();
            if ((y_src < y_dst + h_dst) && (y_src + h_src > y_dst)) {
                
                int start = y_dst - y_src;
                if (start < 0) start = 0;
                int length;
                if (y_dst + h_dst > y_src + h_src) {
                    length = h_src - start;
                } else {
                    length = y_dst + h_dst - y_src - start;
                }
                
                // DBG:                    cout << "Overlap: " << p_src->getRank() << "|" << p_dst->getRank()
                // << " from: " << start << " in: " << length << endl;
                
                int m = 1;
                int chn = (p_src->getNode() == p_dst->getNode()) ? 0 : 1;
                int b = r->getB();
                
                // Blocks to bytes
                length = length * b * b * DATATYPE_SIZE;
                
                c = new Transmission(p_src, p_dst, chn, m, length, 1);
                seq->add(c);
                
            }
        }
        
    } else {
        cout << "DBG: Error, PBC algorithm not found or defined " << endl;
    }
    
}



//  PUBLIC Interface


TauLopPBC::TauLopPBC () {
}

TauLopPBC::~TauLopPBC () {
}


void TauLopPBC::getPBCCost (Arrangement *r, double &time) {
    
    int  b_size = 0;
    int  b_ini  = 0;
    int  col_nr = 0;
    int  nprocs = 0;
    
    
    if (row_algorithm == PT2PT) {
        
        do { // Loop over all the horizontal intervals of the matrix arrangements
            
            // 1. Find the next block size
            r->getNextHInterval(b_ini, b_size);
            nprocs = r->getRowNr(col_nr);
            
#if TLOP_DEBUG == 1
            // DBG:
            cout << "b_ini: " << b_ini << "  size: " << b_size << ". With num. procs: " << nprocs << endl;
#endif
            
            // 2. Row communications for this interval (concurrents)
            TauLopConcurrent *concurrent = new TauLopConcurrent();
            for (int p = 0; p < nprocs; p++) {
                
                // 2.1. Fill a row communications. It is a sequence object.
                TauLopSequence *seq = new TauLopSequence ();
                this->fillRow (seq, r, b_ini, col_nr, p);
                
                seq->compact();
                concurrent->add(seq);
            }
            
#if TLOP_DEBUG == 1
            concurrent->show();
#endif
            
            // 3. Create Cost structure
            TauLopCost *tc = new TauLopCost();
            
            // 4. Apply the communication cost operation for only ONE pbc
            concurrent->evaluate(tc);
            
#if TLOP_DEBUG == 1
            // DBG:
            tc->show();
            cout << endl << " PBC Time: " << tc->getTime() << endl << endl;
#endif
            
            // 4.1. The cost is multiplied for the range of rows evaluated.
            time += (tc->getTime() * b_size);
            
            delete tc;
            delete concurrent;
            
            // 5. Next interval
            col_nr++;
            b_ini += b_size;
            
        } while (b_ini < r->getWidth());
        
        
    } else if (row_algorithm == RING) {
        
        int col_src;
        int col_dst;
        
        do { // Loop over all the horizontal intervals of the matrix arrangements
            
            // 1. Find the next block size
            r->getNextHInterval(b_ini, b_size);
            
            col_src = col_nr;
            
            nprocs = r->getRowNr(col_nr);
            
#if TLOP_DEBUG == 1
            // DBG:
            cout << "b_ini: " << b_ini << "  size: " << b_size << ". With num. procs: " << nprocs << endl;
#endif
      
            double t = 0.0;
            
            for (int col = 0; col < r->getColNr() - 1; col++) {
                
                col_dst = (col_src + 1) % r->getColNr();
                
                
                // 2. Row communications for this interval (concurrents)
                TauLopConcurrent *concurrent = new TauLopConcurrent();
                for (int p = 0; p < nprocs; p++) {
                    
                    // 2.1. Fill a row communications. It is a sequence object.
                    TauLopSequence *seq = new TauLopSequence ();
                    this->fillRow (seq, r, b_ini, col_nr, p);
                    
                    seq->compact();
                    concurrent->add(seq);
                }
                
#if TLOP_DEBUG == 1
                concurrent->show();
#endif
                
                // 3. Create Cost structure
                TauLopCost *tc = new TauLopCost();
                
                // 4. Apply the communication cost operation for only ONE pbc
                concurrent->evaluate(tc);
                
#if TLOP_DEBUG == 1
                // DBG:
                tc->show();
                cout << endl << " PBC Time: " << tc->getTime() << endl << endl;
#endif
                
                // 4.1. The cost is multiplied for the range of rows evaluated.
                t += tc->getTime();
                
                delete tc;
                delete concurrent;
                
            }
            
            time = time + (t * b_size);
            
            // 5. Next interval
            col_nr++;
            b_ini += b_size;
            
        } while (b_ini < r->getWidth());
        
        
    } else {
        cerr << "ERROR: no algorithm found for PBC communication." << endl;
    }
}



