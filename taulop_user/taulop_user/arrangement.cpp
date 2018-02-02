//
//  arrangement.cpp
//  TauLopCost
//
//  Created by jarico on 18/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "arrangement.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <algorithm>
using namespace std;



// UTILITY FUNCTIONS

static int MIN (int a, int b) {
    if (a < b) return a;
    return b;
}


// PRIVATE methods

int Arrangement::calculateProcNr () {
    
    int nprocs = 0;
    
    for (int col = 0; col < this->col_nr; col++) {
        for (int row = 0; row < this->getRowNr(col); row++) {
            nprocs++;
        }
    }
    return nprocs;
}


int Arrangement::calculateNodeNr () {
    
    int max_nodes = 0;
    
    for (int col = 0; col < this->col_nr; col++) {
        for (int row = 0; row < this->getRowNr(col); row++) {
            max_nodes++;
        }
    }
    
    int *nodes = new int [max_nodes];
    int num_nodes = 0;
    
    for (int col = 0; col < this->col_nr; col++) {
        for (int row = 0; row < this->getRowNr(col); row++) {
            Rectangle *p = this->get(row, col);
            if (nodes[p->getNode()] == 0) {
                num_nodes++;
            }
            nodes[p->getNode()] = 1;
        }
    }
    
    delete [] nodes;
    return num_nodes;
}




// PUBLIC interface


Arrangement::Arrangement () {

    this->v_cols = nullptr;
    
    this->col_nr = 0;
    this->P      = 0;
    this->M      = 0;
    this->H      = 0;
    this->W      = 0;
    this->B      = 0;
}


Arrangement::Arrangement (int col_nr, int row_nr[]) {
    
    this->v_cols = new Column * [col_nr];
    
    for (int col = 0; col < col_nr; col++) {
        this->v_cols[col] = new Column (row_nr[col]);
    }
    
    this->col_nr = col_nr;    
}


Arrangement::Arrangement (const Arrangement *r) {
    
    this->col_nr = r->col_nr;
    this->v_cols = new Column * [r->col_nr];
    
    for (int col = 0; col < r->col_nr; col++) {
        
        int row_nr = r->getRowNr(col);
        this->v_cols[col] = nullptr;
        this->v_cols[col] = new Column (row_nr);
        
        for (int row = 0; row < r->getRowNr(col); row++) {
            
            Rectangle *p = r->get(row, col);
            Rectangle *p_new = new Rectangle(p);
            
            this->insert(p_new, row, col);
        }
    }

    this->P = r->P;
    this->M = r->M;
    //this->N = r->N;
    this->H = r->H;
    this->W = r->W;
    this->B = r->B;
}


Arrangement::~Arrangement () {
    
    for (int i = 0; i < this->col_nr; i++) {
        Column *c = this->v_cols[i];
        delete c;
    }
    delete this->v_cols;
}


void Arrangement::insert (Rectangle *p, int row, int col) {
    this->v_cols[col]->insert(p, row);
    this->v_cols[col]->setWidth(p->getWidth());
}


void Arrangement::setProcNr(int P) {
    this->P = P;
}


void Arrangement::setNodeNr(int M) {
    this->M = M;
}


void Arrangement::setWidth(int W) {
    this->W = W;
    //this->N = W; // TEMP
}

void Arrangement::setHeight(int H) {
    this->H = H;
}


void Arrangement::setBlockSize(int B) {
    this->B = B;
}


int Arrangement::getWidth () {
    return this->W;
}

int Arrangement::getHeight () {
    return this->H;
}


/*int Arrangement::getN () {  // TEMP
    return this->W;
}*/


int Arrangement::getB () {
    return this->B;
}


int Arrangement::getP () {
    return this->P;
}

int Arrangement::getNodeNr () {
    return this->M;
}

int Arrangement::getNodeNr (int col) const {
    Column *c = this->v_cols[col];
    return c->getNodeNr();
}

Rectangle * Arrangement::get (int row, int col) const {
    return this->v_cols[col]->get(row);
}


int Arrangement::getRowNr(int col) const {
    return this->v_cols[col]->getRowNr();
}


int Arrangement::getColNr() const {
    return this->col_nr;
}


int Arrangement::getWidth (int col) {
    return this->v_cols[col]->getWidth();
}


/*
int Arrangement::getSize () {
    
    static int N = 0;
    
    if (N == 0) {
        for (int i = 0; i < col_nr; i++) {
            N += this->v_cols[i]->getWidth();
        }
    }
    
    return N;
}
*/


// Set (and Test) the arrangement block assignation (coords)
void Arrangement::establish () {
    
    Rectangle *p;
    int row_block = 0;
    for (int col = 0; col < this->getColNr(); col++) {
        
        int col_block = 0;
        for (int row = 0; row < this->getRowNr(col); row++) {
            
            p = this->get(row, col);
            p->setCoords(row_block, col_block);
            col_block = col_block + p->getHeight();
        }
        row_block = row_block + this->getWidth(col);
        //row_block = row_block + p->getWidth();
    }
    
}


void Arrangement::changeCols (int c_orig, int c_dest) {
    
    if ((c_orig >= this->col_nr) || (c_dest >= this->col_nr)) {
        cout << "ERROR: column number out of range" << endl;
        return;
    }
    
    // Change position
    for (int row = 0; row < this->getRowNr(c_orig); row++) {
        Rectangle *p = this->get(row, c_orig);
        p->setPos(row, c_dest);
    }
    
    for (int row = 0; row < this->getRowNr(c_dest); row++) {
        Rectangle *p = this->get(row, c_dest);
        p->setPos(row, c_orig);
    }
    
    // Change columns
    Column *c_aux;
    
    c_aux = this->v_cols[c_orig];
    this->v_cols[c_orig] = this->v_cols[c_dest];
    this->v_cols[c_dest] = c_aux;
    
    // Put new coords
    this->establish();
}


void Arrangement::changeCols (int *c_vec) {
    
    int col_nr = this->getColNr();
    
    for (int col = 0; col < col_nr; col++) {
        
        // Change position
        for (int row = 0; row < this->getRowNr(col); row++) {
            Rectangle *p = this->get(row, col);
            p->setPos(row, c_vec[col]);
        }
    }
    
    // Change columns
    Column *c_aux[col_nr];
    for (int col = 0; col < col_nr; col++) {
        //c_aux[col] = this->v_cols[c_vec[col]]; <-- MAL
        c_aux[c_vec[col]] = this->v_cols[col]; 
    }
    for (int col = 0; col < col_nr; col++) {
        this->v_cols[col] = c_aux[col];
    }
    
    // Put new coords
    this->establish();
}



void Arrangement::changeRows (int col, int r_orig, int r_dest) {
    
    int row_nr = this->getRowNr(col);
    
    if ((r_orig >= row_nr) || (r_dest >= row_nr)) {
        cout << "ERROR: Rows number out of range" << endl;
        return;
    }
    
    // Change position
    Rectangle *p_orig = this->get(r_orig, col);
    Rectangle *p_dest = this->get(r_dest, col);
    p_orig->setPos(r_dest, col);
    p_dest->setPos(r_orig, col);
    
    // Change row
    this->v_cols[col]->change(r_orig, r_dest);
    
    // Put new coords
    this->establish();
}


// TBD: Creating a new column and copy the rectangles in order
//      could be more efficient. 
void Arrangement::changeRows (int col, int *rows) {

    int row_nr = this->getRowNr(col);
    
    int row = 0;
    
    for (int i = 0; i < row_nr; i++) {
        
        for (int k = row; k < row_nr; k++) {
            
            Rectangle *p = this->get(k, col);
            if (p->getRank() == rows[i]) {
                
                if (row != k) {
                    Rectangle *p_aux = this->get(row, col);
                    
                    this->insert(p, row, col);
                    this->insert(p_aux, k, col);
                    
                    p->setPos(row, col);
                    p_aux->setPos(k, col);
                }
                
                row++;
            }
        }
    }
    
    // Put new coords
    this->establish();
}



Arrangement * Arrangement::createSubArrangement (int ncols, int *cols) {
    
    Arrangement *r = new Arrangement();
    
    r->col_nr = ncols;
    r->v_cols = new Column * [ncols];
    
    for (int col = 0; col < ncols; col++) {
        int prevcol = cols[col];
        Column *c = new Column (this->v_cols[prevcol]);
        r->v_cols[col] = c;
        
        r->P += c->getRowNr();
        r->W += c->getWidth();
    }
    
    // Node number
    int nnodes[r->P];
    for (int i = 0; i < r->P; i++) nnodes[i] = -1;
    int j = 0;
    
    for (int nc = 0; nc < r->col_nr; nc++) {
        for (int nr = 0; nr < r->getRowNr(nc); nr++) {
            
            bool found = false;
            Rectangle *p = r->get(nr, nc);
            int node = p->getNode();
            
            // cout << p->getRank() << " on node: " << p->getNode() << endl;
            
            int i;
            for (i = 0; nnodes[i] != -1; i++) {
                if (node == nnodes[i]) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                nnodes[i] = node;
                j = i + 1;
            }
            
            //for (int k = 0; k < r->P; k++) {
            //    cout << nnodes[k] << " ";
            //}
            //cout << " -> " << j << endl;
        }
    }
    r->M = j;
    
    
    // Height and B do not change
    r->H = this->H;
    r->B = this->B;

    r->establish();
    
    return r;
}



void Arrangement::addColumn (Column *c) {

    Column *new_c = new Column(c);
    
    Column **aux = new Column * [this->col_nr + 1];
    // Copy an array
    for (int col = 0; col < this->col_nr; col++) {
        aux[col] = this->v_cols[col];
    }
    
    aux[this->col_nr] = new_c;
    
    if (this->v_cols)
        delete [] this->v_cols;
    
    this->v_cols = aux;
    this->col_nr++;

    this->P += new_c->getRowNr();
    this->W += new_c->getWidth();
    
    // Height does not change
    // B does not change
    
    // Node number
    int nnodes[this->P];
    for (int i = 0; i < this->P; i++) nnodes[i] = -1;
    int j = 0;
    
    for (int nc = 0; nc < this->col_nr; nc++) {
        for (int nr = 0; nr < this->getRowNr(nc); nr++) {
            
            bool found = false;
            Rectangle *p = this->get(nr, nc);
            int node = p->getNode();
            
            // cout << p->getRank() << " on node: " << p->getNode() << endl;
            
            int i;
            for (i = 0; nnodes[i] != -1; i++) {
                if (node == nnodes[i]) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                nnodes[i] = node;
                j = i + 1;
            }
            
            //for (int k = 0; k < r->P; k++) {
            //    cout << nnodes[k] << " ";
            //}
            //cout << " -> " << j << endl;
        }
    }
    this->M = j;
    
    // New coords
    this->establish();
}


//   TBD:  NOT TESTED
void Arrangement::delColumn (int col_nr) {
    
    if (this->col_nr == 0)
        return;
    
    Column **aux = new Column * [this->col_nr - 1];
    int aux_col = 0;
    for (int col = 0; col < this->col_nr; col++) {
        if (col != col_nr) {
            aux[aux_col] = this->v_cols[col];
            aux_col++;
        }
    }
    Column *c = this->v_cols[col_nr];
    delete [] this->v_cols;
    this->v_cols = aux;
    this->col_nr--;
    
    this->P -= this->v_cols[col_nr]->getRowNr();
    this->W -= this->v_cols[col_nr]->getWidth();

    // Node number
    int nnodes[this->P];
    for (int i = 0; i < this->P; i++) nnodes[i] = -1;
    int j = 0;
    
    for (int nc = 0; nc < this->col_nr; nc++) {
        for (int nr = 0; nr < this->getRowNr(nc); nr++) {
            
            bool found = false;
            Rectangle *p = this->get(nr, nc);
            int node = p->getNode();
            
            // cout << p->getRank() << " on node: " << p->getNode() << endl;
            
            int i;
            for (i = 0; nnodes[i] != -1; i++) {
                if (node == nnodes[i]) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                nnodes[i] = node;
                j = i + 1;
            }
            
            //for (int k = 0; k < r->P; k++) {
            //    cout << nnodes[k] << " ";
            //}
            //cout << " -> " << j << endl;
        }
    }
    this->M = j;

    
    // Height does not change
    // B does not change

    delete c;
    // New coords
    this->establish();
}



Column * Arrangement::getColumn (int col_nr) {
    
    if (col_nr >= this->col_nr) {
        cerr << "ERROR [getColumn]: column does not exist " << col_nr << endl;
        return nullptr;
    }
    
    Column *c = this->v_cols[col_nr];
    return c;
}



void Arrangement::groupByNode () {
    
    for (int col = 0; col < this->getColNr(); col++) {
        
        bool exch = true;
        while (exch) {
            
            exch = false;
            for (int row = 0; row < this->getRowNr(col) - 1; row++) {
                
                Rectangle *p_orig = this->get(row, col);
                Rectangle *p_dest = this->get(row + 1, col);
                
                if (p_orig->getNode() > p_dest->getNode()) {
                    
                    p_orig->setPos(row + 1, col);
                    p_dest->setPos(row,     col);
                    
                    this->insert(p_dest, row,     col);
                    this->insert(p_orig, row + 1, col);
                    
                    //cout << "Change : " << p_orig->getNode() << " by " << p_dest->getNode() << endl;
                    exch = true;
                    
                }
            }
        }
    }
    
    this->establish();
}


void Arrangement::groupByNode (int col) {
    
    if (col >= this->col_nr) {
        cerr << "ERROR: [groupByNode(col)] column number incorrect: " << col << endl;
        return;
    }
    
    bool exch = true;
    while (exch) {
        
        exch = false;
        for (int row = 0; row < this->getRowNr(col) - 1; row++) {
            
            Rectangle *p_orig = this->get(row, col);
            Rectangle *p_dest = this->get(row + 1, col);
            
            if (p_orig->getNode() > p_dest->getNode()) {
                
                p_orig->setPos(row + 1, col);
                p_dest->setPos(row,     col);
                
                this->insert(p_dest, row,     col);
                this->insert(p_orig, row + 1, col);
                
                //cout << "Change : " << p_orig->getNode() << " by " << p_dest->getNode() << endl;
                exch = true;
                
            }
        }
    }
    
    this->establish();
}



Rectangle * Arrangement::getProcByRank (int rank) {
    
    for (int col = 0; col < this->getColNr(); col++) {
        
        for (int row = 0; row < this->getRowNr(col); row++) {
            
            Rectangle *p_orig = this->get(row, col);
            if (p_orig->getRank() == rank) return p_orig;
            
        }
    }
    return nullptr;
}


void Arrangement::getNextVInterval (int b_ini, int &b_size) {
    
    int curr_b_size = this->H;
    int curr_b_ini;
    
    if (b_ini >= this->H) {
        b_size = 0;
        b_ini  = this->H;
        return;
    }
    
    for (int col = 0; col < this->col_nr; col++) {
        
        curr_b_ini = 0;
        
        for (int row = 0; row < getRowNr(col); row++) {
            
            Rectangle *p = get(row, col);
            
            curr_b_ini = curr_b_ini + p->getHeight();
            if (curr_b_ini > b_ini) {
                curr_b_size = MIN (curr_b_size, curr_b_ini - b_ini);
                break;
            }
        }
        
    }
    
    b_size = curr_b_size;
}


void Arrangement::getNextHInterval (int b_ini, int &b_size) {
    
    int curr_b_size = this->W;
    int curr_b_ini;
    
    if (b_ini >= this->W) {
        b_size = 0;
        b_ini  = this->W;
        return;
    }
    
    curr_b_ini = 0;
    
    for (int col = 0; col < this->col_nr; col++) {
        
        Rectangle *p = get(0, col);
        
        curr_b_ini = curr_b_ini + p->getWidth();
        if (curr_b_ini > b_ini) {
            curr_b_size = MIN (curr_b_size, curr_b_ini - b_ini);
            break;
        }
    }
    
    b_size = curr_b_size;
}


void Arrangement::show () {
    
    bool data = true;
    int  curr_row = 0;
    
    cout << "ARRANGEMENT. Col_nr: " << this->col_nr << " Proc_nr: " << this->P << " Nodes: " << this->M << " Blocks(WxH): " << this->W << " x " << this->H << endl;
    while (data) {
        
        data = false;
        
        for (int col_nr = 0; col_nr < this->col_nr; col_nr++) {
            
            int row_nr = this->getRowNr(col_nr);
            if (row_nr > curr_row) {
                Rectangle *p = this->get(curr_row, col_nr);
                cout << p->getRank() << " \t";
                data = true;
            }
            else
                cout << " \t";
            
        }
        curr_row++;
        cout << endl;
    }

#if TLOP_DEBUG == 1
    for (int col = 0; col < this->getColNr(); col++) {
        
        for (int row = 0; row < this->getRowNr(col); row++) {
            Rectangle *p = this->get(row, col);
            cout << p->getRank() << ": \t w: " << p->getWidth() << " \t h: " << p->getHeight()
                 << " \t s: " << p->getHeight() * p->getWidth() << endl;
        }
    }
#endif

}

ostream& Arrangement::operator<< (ostream& outs) {
    
    for (int i = 0; i < this->col_nr; i++) {
        outs << this->v_cols[i] << " " << endl;
    }
    return outs;
}


static string itos(int numero) {
    stringstream flujo;
    string cadena;
    flujo << numero;
    flujo >> cadena;
    return cadena;
}

static string ftos(float numero) {
    stringstream flujo;
    string cadena;
    flujo << numero;
    flujo >> cadena;
    return cadena;
}

void Arrangement::plot (string file) {
    
    int n_width  = this->W;
    int n_height = this->H;
    
    ofstream ofs(file, ios::trunc);
    
    string nfile = file;
    size_t found = nfile.find_last_of(".");
    nfile.replace(found, 1, "_");
    
    
    string line = "# gnuplot Partition file {config_name}. P = " + itos(this->P) + ".\n set terminal postscript eps enhanced color font \"Helvetica\" 24\n set output '" + nfile + ".eps'\n";
    
    ofs << line;
    ofs << "set title \"Partition (WxH=" + itos(this->W) + "x" + itos(this->H) + ", B=" + itos(this->B) + ")\"\n";
    ofs << "unset key\n";
    ofs << "set xrange[0:" + itos(n_width)  + "]\n";
    ofs << "set yrange[0:" + itos(n_height) + "]\n";
    if (n_width == n_height)
        ofs << "set size square\n";
    else // Suppose width < height
        ofs << "set size " << (float)n_width/(float)n_height << ", " << float(n_height)/float(n_height) << "\n";
    ofs << "unset xtics\n";
    ofs << "unset ytics\n";
    ofs << "set style line  1 lt -1 lw 1\n";
    
    //# How many nodes
    float gray_scale = 0.8 / this->M;
    
    // # write rectangles
    int p_nr = 0;
    
    for (int col = 0; col < this->col_nr; col++) {
        
        for (int row = 0; row < this->getRowNr(col); row++) {
            
            Rectangle *p = this->get(row, col);
            
            int X1 = p->getCoordX();
            
            //int Y1 = n_width - p->getCoordY();
            int Y1 = n_height - p->getCoordY();
            
            int X2 = p->getCoordX() + p->getWidth();
            
            //int Y2 = n_width - (p->getCoordY() + p->getHeight());
            int Y2 = n_height - (p->getCoordY() + p->getHeight());
            
            float fill_type = gray_scale * p->getNode(); // atoi(p->getNode().c_str());
            //fill_type = 0;
            
            string lab_rank = "\nset label \""+ itos(p->getRank()) + "\" at " + itos(X1 + 5) + "," + itos(Y1 - 10) + " tc rgb \"red\" font \"Helvetica,16\" front\n";
            
            string lab_node = "\nset label \" /" + itos(p->getNode()) + "\" at " + itos(X1 + 15) + "," + itos(Y1 - 10) + " tc rgb \"blue\" font \"Helvetica,16\" front\n";
            
            string rect = "\nset object " + itos(p_nr + 1) + " rect from " + itos(X1) + "," + itos(Y1) + " to " + itos(X2) + "," + itos(Y2) + " back fc lt -1 fs solid " + ftos(fill_type) + " behind";
            
            ofs << rect;
            ofs << lab_rank;
            ofs << lab_node;
            p_nr++;
            
        }
        
    }
    
    ofs << "\n\n plot NaN\n";
    
    ofs.close();
}


void Arrangement::part2D (string file) {
    
    // Supposing square matrix (required by FuPerMod)
    
    ofstream ofs(file, ios::trunc);
    
    ofs << "#size " + itos(this->P) + "\n";
    ofs << "#N "    + itos(this->W) + "\n";
    ofs << "#kb "   + itos(this->B) + "\n";
    ofs << "#host            rank_intra	i	j	x	y	w	h	w*h	mem	time \n";
    
    
    for (int p = 0; p < this->P; p++) {
        
        Rectangle *r = this->getProcByRank(p);
        
        string node      = r->getName();
        string rankintra = itos(r->getRankIntra());
        string i         = itos(r->getRow());
        string j         = itos(r->getCol());
        string x         = itos(r->getCoordX());
        string y         = itos(r->getCoordY());
        string w         = itos(r->getWidth());
        string h         = itos(r->getHeight());
        string wxh       = itos(r->getWidth() * r->getHeight());
        string mem       = "0";
        string time      = "0";
        
        
        string line = node + "\t" + rankintra + "\t" + i + "\t" + j + "\t" +  x + "\t" + y + "\t" + w + "\t" + h + "\t" + wxh + "\t" + mem + "\t" + time + "\n";
        
        ofs << line;
    }
    
    ofs.close();
}

