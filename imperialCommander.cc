// 74379726A PASCUAL ANTOLINOS, DIEGO 
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace std;

// ship constants
const bool IMPERIAL=false;
const bool REBEL=true;

const int IMPSHIPCAPACITY=30;
const int REBSHIPCAPACITY=35;

//const int initialRebelShipDotation[] = {0,0,0,0,10,5,8,5};
//const int initialImperialShipDotation[] = {10,5,5,5,0,0,0,0};

const string initialImperialShipDotation = "10tf/ 5 tb, 5 ti, 5;ta";
// 10 TIE-Fighters, 5 TIE-Bombers, 5 TIE-Interceptors, 5 TIE-Advanced, 0 X-Wing, ....
const string initialRebelShipDotation="10xw,5yw, 8aw, 5bw";
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
const int CANT_OPEN_FILE=7;
const int WRONG_SIDE=8;
const int WRONG_ARGUMENTS=9;
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

typedef struct {
    bool side;
    int maxCapacity;
    int credits;
    int wins;
    int losses;
} ShipBin;

typedef struct {
    char type[3];	// tf, xw, ... y el caracter "\0"
    int velocity;
    int attack;
    int shield;
    int cost;
} FighterBin;

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
const string OPTION4 = "4- Launch squadron";
const string OPTION5 = "5- List enemy info";
const string OPTIONS1 = "s- Save data";
const string OPTIONS2 = "S- Save enemy data";
const string OPTIONL1 = "l- Load data";
const string OPTIONL2 = "L- Load enemy data";
const string OPTIONX = "x- Export data";
const string OPTIONI = "i- Import data";
const string OPTIONQ = "q- Quit";
const string OPTION = "Option: ";
const string TXT_ENTERFIGHTERTYPE = "Enter fighters: ";
const string TXT_NUMBEROFIGHTERS = "Number of fighters: ";
const string TXT_SELECTFIGHTERNUMBERS = "Select fighter numbers: ";
const string TXT_SELECTFIGHTERNUMBER = "Select fighter number: ";
const string TXT_WHATTOIMPROVE = "What to improve (v/a/s)? "; 
const string TXT_AMOUNT = "Amount: ";
const string TXT_THATWILLCOSTYOU = "That will cost you ";
const string TXT_CONFIRMCREDITS = " credits. Confirm? (y/n)";
const string TXT_FIGHTERIMPROVED = "Fighter improved: ";
const string FILENAME = "Filename: ";
const string CONFIRM_ERASE_DATA = "This operation will erase current data. Confirm? (y/n)";
const string SQUADRONA = "Squadron A";
const string SQUADRONB = "Squadron B";

const int RANDOMNUMBER = 100;
const int NUMBERFIGHTDOWN = 5;
const int NUMBERFIGHTUP = 95;
const int NUMBERFIGHT = 300;
const int NUMMARGEN = 7;

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
void launchSquadron(Ship &imperial,Ship &rebel);
void saveData(Ship &ship, bool side, const Fighter &f);
void saveEnemyData(Ship &ship, bool side, const Fighter &f);
bool loadData(Ship &ship, string ImperialData);
bool loadEnemyData(Ship &ship, string RebelData);
void exportData(Ship &ship, bool side);
bool importData(Ship &ship, string exportRebelData);
bool checkProgramArguments(int argc, char *argv[], Ship &imperialDestroyer, Ship &rebelShip);
bool introducingShips(Ship &ship, string typeFighter);//Inserta en el vector Fighters, los fighters escritos por el usuario. 
int selectedShip(string &typeOfFighter); //Devuelve la posicion que ocupa la nave elegida.
int costFighter(string &typeOfFighter); //Devuelve el coste de la nave elegida.
void pushBackFighters(Ship &ship, string &typeOfFighter, int &numberFighter); //Inserta en el vector Fighters, más fighters.
void leerFichero(Fighter &fighterFichero, string leido);
bool letra(char caracter);
bool numero(char caracter);

int main(int argc, char *argv[]){
  
   Ship imperialDestroyer, rebelShip;
   char option; 
   bool side, correctArguments;
   Fighter f;
  
   srand(1); // inicializar secuencia de números aleatorios: NO TOCAR
   
   side = IMPERIAL;
   initializeShip(imperialDestroyer, side);
   
   side = REBEL;
   initializeShip(rebelShip, side);
   
   correctArguments = checkProgramArguments(argc, argv, imperialDestroyer, rebelShip);
   
   if(correctArguments == true){
    
    do {

      muestraMenu();
      cin>>option;
      cin.get();
      
	  switch (option){
	    
	    case '1':
	      side = IMPERIAL;
	      listShip(imperialDestroyer);
	      break;
	    case '2':
	      addFighter(imperialDestroyer);
	      break;
	    case '3':
	      improveFighter(imperialDestroyer);
	      break;
	    case '4':
	      launchSquadron(imperialDestroyer,rebelShip);
	      break;
	    case '5':
	      side = REBEL;
	      listShip(rebelShip);
	      break;
	    case 's':
	      side = IMPERIAL;
	      saveData(imperialDestroyer, side, f);
	      break;
	    case 'S':
	      side = REBEL;
	      saveEnemyData(rebelShip, side, f);
	      break;
	    case 'l':
	      loadData(imperialDestroyer, "");
	      break;
	    case 'L':
	      loadEnemyData(rebelShip, "");
	      break;
	    case 'x':
	      exportData(imperialDestroyer, side);
	      break;
	    case 'i':
	      importData(imperialDestroyer, "");
	      break;
	    case 'q':
	      
	      break;
	    default: error(UNKNOWN_OPTION);
	      
	  }
	  
    }while (option != 'q');
    
  }
   
}

void muestraMenu(){
  
  cout<<OPTION1<<endl;
  cout<<OPTION2<<endl;
  cout<<OPTION3<<endl;
  cout<<OPTION4<<endl;
  cout<<OPTION5<<endl;
  cout<<OPTIONS1<<endl;
  cout<<OPTIONS2<<endl;
  cout<<OPTIONL1<<endl;
  cout<<OPTIONL2<<endl;
  cout<<OPTIONX<<endl;
  cout<<OPTIONI<<endl;
  cout<<OPTIONQ<<endl;
  cout<<OPTION;
  
}
//---------------------------------------------------
void error(int n){
  
  cout << "ERROR (" << n << "): " ;
  
  switch (n){
    
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
    case CANT_OPEN_FILE:
      cout << "can’t open file" << endl;
      break;
    case WRONG_SIDE:
      cout << "wrong binary data" << endl;
      break;
    case WRONG_ARGUMENTS:
      cout << "Wrong arguments, syntax: imperialCommander [-l file | -L file | -i file]" << endl;
      break;  
    
  }
  
}
//---------------------------------------------------
int getRandomNumber(int max){ // Para un numero aleatorio entre -3 y 3 pones getRandomNumber(7)-3
  
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
        
    introducingShips(ship, initialImperialShipDotation);
    
  }
  
  else{
    
    ship.side = REBEL;
    ship.maxCapacity = REBSHIPCAPACITY;
    ship.credits = CREDITS_INITIAL; 
    ship.wins = 0;
    ship.losses = 0;
       
    introducingShips(ship, initialRebelShipDotation);
      
  }
  
}
//---------------------------------------------------
void listFighter(const Fighter &f){ // Muestra los datos de una sola nave

  cout<< f.type << " (v=" << f.velocity << ", a=" << f.attack << ", s=" << f.shield << ", c=" << f.cost << ")";
  
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

  string typeFighter;
  bool fighterAdded;
  
  cout<< TXT_ENTERFIGHTERTYPE;
  getline(cin,typeFighter);
  
  fighterAdded = introducingShips(ship, typeFighter);
  
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

  int testFight, fightResult;
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
void launchSquadron(Ship &imperial,Ship &rebel){//Terminado
  
  int num, temp, margen, totalCazas, randomFighter, i, endFight, result, wrongNumberFighter;
  string fighterFight, numAux;
  vector<int> selectedFighters;
  vector<Fighter> rebelTemp,imperialTemp;
  Fighter imperialShip, rebelShip;
  Fighter fg1, fg2;

  
  numAux = "";
  wrongNumberFighter = 0;
  num = 0;
  i = 0;
  
  if(imperial.fighters.size() <= 0 || rebel.fighters.size() <= 0){
    error(NO_FIGHTERS);
  }
  
  else{
    cout<< TXT_SELECTFIGHTERNUMBERS;
    getline(cin, fighterFight);

   while(i < fighterFight.length()){//Mete los numeros introducidos por el usuario en un vector

      while(fighterFight[i] == ' ' && i < fighterFight.length()){
	i++;
      }
      
      numAux = "";
	
      while(fighterFight[i] >= '0' && fighterFight[i] <= '9' && i < fighterFight.length() ){
	numAux = numAux + fighterFight[i];
	i++;
      }

      if(numAux != ""){
	num = atoi(numAux.c_str());
	selectedFighters.push_back(num);
      }
      
    }
 
    for (int i = 1; i < selectedFighters.size(); i++){//Ordenamos el vector de mayor a menor
      
          for ( int j = 0 ; j < selectedFighters.size() - 1; j++){
	    
               if (selectedFighters[j] < selectedFighters[j+1]){
                    temp = selectedFighters[j];
                    selectedFighters[j] = selectedFighters[j+1];
                    selectedFighters[j+1] = temp;
	       }
	       
	  }
	  
    }
       
    for (int j = 0; j < selectedFighters.size(); j++){
      
      if(selectedFighters[j] >= 1 && selectedFighters[j] <= imperial.fighters.size()){
	imperialTemp.push_back(imperial.fighters[(selectedFighters[j] - 1)]);//Metemos los cazas en un vector de cazas auxiliar
	imperial.fighters.erase(imperial.fighters.begin() + (selectedFighters[j] - 1));//Y lo borramos del vector de cazas original
	wrongNumberFighter++;
	
      }
            
    }
    
    if(wrongNumberFighter == 0){
      error(WRONG_NUMBER);
    }
    
    else{
    
      //Creamos los cazas que se meteran en el escuadrón rebelde
      margen = getRandomNumber(NUMMARGEN);
      margen = margen - 3;
      totalCazas = imperialTemp.size() + margen;
      
      if(totalCazas <= 0){
	totalCazas = 1;
      }
      
      else if(totalCazas > rebel.fighters.size()){
	totalCazas = rebel.fighters.size();
      }
      
      for(int i = 1; i <= totalCazas; i++){//Elegimos los cazas rebeldes aleatoriamente
	randomFighter = getRandomNumber(rebel.fighters.size());
	rebelTemp.push_back(rebel.fighters[randomFighter]);
	rebel.fighters.erase(rebel.fighters.begin() + randomFighter);
      }
      
      cout << SQUADRONA << endl;
      listFighters(imperialTemp);
      cout << SQUADRONB << endl;
      listFighters(rebelTemp);
      
      do{
	  
	  endFight = getRandomNumber(RANDOMNUMBER);
	  
	  if(endFight >= 10){
	    //Seleccionamos y borramos un fighter del vector temporal de fighters y lo asignamos a una variable que será lanzada a luchar
	    randomFighter = getRandomNumber(imperialTemp.size());
	    imperialShip = imperialTemp[randomFighter];
	    imperialTemp.erase(imperialTemp.begin() + randomFighter);
	    
	    randomFighter = getRandomNumber(rebelTemp.size());
	    rebelShip = rebelTemp[randomFighter];
	    rebelTemp.erase(rebelTemp.begin() + randomFighter);
	    
	    result = fight(imperialShip, rebelShip);
	    
	    if(result == 1){//Ganan la nave rebelde
	      
	      rebelTemp.push_back(rebelShip);
	      rebel.credits = rebel.credits + imperialShip.cost;
	      rebel.wins++;
	      imperial.losses++;
	      
	    }
	    
	    else if(result == -1){//Ganan la nave imperial

	      imperialTemp.push_back(imperialShip);
	      imperial.credits = imperial.credits + rebelShip.cost;
	      imperial.wins++;
	      rebel.losses++;
	      
	    }
	    
	    else if(result == 0){//Empate
	      
	      imperialTemp.push_back(imperialShip);
	      rebelTemp.push_back(rebelShip);
	      
	    }
	    
	  }
	
      }while(endFight >= 10 && rebelTemp.size() > 0 && imperialTemp.size() > 0);
      
      cout << SQUADRONA << endl;
      listFighters(imperialTemp);
      cout << SQUADRONB << endl;
      listFighters(rebelTemp);
      
      for(int i = 0; i < imperialTemp.size(); i++){
	imperial.fighters.push_back(imperialTemp[i]);
      }
      
      for(int j = 0; j < rebelTemp.size(); j++){
	rebel.fighters.push_back(rebelTemp[j]);
      }
      
    }
    
  }
  
}
//---------------------------------------------------
void saveData(Ship &ship, bool side, const Fighter &f){//Terminado
  
  ShipBin imperialBin;
  FighterBin fighterBin;
  unsigned int tam;
  string imperialData;
  ofstream saveImperialFile;
  
  cout<<FILENAME; 
  getline(cin, imperialData);
  
  tam = imperialData.length();
  char cadena[tam];
  strcpy(cadena, imperialData.c_str());
  saveImperialFile.open(cadena,ios::out | ios::binary);
  
  if(saveImperialFile.is_open()){
    
    imperialBin.side = ship.side;
    imperialBin.maxCapacity = ship.maxCapacity;
    imperialBin.credits = ship.credits;
    imperialBin.wins = ship.wins;
    imperialBin.losses = ship.losses;
    
    saveImperialFile.write((const char *)&imperialBin, sizeof(imperialBin));
    
    for(int i = 0; i < ship.fighters.size(); i++){
      fighterBin.velocity = ship.fighters[i].velocity;
      fighterBin.attack = ship.fighters[i].attack;
      fighterBin.shield = ship.fighters[i].shield;
      fighterBin.cost = ship.fighters[i].cost;
      
      if(ship.fighters[i].type == "TIE-Fighter"){
	
	strcpy(fighterBin.type, "tf");
	
      }
      
      else if(ship.fighters[i].type == "TIE-Bomber"){
	
	strcpy(fighterBin.type, "tb");
	
      }
      
      else if(ship.fighters[i].type == "TIE-Interceptor"){
	
	strcpy(fighterBin.type, "ti");
	
      }
      
      else if(ship.fighters[i].type == "TIE-Advanced"){

	strcpy(fighterBin.type, "ta");
      
      }
      saveImperialFile.write((const char *)&fighterBin, sizeof(fighterBin));
      
    }
    
    saveImperialFile.close();
    
  }
  
  else{
    
    error(CANT_OPEN_FILE);
    
  }
  
}
//---------------------------------------------------
void saveEnemyData(Ship &ship, bool side, const Fighter &f){//Terminado
  
  ShipBin rebelBin;
  FighterBin fighterBin;
  unsigned int tam;
  string rebelData;
  ofstream saveRebelFile;
  
  cout<<FILENAME;
  getline(cin, rebelData);
  
  tam = rebelData.length();
  char cadena[tam];
  strcpy(cadena, rebelData.c_str());
  saveRebelFile.open(cadena,ios::out | ios::binary);
  
  if(saveRebelFile.is_open()){
    
    rebelBin.side = ship.side;
    rebelBin.maxCapacity = ship.maxCapacity;
    rebelBin.credits = ship.credits;
    rebelBin.wins = ship.wins;
    rebelBin.losses = ship.losses;
    
    saveRebelFile.write((const char *)&rebelBin, sizeof(rebelBin));
    
    for(int i = 0; i < ship.fighters.size(); i++){
      fighterBin.velocity = ship.fighters[i].velocity;
      fighterBin.attack = ship.fighters[i].attack;
      fighterBin.shield = ship.fighters[i].shield;
      fighterBin.cost = ship.fighters[i].cost;
      
      if(ship.fighters[i].type == "A-Wing"){
	strcpy(fighterBin.type, "aw");
      }
      
      else if(ship.fighters[i].type == "X-Wing"){
	strcpy(fighterBin.type, "xw");
      }
      
      else if(ship.fighters[i].type == "B-Wing"){
	strcpy(fighterBin.type, "bw");
      }
      
      else if(ship.fighters[i].type == "Y-Wing"){
	strcpy(fighterBin.type, "yw");
      }
      
      saveRebelFile.write((const char *) &fighterBin, sizeof(fighterBin));
    
    }
  
  saveRebelFile.close();
  
  }
  
  else{
    
    error(CANT_OPEN_FILE);
    
  }
  
}
//---------------------------------------------------
bool loadData(Ship &ship, string ImperialData){//Terminado
  
  ShipBin imperialBin;
  FighterBin fighterBin;
  Fighter fighter;
  unsigned int tam;
  bool dataLoaded;
  char answer;
  ifstream loadImperialData;
  
  dataLoaded = false;
  
  if(ImperialData == ""){
    cout<<FILENAME; 
    getline(cin, ImperialData);
  }
  
  tam = ImperialData.length();
  char cadena[tam];
  strcpy(cadena, ImperialData.c_str());
  loadImperialData.open(ImperialData.c_str(),ios::in | ios::binary);
  
  if(loadImperialData.is_open()){
    
    cout<<CONFIRM_ERASE_DATA;
    cin>>answer;
    cin.get();
    
    if(answer == 'y'){
      
      loadImperialData.read((char *)&imperialBin, sizeof(imperialBin)); //Leemos el ficher en busca del bando de la nave y lo comprobamos
      if(imperialBin.side == IMPERIAL){ // si es del bando imperial mete toda la información en su posicion correspondiente
	
	dataLoaded = true;
	ship.side = imperialBin.side;
	ship.maxCapacity = imperialBin.maxCapacity;
	ship.credits = imperialBin.credits;
	ship.losses = imperialBin.losses;
	ship.wins = imperialBin.wins;
	ship.fighters.clear();
	loadImperialData.read((char *)&fighterBin, sizeof(fighterBin));
	
	do{
	  
	  fighter.velocity = fighterBin.velocity;
	  fighter.attack = fighterBin.attack;
	  fighter.shield = fighterBin.shield;
	  fighter.cost = fighterBin.cost;
	  
	  if(strcmp(fighterBin.type, "tf")){
	    fighter.type = "TIE-Fighter";
	  }
	  
	  else if(strcmp(fighterBin.type, "tb")){
	    fighter.type = "TIE-Bomber";
	  }
	  
	  else if(strcmp(fighterBin.type, "ti")){
	    fighter.type = "TIE-Interceptor";
	  }
	  
	  else if(strcmp(fighterBin.type, "ta")){
	    fighter.type = ("TIE-Advanced");
	  }
	  
	  ship.fighters.push_back(fighter);
	  loadImperialData.read((char *)&fighterBin, sizeof(fighterBin));
	  
	}while(loadImperialData.eof() == false);
	
      }
            
    }
    
    loadImperialData.close();
    
  }
  
  else{
    
    error(CANT_OPEN_FILE);
    
  }
  
  return (dataLoaded);
  
}
//---------------------------------------------------
bool loadEnemyData(Ship &ship, string RebelData){//Terminado
  
  ShipBin rebelBin;
  FighterBin fighterBin;
  Fighter fighter;
  unsigned int tam;
  bool dataLoaded;
  char answer;
  ifstream loadRebelData;
  
  dataLoaded = false;
  
  if(RebelData == ""){
    cout<<FILENAME; 
    getline(cin, RebelData);
  }
    
  tam = RebelData.length();
  char cadena[tam];
  strcpy(cadena, RebelData.c_str());
  loadRebelData.open(RebelData.c_str(), ios::in | ios::binary);
  
  if(loadRebelData.is_open()){
    
    cout<<CONFIRM_ERASE_DATA;
    cin>>answer;
    cin.get();
    
    if(answer == 'y'){
      
      loadRebelData.read((char *)&rebelBin, sizeof(rebelBin)); //Leemos el ficher en busca del bando de la nave y lo comprobamos
      if(rebelBin.side != REBEL){ // si el bando no es rebelde salta el error
	error(WRONG_SIDE);
      }
      else{ // si es del bando rebelde mete toda la información en su posicion correspondiente
	
	dataLoaded = true;
	ship.side = rebelBin.side;
	ship.maxCapacity = rebelBin.maxCapacity;
	ship.credits = rebelBin.credits;
	ship.losses = rebelBin.losses;
	ship.wins = rebelBin.wins;
	ship.fighters.clear();
	loadRebelData.read((char *)&fighterBin, sizeof(fighterBin));
	
	do{
	  
	  fighter.velocity = fighterBin.velocity;
	  fighter.attack = fighterBin.attack;
	  fighter.shield = fighterBin.shield;
	  fighter.cost = fighterBin.cost;
	  
	  if(strcmp(fighterBin.type,"xw")){
	    fighter.type = "X-Wing";
	  }
	  
	  if(strcmp(fighterBin.type,"yw")){
	    fighter.type = "Y-Wing";
	  }
	  
	  if(strcmp(fighterBin.type,"aw")){
	    fighter.type = "A-Wing";
	  }
	  
	  if(strcmp(fighterBin.type,"bw")){
	    fighter.type = "B-Wing";
	  }
	    
	  ship.fighters.push_back(fighter);
	  loadRebelData.read((char *)&fighterBin, sizeof(fighterBin));
	  
	}while(loadRebelData.eof() == false);
	
      }
      
    }
    
    loadRebelData.close();
    
  }
  
  else{
    
    error(CANT_OPEN_FILE);
    
  }
  
  return (dataLoaded);

  
}
//---------------------------------------------------
void exportData(Ship &ship, bool side){//Terminado
  
  unsigned int tam;
  string exportImperialData;
  ofstream exportImperialFile;
  
  cout<<FILENAME; 
  getline(cin, exportImperialData);
  
  tam = exportImperialData.length();
  char cadena[tam];
  strcpy(cadena, exportImperialData.c_str());
  exportImperialFile.open(cadena,ios::out);
  
  if(exportImperialFile.is_open()){
    
    for(int i = 0; i < ship.fighters.size(); i++){
    
      exportImperialFile << ship.fighters[i].type;
      exportImperialFile << " (v=" << ship.fighters[i].velocity;
      exportImperialFile << ", a=" << ship.fighters[i].attack;
      exportImperialFile << ", s=" << ship.fighters[i].shield;
      exportImperialFile << ", c=" << ship.fighters[i].cost << ")";
      exportImperialFile << endl;
      
    }
    
    exportImperialFile.close();
    
  }
  
  else{
    
    error(CANT_OPEN_FILE);
    
  }
  
}
//---------------------------------------------------
bool importData(Ship &ship, string exportRebelData){//Terminado
  
  Fighter ff;
  int tam;
  bool importedData, capacityExceeded;
  string line;
  ifstream exportRebelFile;
  
  importedData = false;
  capacityExceeded = false;
  
  if(exportRebelData == ""){
    cout<<FILENAME; 
    getline(cin, exportRebelData);
  }
  
  tam = exportRebelData.length();
  char cadena[tam];
  strcpy(cadena, exportRebelData.c_str());
  exportRebelFile.open(exportRebelData.c_str(), ios::in);
  
  if(exportRebelFile.is_open()){
    
    while(exportRebelFile.eof() == false && capacityExceeded == false){ 
    
      importedData = true;
      leerFichero(ff, line);
      
      if(ship.fighters.size() >= ship.maxCapacity){
	
	error(CAPACITY_EXCEEDED);
	capacityExceeded = true;
	
      }
      
      else {
	
	if(ff.type != "TIE-Fighter" && ff.type != "TIE-Bomber" && ff.type != "TIE-Advanced" && ff.type != "TIE-Interceptor"){
	  error(WRONG_FIGHTER_TYPE);	
	  getline(exportRebelFile, line);
	}
	
	else {
	  
	  ship.fighters.push_back(ff);
	  getline(exportRebelFile, line);
	  
	}
      }
    }
    
    exportRebelFile.close();
   
  }
  
  else{
    
    error(CANT_OPEN_FILE);
    
  }
  
  return (importedData);
  
}
//---------------------------------------------------
bool checkProgramArguments(int argc, char *argv[], Ship &imperialDestroyer, Ship &rebelShip){//Terminado
  
  bool correctArguments;
  char checkArguments;
  string imperialDataBin, rebelDataBin, importImperialDestroyerData;
  
  correctArguments = false;
  imperialDataBin = "";
  rebelDataBin = "";
  importImperialDestroyerData = "";
  
  if(argc == 1 || argc == 3 || argc == 5 || argc == 7){
    
    correctArguments = true;
    
    for(int i = 1; i < argc && correctArguments == true; i = i+2){
            
      if(strcmp(argv[i], "-l") == 0){
	
	if(imperialDataBin == ""){
	  checkArguments = 'l';
	}
	 
	 else{
	   checkArguments = 'w';
	}
	 
      }
      
      else if(strcmp(argv[i], "-L") == 0){
	
	if(rebelDataBin == ""){
	  checkArguments = 'L';
	}
	 
	 else{
	   checkArguments = 'w';
	}
	
      }
      
      else if(strcmp(argv[i], "-i") == 0){
	
	if(importImperialDestroyerData == ""){
	  checkArguments = 'i';
	}
	 
	 else{
	   checkArguments = 'w';
	}
	
      }
      
      switch(checkArguments){
	
	case 'l':
	  imperialDataBin = argv[i+1];
	  break;
	case 'L':
	  rebelDataBin = argv[i+1];
	  break;
	case 'i':
	  importImperialDestroyerData = argv[i+1];
	  break;
	case 'w':
	  correctArguments = false;
	  break;
      }
      
    }
    
    if(correctArguments == true){
      
      if(rebelDataBin != ""){
	correctArguments = loadEnemyData(rebelShip, rebelDataBin);
      }
      
      if(importImperialDestroyerData != ""){
	correctArguments = importData(imperialDestroyer, importImperialDestroyerData);
      }
      
      if(imperialDataBin != ""){
	correctArguments = loadData(imperialDestroyer, imperialDataBin);
      }
      
    }
    
    else{
      error(WRONG_ARGUMENTS);
    }
    
  }
  
  else{
    error(WRONG_ARGUMENTS);
  }
  
  return (correctArguments);
  
}
//---------------------------------------------------
bool introducingShips(Ship &ship, string typeFighter){
  
    bool fighterIntroduced, noFunds, capacityExceeded, igual, wrongFighter;
    int numFighters, prize, selectShip;
    unsigned int tam, i;
    string numberOfFighters, typeOfFighter;
    
    fighterIntroduced = false;
    i = 0;
    igual = false;
    noFunds = false;
    wrongFighter = false;
    capacityExceeded = false;
    tam = typeFighter.length();
    
    while(i < tam && noFunds == false && capacityExceeded == false && wrongFighter == false){
      
      while(numero(typeFighter[i]) == false && i < tam){//Saltamos todo lo que no sea letra o numero
	i++;
      }
      
      numberOfFighters = "";
      
      while(numero(typeFighter[i]) == true && i < tam){//Recorremos el string en busca de numeros y los almacenamos en una variable
	numberOfFighters = numberOfFighters + typeFighter[i];
	i++;
      }
      
      while(i < tam && letra(typeFighter[i]) == false){
	i++;
      }
      
      typeOfFighter = "";
      
      while(letra(typeFighter[i]) == true && i < tam){//Recorremos el string en busca de letras y las almacenamos en una variable
	typeOfFighter = typeOfFighter + typeFighter[i];
	i++;
      }
      
	  if(ship.credits <= 0){
	    error(NO_FUNDS);
	  }
	  
	  else{    
	    
	      for(int i = 0; i < MAXFIGHTERS; i++){
	      
		  if(typeOfFighter == FIGHTERABR[i]){
		    igual = true;
		  }
		
		}
	      
	      if(igual == false){
		  error(WRONG_FIGHTER_TYPE);
		  wrongFighter = true;
	      }
	      
	      else{

			  numFighters = atoi(numberOfFighters.c_str());
			  prize = costFighter(typeOfFighter); //Devuelve el precio de la nave que hemos seleccionado
			  selectShip = selectedShip(typeOfFighter); //Convierte la opción elegida en un número
			  
			  if(numFighters*prize > ship.credits){
				    error(NO_FUNDS);
				    noFunds = true;
			  }
			      
			  else if(numFighters + ship.fighters.size() > IMPSHIPCAPACITY){
			    error(CAPACITY_EXCEEDED);
			    capacityExceeded = true;
			  }
				
			  else if(numFighters <= 0){
			  error(WRONG_NUMBER);
			  }
			  
			  else{
			  
			    for(int z = 1; z <= numFighters; z++){
			      ship.credits = ship.credits - prize;
			      ship.fighters.push_back(FIGHTERTABLE[selectShip]);
			    }
			    
			  }
		  
	      }
		
	  }
	  
  } 
    
  return (fighterIntroduced);
    
}
//---------------------------------------------------
int costFighter(string &typeOfFighter){
  
  int prize, numNave;
  
    numNave = selectedShip(typeOfFighter);
    prize = FIGHTERTABLE[numNave].cost;

    return (prize);
  
}
//---------------------------------------------------
int selectedShip(string &typeOfFighter){//Compara el string "typeFighter" con la tabla de naves y cuando encuentra una coincidencia saca el valor de la nave.
  
  int numFighter;
  
    for (int i = 0; i < MAXFIGHTERS; i++){
    
    if(typeOfFighter.compare(FIGHTERABR[i]) == 0){
      numFighter = i;
      }
      
    }

  
  return(numFighter);
  
}
//---------------------------------------------------
void pushBackFighters(Ship &ship, string &typeOfFighter, int &numberFighter){
  
  int cont, prize;
  
  cont = 0;
  
  do{
      
      for(int i = 0; i < MAXFIGHTERS; i++){
	      
	    if(typeOfFighter.compare(FIGHTERABR[i]) == 0){
	  
	      ship.fighters.push_back(FIGHTERTABLE[i]);
	      prize = costFighter(typeOfFighter);
	      ship.credits = ship.credits - prize;
	
	    }
	
	}
      
      cont++;
	
  }while(cont < numberFighter);
  
}
//---------------------------------------------------
void leerFichero(Fighter &fighterFichero, string leido){//Lee una linea del ficher de texto
  
  int i;
  string type;
  
  type = "";
  i = 0;
  
  do{
    type = type + leido[i];
    i++;
  }while(leido[i] != ' ');
  
  fighterFichero.type = type;
  type = "";
  i = i + 4; // Como entre cada parámetro que queremos almacenar de un fighter hay 4 espacios, los saltamos directamente
  
  do{
    type = type + leido[i];
    i++;
  }while(leido[i] != ',');
  
  fighterFichero.velocity = atoi(type.c_str());
  type = "";
  i = i + 4;
  
  do{
    type = type + leido[i];
    i++;
  }while(leido[i] != ',');
  
  fighterFichero.attack = atoi(type.c_str());
  type = "";
  i = i + 4;
  
  do{
    type = type + leido[i];
    i++;
  }while(leido[i] != ',');
  
  fighterFichero.shield = atoi(type.c_str());
  type = "";
  i = i + 4;
  
  do{
    type = type + leido[i];
    i++;
  }while(leido[i] != ')');
  
  fighterFichero.cost = atoi(type.c_str());
  
}
//---------------------------------------------------
bool letra(char caracter){
  
  bool es = false;
  
  if((caracter >= 'a' && caracter <= 'z') || (caracter >= 'A' && caracter <= 'Z')){
    es = true;
  }
  
  return (es);
  
}
//---------------------------------------------------
bool numero(char caracter){
  
  bool es = false;
  
  if(caracter >= '0' && caracter <= '9'){
    es = true;
  }
  
  return (es);
  
}