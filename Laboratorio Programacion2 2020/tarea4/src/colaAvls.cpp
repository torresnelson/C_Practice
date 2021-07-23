#include "../include/binario.h"
#include "../include/colaAvls.h"
#include <assert.h>
#include <stdio.h>

struct nodo {
	TAvl dato;
	nodo *siguiente;
};

struct repColaAvls{
	nodo *primero;
	nodo *ultimo;
};

TColaAvls crearColaAvls() {
	TColaAvls res = new repColaAvls;
	res->primero = res->ultimo = NULL;
	return res;
}

TColaAvls encolar(TAvl avl, TColaAvls c) {
	nodo *nuevo = new nodo;
	nuevo->dato = avl;
	nuevo->siguiente = NULL;
	if (estaVaciaColaAvls(c)) 
		c->primero = nuevo;
	else 
		c->ultimo->siguiente = nuevo;
	c->ultimo = nuevo;
	return c;
}

TColaAvls desencolar(TColaAvls c)  {
	assert(c->primero != NULL);
	nodo *temp = c->primero;
	c->primero = c->primero->siguiente;
	if (c->primero == NULL) 
		c->ultimo = NULL;
	delete temp;
	return c;
}

void liberarColaAvls(TColaAvls c) {
	nodo *temp;
	while (c->primero != NULL){
		temp = c->primero;
		c->primero = c->primero->siguiente;
		delete temp;
	}
	delete c;
}

bool estaVaciaColaAvls(TColaAvls c) {
	return c->primero == NULL;
}

TAvl frente(TColaAvls c){ 
	assert(!estaVaciaColaAvls(c));
	return c->primero->dato;
}