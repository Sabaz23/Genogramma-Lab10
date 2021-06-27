#include "graph.h"

using namespace graph;

/*******************************************************************************************************/
// Struct
/*******************************************************************************************************/

// Mezzo arco, non tiene il nodo sorgente
struct halfEdgeNode {
  vertexNode *vertPtr;
  Relation rel; // relazione dell'arco
  halfEdgeNode* next; // puntatore al mezzo arco successivo
};

// Un vertice è caratterizzato dall'etichetta, il puntatore alla lista dei vertici adiacenti, il puntatore al prossimo vertice e un booleano che indica se il nodo è già stato visitato (serve per implementare il cammino tra due città
struct graph::vertexNode {
    Label label;
    char sex;
    Label birth;
    Label death;
    halfEdgeNode *adjList;
    vertexNode *next;
    bool visited;      // marcatura se nodo visitato
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

int getNumVertex(const Graph& g)
{
  vertexNode* v = g;
  int cont=0;
  for(v;v!=emptyGraph;v = v->next) cont++;
  cout<<cont<<endl;
  return cont;
}
// Ritorna true se il vertice e' presente nel grafo

bool isVertexInGraph(Label l, const Graph& g) {
  return (getVertex(l, g)!=emptyGraph);
}



// Ritorna true se l'arco e' presente nel grafo
bool isEdgeInGraph(Label from, Label to, const Graph& g) {
 vertexNode* vNode = getVertex(from, g);
 if (vNode == emptyGraph) return false;
 for (halfEdgeNode* n = vNode->adjList; n != emptyHalfEdgeNode; n = n->next) {
    if (n->vertPtr->label == to ) return true;
  }
 return false;
}

bool gotRelAlready(Label from, Label to, Relation r, const Graph& g)
{
  vertexNode* v = getVertex(from,g);
  vertexNode* v2 = getVertex(to,g);
  if(v==emptyGraph || v2==emptyGraph) return true;
  halfEdgeNode* n = v->adjList;
  for(n;n != emptyHalfEdgeNode; n = n->next)
    if(n->vertPtr == v2 && n->rel == r) return true;
  return false;
}

bool isCoupled(Label name, const Graph& g)
{
  vertexNode* v = getVertex(name,g);
  if(v==emptyGraph) return true;
  halfEdgeNode* n = v->adjList;
  for(n;n != emptyHalfEdgeNode; n = n->next)
    if(n->rel == 'C') return true;
  return false;
}

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


bool isSonAlready(Label son, Label parent, const Graph& g)
{
  vertexNode* sonV = getVertex(son, g);
  vertexNode* parentV = getVertex(parent, g);
  if(sonV == emptyGraph || parentV == emptyGraph) return true;
  halfEdgeNode* parentN = parentV->adjList;
  for(parentN;parentN != emptyHalfEdgeNode; parentN = parentN->next)
    if((parentN->rel == 'F' || parentN->rel == 'M') && parentN->vertPtr == sonV)  return true;
  return false;
}


void deleteRelations(Label name, Graph &g)
{
  cout << "Name vale " << name << endl;
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
      Label tmpName = n->vertPtr->label; //Salva il nome della persona da eliminare
      deleteRelations(tmpName,g); //Qui elimina le relazioni
      deleteVertex(tmpName, g); //Elimina i figli di phil
      n = aux; //Se lo elimina torna l'indirizzo del prossimo nodo, se non ha eliminato comunque procede
    }
    else  n = n->next; //Altrimenti se non elimina nulla e scorre normalmente
  }
}


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

int parseDate(const Label date) { //Torna utile per convertire la data in un intero, per i confronti
    int month,day,year;
    day = stoi(date.substr(0,2));
    month = stoi(date.substr(3,2));
    year = stoi(date.substr(6,4));
    return 10000 * year + 100 * month + day;
}

bool goodDate(const Graph& g)
{
  vertexNode* v = g;
  for(v;v != emptyGraph; v = v->next)
  {
    if(v->death != "-")
    {
      if((parseDate(v->death))<(parseDate(v->birth))) return false; //Se la data di morte è più piccola di quella di nascita, ritorna false!
    }
  }
  return true;
}

bool goodDescend(const Graph& g)
{
  vertexNode* v = g;
  halfEdgeNode* h = new halfEdgeNode;
  for(v;v != emptyGraph; v = v->next)
  {
    h = v->adjList;
    for(h; h != emptyHalfEdgeNode; h = h->next)
    {
      if(h->rel == 'M' || h->rel == 'F')
      {
        if(parseDate(v->birth) > parseDate(h->vertPtr->birth)) return false;
      }
    }
  }
  return true;
}

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

bool goodConnections(const Graph &g)
{
  vertexNode* v = g;
  halfEdgeNode* h = new halfEdgeNode;
  for(v;v != emptyGraph; v = v->next)
  {
    if(!exists(v->label,g)) return false;
  }
  return true;
}



// Aggiunge il "mezzo edge" alla lista di adiacenza
// Da usare solo se i vertici "from" e "to" sono presenti nel grafo
void addHalfEdge(Label from, Label to, Relation w, Graph& g) {
  halfEdgeNode *e = new halfEdgeNode;
  e->vertPtr = getVertex(to, g);
  e->rel = w;

  vertexNode* vNode = getVertex(from, g);

  if (vNode->adjList == emptyHalfEdgeNode) {
    vNode->adjList = e;
    e->next = emptyHalfEdgeNode;
  } else {
    e->next = vNode->adjList;
    vNode->adjList = e;
  }
}






// Stampa la lista di adiacenza
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
 	for (n = vNode->adjList; n!= emptyHalfEdgeNode; n = n->next)
 	{
 		if(n->rel=='M') cout<<n->vertPtr->label<<" ";
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
 		if(n->rel=='F') cout<<n->vertPtr->label<<" ";
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

bool findPathRec(vertexNode *here, vertexNode *to, list::List &path, int &len, const Graph& g) {

    // si suppone che "here" sia diverso da "to"
    // (controllo eseguito dal chiamante)

    // marca nodo corrente come visitato
    here->visited = true;

    // esamino ciascuno dei nodi adiacenti...
    for (halfEdgeNode* ee = here->adjList; ee != emptyHalfEdgeNode; ee = ee->next) {

        // se gia' visitato passo oltre
        if (ee->vertPtr->visited)
            continue;

        // se e' la destinazione aggiorno il cammino con l'ultima tratta,
        // aggiorno la lunghezza totale, e torno al chiamante
        if (ee->vertPtr->label == to->label) {
            list::addFront(ee->vertPtr->label,path);
            len += ee->rel;
            return true;
        }

        // provo a inoltrare il cammino verso quel nodo (chiamata ricorsiva)
        bool res = findPathRec(ee->vertPtr, to, path, len, g);

        // se ce l'ha fatta, aggiungo "here" al cammino, aggiorno la
        // lunghezza totale, e ritorno al chiamante
        if (res) {
            list::addFront(ee->vertPtr->label,path);
            len += ee->rel;
            return true;
        }

        // se invece non ce l'ha fatta,
        // passo al prossimo nodo adiacente e provo da li'
    }

    // evidentemente nessuno dei nodi adiacenti mi ha portato a destinazione.
    // Segnalo al chiamante che da "here" non si va alla destinazione
    return false;
}

vertexNode* findOldestNotVisited(const Graph &g)
{
  vertexNode* v = g;
  while(v->visited==1)
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

// Aggiunge un arco di peso "w" tra i nodi con etichetta "f" e "t"
/*bool graph::addEdge(Label from, Label to, Relation w, Graph& g) {
  // non permetto arco tra un nodo ed esso stesso
  if (from == to)
    return false;
  // entrambi i nodi devono gia' esistere nel grafo
  if (!isVertexInGraph(from, g) || !isVertexInGraph(to, g))
    return false;
  // tra i due nodi non deve gia' esserci una relazione
  if (gotRelAlready(from,w,g) || gotRelAlready(to,w,g))
    return false;
  // tutto ok, procediamo
  addHalfEdge(from, to, w, g);
  return true;
}*/

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
    if(goodDate(g))
    {
      if(goodDescend(g))
      {
        if(goodConnections(g))
        {
          return "Il grafo è valido!";
        }
        else
        {
          return "Grafo non valido! Il grafo è sconnesso!";
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
  //2.La data di nascita di tutte le persone deve essere antecedente a quella di Morte
  //3.La data di nascita di una persona deve essere precedente a quella di tutta la sua discendenza
  //4.Il grafo non deve essere sconnesso
}


// Restituisce true se il grafo e' vuoto, false altrimenti
bool graph::isEmpty(const Graph& g)
{
   return (g==emptyGraph);
}

// Ritorna il numero di vertici del grafo
int graph::numVertices(const Graph& g){
 int num = 0;
 for (Graph v = g; v != emptyGraph; v = v->next) {
   num++;
 }
 return num;
}

// Ritorna il numero di archi del grafo
int graph::numEdges(const Graph& g){
 int num = 0;
 for (Graph v = g; v != emptyGraph; v = v->next) {
   num = num + numHalfEdges(v->adjList);
 }
 return (int)num/2; // sono sempre pari per costruzione!
}

// Calcola e ritorna (nel secondo parametro) il grado del nodo. Fallisce
// se il nodo non esiste
bool graph::nodeDegree(Label l, int& degree, const Graph& g) {
  if (!isVertexInGraph(l,g)) return false;
  vertexNode* vNode = getVertex(l, g);
  degree = numHalfEdges(vNode->adjList);
  return true;
}

// Verifica se i due vertici v1 e v2 sono adiacenti (ovvero se esiste un arco)
bool graph::areAdjacent(Label v1, Label v2, const Graph& g) {
  return (isEdgeInGraph(v1, v2, g));
}

// Restituisce la lista di adiacenza di un vertice
list::List graph::adjacentList(Label v1, const Graph& g) {
  list::List lst = list::createEmpty();
  vertexNode* vNode = getVertex(v1, g);
  if (vNode==emptyGraph) return lst;
  for (halfEdgeNode* n = vNode->adjList; n != emptyHalfEdgeNode; n = n->next) {
    list::addFront(n->vertPtr->label,lst);
  }
  return lst;
}

// Ritorna cammino tra v1 e v2
// il cammino ammino da "v1" a "v2" alla fine sara' in "path"
// e la lunghezza sara' in "len".
// Si assume che il chiamante fornisca inizialmente un cammino vuoto.
//
// La funzione rappresenta una variante della visita DFS

void graph::findPath(Label v1, Label v2, list::List &path, int &len, const Graph& g) {

   vertexNode* from = getVertex(v1, g);
   vertexNode* to = getVertex(v2, g);

    //se partenza e arrivo concidono, oppure partenza e/o arrivo non esistono,
    // allora cammino rimane vuoto e si finisce qui
    if (from == to || from == emptyGraph || to == emptyGraph)
        return;

    // preliminari: marco tutti i nodi/vertici come non visitati
    for (graph::Graph v = g; v != emptyGraph; v = v->next) {
          v->visited = false;
    }

    // inizia la ricorsione
    len = 0;
    findPathRec(from, to, path, len, g);

    return;
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
}
