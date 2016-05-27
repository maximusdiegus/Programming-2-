// 74379726A PASCUAL ANTOLINOS, DIEGO
#include <iostream>
#include <ctype.h>
#include "Fleet.h"
#include "Util.h"
#include "Ship.h"

using namespace std;

const int IMPSHIPCAPACITY = 30;
const int REBSHIPCAPACITY = 35;
const string initialImperialShipDotation = "10tf/  5  tb, 5 ti, 5;ta";
const string initialRebelShipDotation = "10xw,5yw, 8aw, 5bw";

Ship::Ship(bool side){
  this->side = side;
  if(side == IMPERIAL){
    maxCapacity = IMPSHIPCAPACITY;
    addFighters(initialImperialShipDotation);
  }
  else{
    maxCapacity = REBSHIPCAPACITY;
    addFighters(initialRebelShipDotation);
  }
}

bool Ship::addFighters(string fighters){
  
  bool fighterIntroduced, stop, introduced;
  string typeOfFighter, numberOfFighters;
  int i, numFighters, length;

  fighterIntroduced = true;
  stop = false;
  i = 0;
  length = fighters.length();
  
  while(i < length && !stop){
    while(!isdigit(fighters[i]) && i < length){
      i++;
    }
    numberOfFighters = "";
    while(isdigit(fighters[i]) && i < length){
      numberOfFighters += fighters[i];
      i++;
    }
    typeOfFighter = "";
    while(!isalpha(fighters[i]) && i < length){
      i++;
    }
    while(isalpha(fighters[i]) &&i < length){
      typeOfFighter = typeOfFighter + fighters[i];
      i++;
    }
    if(typeOfFighter != "" && numberOfFighters != ""){
      numFighters = atoi(numberOfFighters.c_str());
      if(fleet.getNumFighters() + numFighters <= maxCapacity){		
	introduced = fleet.addFighters(typeOfFighter, numFighters, side);
	if(introduced == false){
	  stop = true;
	  fighterIntroduced = false;
	}
      }
      else{
	Util::error(CAPACITY_EXCEEDED);
	stop = true;
	fighterIntroduced = false;		
      }
    }
  }
return fighterIntroduced;
}
   

bool Ship::improveFighter(){
  int fighterNumber, amount, cost;
  char improveSpec, answer;
  bool fighterImproved;
  
  fighterImproved = false;
  
  cout<< "Select fighter number: ";
  cin>>fighterNumber;
  cin.get();
  if(fighterNumber < 1 || fighterNumber > fleet.getNumFighters()){
   Util::error(WRONG_NUMBER);
  }  
  else{
    cout<< "What to improve (v/a/s)? ";
    cin>>improveSpec;
    cin.get();
    if(improveSpec != 'v' && improveSpec != 'a' && improveSpec != 's'){
      Util::error(UNKNOWN_OPTION);
    }
    else{
      cout<< "Amount: ";
      cin>>amount;
      cin.get();
      if(fleet.getCredits() <= 0){
	Util::error(NO_FUNDS);
      }
      else if(amount <= 0){
	Util::error(WRONG_NUMBER);
      }
      else{
	switch(improveSpec){
	  case 'v':
	    if(fleet.getCredits() - 2*amount < 0){ //Comprobamos que hay dinero suficiente para realizar la operacion
	      Util::error(NO_FUNDS);
	    }
	    else{
	      cost = 2*amount;
	      cout<< "That will cost you " << cost << " credits. Confirm? (y/n)";	    
	      cin>>answer;
	      cin.get();
	      if(answer == 'y'){
		fleet.improveFighter(fighterNumber-1, improveSpec, amount, cost); //Llamamos al metrodo improvefighter alojado en fleet.cc para introducir el fighter
		cout << "Fighter improved: ";
		cout << fleet.getFighter(fighterNumber-1); //Mostramos los fighters
		cout<<endl;
		fighterImproved = true;
	      }
	    }
	    break;
	  case 'a':
	    if(fleet.getCredits() - 3*amount < 0){
	      Util::error(NO_FUNDS);
	    }
	    else{
	      cost = 3*amount;
	      cout<< "That will cost you " << cost << " credits. Confirm? (y/n)";	    
	      cin>>answer;
	      cin.get();
	      if(answer == 'y'){
		fleet.improveFighter(fighterNumber-1, improveSpec, amount, cost);
		cout << "Fighter improved: ";
		cout << fleet.getFighter(fighterNumber-1);
		cout<<endl;
		fighterImproved = true;
	      }
	    }
	    break;
	  case 's':
	    if(fleet.getCredits() - (amount + 1)/2 < 0){
	      Util::error(NO_FUNDS);
	    }
	    else{
	     cost = (amount + 1)/2;
	     cout<< "That will cost you " << cost << " credits. Confirm? (y/n)";	    
	     cin>>answer;
	     cin.get();
	     if(answer == 'y'){
	      fleet.improveFighter(fighterNumber-1, improveSpec, amount, cost);
	      cout << "Fighter improved: ";
	      cout << fleet.getFighter(fighterNumber-1);
	      cout<<endl;
	      fighterImproved = true;
	     }
	    }
	  break;
	}
      }
    }
  }
  return(fighterImproved);
}

void Ship::fight(Ship &enemy){
  fleet.fight(enemy.fleet);
}

ostream &operator<<(ostream &os, const Ship &ship){
  os << "Ship info: max. capacity=";
  os << ship.maxCapacity;
  os << ", side=";
  if(ship.side == IMPERIAL){
    os << "IMPERIAL";
  }
  else{
    os << "REBEL";
  }
  os << ",";
  os << ship.fleet;
  return os;
}