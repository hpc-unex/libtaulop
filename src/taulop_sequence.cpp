//
//  algorithm_taulop_pbrcol.cpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_sequence.hpp"
#include "transmission.hpp"
#include "ilist.h"
#include "taulop_config.h"

#include <iostream>
using namespace std;


TauLopSequence::TauLopSequence () {
    
    this->l_seq = new IList<Transmission *> ();
    
}

TauLopSequence::~TauLopSequence () {
    
    Transmission *c = NULL;
    
    this->l_seq->moveToBegin();
    while (! this->l_seq->empty()) {
        this->l_seq->consult(c);
        delete c;
        this->l_seq->remove();
    }
    
    delete this->l_seq;
}



void TauLopSequence::add (Transmission *c) {
    
    this->l_seq->moveToEnd();
    this->l_seq->next();
    this->l_seq->insert(c);
}



Transmission * TauLopSequence::get () {
    
    Transmission *c = NULL;
    
    this->l_seq->moveToBegin();
    this->l_seq->consult(c);
    
    return c;
}


void TauLopSequence::substract (Transmission *min_c, int tau) {
    
    Transmission *c = NULL;
    
    double t_min = min_c->getCost();
    
    this->l_seq->moveToBegin();
    if (! this->l_seq->empty()) {
        
        this->l_seq->consult(c);
        
#if BUG_T == 0
        // Substract proportional time in blocks
        //long overlap = (c->getN() * t_min) / c->getCost();
        long overlap = 0;
        
        //        if (c->areConcurrent(min_c)) {
        //            overlap = min_c->getN();
        //        } else {
        //            overlap = c->getBytes(t_min, 1);
        //        }
        
        overlap = c->getBytes(t_min, tau);
        
        if (overlap < 0)
            cout << "DBG ERROR: pbrcol - not allowed overlap < 0" << endl;
        
        long curr_m = c->getM();
        
        // DBG: cout << "Overlap in " << overlap << " bytes with " << curr_n << " and " << tau << "||. ";
        
        if ((curr_m - overlap) > 0) { // TBD: a threshold for rounding issues could be convenient
            c->putM(curr_m - overlap);
            // DBG: cout << "Left: " << c->getN() << endl;
        } else { // == 0
            this->l_seq->remove();
            delete c;
            // DBG: cout << "Deleted." << endl;
        }
        
#else
        
        double tprime = c->getCost() - t_min;
        
        long   mx = 0;
        double epsilon = 0.001;
        
        if (tprime < epsilon) {
            tprime = 0.0;
            mx = 0;
        } else {
            mx = c->getBytes(tprime, tau);
        }
        
        if (mx < 0) {
            cout << "DBG ERROR: pbrcol - not allowed overlap < 0" << endl;
            mx = 0;
        }
        
        if (mx == 0) {
            this->l_seq->remove();
            delete c;
        } else {
            c->putN(mx);
        }
        
#endif
    }
}


bool TauLopSequence::empty () {
    if (this->l_seq->empty())
        return true;
    return false;
}


void TauLopSequence::compact () {
    
    Transmission *c  = NULL;
    Transmission *c2 = NULL;
    
    if (! this->l_seq->empty()) {
        this->l_seq->moveToBegin();
        this->l_seq->consult(c);
        this->l_seq->next();
    }
    while (! this->l_seq->end()) {
        
        this->l_seq->consult(c2);
        
        if (c->areConcurrent(c2) && (c->getTau() == c2->getTau())) {
            
            c->putM(c->getM() + c2->getM());
            this->l_seq->remove();
            delete c2;
            
        } else {
            
            this->l_seq->next();
            c = c2;
        }
    }
}


void TauLopSequence::show () {
    
    Transmission *c = NULL;

    
    for (int field = 0; field < 3; field++) {
        
        this->l_seq->moveToBegin();
        
        while (! this->l_seq->end()) {
            
            this->l_seq->consult(c);
            
            switch (field) {
                case 0: // show procs
                    if (c != NULL) {
                        cout << c->getSrcRank() << " -> " << c->getDstRank() << "\t\t\t";
                    } else {
                        cout << "       " << "\t\t";
                    }
                    break;
                case 1: // show Transmission
                    if (c != NULL) {
                        cout << c->getTau() << "||"  << c->getN() << "xT^" << c->getChannel() << "(" << c->getM() << ")";
                        if (this->l_seq->last()) cout << "  *-/ ";
                        else                     cout << "  *-> \t";
                    } else {
                        cout << "           " << "\t";
                    }
                    break;
                case 2: // show nodes
                    if (c != NULL) {
                        cout << c->getSrcNode() << " -> " << c->getDstNode() << "\t\t\t";
                    } else {
                        cout << "          " << "\t\t";
                    }
                    break;
                    
            }
            
            this->l_seq->next();
        }
        cout << endl;
    }
    cout << endl;
}

