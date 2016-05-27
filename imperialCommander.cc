// 74379726A PASCUAL ANTOLINOS, DIEGO
#include <iostream>
#include <cstdlib>

using namespace std;

#include "Util.h"
#include "Ship.h"

void menu()
{
  cout << "1- List ship info" << endl
       << "2- Add fighters" << endl
       << "3- Repair/improve fighter" << endl
       << "4- Launch squadron" << endl
       << "5- List enemy info" << endl
       << "q- Quit" << endl
       << "Option: " ;
}



int main()
{
   Ship imperialDestroyer(IMPERIAL), rebelShip(REBEL);
   char option;

   srand(1); // inicializar secuencia de números aleatorios: NO TOCAR

   do {
        menu();
        cin >> option; cin.get();
        
        switch (option) {
          case '1': cout << imperialDestroyer;
            break;
          case '2':
            { 
              string dotation;
  
              cout << "Enter fighters: " ;    
              getline(cin,dotation);
              imperialDestroyer.addFighters(dotation);
            }
            break;
          case '3': imperialDestroyer.improveFighter();
            break;
          case '4': imperialDestroyer.fight(rebelShip);
            break;
          case '5': cout << rebelShip;
            break;

          case 'q':
            break;
          default: Util::error(UNKNOWN_OPTION);
                  
        }
   } while (option != 'q');
}
