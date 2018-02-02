//
//  taulop_operator.cpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_operator.hpp"



// UTILITY Functions

static Transmission *MIN (Transmission *a, Transmission *b) {
    
    if (a == NULL) return b;
    if (b == NULL) return a;
    
    if (a->getCost() > b->getCost()) return b;
    else return a;
}



// PRIVATE methods



void TauLopOperator::show (IList<Transmission *> *l) {
    
    Transmission *c = NULL;
    
    for (int field = 0; field < 7; field++) {
        
        l->moveToBegin();
        while (!l->end()) {
            
            l->consult(c);
            
            switch (field) {
                case 0: // show procs
                    if (c != NULL) {
                        cout << c->getSrcRank() << " -> " << c->getDstRank() << "\t\t";
                    } else {
                        cout << "      " << "\t\t";
                    }
                    break;
                case 1: // show channel
                    if (c != NULL) {
                        cout << "  " << c->getChannel() << "    \t\t";
                    } else {
                        cout << "        " << "\t\t";
                    }
                    break;
                case 2: // show n and m
                    if (c != NULL) {
                        cout << c->getM() << "  " << c->getN() << " \t\t";
                    } else {
                        cout << "        " << "\t\t";
                    }
                    break;
                case 3: // show m x n
                    if (c != NULL) {
                        cout << "(" << c->getN() * c->getM() << ")  \t\t";
                    } else {
                        cout << "        " << "\t\t";
                    }
                    break;
                case 4: // show tau
                    if (c != NULL) {
                        cout << " " << c->getTau() << " || \t\t";
                    } else {
                        cout << "       " << "\t\t";
                    }
                    break;
                case 5: // show node_dst
                    if (c != NULL) {
                        cout << " -> " << c->getDstNode() << "  \t\t";
                    } else {
                        cout << "        " << "\t\t";
                    }
                    break;
                case 6: // Time cost
                    if (c != NULL) {
                        cout << "t=" << c->getCost() << " \t";
                    } else {
                        cout << "      " << "\t\t";
                    }
                    break;
                    
            }
            
            l->next();
        }
        cout << endl;
        
    }
    cout << endl;
}




// PUBLIC interface

TauLopOperator::TauLopOperator () {
    
    this->l_comm = new IList<Transmission *> ();
    this->l_real_conc = new IList<Transmission *> ();
}

TauLopOperator::~TauLopOperator () {
    
    // list of possibly concurrent held pointers to original Comms
    delete this->l_comm;
    
    
    // List of real comms held pointers to new created objects. Erase them.
    Transmission *c = NULL;
    
    this->l_real_conc->moveToBegin();
    while (! this->l_real_conc->end()) {
        this->l_real_conc->consult(c);
        delete c;
        this->l_real_conc->remove();
    }
    delete this->l_real_conc;
}


void TauLopOperator::add (Transmission *c) {
    
    this->l_comm->moveToEnd();
    this->l_comm->next();
    this->l_comm->insert(c);
}


void TauLopOperator::evaluate () {
    
    Transmission *c_comm = NULL;
    Transmission *c_real = NULL;
    
    
    this->l_comm->moveToBegin();
    while (!this->l_comm->empty()) {
        
        this->l_comm->consult(c_comm);
        
        bool found = false;
        
        this->l_real_conc->moveToBegin();
        while ((!this->l_real_conc->end()) && (!found)) {
            this->l_real_conc->consult(c_real);
            
            if (c_real->areConcurrent(c_comm)) {
                found = true;
                c_real->getOverlap(c_comm);

            } else {
                this->l_real_conc->next();
            }
            
        }
        
        this->l_comm->remove();
        
        if (!found) {
            this->l_real_conc->insert(new Transmission (c_comm));
        }
        
    }
}



double TauLopOperator::getMinCost (Transmission * &c) {
    
    Transmission *c_min = NULL;
    Transmission *c_aux = NULL;
    
    this->l_real_conc->moveToBegin();
    while (!this->l_real_conc->end()) {
        this->l_real_conc->consult(c_aux);
        
        c_min = MIN (c_aux, c_min);
        
        this->l_real_conc->next();
    }
    
    c = c_min;
    return c->getCost();
}



int TauLopOperator::getConcurrency (Transmission * c) {
    
    Transmission *c_aux = NULL;
    bool  found = false;
    int   tau   = 0;
    
    this->l_real_conc->moveToBegin();
    while (!this->l_real_conc->end() && !found) {
        this->l_real_conc->consult(c_aux);
        
        if (c->areConcurrent(c_aux)) {
            tau = c_aux->getTau();
            found = true;
        }
        
        this->l_real_conc->next();
    }
    
    return tau;
}


void TauLopOperator::show_init_comms () {
    cout << "Communications starting at the same time (possibly concurrent): " << endl;
    show(this->l_comm);
}


void TauLopOperator::show_concurrent () {
    cout << "Concurrent communications (really concurrent): " << endl;
    show(this->l_real_conc);
}

