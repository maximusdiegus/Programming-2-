// 74379726A PASCUAL ANTOLINOS, DIEGO
#include <iostream>
#include <vector>
#include <stdexcept>
#include "Util.h"
#include "Fleet.h"
#include "Fighter.h"
#include <algorithm>
using namespace std;

Fleet::Fleet(){
  credits = CREDITS_INITIAL;
  wins = 0;
  losses = 0;
}

void Fleet::listFighters(const vector<Fighter> &vf) const{
  int i, size;
  
  size = vf.size();
  
  for(i = 0; i < size; i++){
    cout << "[" << i + 1 << "] " << vf[i] << endl;	
  }
}

bool Fleet::addFighters(string type, int number, bool side){
  bool added;
  added = false;
  try{
    Fighter ship(type); // Constructor para inicializar la nave(ship)
    if((!ship.isImperial() && side == REBEL) || (ship.isImperial() && side == IMPERIAL)){
      if(ship.getCost()*number <= credits){
	for(int i = 1; i <= number; i++){
	  fighters.push_back(ship);
	}
	credits = credits - ship.getCost()*number;
	added = true;
      }
      else{
	Util::error(NO_FUNDS);
      }
    }
    else{
      Util::error(WRONG_FIGHTER_TYPE);
    }
  }
  catch(invalid_argument &e){
    Util::error(WRONG_FIGHTER_TYPE);
  }
  return added;
}

bool Fleet::fight(Fleet &enemy){ 
  const int RANDOMNUMBER = 100;
  const int NUMMARGEN = 7; 
  int num, temp, margen, totalCazas, randomFighter, i, endFight, result, tam;
  bool wrongNumberFighter;
  string fighterFight, numAux;
  vector<int> selectedFighters;
  vector<Fighter> rebelTemp,imperialTemp;
  Fighter imperialShip, rebelShip;
  Fighter fg1, fg2;
  
  wrongNumberFighter = true;
  i = 0;
  
  if(fighters.size() >= 1 && enemy.fighters.size() >= 1){
    cout << "Select fighter numbers: ";
    getline(cin, fighterFight);
    
    tam = fighterFight.length();
    while(i < tam){//Mete los numeros introducidos por el usuario en un vector
	while(fighterFight[i] == ' ' && i < tam){
	  i++;
	}
	numAux = "";
	while(fighterFight[i] >= '0' && fighterFight[i] <= '9' && i < tam){
	  numAux = numAux + fighterFight[i];
	  i++;
	}
	if(numAux != ""){
	  num = atoi(numAux.c_str());
	  selectedFighters.push_back(num);
	}
      }
      for (int i = 0; i < selectedFighters.size(); i++){//Ordenamos el vector de mayor a menor
	    for ( int j = 0 ; j < selectedFighters.size() - 1; j++){
		if (selectedFighters[j] < selectedFighters[j+1]){
		      temp = selectedFighters[j];
		      selectedFighters[j] = selectedFighters[j+1];
		      selectedFighters[j+1] = temp;
		}
	    }
      }
      for (int j = 0; j < selectedFighters.size(); j++){
	if(selectedFighters[j] >= 1 && selectedFighters[j] <= fighters.size()){
	  imperialTemp.push_back(fighters[(selectedFighters[j] - 1)]);//Metemos los cazas en un vector de cazas auxiliar
	  fighters.erase(fighters.begin() + (selectedFighters[j] - 1));//Y lo borramos del vector de cazas original
	  wrongNumberFighter = false;
	}  
      }
      if(wrongNumberFighter == true){
	Util::error(WRONG_NUMBER);
      }
      else{
	//Creamos los cazas que se meteran en el escuadrón rebelde
	margen = Util::getRandomNumber(NUMMARGEN);
	margen = margen - 3;
	totalCazas = imperialTemp.size() + margen;
	if(totalCazas <= 0){
	  totalCazas = 1;
	}
	else if(totalCazas > enemy.fighters.size()){
	  totalCazas = enemy.fighters.size();
	}
	//Elegimos los cazas rebeldes aleatoriamente
	for(int i = 1; i <= totalCazas; i++){
	  randomFighter = Util::getRandomNumber(enemy.fighters.size());
	  rebelTemp.push_back(enemy.fighters[randomFighter]);
	  enemy.fighters.erase(enemy.fighters.begin() + randomFighter);
	}
	cout << "Squadron A" << endl;
	listFighters(imperialTemp);
	cout << "Squadron B" << endl;
	listFighters(rebelTemp);
	do{
	    endFight = Util::getRandomNumber(RANDOMNUMBER);
	    if(endFight >= 10){
	      //Seleccionamos y borramos un fighter del vector temporal de fighters y lo asignamos a una variable que será lanzada a luchar
	      randomFighter = Util::getRandomNumber(imperialTemp.size());
	      imperialShip = imperialTemp[randomFighter];
	      imperialTemp.erase(imperialTemp.begin() + randomFighter);
	      
	      randomFighter = Util::getRandomNumber(rebelTemp.size());
	      rebelShip = rebelTemp[randomFighter];
	      rebelTemp.erase(rebelTemp.begin() + randomFighter);
	      
	      cout<<"-- begin fight"<<endl;
	      cout<< imperialShip;
	      cout<<endl;
	      cout<< rebelShip;
	      cout<<endl;
	      cout<<"--"<<endl;
	      
	      result = imperialShip.fight(rebelShip);//Lanzamos a luchar a los cazas elegidos llamando al metodo fight
	      
	      cout<< imperialShip;
	      cout<<endl;
	      cout<< rebelShip;
	      cout<<endl;
	      cout<<"-- end fight"<<endl;
	      
	      if(result == 1){//Gana la nave rebelde
		rebelTemp.push_back(rebelShip);
		enemy.credits = enemy.credits + imperialShip.getCost();
		enemy.wins++;
		losses++;
	      }
	      
	      else if(result == -1){//Gana la nave imperial
		imperialTemp.push_back(imperialShip);
		credits = credits + rebelShip.getCost();
		wins++;
		enemy.losses++;
	      }
	      
	      else if(result == 0){//Empate
		imperialTemp.push_back(imperialShip);
		rebelTemp.push_back(rebelShip);
	      }
	    }
	  
	}while(endFight >= 10 && rebelTemp.size() > 0 && imperialTemp.size() > 0);
	
	cout << "Squadron A" << endl;
	listFighters(imperialTemp);
	cout << "Squadron B" << endl;
	listFighters(rebelTemp);
	//Devolvemos los cazas temporales a la nave original
	for(int i = 0; i < imperialTemp.size(); i++){
	  fighters.push_back(imperialTemp[i]);
	}
	
	for(int j = 0; j < rebelTemp.size(); j++){
	  enemy.fighters.push_back(rebelTemp[j]);
	}
      }
  }
  else{
    Util::error(NO_FIGHTERS);
  }
}

Fighter Fleet::getFighter(int n) const{
  return fighters[n];
}

int Fleet::getNumFighters() const{
  return fighters.size();
}

int Fleet::getCredits() const{
  return credits;
}

void Fleet::improveFighter(int nf, char feature, int amount, int cost){
  switch(feature){
    case 'v':
      fighters[nf].increaseVelocity(amount);
      fighters[nf].increaseCost(cost);
      credits = credits - cost;
      break;
    case 'a':
      fighters[nf].increaseAttack(amount);
      fighters[nf].increaseCost(cost);
      credits = credits - cost;
      break;
    case 's':
      fighters[nf].increaseShield(amount);
      fighters[nf].increaseCost(cost);
      credits = credits - cost;
      break;
  }
}

ostream &operator<<(ostream &os, const Fleet &flota){
  os << "credits=" << flota.credits << ", wins=" << flota.wins << ", losses=" << flota.losses << endl;
  for(int i = 0; i < flota.fighters.size(); i++){
    os << "[" << i + 1 << "] " << flota.fighters[i] << endl;	
  }
  return os;
}