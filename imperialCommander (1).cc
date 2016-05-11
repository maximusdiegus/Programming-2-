// 74379726A PASCUAL ANTOLINOS, DIEGO 
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>

using namespace std;

// ship constants
const bool IMPERIAL=false;
const bool REBEL=true;

const int IMPSHIPCAPACITY=30;
const int REBSHIPCAPACITY=35;

const int initialImperialShipDotation[] = {10,5,5,5,0,0,0,0};
// 10 TIE-Fighters, 5 TIE-Bombers, 5 TIE-Interceptors, 5 TIE-Advanced, 0 X-Wing, ....

const int initialRebelShipDotation[] = {0,0,0,0,10,5,8,5}; 
// 0 TIE-Fighters ..., 10 X-Wing, 5 Y-Wing, 8 A-Wing, 5 B-Wing

// ship initial credits
const int CREDITS_INITIAL=2000;

// error numbers
const int UNKNOWN_OPTION=1;
const int WRONG_FIGHTER_TYPE=2;
const int CAPACITY_EXCEEDED=3;
const int WRONG_NUMBER=4;
const int NO_FUNDS=5;
const int NO_FIGHTERS=6;

//---------------------------------------------------

typedef struct {
     string type;  // TIE Fighter, X-Wing, ...
     int velocity;
     int attack;    // attack power
     int shield;    // current shield status. if <=0, the fighter is considered destroyed
     int cost;      // cost in credits
} Fighter;


typedef struct {
    bool side;       // IMPERIAL or REBEL
    int maxCapacity; // maximum capacity of fighters allowed in the ship
    int credits;      // credits remaining to buy fighters, make repairs or improvements
    int wins;         // enemy fighters destroyed
    int losses;        // fighters lost
    vector<Fighter> fighters;  // fighters inside the ship
} Ship;

// Fighters table 

const int MAXFIGHTERS=8;

// Abbreviations (to use in option "Add fighter")
const string FIGHTERABR[]= { "tf", "tb", "ti", "ta", "xw", "yw", "aw", "bw"};

const Fighter FIGHTERTABLE[] = {
  
 { "TIE-Fighter",      150,  75, 30,  45 },
 { "TIE-Bomber",        80, 150, 45,  75 },
 { "TIE-Interceptor",  180,  65, 30,  55 },
 { "TIE-Advanced",     160,  80, 90,  95 },
 { "X-Wing",           175,  90, 75,  65 },
 { "Y-Wing",            90, 150, 90,  90 },
 { "A-Wing",           200,  60, 50,  45 },
 { "B-Wing",           120, 200, 90, 100 }
 
};

// Couts' constants
const string OPTION1 = "1- List ship info";
const string OPTION2 = "2- Add fighters";
const string OPTION3 = "3- Repair/improve fighter";
const string OPTION4 = "4- Launch fighter";
const string OPTION5 = "5- List enemy info";
const string OPTIONQ = "q- Quit";
const string OPTION = "Option: ";
const string TXT_ENTERFIGHTERTYPE = "Enter fighter type (tf/tb/ti/ta): ";
const string TXT_NUMBEROFIGHTERS = "Number of fighters: ";
const string TXT_SELECTFIGHTERNUMBER = "Select fighter number: ";
const string TXT_WHATTOIMPROVE = "What to improve (v/a/s)? "; 
const string TXT_AMOUNT = "Amount: ";
const string TXT_THATWILLCOSTYOU = "That will cost you ";
const string TXT_CONFIRMCREDITS = " credits. Confirm? (y/n)";
const string TXT_FIGHTERIMPROVED = "Fighter improved: ";

const int RANDOMNUMBER = 100;
const int NUMBERFIGHTDOWN = 5;
const int NUMBERFIGHTUP = 95;
const int NUMBERFIGHT = 300;

void muestraMenu();
void error(int n);
int getRandomNumber(int max);
void initializeShip(Ship &ship,bool side);
void listFighter(const Fighter &f);
void listFighters(const vector<Fighter> &vf);
void listShip(const Ship &ship);
bool addFighter(Ship &ship);
bool improveFighter(Ship &ship);
int fight(Fighter &fg1,Fighter &fg2);
void launchFighter(Ship &imperial,Ship &rebel);
int selectedShip(string &tipoFighter); //Devuelve la posicion que ocupa la nave elegida
int costFighter(string &tipoFighter); //Devuelve el coste de la nave elegida.
void pushBackFighters(Ship &ship, string &typeFighter, int &numberFighter); //Insterta en el vector Fighters, más fighters.

main(){
  
   Ship imperialDestroyer, rebelShip;
   char option; 
   bool side;
   Fighter f;
   
   srand(1); // inicializar secuencia de números aleatorios: NO TOCAR
   
   side = IMPERIAL;
   initializeShip(imperialDestroyer, side);
   
   side = REBEL;
   initializeShip(rebelShip, side);
   
   do {

     muestraMenu();
     cin>>option;
     
	switch (option){
	  
	  case '1':
	      
	    side = IMPERIAL;
	    listShip(imperialDestroyer);
	      
	    break;
	    
	  case '2':
	    
	    cin.get();
	    addFighter(imperialDestroyer);
	    
	    break;
	    
	  case '3':
	    
	    improveFighter(imperialDestroyer);
	    
	    break;
	    
	  case '4':
	    
	    launchFighter(imperialDestroyer,rebelShip);
	    
	    break;
	    
	  case '5':
	    
	    side = REBEL;
	    listShip(rebelShip);
	    
	    break;
	    
	  case 'q':
	    
	    break;
	    
	  default: error(UNKNOWN_OPTION);
	    
        }
        
   }while (option != 'q');
   
}

void muestraMenu(){
  
  cout<<OPTION1<<endl;
  cout<<OPTION2<<endl;
  cout<<OPTION3<<endl;
  cout<<OPTION4<<endl;
  cout<<OPTION5<<endl;
  cout<<OPTIONQ<<endl;
  cout<<OPTION;
  
}
//---------------------------------------------------
void error(int n){
  
  cout << "ERROR (" << n << "): " ;
  
  switch (n) 
  {
    case UNKNOWN_OPTION:
      cout << "unknown option" << endl;
      break;
    case WRONG_FIGHTER_TYPE:
      cout << "wrong fighter type" << endl;
      break;
    case CAPACITY_EXCEEDED:
      cout << "ship capactity exceeded" << endl;
      break;
    case WRONG_NUMBER:
      cout << "wrong number" << endl;
      break;
    case NO_FUNDS:
      cout << "not enough credits" << endl;
      break;
    case NO_FIGHTERS:
      cout << "not enough fighters" << endl;
      break;
  }
  
}
//---------------------------------------------------
int getRandomNumber(int max){
  
   // Genera un número aleatorio entre 0 y maximo-1
   //
   int na;

   na = (int)(((double)max)*rand()/(RAND_MAX+1.0));
   return na;
  
}
//---------------------------------------------------
void initializeShip(Ship &ship,bool side){
  
  if(side == IMPERIAL){ 
    
    ship.side = IMPERIAL;
    ship.maxCapacity = IMPSHIPCAPACITY;
    ship.credits = CREDITS_INITIAL;
    ship.wins = 0;
    ship.losses = 0;
        
    for(int i = 0; i < MAXFIGHTERS; i++){
	
	if(initialImperialShipDotation[i] != 0){
	
	  for(int j = 0; j < initialImperialShipDotation[i]; j++){
	    
	    ship.fighters.push_back(FIGHTERTABLE[i]);
	    ship.credits = ship.credits - FIGHTERTABLE[i].cost;
	    
	  }
	    
	}
	  
      }
    
  }
  
  else{
    
    ship.side = REBEL;
    ship.maxCapacity = REBSHIPCAPACITY;
    ship.credits = CREDITS_INITIAL; 
    ship.wins = 0;
    ship.losses = 0;
       
    for(int i = 0; i < MAXFIGHTERS; i++){
	
	if(initialRebelShipDotation[i] != 0){
	
	  for(int j = 0; j < initialRebelShipDotation[i]; j++){
	    
	    ship.fighters.push_back(FIGHTERTABLE[i]);
	    ship.credits = ship.credits - FIGHTERTABLE[i].cost;
	    
	  }
	    
	}
	  
      }
      
  }
  
}
//---------------------------------------------------
void listFighter(const Fighter &f){ // Muestra los datos de una sola nave

  cout<< f.type << "(v=" << f.velocity << ", a=" << f.attack << ", s=" << f.shield << ", c=" << f.cost << ")";
  
}
//---------------------------------------------------
void listFighters(const vector<Fighter> &vf){
  
  for(int i = 0; i < vf.size(); i++){
  
    cout<<"["<< i + 1 <<"] ";
    listFighter(vf[i]);
    cout<<endl;
    
  }
    
}
//---------------------------------------------------
void listShip(const Ship &ship){
  
   if(ship.side == IMPERIAL){ 
	
      cout<<"Ship info: max. capacity="<<ship.maxCapacity;
      cout<<", side=IMPERIAL, credits="<<ship.credits;
      cout<<", wins="<<ship.wins<<", losses="<<ship.losses<<endl;
	    
      listFighters(ship.fighters);
      
   }

  else{ 
    
    cout<<"Ship info: max. capacity="<<ship.maxCapacity;
    cout<<", side=REBEL, credits="<<ship.credits;
    cout<<", wins="<< ship.wins<<", losses="<<ship.losses<<endl;
	    
    listFighters(ship.fighters);
	    
  }
  
}
//---------------------------------------------------
bool addFighter(Ship &ship){

  string tipoFighter;
  bool igual, fighterAdded;
  int numFighters, prize, selectShip;
  
  igual = false;
  fighterAdded = false;
  
  cout<< TXT_ENTERFIGHTERTYPE;
  cin>>tipoFighter;
  
  if(ship.credits <= 0){
	  
    error(NO_FUNDS);
	      
  }
  
  else{    
      
      for(int i = 0; i < MAXFIGHTERS/2; i++){
      
	  if(tipoFighter.compare(FIGHTERABR[i]) == 0){
	    
	    igual = true;
	    
	  }
	
	}
      
      if(igual == false){
	
	  error(WRONG_FIGHTER_TYPE);
	
      }
      
      else{
	
	selectShip = selectedShip(tipoFighter); //Convierte la opción elegida en un número
	
	cout<< TXT_NUMBEROFIGHTERS;
	cin>>numFighters; 
	
	if(numFighters <= 0){
	  
	  error(WRONG_NUMBER);
	  
	}
	
	else if(ship.fighters.size() >= IMPSHIPCAPACITY){
	      
	  error(CAPACITY_EXCEEDED);
		
	}
	
	else{
	
	  prize = costFighter(tipoFighter); //Devuelve el precio de la nave que hemos seleccionado
	  
	  switch(selectShip){
	  
	      case 0: //Nave seleccionada tf
		
		  if(numFighters*prize > ship.credits){ 
		  
		      error(NO_FUNDS);
	    
		  }
		
		  else if(numFighters + ship.fighters.size() > IMPSHIPCAPACITY){
		    
		    error(CAPACITY_EXCEEDED);
		    
		  }
		
		  else{
	    
		    pushBackFighters(ship, tipoFighter, numFighters);  
		    fighterAdded = true;
		    
		  }
		
	      break;
	      
	      case 1: //Nave seleccionada tb
		
		  if(numFighters*prize > ship.credits){ 
		  
		      error(NO_FUNDS);
	    
		  }
		
		  else if(numFighters + ship.fighters.size() > IMPSHIPCAPACITY){
		    
		    error(CAPACITY_EXCEEDED);
		    
		  }
		
		  else{
	    
		    pushBackFighters(ship, tipoFighter, numFighters);
		    fighterAdded = true;
		    
		  }
		
	      break;
		
	      case 2: //Nave seleccionada ti
		
		  if(numFighters*prize > ship.credits){ 
		  
		      error(NO_FUNDS);
	    
		  }
		
		  else if(numFighters + ship.fighters.size() > IMPSHIPCAPACITY){
		    
		    error(CAPACITY_EXCEEDED);
		    
		  }
		
		  else{
	    
		      pushBackFighters(ship, tipoFighter, numFighters);
		      fighterAdded = true;
		      
		    }
		
	      break;
		
	      case 3: //Nave seleccionada ta
		
		  if(numFighters*prize > ship.credits){ 
		  
		      error(NO_FUNDS);
	    
		  }
		
		  else if(numFighters + ship.fighters.size() > IMPSHIPCAPACITY){
		    
		    error(CAPACITY_EXCEEDED);
		    
		  }
		
		  else{
		    
		    pushBackFighters(ship, tipoFighter, numFighters);
		    fighterAdded = true;
		    
		  }
		
		break;
	    
	  }
	  
	}
	
      
      
    }
  
  }
  
  return(fighterAdded);
  
}
//---------------------------------------------------
bool improveFighter(Ship &ship){
  
  int fighterNumber, amount;
  char improveSpec, answer;
  bool fighterImproved;
  
  fighterImproved = false;
  
  cout<< TXT_SELECTFIGHTERNUMBER;
  cin>>fighterNumber;
  
  if(fighterNumber < 1 || fighterNumber > ship.fighters.size()){
    
    error(WRONG_NUMBER);
    
  }
  
  else if(ship.fighters.size() <= 0){
	      
    error(NO_FIGHTERS);
	      
  }
  
  else{	
    
    cin.get();
    cout<< TXT_WHATTOIMPROVE;
    cin>>improveSpec;
    
    if(improveSpec != 'v' && improveSpec != 'a' && improveSpec != 's'){
      
      error(UNKNOWN_OPTION);
      
    }
    
    else{
      
      cout<< TXT_AMOUNT;
      cin>>amount;
      
      if(ship.credits <= 0){
	  
	error(NO_FUNDS);
	      
      }
      
      else if(amount <= 0){
	
	error(WRONG_NUMBER);
	
      }
      
      else{
      
	switch(improveSpec){
    
	  case 'v':
	    
	    if(ship.credits - 2*amount < 0){
	      
	      error(NO_FUNDS);
	      
	    }
	    
	    else{

	      cout<< TXT_THATWILLCOSTYOU << 2*amount << TXT_CONFIRMCREDITS;	    
	      cin>>answer;
	      
	      if(answer == 'y'){
		
		ship.credits = ship.credits - 2*amount; 
		ship.fighters[fighterNumber - 1].velocity = ship.fighters[fighterNumber - 1].velocity + amount;
		ship.fighters[fighterNumber - 1].cost = ship.fighters[fighterNumber - 1].cost + 2*amount;
		
		cout<< TXT_FIGHTERIMPROVED;
		listFighter(ship.fighters[fighterNumber - 1]);
		cout<<endl;
		
		fighterImproved = true;
		
	      }
	      
	      
	      
	    }
	    
	    break;
	    
	  case 'a':
	    
	    if(ship.credits - 3*amount < 0){
	      
	      error(NO_FUNDS);
	      
	    }
	    
	    else{
	    
	      cout<< TXT_THATWILLCOSTYOU << 3*amount << TXT_CONFIRMCREDITS;	    
	      cin>>answer;
	    
	      if(answer == 'y'){
		
		ship.credits = ship.credits - 3*amount; 
		ship.fighters[fighterNumber - 1].attack = ship.fighters[fighterNumber - 1].attack + amount;
		ship.fighters[fighterNumber - 1].cost = ship.fighters[fighterNumber - 1].cost + 3*amount;
		
		cout<< TXT_FIGHTERIMPROVED;
		listFighter(ship.fighters[fighterNumber - 1]);
		cout<<endl;
		
		fighterImproved = true;
		
	      }
	      
	    }
	    
	    break;
	    
	  case 's':
	    
	    if(ship.credits - (amount + 1)/2 < 0){
	      
	      error(NO_FUNDS);
	      
	    }
	    
	    else{
	    
	      cout<< TXT_THATWILLCOSTYOU << (amount + 1)/2 << TXT_CONFIRMCREDITS;	    
	      cin>>answer;
	      
	      if(answer == 'y'){
	      
		ship.credits = ship.credits - (amount + 1)/2; 
		ship.fighters[fighterNumber - 1].shield = ship.fighters[fighterNumber - 1].shield + amount;
		ship.fighters[fighterNumber - 1].cost = ship.fighters[fighterNumber - 1].cost + (amount + 1)/2;
		
		cout<< TXT_FIGHTERIMPROVED;
		listFighter(ship.fighters[fighterNumber - 1]);
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
//---------------------------------------------------
int fight(Fighter &fg1,Fighter &fg2){

  int testFight, randomNumber, fightResult;
  
  double u, damage;
  
  bool endBattle;
  
  endBattle = false;
  
  cout<<"-- begin fight"<<endl;
  
  listFighter(fg1);
  cout<<endl;
  listFighter(fg2);
  cout<<endl;
  
  cout<<"--"<<endl;
  
  do{
    
    testFight = getRandomNumber(RANDOMNUMBER);
    
    if(testFight < NUMBERFIGHTDOWN || testFight > NUMBERFIGHTUP){
      
      endBattle = true;
      fightResult = 0;
      
    }
    
    else{
      
      u = (100*fg1.velocity)/(fg1.velocity + fg2.velocity);
      
      if(testFight <= u){//El caza imperial (fg1) ha acertado

	damage = (testFight*fg1.attack)/NUMBERFIGHT;
	fg2.shield = fg2.shield - damage;
	
	if(fg2.shield <= 0){
	  
	  endBattle = true;
	  fightResult = -1;
	  
	}
	
      }
      
      else{//El caza rebelde (fg2) te ha acertado

	damage = ((100 - testFight)* fg2.attack)/NUMBERFIGHT;
	fg1.shield = fg1.shield - damage;
	
	if(fg1.shield <= 0){
	  
	  endBattle = true;
	  fightResult = 1;
	  
	}
	
      }
      
    }
    
  }while(endBattle == false);
  
  listFighter(fg1);
  cout<<endl;
  
  listFighter(fg2);
  cout<<endl;
  
  cout<<"-- end fight"<<endl;
  
  return(fightResult);
  
}
//---------------------------------------------------
void launchFighter(Ship &imperial,Ship &rebel){
  
  int fighterFight, numRebelFighter, imperialSize, rebelSize;
  
  Fighter fg1, fg2;
  
  if(imperial.fighters.size() <= 0 || rebel.fighters.size() <= 0){
    
    error(NO_FIGHTERS);
      
  }
  
  else{
  
    cout<< TXT_SELECTFIGHTERNUMBER;
    cin>>fighterFight;
    
    if(fighterFight < 1 || fighterFight > imperial.fighters.size()){
      
      error(WRONG_NUMBER);
      
    }
      
    else{
    
      imperialSize = imperial.fighters.size() - 1;
      rebelSize = rebel.fighters.size();
      fg1 = imperial.fighters[fighterFight - 1];// 
      numRebelFighter = getRandomNumber(rebel.fighters.size());
      fg2 = rebel.fighters[numRebelFighter];
      imperial.fighters.erase (imperial.fighters.begin() + (fighterFight - 1));
      rebel.fighters.erase (rebel.fighters.begin() + (numRebelFighter));
      fight(fg1, fg2);
      
      if(fg1.shield <= 0){
	
	rebel.fighters.push_back(fg2);
	rebel.credits = rebel.credits + fg1.cost;
	rebel.wins++;
	imperial.losses++;
	
      }
      
      else if(fg2.shield <= 0){

	imperial.fighters.push_back(fg1);
	imperial.credits = imperial.credits + fg2.cost;
	imperial.wins++;
	rebel.losses++;
	
      }
      
      else if(fg1.shield > 0 && fg2.shield > 0){
	
	imperial.fighters.push_back(fg1);
	rebel.fighters.push_back(fg2);
	
      }
      
    }
    
  }
  
}
//---------------------------------------------------
int costFighter(string &tipoFighter){
  
  int prize, numNave;
  
    numNave = selectedShip(tipoFighter);
    prize = FIGHTERTABLE[numNave].cost;

    return (prize);
  
}
//---------------------------------------------------
int selectedShip(string &tipoFighter){//Compara el string "tipoFighter" con la tabla de naves y cuando encuentra una coincidencia saca el valor de la nave.
  
  int numFighter;
  
  for (int i = 0; i < MAXFIGHTERS; i++){
  
   if(tipoFighter.compare(FIGHTERABR[i]) == 0){
     
     numFighter = i;
     
    }
    
  }

  return(numFighter);
  
}

void pushBackFighters(Ship &ship, string &typeFighter, int &numberFighter){
  
  int cont, prize;
  
  cont = 0;
  
  do{
      
      for(int i = 0; i < MAXFIGHTERS; i++){
	      
	    if(typeFighter.compare(FIGHTERABR[i]) == 0){
	  
	      ship.fighters.push_back(FIGHTERTABLE[i]);
	      prize = costFighter(typeFighter);
	      ship.credits = ship.credits - prize;
	
	    }
	
	}
      
      cont++;
	
  }while(cont < numberFighter);
  
}

/*
  
  
  
*/

