//
//  matrix.cpp
//  TauLopCost
//
//  Created by jarico on 18/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "matrix.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;




// PRIVATE METHODS
void Matrix::setInitialArrangement () {
    
    //Arrangement *r = new Arrangement (this->col_nr);
    
    int v_row_nr[this->col_nr];
    for (int col = 0; col < this->col_nr; col++) {
        v_row_nr[col] = 0;
    }
    
    for (int p = 0; p < this->P; p++) {
        int col = this->v_proc[p]->getCol();
        v_row_nr[col] = v_row_nr[col] + 1;
    }

    Arrangement *r = new Arrangement (this->col_nr, v_row_nr);
    
    for (int i = 0; i < this->P; i++) {
        Rectangle *p = this->v_proc[i];
        int row = p->getRow();
        int col = p->getCol();
        
        //cout << "Rectangle: " << p->getRank() << "(" << p->getRankIntra() << ")" << " in " << row << "/" << col << endl;
        r->insert(p, row, col);
    }
    r->setProcNr(this->P);
    r->setNodeNr(this->M);
    r->setWidth  (this->W);
    r->setHeight (this->H);
    r->setBlockSize(this->B);
    
    // First position of the list
    l_arrang->moveToBegin();
    l_arrang->insert(r);
    
    this->arrang_nr = 1;
    
}

Arrangement * Matrix::getInitialArrangement () {
    
    Arrangement *r;
    
    l_arrang->moveToBegin();
    l_arrang->consult(r);
    
    return r;
}


static string itos(int numero) {
    stringstream flujo;
    string cadena;
    flujo << numero;
    flujo >> cadena;
    return cadena;
}

void Matrix::generateSynthetic () {
    
    int  rest;
    int  total;
    int  p = 0; // Rank of the processes (rectangle)
    int  r_intra[synt_M]; // rank intra
    
    IList<string> *entries = new IList<string> ();
    
    //srand(time(NULL));
    srand(12);
    
    for (int i = 0; i < synt_M; i++)  r_intra[i] = 0;
    
    int coord_x = 0;
    int col = 0;
    
    while (coord_x < synt_N) {
        
        rest = synt_N;
        total = 0;
        
        bool complete = false;
        int row = 0;
        
        int w = 0;
        while (w < (synt_N / (synt_colNr * synt_ratio))) { // Not too small
            w = rand() % ((synt_N * synt_ratio) / synt_colNr);
        }
        
        if ((coord_x + w) >= synt_N) {
            w = synt_N - coord_x;
        }
        
        while (!complete) {
            
            int from  = rest / (synt_rowNr * synt_ratio); // Half
            int to    = (rest * synt_ratio) / synt_rowNr; // Double
            int range = to - from;
            
            int h = (rand() % range) + from;
            
            if ((total + h) > synt_N) {
                h = synt_N - total;
                complete = true;
            }
            
            total = total + h;
            
            int m = rand() % synt_M;
            
            //cout << "Col: " << col << " Rectangle: " << p << " Node: " << m << " height: " << h << " (total: " << total << ") " << endl;
            
            string s;
            s.append(itos(m));
            s.append("\t");
            s.append(itos(r_intra[m]));
            s.append("\t");
            s.append(itos(row));
            s.append("\t");
            s.append(itos(col));
            s.append("\t");
            s.append(itos(coord_x));
            s.append("\t");
            s.append(itos(total - h));
            s.append("\t");
            s.append(itos(w));
            s.append("\t");
            s.append(itos(h));
            s.append("\t");
            s.append(itos(h * w));
            s.append("\t");
            s.append("MEM-xx");
            s.append("\t");
            s.append("TIME-xx");
            s.append("\n");
            
            //cout << s << endl;
            
            entries->next();
            entries->insert(s);
            
            r_intra[m] = r_intra[m] + 1;
            p++;
            row++;
        }
        
        coord_x = coord_x + w;
        col++;
    }
    
    
    ofstream ofs(synt_file, ios::trunc);
    if (!ofs.is_open()) return;
    ofs << "#size " << p << "\n";
    ofs << "#N " << synt_N << "\n";
    ofs << "#kb " << synt_KB << "\n";
    ofs << "#host            rank_intra	i	j	x	y	w	h	w*h	mem	time" << "\n";
    
    string s;
    
    entries->moveToBegin();
    while (! entries->end()) {
        entries->consult(s);
        ofs << s;
        entries->next();
    }
    
    ofs.close();
    
    delete entries;
}




// PUBLIC INTERFACE

Matrix::Matrix  () {
    
    //this->N = 0;
    this->W = 0;
    this->H = 0;
    this->n = 0;
    this->P = 0;
    this->B = 0;
    this->M = 0;
    
    this->l_arrang = new IList<Arrangement *> ();
    
    this->nodes = new IList<string> ();
    
    this->arrang_nr = 0;
}




Matrix::~Matrix () {
    
    Arrangement *r;
    
    l_arrang->moveToBegin();
    while (! l_arrang->end()) {
        l_arrang->consult(r);
        l_arrang->remove();
        delete r;
    }
    
    delete l_arrang;
    
    
    nodes->moveToBegin();
    while (! nodes->end()) {
        nodes->remove();
    }
    
    delete nodes;
  
}


int Matrix::find_node_nr (string name) {
    
    string s;
    int    idx = 0;
    bool   found = false;
    
    nodes->moveToBegin();
    while ((! nodes->end()) && (! found)) {
        nodes->consult(s);
        if (name == s) {
            found = true;
        } else {
            idx++;
        }
        nodes->next();
    }
    
    if (!found) {
        nodes->insert(name);
    }
    // DBG: cout << name << " is the node " << idx << endl;
    return idx;
}

void Matrix::load (string file) {
    
    ifstream ifs;
    string   line;
    unsigned int pos = 0;
    string   name;
    int      rank_global = 0;
    int      node_nr;
    
    if (file == "") {
        file = synt_file;
        generateSynthetic();
    }
    
    
    // 1. Read data from the file.
    ifs.open(file.c_str());
    
    if (ifs.is_open()) {
        
        // Size
        getline(ifs, line);
        if (! ifs.eof()) {
            pos  = (unsigned int)line.find_first_of(" ");
            line = line.substr (pos);
            //cout << line << endl;
            this->P = atoi(line.c_str());
        }
        
        // N
        getline(ifs, line);
        if (! ifs.eof()) {
            pos  = (unsigned int)line.find_first_of(" ");
            line = line.substr (pos);
            //cout << "N: " << line << endl;
            // Square matrix, so H==W (and ==N)
            this->W = atoi(line.c_str());
            this->H = atoi(line.c_str());
        }
        
        // b
        getline(ifs, line);
        if (! ifs.eof()) {
            pos  = (unsigned int)line.find_first_of(" ");
            line = line.substr (pos);
            //cout << line << endl;
            this->B = atoi(line.c_str());
        }
        
        // Header line. Discard it.
        getline(ifs, line);
        if (! ifs.eof()) {
            //cout << line << endl;
        }
        
        
        while (!ifs.eof()) {
            
            string delimiter = "\t";
            string token;
            
            getline(ifs, line);
            if (ifs.eof()) break;
            
            int i = 0;
            int j = 0;
            int x = 0;
            int y = 0;
            int w = 0;
            int h = 0;
            int r = -1;
            
            Rectangle *p = new Rectangle();
            
            int n = 0;
            while (n < 11) { // 11 tokens in a line.
                pos = (unsigned int)line.find(delimiter);
                token = line.substr(0, pos);
                
                switch(n) {
                    case 0: name = token.substr(0, token.find(" "));
                        node_nr = find_node_nr(name);
                        //node_nr = atoi(name.c_str());
                        p->setNode(node_nr);
                        break;
                    case 1: //p->setName(itos(rank_global) + "-" + name);
                        p->setName(name);
                        r = atoi(token.c_str());
                        p->setRankIntra(r);
                        p->setRank(rank_global);
                        break;
                    case 2: i = atoi(token.c_str());  break;
                    case 3: j = atoi(token.c_str());
                        p->setPos(i, j);
                        if (j >= this->col_nr)
                            this->col_nr = j + 1;
                        break;
                    case 4: x = atoi(token.c_str());  break;
                    case 5: y = atoi(token.c_str());
                        p->setCoords(x, y);
                        break;
                    case 6: w = atoi(token.c_str());  break;
                    case 7: h = atoi(token.c_str());
                        p->setSize(w, h);
                        break;
                        
                    default:  break;
                        
                }
                
                line.erase(0, pos + delimiter.length());
                n++;
            }
            
            bool found = false;
            for (int i = 0; i < rank_global; i++) {
                if (this->v_proc[i]->getNode() == p->getNode()) {
                    found = true;
                }
            }
            if (!found) this->M++;
            
            this->v_proc[rank_global++] = p;
            
        }
    
        // 2. Create the initial arrangement from data read from the file.
        this->setInitialArrangement();
        
    } else {
        cout << "ERROR: file not found: " << name << endl;
    }
    
}


int Matrix::addArrangement (int P, int W, int H, int B, int col_nr, Rectangle *p[]) {
    
    int  rank_global = 0;

    this->P = P;
    //this->N = N;
    this->W = W;
    this->H = H;
    this->B = B;
    this->col_nr = col_nr;
    
    for (int p_nr = 0; p_nr < P; p_nr++) {
        
        bool found = false;
        for (int i = 0; i < rank_global; i++) {
            if (this->v_proc[i]->getNode() == p[p_nr]->getNode()) {
                found = true;
            }
        }
        if (!found) this->M++;
        
        this->v_proc[rank_global++] = p[p_nr];
    }
    
    // 2. Create a new arrangement. Return the index.
    int v_row_nr[this->col_nr];
    for (int col = 0; col < this->col_nr; col++) {
        v_row_nr[col] = 0;
    }
    
    for (int p_nr = 0; p_nr < this->P; p_nr++) {
        int col = p[p_nr]->getCol();//this->v_proc[p]->getCol();
        v_row_nr[col] = v_row_nr[col] + 1;
    }

    Arrangement *r = new Arrangement (this->col_nr, v_row_nr);
    
    
    for (int i = 0; i < this->P; i++) {
        Rectangle *p = this->v_proc[i];
        int row = p->getRow();
        int col = p->getCol();
        
        //cout << "Rectangle: " << p->getRank() << "(" << p->getRankIntra() << ")" << " in " << row << "/" << col << endl;
        r->insert(p, row, col);
    }
    r->setProcNr    (this->P);
    r->setNodeNr    (this->M);
    r->setWidth     (this->W);
    r->setHeight    (this->H);
    r->setBlockSize (this->B);
    
    return this->newArrangement(r);
}


int Matrix::newArrangement (Arrangement *r) {
    
    int idx = 0;
    
    this->l_arrang->moveToEnd();
    this->l_arrang->next();
    this->l_arrang->insert(r);
    
    idx = this->arrang_nr;
    this->arrang_nr++;
    
    return idx;
}


void Matrix::getArrangement (Arrangement *& r, int index) {
    
    Arrangement *r_aux = nullptr;
    
    l_arrang->moveToBegin();
    while ((!l_arrang->end()) && (index > 0)) {
        l_arrang->next();
        index --;
    }
    if (!l_arrang->end()) {
        l_arrang->consult(r_aux);
    }
    r = r_aux;
}



Arrangement* Matrix::getArrangement (int index) {
    
    Arrangement *r = nullptr;
    
    l_arrang->moveToBegin();
    while ((!l_arrang->end()) && (index > 0)) {
        l_arrang->next();
        index --;
    }
    if (!l_arrang->end()) {
        l_arrang->consult(r);
    }
    
    return r;
}


void Matrix::removeArrangement () {
    
    cout << "TBD: delete in removeArrangement" << endl;
}



void Matrix::toString() {
    
    cout << "Default matrix: " << endl;
    cout << "P : " << this->P << endl;
    cout << "WxH : " << this->W << "x" << this->H << endl;
    cout << "n : " << this->n << endl;
    cout << "b : " << this->B << endl;
    cout << "col_nr : " << this->col_nr << endl;
    
    Arrangement *r;
    
    l_arrang->moveToBegin();
    while (! l_arrang->end()) {
        l_arrang->consult(r);
        r->show();
        l_arrang->next();
    }
}
