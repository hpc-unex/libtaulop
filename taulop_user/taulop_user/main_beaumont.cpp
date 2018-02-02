//
//  main.cpp
//  TauLopCost
//
//  Created by jarico on 18/4/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "matrix.hpp"
#include "algorithm.hpp"
#include "arrangement.hpp"
#include "cost.hpp"

#include "algorithm_basic.hpp"
#include "algorithm_malik.hpp"
#include "algorithm_MxM.hpp"
#include "algorithm_half_perimeter.hpp"

#include "heuristic_malik.hpp"
#include "heuristic_combinatorial.hpp"
#include "heuristic_beaumont.hpp"
#include "heuristic_beaumont_mod.hpp"

#include "taulop_operator.hpp"
#include "taulop_cost.hpp"

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <fstream>
#include "ilist.h"
#include <string>
#include <sstream>
#include <limits>
#include <iomanip>
#include <cmath>
#include <unistd.h>
#include <fstream>

using namespace std;




const int N  = 256;
const int P  = 16;
const int M  = 8;
const int kb = 16;
const int V  = 100; // % of variability between processes speeds

const int min_M = 8;
const int min_P = 8;

#define  SEQ     0
#define  RR      1
#define  RANDOM  2
#define  WORST   3

#define PLACEMENT RR

#define  CPM  1

class Improvement {
    
private:
    
    double beaumont     [M+1] [P+1]; // Cost according to beaumont
    double modified     [M+1] [P+1]; // Cost according to Beaumont modified
    float  improvements [M+1] [P+1]; // % of improvement
    
    float  avg_m  [P+1]; // Average per number of process across nodes.
    float  avg_p  [M+1]; // Average per number of nodes across processes.
    
    
public:
    Improvement() {
        for (int m = 0; m <= M; m++) {
            for (int np = 0; np <= P; np++) {
                this->beaumont [m][np] = 0.0;
                this->modified [m][np] = 0.0;
                this->improvements [m][np] = 0.0;
            }
        }
    }

    void putB (int m, int p, double b) {
        this->beaumont[m][p] = b;
    }

    void putM (int m, int p, double d) {
        this->modified[m][p] = d;
    }

    float get (int m, int p) {
        float imp = 100 - ((this->modified[m][p] * 100) / this->beaumont[m][p]);
        return imp;
    }
    
    void statistics () {

        float avg = 0.0;
        float min = 0.0;
        float max = 0.0;
        
        
        // 1. Fill the improvements (%) matrix
        for (int m = 0; m <= M; m++) {
            for (int p = 0; p <= P; p++) {
                float imp = 100 - ((this->modified[m][p] * 100) / this->beaumont[m][p]);
                this->improvements [m][p] = imp;
            }
        }

        
        // Average of improvement per number of Rectangles
        for (int p = min_P; p <= P; p++) {
            avg = 0.0;
            for (int m = min_M; m <= M; m++) {
                avg += this->improvements[m][p];
            }
            avg_m[p] = avg / (M-1);
        }
        
        // Average of improvement per number of nodes
        for (int m = min_M; m <= M; m++) {
            avg = 0.0;
            for (int p = min_P; p <= P; p++) {
                avg += this->improvements[m][p];
            }
            avg_p[m] = avg / (P-1);
        }

    }
    
    
    void print () {
        cout << " P\t ";
        for (int m = min_M; m <= M; m++) {
            cout << "M=" << m << " \t ";
        }
        cout << endl;
        
        for (int np = min_P; np <= P; np++) {
            
            cout << np << " \t ";
            
            for (int m = min_M; m <= M; m++) {
                
                if (np < m) cout << "  - \t ";
                else {
                    float imp = 100 - ((this->modified[m][np] * 100) / this->beaumont[m][np]);
                    cout << fixed << setprecision(1) << imp << " \t ";
                }
                
            }
            cout << endl;
        }
        
        cout << endl << "----------  AVG  ----------" << endl;
        for (int p = min_P; p <= P; p++) {
            cout << fixed << setprecision(1) << avg_m[p] << " \t ";
        }
        cout << endl;
        for (int m = min_M; m <= M; m++) {
            cout << fixed << setprecision(1) << avg_p[m] << " \t ";
        }
        cout << endl;

    }

    void output (string fname) {
        
        ofstream ofs;
        ofs.open(fname.c_str());
        if (!ofs.is_open()) {
            cout << "ERROR: opening output file" << endl;
            return;
        }
        
        ofs << " P\t ";
        for (int m = min_M; m <= M; m++) {
            ofs << "M=" << m << " \t ";
        }
        ofs << endl;
        
        for (int np = min_P; np <= P; np++) {
            
            ofs << np << " \t ";
            
            for (int m = min_M; m <= M; m++) {
                
                if (np < m) cout << "  - \t ";
                else {
                    float imp = 100 - ((this->modified[m][np] * 100) / this->beaumont[m][np]);
                    ofs << fixed << setprecision(1) << imp << " \t ";
                }
                
            }
            ofs << endl;
        }
     
        ofs.close();
    }
    
    
};


// Return the index of the arrangement in the matrix.
int generate_arrangement (Matrix *mat, int m, int np) {
    
    
#if CPM == 1
    int cpm [np];  // CPM per process

    int eq  = N / np;
    int v   = eq * ((double)V / 100);
    v = V;
    int qt  = 100;
    int sum = 0;
    
    //cout << "MxP: " << m << " x " << np << endl;
    cout << "Absolute speeds: ";
    for (int i = 0; i < np; i++) {
        cpm[i] = qt;
        cout << cpm[i] << "  " << flush;
        sum += cpm[i];
        qt += v / (np - 1);
    }
    cout << endl;
    
    cout << "Relative speeds: ";
    for (int i = 0; i < np; i++) {
        int w = (N * cpm[i]) / sum;
        cout << w << "  " << flush;
    }
    cout << endl;
#else
    int sum = 0;
    
    cout << "Absolute speeds: ";
    int cpm[8] = {5, 5, 8, 10, 10, 12, 20, 30};
    for (int i = 0; i < 8; i++) {
        cout << cpm[i] << "  " << flush;
        sum += cpm[i];
    }
    cout << endl;
    
    cout << "Relative speeds: ";
    for (int i = 0; i < 8; i++) {
        int w = (N * cpm[i]) / sum;
        cout << w << "  " << flush;
    }
    cout << endl;
#endif
    
    
    int  t = 0;
    int  w = 0;
    
    
    int n_m [np];  // Node numbers of the processes
#if PLACEMENT == SEQ
    int q = ceil((double)np / (double)m);
    cout << "SEQ - Node number: ";
    for (int p = 0; p < np; p++) {
        //n_m[p] = rand() % m; // RANDOM
        n_m[p] = p / q;  // SEQ
        //n_m[p] = p % m; // RR
        cout << n_m[p] << "  " << flush;
    }
    cout << endl;
#elif PLACEMENT == RR
    cout << "RR - Node number: ";
    for (int p = 0; p < np; p++) {
        n_m[p] = p % m; // RR
        cout << n_m[p] << "  " << flush;
    }
    cout << endl;
#elif PLACEMENT == RANDOM
    cout << "RANDOM - Node number: ";
    for (int p = 0; p < np; p++) {
        n_m[p] = rand() % m; // RANDOM
        cout << n_m[p] << "  " << flush;
    }
    cout << endl;
#elif PLACEMENT == WORST
    int q = ceil((double)np / (double)m);
    int i = 0;
    int j = np - 1;
    for (int p = 0; p < np; p++) {
        if (p % 2) {
            n_m[i] = p / q;  // begin
            i++;
        } else {
            n_m[j] = p / q;  // end
            j--;
        }
    }
    cout << "WORST - Node number: ";
    for (int p = 0; p < np; p++) {
        cout << n_m[p] << "  " << flush;
    }
    cout << endl;
    
#else
#error "Choose a placement for the processes in the nodes !!"
#endif
    
    
    int intranode_r [np];  // Intra-node rank for processes
    for (int p = 0; p < np; p++) {
        intranode_r[p] = 0;
    }
    cout << "Intra-node rank: ";
    for (int p = 0; p < np; p++) {
        
        for (int r = 0; r < p; r++) {
            if (n_m[p] == n_m[r])
                intranode_r[p] = intranode_r[p] + 1;
        }
        cout << intranode_r[p] << "  " << flush;
    }
    cout << endl;
    
    
    Rectangle *v_proc[np];
    int col_nr = 0;
    
    for (int p = 0; p < np; p++) {
        
        int   i = 0;
        int   j = p;
        int   x;
        int   y = 0;
        int   h = N;
        
        // Column block coord X
        if (p == 0) {           // First rectangle
            x = 0;
        } else {                // Intermediate rectangles
            x = t;
        }
        
        x = t;
        
        // Width
        w = (N * cpm[p]) / sum;
        if (p == np-1) {
            w = N - t;
        }
        
        
        Rectangle *proc = new Rectangle();
        
        stringstream flujo;
        string name;
        flujo << p;
        flujo >> name;
        
        proc->setNode      (n_m[p]);
        //proc->setName      ("R_" + name);
        proc->setName      (name);
        proc->setRankIntra (intranode_r[p]);
        proc->setRank      (p);
        proc->setPos       (i, j);
        if (j >= col_nr)
            col_nr = j + 1;
        proc->setCoords    (x, y);
        proc->setSize      (w, h);

        v_proc[p] = proc;
        
        t += w;
    }
        
    return mat->addArrangement(np, N, N, kb, col_nr, v_proc);
}




void generate_file (int m, int np) {
    
    int cpm [np];  // CPM per process
    
    int eq  = N / np;
    int v   = eq * ((double)V / 100);
    v = V;
    int qt  = 100;
    int sum = 0;
    
    //cout << "MxP: " << m << " x " << np << endl;
    cout << "Absolute speeds: ";
    for (int i = 0; i < np; i++) {
        cpm[i] = qt;
        cout << cpm[i] << "  " << flush;
        sum += cpm[i];
        qt += v / (np - 1);
    }
    cout << endl;

    cout << "Relative speeds: ";
    for (int i = 0; i < np; i++) {
        int w = (N * cpm[i]) / sum;
        cout << w << "  " << flush;
    }
    cout << endl;
    
    
    int  t = 0;
    int  w = 0;
    
    
    int n_m [np];  // Node numbers of the processes
#if PLACEMENT == SEQ
    int q = ceil((double)np / (double)m);
    cout << "SEQ - Node number: ";
    for (int p = 0; p < np; p++) {
        //n_m[p] = rand() % m; // RANDOM
        //n_m[p] = p / q;  // SEQ
        n_m[p] = p % m; // RR
        cout << n_m[p] << "  " << flush;
    }
    cout << endl;
#elif PLACEMENT == RR
    cout << "RR - Node number: ";
    for (int p = 0; p < np; p++) {
        n_m[p] = p % m; // RR
        cout << n_m[p] << "  " << flush;
    }
    cout << endl;
#elif PLACEMENT == RANDOM
    cout << "RANDOM - Node number: ";
    for (int p = 0; p < np; p++) {
        n_m[p] = rand() % m; // RANDOM
        cout << n_m[p] << "  " << flush;
    }
    cout << endl;
#elif PLACEMENT == WORST
    int q = ceil((double)np / (double)m);
    int i = 0;
    int j = np - 1;
    for (int p = 0; p < np; p++) {
        if (p % 2) {
            n_m[i] = p / q;  // begin
            i++;
        } else {
            n_m[j] = p / q;  // end
            j--;
        }
    }
    cout << "WORST - Node number: ";
    for (int p = 0; p < np; p++) {
        cout << n_m[p] << "  " << flush;
    }
    cout << endl;

#else
#error "Choose a placement for the processes in the nodes !!"
#endif
    
    
    int intranode_r [np];  // Intra-node rank for processes
    for (int p = 0; p < np; p++) {
        intranode_r[p] = 0;
    }
    cout << "Intra-node rank: ";
    for (int p = 0; p < np; p++) {
        
        for (int r = 0; r < p; r++) {
            if (n_m[p] == n_m[r])
                intranode_r[p] = intranode_r[p] + 1;
        }
        cout << intranode_r[p] << "  " << flush;
    }
    cout << endl;
        
        
        
    
    
    ofstream ofs;
    ofs.open("/Users/jarico/tmp/ex_part.2dist");
    
    ofs << "#size " << np << endl;
    ofs << "#N "    << N << endl;
    ofs << "#kb "   << kb << endl;
    ofs << "#host  rank_i	i	j	x	y	w	h	w*h	mem	time" << endl;
    
    
    for (int p = 0; p < np; p++) {
        
        int   i = 0;
        int   j = p;
        int   x;
        int   y = 0;
        int   h = N;
        int mem = 0;
        int  tm = 0;
        
        
        // Column block coord X
        if (p == 0) {           // First process
            x = 0;
        } else {                // Intermediate processes
            x = t;
        }
        
        x = t;
        
        // Width
        w = (N * cpm[p]) / sum;
        if (p == np-1) {
            w = N - t;
        }
        
        
        ofs << n_m[p] << "\t" << intranode_r[p] << "\t" << i << "\t" << j << "\t"
            << x << "\t" << y << "\t" << w << "\t" << h << "\t" << w * h
            << "\t" << mem << "\t" << tm << endl;
        
        t += w;
    }
    
    ofs.close();
}

// Return the cost of communications using the Beaumont algorithm in time (t-Lop cost)
double beaumont_cost (Matrix *m, int idx) {
    
    
    // 1. Original arrangement and creation of algorithms.
    //Matrix *m = new Matrix ();
    //m->load("/Users/jarico/tmp/ex_part.2dist");
    
    Algorithm *alg_tlop = new MxM_Algorithm ();
    Algorithm *alg_bem  = new HalfPerimeterAlgorithm ();

    Arrangement *r_orig = m->getArrangement(idx);
    
    // 2. Find out the best arrangment using half-perimeter and Beaumont algorithm
    Heuristic *b = new BeaumontHeuristic (alg_bem);
    Arrangement **r_bem = b->apply(r_orig);
    (*r_bem)->show();
    //Cost *c_hper  = alg_bem->execute (r_bem); cout << "Beaumont H-Per cost: " << c_hper->getTime() << endl;
    Cost *c_tlop = alg_tlop->execute (*r_bem);
    
    double cost = c_tlop->getTime();
    
    // 4. Delete structures and objects !!!
    delete r_bem;
    delete c_tlop;
    delete (MxM_Algorithm *)alg_tlop;
    delete (HalfPerimeterAlgorithm *)alg_bem;
    delete (BeaumontHeuristic *)b;
    //delete m;
    
    return cost;
}



// Return the cost of communications using the Beaumont algorithm in time (t-Lop cost)
double beaumont_modified_cost (Matrix *m, int idx) {
    
    
    // 1. Original arrangement and creation of algorithms.
    //Matrix *m = new Matrix ();
    //m->load("/Users/jarico/tmp/ex_part.2dist");
    
    Algorithm *alg_tlop = new MxM_Algorithm ();
    Algorithm *alg_bem  = new HalfPerimeterAlgorithm ();
    
    Arrangement *r_orig = m->getArrangement(idx);
    
    // 2. Find out the best arrangment using half-perimeter and Beaumont algorithm
    Heuristic *b = new BeaumontHeuristicMod (alg_tlop);
    Arrangement **r_tlop = b->apply(r_orig);
    (*r_tlop)->show();
    //Cost *c_hper  = alg_bem->execute (r_tlop);
    Cost *c_tlop = alg_tlop->execute (*r_tlop);
    
    double cost = c_tlop->getTime();
    
    // 4. Delete structures and objects !!!
    delete r_tlop;
    delete c_tlop;
    delete (MxM_Algorithm *)alg_tlop;
    delete (HalfPerimeterAlgorithm *)alg_bem;
    delete (BeaumontHeuristic *)b;
    //delete m;
    
    return cost;
}





int main_beaumont (int argc, const char * argv[]) {
    
    srand(100);
    
    Improvement *imps = new Improvement ();
    
    for (int m = min_M; m <= M; m++) {
        
        for (int np = min_P; np <= P; np++) {
            
            if (np < m) continue;
            
            Matrix *mat = new Matrix ();
            
            //generate_file(m, np);
            int idx = generate_arrangement(mat, m, np);
            
            double b = beaumont_cost(mat, idx);
            double d = beaumont_modified_cost(mat, idx);
            imps->putB(m, np, b);
            imps->putM(m, np, d);
            
            cout << m << " x " << np << " = " << fixed << setprecision(2) << imps->get(m, np) << flush << endl;
            
            delete mat;
        }

        imps->statistics();
        imps->print();
        imps->output("/Users/jarico/Google Drive/code_tau_lop/taulop_code/generate/data.txt");

    }
        
    delete imps;
    return 0;
}


int main_otros(int argc, const char * argv[]) {
    
    
    Matrix *m = new Matrix ();
    // m->load(); // Synthetic matrix for testing
    m->load("/Users/jarico/Google Drive/code_tau_lop/TauLopCost/5_part.2dist");
    
    // !. Algorithms
    Algorithm *alg_tlop = new MxM_Algorithm ();
    Algorithm *alg_bem  = new HalfPerimeterAlgorithm ();
    Arrangement *r_orig = m->getArrangement(0);
    
    // 2. Find out the best arrangment using half-perimeter and Beaumont algorithm
    cout << endl << "BEAUMONT _____________________" << endl;
    Heuristic *b = new BeaumontHeuristic (alg_bem);
    cout << "Number of combinations (Beaumont): " << b->combinations(r_orig) << endl;
    Arrangement **r_bem = b->apply(r_orig);
    cout << "Best BEAUMONT arrangement: " << endl;
    Cost *c_hper  = alg_bem->execute (*r_bem);
    cout << fixed << setprecision(2) << "Half Perimeter Cost: " << c_hper->getTime() << endl;
    (*r_bem)->show();
    (*r_bem)->plot("./squares/squares_beaumont.gpl");
    
    
    // 3. Find out the best arrangment using tau-Lop cost and Beaumont algorithm
    cout << endl << "tau-Lop _____________________" << endl;
    Heuristic *t = new BeaumontHeuristic (alg_tlop);
    cout << "Number of combinations (tau-Lop): " << t->combinations(r_orig) << endl;
    Arrangement **r_tlop = t->apply(r_orig);
    cout << "Best tau-Lop arrangement: " << endl;
    Cost *c_tlop  = alg_tlop->execute (*r_tlop);
    cout << fixed << setprecision(2) << "tau-Lop Cost: " << c_tlop->getTime() << endl;
    (*r_tlop)->show();
    (*r_tlop)->plot("./squares/squares_tau_Lop.gpl");
    
    
    // 4. Find out the best arrangment using Malik heuristic on tau-Lop cost
    cout << endl << "Malik (tau-Lop) _____________________" << endl;
    Heuristic *hm = new MalikHeuristic (alg_tlop, "", "");
    cout << "Number of combinations (Malik): " << hm->combinations(r_orig) << endl;
    Arrangement **rm = hm->apply(r_orig);
    cout << "Best Malik arrangement: " << endl;
    Cost *cm  = alg_tlop->execute (*rm);
    cout << fixed << setprecision(2) << "Malik (tau-Lop) Cost: " << cm->getTime() << endl;
    (*rm)->show();
    (*rm)->plot("./squares/squares_malik.gpl");
    
    
    return 0;
}




