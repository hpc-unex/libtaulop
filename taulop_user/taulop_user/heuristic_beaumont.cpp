//
//  heuristic_beaumont.cpp
//  TauLopCost
//
//  Created by jarico on 10/10/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#include "heuristic.hpp"
#include "heuristic_beaumont.hpp"

#include "algorithm.hpp"
#include "rectangle.hpp"
#include "arrangement.hpp"

#include <limits>
#include <iomanip>



/* Return the number of rectangles (processes) per column combinations ordered */
void BeaumontHeuristic::agrupate (int *v, int c, int C, long int &n, Group *agr[]) {
    
    
    Group *g = new Group(C);
    
    for (int i = 0; i < C; i++) {
        g->putGroup(i, v[i]);
    }
    agr[n] = g;
    n++;
    
    
    
    for (int x = c; x < C - 1; x++) {

        int y = x + 1;
        for (int val = 1; val < v[x]; val++) {
            v[y] += val;
            v[x] -= val;
            agrupate (v, y, C, n, agr);
            v[y] -= val;
            v[x] += val;
        }
        
    }
    
}


void BeaumontHeuristic::agrupation_nr (int *v, int c, int C, long int &n) {
    
    n++;
    
    for (int x = c; x < C - 1; x++) {
        int y = x + 1;
        for (int val = 1; val < v[x]; val++) {
            v[y] += val;
            v[x] -= val;
            agrupation_nr (v, y, C, n);
            v[y] -= val;
            v[x] += val;
        }
    }
    
}



BeaumontHeuristic::BeaumontHeuristic (Algorithm *alg) {
    this->alg = alg;
}


BeaumontHeuristic::~BeaumontHeuristic () {
    
}


Arrangement ** BeaumontHeuristic::apply (Arrangement *r) {
    
    // 1. How many aggrupations?
    long int n = this->combinations(r);

    int col_nr = r->getP();
    int p_nr = r->getP();

    // 2. Calculate possible aggrupations (ordered number of proceses per column).
    Group *agr [n];
    n = 0;
    for (int c = 1; c <= col_nr; c++) {
        
        int *groups = new int [c];
        
        for (int i = 1; i < c; i++)
        groups[i] = 1;
        groups[0] = p_nr - c + 1;
        
        agrupate(groups, 0, c, n, agr);
        
        delete [] groups;
    }
    
    // 2.1. Put the r value. It seems to contain the number of rectangles minus the last column
    //      number of rectangles. (TODO ?)
    for (int i = 0; i < n; i++) {
        int rest = agr[i]->getGroup(agr[i]->getCols() - 1);
        agr[i]->putR(r->getP() - rest);
    }
    
    // 3. Calculate the cost of the aggrupations [Beaumont algorithm]
    // 3.1. Put rectangles (duplicate) in a vector
    int P = r->getP();
    Rectangle *v_proc[P];

    p_nr = 0;
    for (int col = 0; col < r->getColNr(); col++) {
        for (int row = 0; row < r->getRowNr(col); row++) {
            Rectangle *p = r->get(row, col);
            Rectangle *p_new = new Rectangle(p);
            v_proc[p_nr++] = p_new;
        }
    }
    // 3.1.2. Order rectangles by area (bubble sort)
    for (p_nr = P-1; p_nr >= 1; p_nr--) {
        for (int j = 0; j < p_nr; j++) {
            
            int area_j   = v_proc[j]->getArea();
            int area_sig = v_proc[j+1]->getArea();
            
            if (area_j > area_sig) {
                Rectangle *aux = v_proc[j];
                v_proc[j] = v_proc[j+1];
                v_proc[j+1] = aux;
            }
        }
    }
    
#if TLOP_DEBUG == 1
    cout << "Order: " << endl;
    for (int i = 0; i < P; i++) {
        Rectangle *p = v_proc[i];
        cout << "P " << p->getRank() << "  " << p->getNode() << endl;
    }
    cout << endl;
#endif
    
    
    // 3.2. Create the arrangements: n
    Arrangement *arrang[n];
        
    for (int i = 0; i < n; i++) {  // For each agrupation we create an arrangement
        
        int acum_rows = 0;
        p_nr = 0;
        col_nr = agr[i]->getCols();
        
        /*****************************************************************/
        // ESTO ESTA DESACTIVADO PORQUE ESTA FUNCION HA SIDO ELIMINADA.
        // HAY QUE PASAR EL COL_NR y EL VECTOR DE COL_NR POSICIONES INDICANDO LAS FILAS.
        /*****************************************************************/
        //arrang[i] = new Arrangement(col_nr);  // Arrangement with col_nr columns
        /*****************************************************************/
        /*****************************************************************/

        for (int col = 0; col < col_nr; col++) {
            
            int acum_cols = 0;
            int row_nr = agr[i]->getGroup(col);
            
            // Get sum of widths of the proceses in this column
            int width = 0;
            for (int row = 0; row < row_nr; row++) {
                Rectangle *p2 = v_proc[p_nr + row];
                width = width + (p2->getArea() / r->getWidth());
            }
            
            // In the column col there are row_nr rectangles
            for (int row = 0; row < row_nr; row++) {
                Rectangle *p = new Rectangle(v_proc[p_nr + row]); // Duplicate
                int height = p->getArea() / width;
                
                p->setPos(row, col);
                
                if (col == col_nr - 1) {
                    width = r->getWidth() - acum_rows;
                }

                if (row == row_nr - 1) {
                    height = r->getWidth() - acum_cols;
                }

                p->setSize(width, height);
                
                p->setNode(v_proc[p_nr + row]->getNode());
                
                arrang[i]->insert(p, row, col);
                
                acum_cols += height;
            }
            
            acum_rows += width; // Round problems. Put the last column width to complete N blocks
                                //                 height is automatically put in establish.
            
            p_nr += row_nr;
        }
        
        // Data needed for the arrangment
        arrang[i]->setNodeNr(r->getNodeNr());
        arrang[i]->setProcNr(p_nr);
        arrang[i]->setBlockSize(r->getB());
        arrang[i]->setWidth(r->getWidth());
        arrang[i]->establish();
        
#if TLOP_DEBUG == 1
        agr[i]->show();
        arrang[i]->show();
#endif
        
    }
    
    // 4. Calculate the cost and get the best arrangement
    Arrangement *best_r = arrang[0];
    Cost *t_min = alg->execute(arrang[0]);
#if TLOP_DEBUG == 1
    arrang[0]->show();
    cout << "COST: " << t_min->getTime() << endl << endl;
#endif
    for (int i = 1; i < n; i++) {
        Cost *t = alg->execute(arrang[i]);
#if TLOP_DEBUG == 1
        arrang[i]->show();
        cout << "COST: " << t->getTime() << endl << endl;
#endif
        if (t->getTime() < t_min->getTime()) {
            best_r = arrang[i];
            t_min = t;
        }
    }
    
    // 5. Show
#if TLOP_DEBUG == 1
    cout << "Best Arrangement: " << endl;
    best_r->show();
#endif

    
    for (int i = 0; i < n; i++) {
        delete agr[i];
        if (best_r != arrang[i]) {
            delete arrang[i];
        }
    }
    
    return &best_r;
}


long int BeaumontHeuristic::combinations (Arrangement *r) {
    
    
    int col_nr = r->getP();
    int p_nr = r->getP();
    
    long int n = 0;
    for (int c = 1; c <= col_nr; c++) {
        int *groups = new int [col_nr];
        
        for (int i = 1; i < c; i++)
            groups[i] = 1;
        groups[0] = p_nr - c + 1;

        agrupation_nr(groups, 0, c, n);
        
        delete [] groups;
    }

    return n;
    
}

