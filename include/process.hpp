//
//  process.hpp
//  TauLopCost
//
//  Created by jarico on 04/11/16.
//  Copyright © 2016 Juan A. Rico. All rights reserved.
//

#ifndef process_hpp
#define process_hpp

#include <iostream>
#include <string>
using namespace std;

class Process {
   
protected:
   
   int    rank;      // Global rank
   int    rankIntra; // Rank in the node
   int    node;      // Number of the node
   
public:
   
   Process  ();
   Process  (int rank);
   Process  (int rank, int node);
   Process  (const Process *p);
   ~Process ();
   
   void   setRank       (int rank);
   void   setRankIntra  (int rank);
   void   setNode       (int node);
   
   int    getRank       () const;
   int    getRankIntra  () const;
   int    getNode       () const;
   
   void show  ();
   ostream& operator<< (ostream& outs);
   
   void operator = (const Process &c);
   
};

#endif /* process_hpp */
