//
//  W1D_algorithm.cpp
//  TauLopCost
//
//  Created by jarico on 5/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "algorithm_W1D.hpp"

#include "cost.hpp"
#include "algorithm.hpp"
#include "arrangement.hpp"
#include "taulop_cost.hpp"
#include "taulop_operator.hpp"
#include "taulop_sequence.hpp"
#include "taulop_concurrent.hpp"


#include <iostream>
using namespace std;



bool W1D_Algorithm::addCol (Arrangement *r, Rectangle *p_src, int row_nr, int col_nr, TauLopSequence *seq) {
    
    Transmission *c;
    
    Rectangle *p_dst = nullptr;
    
    // In borders cases
    if (row_nr < 0)
        return false;
    
    if (row_nr >= r->getRowNr(col_nr))
        return false;
    
    p_dst = r->get(row_nr, col_nr);
    
    int chn = (p_src->getNode() == p_dst->getNode()) ? 0 : 1; // TBD: What the channel_nr means?
    int m   = p_src->getWidth() * DATATYPE_SIZE * this->DEPTH * r->getB();
    int n   = 1;
    
    cout << p_src->getRank() << " --- " << m << "-->" << p_dst->getRank() << endl;
    
    c = new Transmission(p_src, p_dst, chn, n, m, 1);
    seq->add(c);
    
    return true;
}


bool W1D_Algorithm::addRow (Arrangement *r, Rectangle *p_src, int row_nr, int col_nr, TauLopSequence *seq) {
    
    Transmission *c;
    
    Rectangle *p_dst = nullptr;
    
    // In borders cases
    if (col_nr < 0)
        return false;
    
    if (col_nr >= r->getColNr())
        return false;
    
    
    for (int r_prime = 0; r_prime < r->getRowNr(col_nr); r_prime++) {
        
        p_dst = r->get(r_prime, col_nr);
        
        if (p_src == p_dst)
            continue;
        
        // Do they Overlap ??
        int y_src = p_src->getCoordY();
        int h_src = p_src->getHeight();
        int y_dst = p_dst->getCoordY();
        int h_dst = p_dst->getHeight();
        if ((y_src < y_dst + h_dst) && (y_src + h_src > y_dst)) {
            
            int start = y_dst - y_src;
            if (start < 0) start = 0;
            int length = 0;
            if (y_dst + h_dst > y_src + h_src) {
                length = h_src - start;
            } else {
                length = y_dst + h_dst - y_src - start;
            }
            
            // DBG:                    cout << "Overlap: " << p_src->getRank() << "|" << p_dst->getRank()
            // << " from: " << start << " in: " << length << endl;
            
            int chn = (p_src->getNode() == p_dst->getNode()) ? 0 : 1; // TBD: What the channel_nr means?
            int m   = length * DATATYPE_SIZE * this->DEPTH * r->getB();
            int n   = 1;
            
            c = new Transmission(p_src, p_dst, chn, n, m, 1);
            seq->add(c);
            
        }
        
    }
    
    return true;
}



bool W1D_Algorithm::addColConcurrent (Arrangement *r, Rectangle *p_src, int row_nr, int col_nr, TauLopConcurrent *conc) {
    
    Transmission *c;
    
    Rectangle *p_dst = nullptr;
    
    // In borders cases
    if (row_nr < 0)
        return false;
    
    if (row_nr >= r->getRowNr(col_nr))
        return false;
    
    p_dst = r->get(row_nr, col_nr);
    
    int chn = (p_src->getNode() == p_dst->getNode()) ? 0 : 1; // TBD: What the channel_nr means?
    int m   = p_src->getWidth() * DATATYPE_SIZE * this->DEPTH * r->getB();
    int n   = 1;
    
    cout << p_src->getRank() << " -- " << m << "-->" << p_dst->getRank() << endl;
    
    
    c = new Transmission(p_src, p_dst, chn, n, m, 1);
    TauLopSequence *seq = new TauLopSequence();
    seq->add(c);
    conc->add(seq);
    
    return true;
}


bool W1D_Algorithm::addRowConcurrent (Arrangement *r, Rectangle *p_src, int row_nr, int col_nr, TauLopConcurrent *conc) {
    
    Transmission *c;
    
    Rectangle *p_dst = nullptr;
    
    // In borders cases
    if (col_nr < 0)
        return false;
    
    if (col_nr >= r->getColNr())
        return false;
    
    
    for (int r_prime = 0; r_prime < r->getRowNr(col_nr); r_prime++) {
        
        p_dst = r->get(r_prime, col_nr);
        
        if (p_src == p_dst)
            continue;
        
        // Do they Overlap ??
        int y_src = p_src->getCoordY();
        int h_src = p_src->getHeight();
        int y_dst = p_dst->getCoordY();
        int h_dst = p_dst->getHeight();
        if ((y_src < y_dst + h_dst) && (y_src + h_src > y_dst)) {
            
            int start = y_dst - y_src;
            if (start < 0) start = 0;
            int length = 0;
            if (y_dst + h_dst > y_src + h_src) {
                length = h_src - start;
            } else {
                length = y_dst + h_dst - y_src - start;
            }
            
            // DBG:                    cout << "Overlap: " << p_src->getRank() << "|" << p_dst->getRank()
            // << " from: " << start << " in: " << length << endl;
            
            int chn = (p_src->getNode() == p_dst->getNode()) ? 0 : 1; // TBD: What the channel_nr means?
            int m   = length * DATATYPE_SIZE * this->DEPTH * r->getB();
            int n   = 1;
            
            c = new Transmission(p_src, p_dst, chn, n, m, 1);
            TauLopSequence *seq = new TauLopSequence();
            seq->add(c);
            conc->add(seq);
            
        }
        
    }
    
    return true;
}




W1D_Algorithm::W1D_Algorithm () {
    
}

W1D_Algorithm::~W1D_Algorithm () {

}


Cost * W1D_Algorithm::execute (Arrangement *r) {
    
    
#if TLOP_DEBUG == 1
    // DBG:
    cout << "P: " << r->getP() << endl;
#endif
  
    // 1. Communication from every process to the rest
    TauLopConcurrent *concurrent = new TauLopConcurrent();
    for (int col_nr = 0; col_nr < r->getColNr(); col_nr++) {
        
        for (int row_nr = 0; row_nr < r->getRowNr(col_nr); row_nr++) {

            // 2.1. Fill a process communications. It is a sequence object.
            TauLopSequence *seq = new TauLopSequence ();

            Rectangle *p_src = r->get(row_nr, col_nr);
            
            // N
            this->addCol(r, p_src, row_nr - 1, col_nr, seq);
            
            // E
            //this->addRow(r, p_src, row_nr, col_nr + 1, seq);
            
            // S
            this->addCol(r, p_src, row_nr + 1, col_nr, seq);
            
            // W
            //this->addRow(r, p_src, row_nr, col_nr - 1, seq);
            
            seq->compact();
            concurrent->add(seq);
        }
    }
 
    
    
/*
    for (int col_nr = 0; col_nr < r->getColNr(); col_nr++) {
        
        for (int row_nr = 0; row_nr < r->getRowNr(col_nr); row_nr++) {
            
            // 2.1. Fill a process communications.
            TauLopSequence *seqN = new TauLopSequence ();
            TauLopSequence *seqS = new TauLopSequence ();
            //TauLopSequence *seqE = new TauLopSequence ();
            //TauLopSequence *seqW = new TauLopSequence ();
            
            Rectangle *p_src = r->get(row_nr, col_nr);
            
            // N
            if (this->addCol(r, p_src, row_nr - 1, col_nr, seqN)) {
                seqN->compact();
                concurrent->add(seqN);
            }
            
            // E
            //if (this->addRow(r, p_src, row_nr, col_nr + 1, seqE)) {
            //    seqE->compact();
            //    concurrent->add(seqE);
            //}
            
            // S
            if (this->addCol(r, p_src, row_nr + 1, col_nr, seqS)) {
                seqS->compact();
                concurrent->add(seqS);
            }
            
            // W
            //if (this->addRow(r, p_src, row_nr, col_nr - 1, seqW)) {
            //    seqW->compact();
            //    concurrent->add(seqW);
            //}
    
        }
    }
*/
    
    /*
    
    for (int col_nr = 0; col_nr < r->getColNr(); col_nr++) {
        
        for (int row_nr = 0; row_nr < r->getRowNr(col_nr); row_nr++) {
            
            Rectangle *p_src = r->get(row_nr, col_nr);
            
            // N
            this->addColConcurrent(r, p_src, row_nr - 1, col_nr,     concurrent);
            // E
            // this->addRowConcurrent(r, p_src, row_nr,     col_nr + 1, concurrent);
            // S
            this->addColConcurrent(r, p_src, row_nr + 1, col_nr,     concurrent);
            // W
            //this->addRowConcurrent(r, p_src, row_nr,     col_nr - 1, concurrent);
        }
    }

     */
    
#if TLOP_DEBUG == 1
    concurrent->show();
#endif
    
    // 3. Create Cost structure
    TauLopCost *tc = new TauLopCost();
    
    // 4. Apply the communication cost operation
    concurrent->evaluate(tc);
    
#if TLOP_DEBUG == 1
    // DBG:
    tc->show();
    cout << endl << " W1D Time: " << tc->getTime() << endl << endl;
#endif
    
    // 4.1. The cost
    Cost *t = new Cost(0, tc->getTime());
    
    // Fill Cost object
    t->putTimeVert  (0.0);
    t->putTimeHoriz (0.0);

    delete tc;
    delete concurrent;

    return t;
}


