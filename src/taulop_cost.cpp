//
//  taulop_cost.cpp
//  TauLopCost
//
//  Created by jarico on 9/5/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "taulop_cost.hpp"

#include "transmission.hpp"
#include "ilist.h"

#include <iostream>
using namespace std;


TauLopCost::TauLopCost () {
    this->cost = new IList<Transmission *> ();
}

TauLopCost::TauLopCost (const TauLopCost *tc) {

    this->cost = new IList<Transmission *> ();
    Transmission *T;

    tc->cost->moveToBegin();
    while (! tc->cost->end()) {
        tc->cost->consult(T);
        this->cost->insert(new Transmission(T));
        tc->cost->next();
    }
}

TauLopCost::~TauLopCost () {
    
    Transmission *c = nullptr;
    
    this->cost->moveToBegin();
    while (! this->cost->end()) {
        this->cost->consult(c);
        delete c;
        this->cost->next();
    }
    delete this->cost;
}


void TauLopCost::add (Transmission *c) {
    
    // Duplicate the Transmission object.
    Transmission *c_aux = new Transmission (c);
    
    this->cost->moveToEnd();
    this->cost->next();
    this->cost->insert(c_aux);
    
}


void TauLopCost::add (const TauLopCost *tc) {
    
    Transmission *c   = nullptr;
    Transmission *aux = nullptr;
    
    this->cost->moveToEnd();
    
    tc->cost->moveToBegin();
    while (! tc->cost->end()) {
        tc->cost->consult(c);
        this->cost->next();
        aux = new Transmission(c);
        this->cost->insert(aux);
        tc->cost->next();
    }
}


double TauLopCost::getTime () {
    
    Transmission   *T;
    double  t = 0.0;
    
    this->cost->moveToBegin();
    while (!this->cost->end()) {
        this->cost->consult(T);
        
        t = t + T->getCost();
        
        this->cost->next();
    }
    
    return t;
}


double TauLopCost::getTime (int chn) {
    
    Transmission   *T;
    double  t = 0.0;
    
    this->cost->moveToBegin();
    while (!this->cost->end()) {
        this->cost->consult(T);
        
        if (T->getChannel() == chn) {
            t = t + T->getCost();
        }
        
        this->cost->next();
    }
    
    return t;
}



long TauLopCost::getVolume () {
    
    Transmission  *T;
    long   vol = 0;
    
    this->cost->moveToBegin();
    while (!this->cost->end()) {
        this->cost->consult(T);

        vol = vol + (T->getN() * T->getM());
        
        this->cost->next();
    }
    
    return vol;
}


long TauLopCost::getVolume (int chn) {
    
    Transmission  *T;
    long   vol = 0;
    
    this->cost->moveToBegin();
    while (!this->cost->end()) {
        this->cost->consult(T);
        
        if (T->getChannel() == chn) {
            vol = vol + (T->getN() * T->getM());
        }
        
        this->cost->next();
    }
    
    return vol;
}



void TauLopCost::compact () {
    
    Transmission *c  = nullptr;
    Transmission *c2 = nullptr;
    IList<Transmission *> *l_aux = new IList<Transmission *> ();
    
    
    while (! this->cost->empty()) {
        
        this->cost->moveToBegin();
        if (! this->cost->end()) {
            this->cost->consult(c);
            c->putM(c->getM() * c->getN());
            c->putN(1);
            this->cost->remove();
        }
        
        while (! this->cost->end()) {
            
            this->cost->consult(c2);
            
            if (c->areCompactable(c2)) {
                
                c->compact(c2);
                this->cost->remove();
                delete c2;
                
            } else {
                this->cost->next();
            }
            
        }
        
        // Insert in order: channel + tau
        Transmission *c_aux;
        l_aux->moveToBegin();
        bool enc = false;
        while (! l_aux->end() && ! enc) {
            l_aux->consult(c_aux);
            if (c_aux->getChannel() > c->getChannel()) {
                enc = true;
            } else if ((c_aux->getChannel() == c->getChannel()) && (c_aux->getTau() > c->getTau())) {
                enc = true;
            } else {
                l_aux->next();
            }
        }
        l_aux->insert(c);
        
    }
    l_aux->moveToBegin();
    while (! l_aux->empty()) {
        l_aux->consult(c);
        this->cost->moveToEnd();
        this->cost->next();
        this->cost->insert(c);
        l_aux->remove();
    }
    
    delete l_aux;
}


void TauLopCost::show () {
    
    Transmission *c = nullptr;
    
    this->cost->moveToBegin();
    while (! this->cost->end()) {
        this->cost->consult(c);
        cout << c->getN() << "x" << c->getTau() << "||T^" << c->getChannel() << "(" << c->getM() << ")";
        this->cost->next();
        if (! this->cost->end()) cout << " + ";
    }
    cout << endl;
}
