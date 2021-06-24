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
// Questa si rende necessaria a causa di un bug di glibc, prima o poi sparira'
        clearerr(stdin);
        continue;
    }
    return scelta;
  }
}


int main() {

  graph::Graph g = graph::createEmptyGraph();
  list::List adjlst, path;

  string nomefile;
  int scelta = 0;
  int len; // lunghezza cammino tra due citta'


  //Ciclo su scelta,
  // si interrompe quando l'utente sceglie di uscire (scelta==0)

  while (true) {

    // Visualizzazione menu e acquisizione scelta
    scelta = menu();

    // Se l'utente ha scelto "0" vuole uscire dal programma
    if (scelta == 0) break;

    graph::Label name, name2, sex, birth, death;
    char r;
    graph::Relation rel;

    switch (scelta)
    {

      // Lettura della mappa da file (grafo)
      case 1: {
        g = graph::createEmptyGraph(); // serve nel caso in cui il grafo g e' gia' stato inserito (ri-inserimento)
                                       // occorrerebbe fare anche un destroy, cosi' si ha memory leak!
        cout << "Inserisci il nome del file\n";
        cin >> nomefile;
        ifstream ifs(nomefile.c_str()); // apertura di uno stream associato ad un file, in lettura
        if (!ifs) {cout << "\nErrore apertura file, verifica di avere inserito un nome corretto\n";}
        else {
         for(;;) {
          ifs >> r;
          if (r == 'P')
          {
            ifs >> name;
            ifs >> sex;
            ifs >> birth;
            ifs >> death;
            //cout << "Questa e' una persona : " << " Nome: " << name << " sesso: " << sex <<  " Nascita: " << birth << " Morte: " << death << endl;
            graph::addPerson(name,sex,birth,death, g);
          }
          else if (r == 'R')
          {
            ifs >> name;
            ifs >> rel;
            ifs >> name2;
            //cout << name << " " << rel << " " << name2 << endl;
            if(rel == 'M') graph::addRelMother(name,name2,g);
            if(rel == 'F') graph::addRelFather(name,name2,g);
            if(rel == 'C') graph::addRelCouple(name,name2,g);
            //cout << "Questa e' una relazione " << " Persona1: " << name << " Persona2: " << name2 << " Relazione: " << rel << endl;
          }
          ifs.peek();
          if(ifs.eof()) break;
         }
        }
       }
      break;

      case 2:
      {
        graph::Label name, sex, birth, death;
        cout << "Inserisci il nome, il sesso (M/F), la data di nascita (DD/MM/YYYY) e di morte (se non esiste inserire -) della persona:" << endl;
        cin >> name;
        cin >> sex;
        cin >> birth;
        cin >> death;
        graph::addPerson(name,sex,birth,death, g);
      }
        break;
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

      case 9:
      {
        Label n;
        cout << "Inserisci la persona da eliminare (attenzione: eliminerai anche i discendenti)" << endl;
        cin >> n;
        graph::deletePerson(n,g);
      }
      break;
      // Visualizzazione della mappa (grafo)
      case 11:
        cout << "\n\nLa mappa e' cosi' strutturata:\n";
        printGraph(g);
      break;
/*
      // Inserimento di nuova citta' (vertice)
      case 4:
        cout << "\nInserisci etichetta della citta' da inserire\n";
        cin >> s1;
        if (!graph::addPerson(s1, g))
	    cout << "\nCitta' gia' presente nel grafo, nodo non inserito\n";
      break;

      // Inserimento di una nuova strada tra due citta'
      case 5:
        while (true) {
          cout << "\nInserisci prima citta'\n";
          cin >> s1;
          cout << "\nInserisci seconda citta'\n";
          cin >> s2;
          cout << "\nInserisci la distanza tra le due citta'\n";
          cin >> n;
          if (cin.good()) break;
          cout << "\nDati non validi, riprovare\n";
          cin.clear();
          cin.ignore();
// Questa si rende necessaria a causa di un bug di glibc, prima o poi sparira'
          clearerr(stdin);
          continue;
        }
        if (!graph::addEdge(s1, s2, n, g))
	   cout << "\nCitta' non presenti nella mappa o strada tra le due citta' gia' presente nella mappa, strada non inserita\n";
      break;

      // Determinazione del numero di citta'
      case 6:
        cout << "\nNumero di citta': " << graph::numVertices(g) << "\n";
      break;

      // Determinazione del numero di strade nella mappa
      case 7:
        cout << "\nNumero delle strade nella mappa: " << graph::numEdges(g) << "\n";
      break;

      // Determinazione del grado (ovvero strade che portano a) di una citta'
      case 8:
        cout << "\nInserisci citta' di cui vuoi calcolare il grado\n";
        cin >> s1;
        if (graph::nodeDegree(s1,degree,g))
  	  cout << "\nGrado della citta': " << degree << "\n";
        else
	  cout << "\nCitta' non presente nella mappa\n";
      break;

      // Verifica dell'adiacenza tra due citta'
      case 9:
        cout << "\nInserisci prima citta'\n";
        cin >> s1;
        cout << "\nInserisci seconda citta'\n";
        cin >> s2;
        if (graph::areAdjacent(s1,s2,g))
          cout << "\nLe citta' sono adiacenti\n";
        else
          cout << "\nLe citta' NON sono adiacenti\n";
      break;

      // Stampa citta' adiacenti
      case 10:
        cout << "\nInserisci etichetta della citta'\n";
        cin >> s1;
        adjlst = graph::adjacentList(s1, g);
        if (list::isEmpty(adjlst))
           cout << "\nLa lista di adiacenza e' vuota (motivo: l'etichetta e' quella di una citta' senza strade incidenti oppure non e' presente nel grafo)\n";
        else {
           cout << "\nLe citta' adiacenti ad " << s1 << " sono";
           printList(adjlst);
        }
      break;

      // Calcolo di un cammino tra due citta'
      case 11:
        len = 0;
        path = list::createEmpty(); // reinizializzo il cammino
        cout << "\nInserisci citta' di partenza\n";
        cin >> s1;
        cout << "\nInserisci citta' di arrivo\n";
        cin >> s2;
        graph::findPath(s1,s2,path,len,g);
        if (len == 0)
           cout << "Nessun cammino trovato tra " << s1 << " e " << s2 << endl;
        else {
                cout << "Trovato un cammino lungo km " << len << endl;
                cout << s1 << " :: ";
                printList(path);
              }
       break;
       */
    }

  }

  return 0;
}
