#include <cstddef>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <string>


using namespace std;

namespace graph {


typedef string Label;
typedef char Relation;

struct vertexNode; // definita nel file graph.cpp

typedef vertexNode* Graph; // un grafo è identificato dal puntatore al primo vertice inserito

const Graph emptyGraph = NULL;

// createEmptyGraph restituisce il grafo vuoto
Graph createEmptyGraph();

// Aggiunge nuovo vertice con etichetta la stringa. Fallisce se gia' presente
bool addPerson(Label, char, Label, Label, Graph&);

// Restituisce true se il grafo e' vuoto, false altrimenti
bool isEmpty(const Graph&);

// Restituisce true se l'aggiunta relazione padre è andata a buon fine, false altrimenti
bool addRelFather(Label,Label, Graph&);

// Restituisce true se l'aggiunta relazione madre è andata a buon fine, false altrimenti
bool addRelMother(Label,Label, Graph&);

// Restituisce true se l'aggiunta relazione coppia è andata a buon fine, false altrimenti
bool addRelCouple(Label,Label,Graph&);

// Restituisce true se l'aggiunta relazione figlio ad una coppia è andata a buon fine, false altrimenti
bool addRelChildToCouple(Label, Label, Label, Graph&);

// Elimina una persona
void deletePerson(Label, Graph&);

// Imposta una data di nascita
void setBirthDate(Label, Label, Graph&);

// Imposta una data di morte
void setDeathDate(Label, Label, Graph&);

// Restituisce la stringa con il motivo di grafo non valido/la stringa con scritto che è valido
Label isValid(Graph &);

}


/* Funzioni che non caratterizzano il TDD Graph, ma che servono per input/output */
void printGraph(const graph::Graph&);
