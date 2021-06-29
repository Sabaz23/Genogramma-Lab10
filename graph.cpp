#include "graph.h"

using namespace graph;

/*******************************************************************************************************/
// Struct
/*******************************************************************************************************/

// Struttura che contiene il puntatore alla persona, la relazione, e la prossima relazione
struct halfEdgeNode {
  vertexNode *vertPtr;
  Relation rel; // relazione dell'arco
  halfEdgeNode* next; // puntatore al mezzo arco successivo
};

/* Un vertice è caratterizzato dalle informazioni sulla Persona,
il puntatore alla lista delle relazioni, il puntatore al prossimo vertice
e un booleano che indica se il nodo è già stato visitato (utile per la stampa)*/
struct graph::vertexNode {
    Label label;
    char sex;
    Label birth;
    Label death;
    halfEdgeNode *adjList;
    vertexNode *next;
    bool visited;
};

/*******************************************************************************************************/
// Const
/*******************************************************************************************************/

halfEdgeNode* const emptyHalfEdgeNode = NULL;

/*******************************************************************************************************/
// Funzioni ausiliarie
/*******************************************************************************************************/

// Ritorna il puntatore al vertice avente label "l" (se esiste)
vertexNode* getVertex(Label l, const Graph& g) {
  for (graph::Graph v = g; v != emptyGraph; v = v->next) {
   if (v->label == l) return  v; // trovato, esco
  }
  return emptyGraph; // non trovato
}

//Ritorna il numero di vertici (persone) presenti nel genogramma
int getNumVertex(const Graph& g)
{
  vertexNode* v = g;
  int cont=0;
  for(v;v!=emptyGraph;v = v->next) cont++;
  return cont;
}

// Ritorna true se il vertice e' presente nel grafo
bool isVertexInGraph(Label l, const Graph& g) {
  return (getVertex(l, g)!=emptyGraph);
}

vertexNode* getYoungestDesc(vertexNode* v, Graph& g)
{
  halfEdgeNode* n = v->adjList;
  while(n!=emptyHalfEdgeNode){  //Finchè non ho controllato tutte le relazioni
    if(n->rel!='C') //Se il nodo che controllo è madre o padre
    {
      return getYoungestDesc(n->vertPtr,g); //Richiamo la funzione con il figlio e controllo lui
    }
    n=n->next;
  }
  if(n == emptyHalfEdgeNode) return v;  //Se arrivo alla fine senza aver trovato una relazione madre o padre, ritorno il nodo (il più giovane della discendenza)
}

//Ritorna true se le due persone hanno già la relazione r, false altrimenti
bool gotRelAlready(Label from, Label to, Relation r, const Graph& g)
{
  vertexNode* v = getVertex(from,g);
  vertexNode* v2 = getVertex(to,g);
  if(v==emptyGraph || v2==emptyGraph) return true;  //Ritorna true come se avessero già la relazione, per evitare di andare avanti
  halfEdgeNode* n = v->adjList;
  for(n;n != emptyHalfEdgeNode; n = n->next)  //Scorre tutte le relazioni di una delle due persone
    if(n->vertPtr == v2 && n->rel == r) return true;  //Se il nome coincide con la seconda persona e la relazione è quella che stiamo cercando, ritorna true
  return false; //Altrimenti arriva in fondo e ritorna false
}

//Ritora true se la persona è già in una coppia
bool isCoupled(Label name, const Graph& g)
{
  vertexNode* v = getVertex(name,g);
  if(v==emptyGraph) return true;
  halfEdgeNode* n = v->adjList;
  for(n;n != emptyHalfEdgeNode; n = n->next)
    if(n->rel == 'C') return true;
  return false;
}

//Ritorna true se due persone sono in coppia tra di loro
bool areCoupled(Label name, Label name2, const Graph&g)
{
  vertexNode* v1 = getVertex(name,g);
  vertexNode* v2 = getVertex(name2,g);
  if(v1==emptyGraph || v2==emptyGraph) return true;
  halfEdgeNode* n = v1->adjList;
  for(n;n != emptyHalfEdgeNode; n = n->next)
    if(n->vertPtr->label == name2 && n->rel == 'C') return true;
  return false;
}

//Ritorna true se una persona è già figlia di un altra
bool isSonAlready(Label son, Label parent, const Graph& g)
{
  vertexNode* sonV = getVertex(son, g);
  vertexNode* parentV = getVertex(parent, g);
  if(sonV == emptyGraph || parentV == emptyGraph) return true;
  halfEdgeNode* parentN = parentV->adjList;
  for(parentN;parentN != emptyHalfEdgeNode; parentN = parentN->next)  //Scorro le relazioni del genitore
    if((parentN->rel == 'F' || parentN->rel == 'M') && parentN->vertPtr == sonV)  return true;  //Se nelle relazioni è presente "Father" o "Mother" con la persona che sto controllando, riotrno true
  return false;
}

//Elimina le relazioni della persona data in tutto il grafo
void deleteRelations(Label name, Graph &g)
{
  vertexNode* vlabel = getVertex(name,g);
  if(vlabel == emptyGraph) return;
  halfEdgeNode* n = g->adjList; //n sono le relazioni della testa
  vertexNode* v = g; // v punta alla testa
  halfEdgeNode* tmp = new halfEdgeNode; //Nodo Temporaneo con relazioni vuoto
  bool first = true;
  bool eliminato = false;

  for(v;v != emptyGraph; v = v->next) //scorre tutte le persone
  {
    n = v->adjList;
    first = true;

    while(n != emptyHalfEdgeNode)  //Scorre tutte le relazioni
      {

        if(first) //Se è la prima relazione
        {
          if(v->adjList == emptyHalfEdgeNode) break; //Se la adjlist è vuota, esco
          if(v->adjList->vertPtr->label == name)  //Se il collegamento vertice-relazioni è quello che devo eliminare (la prima relazione)
          {
            tmp=v->adjList;
            v->adjList = tmp->next;
            tmp->next = emptyHalfEdgeNode;
            delete tmp;
            continue; //Ritorno all'inizio del ciclo (n non deve incrementare perchè salterebbe il controllo dell'elemento a cui ora punta)
          }
          else //Se non è da eliminare la prima relazione
            {
              first = false;  //Pongo first a false in modo da saltare questi controlli
              n = v->adjList; //N diventa la prima relazione
              continue; //Ricomincio il ciclo
            }
          }

          if(n->next != NULL) //Se non sono all'ultimo elemento
          {
                if(n->next->vertPtr->label == name) //Controllo l'elemento successivo e in caso elimino
                  {
                    tmp=n->next;
                    n->next = tmp->next;
                    tmp->next = emptyHalfEdgeNode;
                    delete tmp;
                    eliminato = true; //Pongo eliminato a true in modo da ripetere il ciclo senza incrementare n (salterebbe un elemento)
                  }
          }

          if(!eliminato)  n=n->next;  //Se non ho eliminato nulla posso continuare
          else eliminato = false; //Altrimenti ripeto il ciclo senza incrementare (così controllo il nuovo elemento a cui è collegato n)

          if(n==NULL) break;  //Se n è NULL, non devo ripetere il while
        }
 }
}

bool deleteVertex(Label name, Graph& g)
{

  vertexNode* toDel = getVertex(name,g);
  if(toDel == emptyGraph) return false;
  vertexNode* v = g; // v punta alla testa
  vertexNode* tmp = new vertexNode; //Nodo Temporaneo con relazioni vuoto

  if(v == toDel) //Se il vertice da eliminare è il primo
  {
    tmp=v;
    g = tmp->next;  //Pongo il grafo al successivo elemento rispetto a quello che devo eliminare
    tmp->next = emptyGraph;
    delete tmp;
    return true;
  }
  else  //Se non è il primo
  {
    while(v != emptyGraph)  //Scorro tutto il grafo
    {
      if(v->next == toDel)  //Finchè trovo quello da eliminare
      {
        tmp=v->next;
        v->next = tmp->next;
        tmp->next = emptyGraph;
        delete tmp;
        return true;
      }
      v = v->next;
    }
  }
  return false;
}

void deleteDescendants(Label name, Graph& g)
{

  vertexNode* toDel = getVertex(name,g);

  if(toDel == emptyGraph) return;

  halfEdgeNode* n = toDel->adjList;
  halfEdgeNode* aux = new halfEdgeNode;

  while(n != emptyHalfEdgeNode) //Scorre tutte le relazioni del nodo da eliminare
  {
    if(n->rel == 'F' || n->rel == 'M')  //Se sono discendenti
    {
      aux = n->next; //Salva l'indirizzo del prossimo nodo
      Label tmpName;
      vertexNode* tmpV = getVertex(tmpName,g);
      do{
        tmpV = getYoungestDesc(toDel,g);
        tmpName = tmpV->label; //Salva il nome della persona della discendenza piu giovane
        deleteRelations(tmpName,g); //Qui elimina le relazioni della persona piu giovane
        deleteVertex(tmpName, g); //E qui elimina il vertice della persona più giovane
      }while(tmpV != toDel);  //Continua ad eliminare fino a che il puntatore temporaneo è uguale al puntatore alla persona da eliminare
      n = aux; //Se lo elimina torna l'indirizzo del prossimo nodo, se non ha eliminato comunque procede
    }
    else  n = n->next; //Altrimenti se non elimina nulla e scorre normalmente
  }
}

//Funzione che verifica se i padri sono maschi e le madri femmine
bool goodParents(const Graph& g)
{
  vertexNode* v = g;
  halfEdgeNode* h = new halfEdgeNode;
  for(v;v != emptyGraph; v = v->next)
  {
    h = v->adjList;
    for(h; h != emptyHalfEdgeNode; h = h->next)
    {
      if(h->rel == v->sex) return false;  //Se la relazione (M)other è uguale al sesso (M)aschio o Se la relazione (F)ather è uguale al sesso (F)emmina
    }
  }
  return true;
}

//Funzione che torna utile per convertire la data in un intero, per i confronti
int parseDate(const Label date) {
    int month,day,year;
    day = stoi(date.substr(0,2));
    month = stoi(date.substr(3,2));
    year = stoi(date.substr(6,4));
    return 10000 * year + 100 * month + day;
}

//Funzione che verifica se le dati di morte e nascita sono valide
bool goodDate(const Graph& g)
{
  vertexNode* v = g;
  for(v;v != emptyGraph; v = v->next)
  {
    if(v->death != "-") //Se esiste una data di morte
    {
      if((parseDate(v->death))<(parseDate(v->birth))) return false; //Se la data di morte è più piccola di quella di nascita, ritorna false!
    }
  }
  return true;
}

//Verifica se la data di nascita di un padre o madre è più giovane di quella della prole (su tutti gli elementi del grafo)
bool goodDescend(const Graph& g)
{
  vertexNode* v = g;
  halfEdgeNode* h = new halfEdgeNode;
  for(v;v != emptyGraph; v = v->next)
  {
    h = v->adjList;
    for(h; h != emptyHalfEdgeNode; h = h->next)
    {
      if(h->rel == 'M' || h->rel == 'F')  //Se è padre o madre
      {
        if(parseDate(v->birth) > parseDate(h->vertPtr->birth)) return false;  //Verifica se la loro data di nascita è precedente a quella dei figli
      }
    }
  }
  return true;
}

//Verifica se esiste una data persona tra le relazioni
bool exists(Label name, const Graph &g)
{
  vertexNode* v = g;
  halfEdgeNode* h = new halfEdgeNode;
  for(v;v != emptyGraph; v = v->next)
  {
    h = v->adjList;
    for(h;h != emptyHalfEdgeNode; h = h->next)
    {
      if(h->vertPtr->label == name) return true;
    }
  }
  return false;
}

//Verifica che il grafo non sia sconnesso
bool goodConnections(const Graph &g)
{
  vertexNode* v = g;
  halfEdgeNode* h = new halfEdgeNode;
  for(v;v != emptyGraph; v = v->next)
  {
    if(!exists(v->label,g)) return false; //Se non sono presenti relazioni con il nome della persona tra tutte le persone, vuol dire che il nodo non è collegato a nulla (di conseguenza, il grafo è sconnesso)
  }
  return true;
}






// Stampa il genogramma
void printAdjList(Label l, const Graph& g) {
 vertexNode* vNode = getVertex(l, g);
 halfEdgeNode* n = new halfEdgeNode;
 if (vNode==emptyGraph) return;
 cout<<"---------------------------"<<endl;
 cout<<"Nome: "<<vNode->label<<endl;
 cout<<"Sesso: "<<vNode->sex<<endl;
 if(vNode->sex=='F')
 {
 	cout<<"Nata in data: "<<vNode->birth<<endl;
 	//if(vNode->death!='-') cout<<"Morta in data: "<<vNode->death<<endl;
 	cout<<"Morta in data: "<<vNode->death<<endl;
 	cout<<"Madre di: ";
 	for (n = vNode->adjList; n!= emptyHalfEdgeNode; n = n->next)  //Da cambiare, si bugga con genitori di sesso sbagliato
 	{
 		if(n->rel != 'C') cout<<n->vertPtr->label<<" ";
 		else cout<<"\nIn coppia con: "<<n->vertPtr->label<<endl;
 	}
 	cout<<endl;
 }
 else
 {
 	cout<<"Nato in data: "<<vNode->birth<<endl;
 	//if(vNode->death!='-') cout<<"Morto in data: "<<vNode->death<<endl;
 	cout<<"Morto in data: "<<vNode->death<<endl;
 	cout<<"Padre di: ";
 	for (n = vNode->adjList; n!=emptyHalfEdgeNode; n = n->next)
 	{
 		if(n->rel != 'C') cout<<n->vertPtr->label<<" ";
 		else cout<<"\nIn coppia con: "<<n->vertPtr->label<<endl;
  	}
  	cout << endl;
 }
}

// Ritorna il numero di "mezzi edge"
int numHalfEdges(halfEdgeNode *head) {
  int num = 0;
  for (halfEdgeNode* n = head; n != emptyHalfEdgeNode; n = n->next) {
    num++;
  }
  return num;
}

vertexNode* findOldestNotVisited(const Graph &g)
{
  vertexNode* v = g;
  while(v->visited)
  	v=v->next;
  vertexNode* max = v;
  v = g;
  for(v;v!=emptyGraph;v = v->next)
  {

    if(parseDate(v->birth)<parseDate(max->birth) && !v->visited) max = v;
    //cout<<" ho visitanto:"<<v->label<<" "<<v->birth<<" "<<v->visited<<"max è:"<<max->birth<<endl;
  }
  max->visited = true;
  return max;
}

void resetVisited(const Graph& g)
{
  vertexNode* v = g;
  for(v;v != emptyGraph; v = v->next) v->visited = false;
}

/*******************************************************************************************************/
// Grafo
/*******************************************************************************************************/

// Restituisce il grafo vuoto
Graph graph::createEmptyGraph()
{
  return emptyGraph;
}

//Fallisce se gia' presente
bool graph::addPerson(Label n, char s, Label d, Label d2, Graph& g) {
  if (isVertexInGraph(n, g))
    return false;  // etichetta gia' presente
  // aggiungi nuovo vertice (in testa per comodita')
  Graph v = new vertexNode;
  v->label = n;
  v->sex = s;
  v->birth = d;
  v->death = d2;
  v->adjList = emptyHalfEdgeNode;
  v->visited = false;
  if (isEmpty(g)) {
    g = v;
    v->next = emptyGraph;
  } else {
    v->next = g;
    g = v;
  }
  return true;
}



bool graph::addRelMother(Label from,Label to, Graph& g)
{

  if(gotRelAlready(from,to,'M',g)) return false;


  halfEdgeNode *e = new halfEdgeNode;
  e->vertPtr = getVertex(to,g);
  e->rel = 'M';

  vertexNode* v = getVertex(from, g);

  if(v->adjList == emptyHalfEdgeNode)
  {
    v->adjList = e;
    e->next = emptyHalfEdgeNode;
  }
  else
  {
    e->next = v->adjList;
    v->adjList = e;
  }
  return true;
}

bool graph::addRelFather(Label from,Label to, Graph& g)
{
  if(gotRelAlready(from,to,'F',g)) return false;
  halfEdgeNode *e = new halfEdgeNode;
  e->vertPtr = getVertex(to,g);
  e->rel = 'F';

  vertexNode* v = getVertex(from, g);
  if(v->adjList == emptyHalfEdgeNode)
  {
    v->adjList = e;
    e->next = emptyHalfEdgeNode;
  }
  else
  {
    e->next = v->adjList;
    v->adjList = e;
  }
  return true;
}

bool graph::addRelCouple(Label from,Label to, Graph&g)
{
  if(isCoupled(from,g) || isCoupled(to,g)) return false;

  halfEdgeNode *e = new halfEdgeNode;
  halfEdgeNode *e2 = new halfEdgeNode;
  e->vertPtr = getVertex(to,g);
  e2->vertPtr = getVertex(from,g);
  e->rel = 'C';
  e2->rel = 'C';

  vertexNode* v = getVertex(from, g);
  vertexNode* v2 = getVertex(to, g);
  if(v->adjList == emptyHalfEdgeNode)
  {
    v->adjList = e;
    e->next = emptyHalfEdgeNode;
  }
  else
  {
    e->next = v->adjList;
    v->adjList = e;
  }

  if(v2->adjList == emptyHalfEdgeNode)
  {
    v2->adjList = e2;
    e2->next = emptyHalfEdgeNode;
  }
  else
  {
    e2->next = v2->adjList;
    v2->adjList = e2;
  }
  return true;
}

bool graph::addRelChildToCouple(Label child, Label father, Label mother, Graph& g)
{
  if(isSonAlready(child,father,g) || isSonAlready(child,mother,g)) return false;
  if(!areCoupled(father,mother,g)) return false;

  addRelMother(mother,child,g);
  addRelFather(father,child,g);

  return true;
}

void graph::deletePerson(Label name, Graph& g)
{
  vertexNode* v = getVertex(name,g);
  if(v == emptyGraph) return;
  deleteDescendants(name,g); //Elimina le relazioni dei discendenti (e i vertici dei discendenti)
  deleteRelations(name,g); //Infine elimina le relazioni dove c'è phil
  deleteVertex(name,g); //Ed elimina Phil

}

Label graph::isValid(Graph& g)
{
  //Per controllare che sia valido:
  //1.La madre di una persona deve essere femmina e il padre maschio
  if(goodParents(g))
  {
    //2.La data di nascita di tutte le persone deve essere antecedente a quella di Morte
    if(goodDate(g))
    {
      //3.La data di nascita di una persona deve essere precedente a quella di tutta la sua discendenza
      if(goodDescend(g))
      {
        //4.Il grafo non deve essere sconnesso
        if(goodConnections(g))
        {
          return "Il grafo e' valido!";
        }
        else
        {
          return "Grafo non valido! Il grafo e' sconnesso!";
        }
      }
      else
      {
        return "Grafo non valido a causa di un genitore più giovane della sua discendenza!";
      }
    }
    else
    {
      return "Grafo non valido a cause di date di nascita e morte non corrette!";
    }
  }
  else
  {
    return "Grafo non valido a causa dei parenti con genere non corretto!";
  }



}

void graph::setBirthDate(Label name, Label date, Graph& g)
{
  if(!isVertexInGraph(name,g)) return;
  vertexNode* v = getVertex(name, g);
  v->birth = date;
}

void graph::setDeathDate(Label name, Label date, Graph& g)
{
  if(!isVertexInGraph(name,g)) return;
  vertexNode* v = getVertex(name, g);
  v->death = date;
}

// Restituisce true se il grafo e' vuoto, false altrimenti
bool graph::isEmpty(const Graph& g)
{
   return (g==emptyGraph);
}


/*******************************************************************************************************/
// Stampa il grafo
void printGraph(const graph::Graph& g) {
  graph::Graph v = g;
  int numVertex=getNumVertex(g);
  for (int i = 0; i < numVertex; i++) {
    vertexNode* tmp = findOldestNotVisited(g);
    //cout << tmp->label << ": ";
    printAdjList(tmp->label, g);
  }
  resetVisited(g);
}
