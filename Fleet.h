// 74379726A PASCUAL ANTOLINOS, DIEGO
#ifndef _fleet
#define _fleet
#include "Fighter.h"
#include <vector>
#include <iostream>

using namespace std;

const bool IMPERIAL = false;
const bool REBEL = true;
const int CREDITS_INITIAL = 2000;

class Fleet{

 protected:
   int credits;
   int wins;
   int losses;
   vector<Fighter> fighters;
   void listFighters(const vector<Fighter> &vf) const;
  
 public:
   Fleet();
   Fighter getFighter(int n) const;
   int getNumFighters() const;
   int getCredits() const;
   bool addFighters (string type, int number, bool side);
   bool fight(Fleet &enemy);
   void improveFighter (int nf, char feature, int amount, int cost);

   friend ostream &operator<<(ostream &os, const Fleet &flota);
};
#endif