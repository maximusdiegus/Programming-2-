// 74379726A PASCUAL ANTOLINOS, DIEGO
#include <iostream>
#include <stdexcept>
#include "Fighter.h"
#include "Util.h"

using namespace std;

struct FighterData{
  string type;
  int velocity;
  int attack;
  int shield;
  int cost;
};

const FighterData FIGHTERTABLE[] = {
{ "TIE-Fighter",      150,  75, 30,  45 },
{ "TIE-Bomber",        80, 150, 45,  75 },
{ "TIE-Interceptor",  180,  65, 30,  55 },
{ "TIE-Advanced",     160,  80, 90,  95 },
{ "X-Wing",           175,  90, 75,  65 },
{ "Y-Wing",            90, 150, 90,  90 },
{ "A-Wing",           200,  60, 50,  45 },
{ "B-Wing",           120, 200, 90, 100 }
};

Fighter::Fighter(string abr){
  
  bool igual;
  int j;
  const int MAXFIGHTERS = 8;
  
  igual = false;
  
  if(abr == ""){
    type = "";
    velocity = 0;
    attack = 0;    
    shield = 0;
    cost = 0;    
  }
  else{
    for(int i = 0; i < MAXFIGHTERS; i++){
      if(FIGHTERABR[i] == abr){
	igual = true;
	j = i;
      }
    }
    if(igual == false){
      throw invalid_argument("wrong fighter type");
    }
      type = FIGHTERTABLE[j].type;
      velocity = FIGHTERTABLE[j].velocity;
      attack = FIGHTERTABLE[j].attack;
      shield = FIGHTERTABLE[j].shield;
      cost = FIGHTERTABLE[j].cost;
  }
}
string Fighter::getType() const{
  return type;
}

int Fighter::getVelocity() const{
  return velocity;
}

int Fighter::getAttack() const{
  return attack;
}

int Fighter::getShield() const{
  return shield;
}

int Fighter::getCost() const{
  return cost;
}

void Fighter::increaseVelocity (int amount){
  velocity = velocity + amount;
}

void Fighter::increaseAttack (int amount){
  attack = attack + amount;
}

void Fighter::increaseShield (int amount){
  shield = shield + amount;
}

void Fighter::increaseCost (int amount){
  cost = cost + amount;
}

bool Fighter::isImperial() const{
  bool igual;
  const int MAXFIGHTERS = 8;
  igual = false;
  for(int i = 0; i < MAXFIGHTERS/2; i++){
    if(FIGHTERTABLE[i].type == type){
      igual = true;
    }
  }
  return igual;
}

int Fighter::fight(Fighter &enemy){
  const int RANDOMNUMBER = 100;
  const int NUMBERFIGHTDOWN = 5;
  const int NUMBERFIGHTUP = 95;
  const int NUMBERFIGHT = 300;
  int testFight, fightResult;
  double u, damage;
  bool endBattle;
  endBattle = false;
  do{
    testFight = Util::getRandomNumber(RANDOMNUMBER);
    if(testFight < NUMBERFIGHTDOWN || testFight > NUMBERFIGHTUP){
      endBattle = true;
      fightResult = 0; //Empate. El combate termina
    }
    else{
      u = (100*velocity)/(velocity + enemy.velocity);
      if(testFight <= u){//El caza imperial ha acertado
	damage = (testFight*attack)/NUMBERFIGHT;
	enemy.shield = enemy.shield - damage;
	if(enemy.shield <= 0){
	  endBattle = true;
	  fightResult = -1;
	}
      }
      else{//El caza rebelde (enemy) te ha acertado
	damage = ((100 - testFight)* enemy.attack)/NUMBERFIGHT;
	shield = shield - damage;
	if(shield <= 0){
	  endBattle = true;
	  fightResult = 1;
	}
      }
    }
  }while(endBattle == false);
  return(fightResult);
}

ostream &operator<<(ostream &os, const Fighter &f){
	os << f.type << " (v=";
	os << f.velocity << ", a=";
	os << f.attack << ", s=";
	os << f.shield << ", c=";
	os << f.cost;
	os << ")";
	return os;
}

