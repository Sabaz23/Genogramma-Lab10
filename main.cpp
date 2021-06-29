#include <cstdlib>
#include <iostream>

////////////////////////////////////////////////////////////////////////
// COMPILAZIONE:
// g++ -Wall *cpp
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "graph.h"

using namespace std;
using namespace graph;

// Visualizza menu' con le scelte possibili e chiede
// all'utente di immetterne una.
// Continua a ripresentare il menu' finche' l'utente non
// immette una scelta valida. Ritorna la scelta fatta.
int menu()
{
  int scelta = -1;
  while (true)
  {
    cout << "--------------------------------------------------------------------------\n";
    cout << "                                  MENU                                    \n";
    cout << "--------------------------------------------------------------------------\n";
    cout << "1. Inserimento del genogramma da file\n";
    cout << "2. Inserimento persona nel genogramma\n";
    cout << "3. Inserimento della relazione madre\n";
    cout << "4. Inserimento della relazione padre\n";
    cout << "5. Inserimento della relazione coppia\n";
    cout << "6. Inserimento della relazione figlio ad una coppia\n";
    cout << "7. Imposta data di nascita\n";
    cout << "8. Imposta data di morte\n";
    cout << "9. Cancella persona\n";
    cout << "10. Controlla validita' genogramma'\n";
    cout << "11. Stampa genogramma'\n";
    cout << "0. Uscita\n";
    cout << "\nImmetti la scelta e premi ENTER ---> ";
    cin >> scelta;
    cin.ignore();
    cout << "--------------------------------------------------------------------------\n";
    if (cin.fail() || scelta < 0 || scelta > 11) {
        cout << "\nScelta non valida\n";
        cin.clear();
        cin.ignore();
        clearerr(stdin);
        continue;
    }
    return scelta;
  }
}


int main() {

  graph::Graph g = graph::createEmptyGraph();

  string nomefile;
  int scelta = 0;


  //Ciclo su scelta,
  // si interrompe quando l'utente sceglie di uscire (scelta==0)
  while (true) {

    // Visualizzazione menu e acquisizione scelta
    scelta = menu();

    // Se l'utente ha scelto "0" vuole uscire dal programma
    if (scelta == 0) break;

    graph::Label name, name2, birth, death;
    char r,sex;
    graph::Relation rel;

    switch (scelta)
    {

      // Lettura del genogramma da file (grafo)
      case 1: {
        g = graph::createEmptyGraph(); // serve nel caso in cui il grafo g e' gia' stato inserito (ri-inserimento), anche se così facendo si ha un memory leak
        cout << "Inserisci il nome del file\n";
        cin >> nomefile;
        ifstream ifs(nomefile.c_str()); // apertura di uno stream associato ad un file, in lettura
        if (!ifs) {cout << "\nErrore apertura file, verifica di avere inserito un nome corretto\n";}
        else {
         for(;;) {
          ifs >> r;
          if (r == 'P') //Se l'inizio è P allora la riga deve aggiungere una persona
          {
            ifs >> name;
            ifs >> sex;
            ifs >> birth;
            ifs >> death;
            graph::addPerson(name,sex,birth,death, g);
          }
          else if (r == 'R')  //Se l'inizio è R allora deve aggiungere una relazione
          {
            ifs >> name;
            ifs >> rel;
            ifs >> name2;
            if(rel == 'M') graph::addRelMother(name,name2,g);
            if(rel == 'F') graph::addRelFather(name,name2,g);
            if(rel == 'C') graph::addRelCouple(name,name2,g);
          }
          ifs.peek(); //Reso necessario per l'ultima riga
          if(ifs.eof()) break;
         }
        }
       }
      break;
      //Aggiunta di una persona
      case 2:
      {
        graph::Label name, birth, death;
        char sex;
        cout << "Inserisci il nome, il sesso (M/F), la data di nascita (DD/MM/YYYY) e di morte (se non esiste inserire -) della persona:" << endl;
        cin >> name;
        cin >> sex;
        cin >> birth;
        cin >> death;
        graph::addPerson(name,sex,birth,death, g);
      }
        break;
      //Aggiunta di relazione madre
      case 3:
        {
          Label n1,n2;
          cout << "Inserisci la madre:" << endl;
          cin >> n1;
          cout << "Inserisci il figlio:" << endl;
          cin >> n2;
          graph::addRelMother(n1,n2,g);
        }
        break;
      //Aggiunta di relazione padre
      case 4:
        {
          Label n1,n2;
          cout << "Inserisci il padre:" << endl;
          cin >> n1;
          cout << "Inserisci il figlio:" << endl;
          cin >> n2;
          graph::addRelFather(n1,n2,g);
        }
          break;
      //Aggiunta di relazione coppia
      case 5:
      {
        Label n1,n2;
        cout << "Inserisci la prima persona:" << endl;
        cin >> n1;
        cout << "Inserisci la seconda:" << endl;
        cin >> n2;
        graph::addRelCouple(n1,n2,g);
      }
      break;
      //Aggiunta di figlio ad una coppia
      case 6:
      {
        Label n1,n2,n3;
        cout << "Inserisci il nome del figlio" << endl;
        cin >> n1;
        cout << "Inserisci il nome del padre" << endl;
        cin >> n2;
        cout << "Inserisci il nome della madre" << endl;
        cin >> n3;
        graph::addRelChildToCouple(n1,n2,n3,g);
      }
      break;
      //Aggiornamento data di nascita
      case 7:
      {
        Label name, date;
        cout << "Inserisci la persona e la data di nascita (DD/MM/YYYY):" << endl;
        cin >> name;
        cin >> date;
        graph::setBirthDate(name,date,g);
      }
      break;
      //Aggiornamento data di morte
      case 8:
      {
        Label name, date;
        cout << "Inserisci la persona e la data di morte (DD/MM/YYYY):" << endl;
        cin >> name;
        cin >> date;
        graph::setDeathDate(name,date,g);
      }
      break;
      //Eliminazione persona
      case 9:
      {
        Label n;
        cout << "Inserisci la persona da eliminare (attenzione: eliminerai anche i discendenti)" << endl;
        cin >> n;
        graph::deletePerson(n,g);
      }
      break;
      //Controllo grafo valido
      case 10:
      {
        cout << graph::isValid(g) << endl;
      }
      break;
      // Visualizzazione del genogramma (grafo)
      case 11:
        cout << "\n\nIl genogramma e' il seguente:\n";
        printGraph(g);
      break;
    }
  }
  return 0;
}
