// 74379726A PASCUAL ANTOLINOS, DIEGO
#ifndef _ship
#define _ship
#include "Fighter.h"
#include "Fleet.h"
#include <iostream>

using namespace std;

class Ship{

  protected:
    bool side;
    int maxCapacity;
    Fleet fleet;
    
 public:
   Ship(bool side);
   bool addFighters(string fighters);
   bool improveFighter();
   void fight(Ship &enemy);

 friend ostream &operator<<(ostream &os, const Ship &ship);  
};
#endif