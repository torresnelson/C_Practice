#include <stdio.h>
#include <assert.h>
#include "../include/binario.h"
#include "../include/cola_avls.h"
// Representación de `cola_avls_t'.
// Se debe definir en cola_avls.cpp
struct nodo {
	avl_t dato;
	nodo *siguiente;
};

struct rep_cola_avls{
	nodo *primero;
	nodo *ultimo;
};


/*  Devuelve una cola_avls_t vacía (sin elementos). */
cola_avls_t crear_cola_avls() {
	cola_avls_t res = new rep_cola_avls;
	res->primero = res->ultimo = NULL;
	return res;
}

/* Encola `avl' en `c'. */
void encolar(avl_t b, cola_avls_t &c){
	nodo *nuevo = new nodo;
	nuevo->dato = b;
	nuevo->siguiente = NULL;
	if (es_vacia_cola_avls(c)) 
		c->primero = nuevo;
	else 
		c->ultimo->siguiente = nuevo;
	c->ultimo = nuevo;
}

/*
	Remueve de `c' el elemento que está en el frente.
	NO libera la memoria del elemento removido.
	Si es_vacia_cola_binarios(c) no hace nada.
 */
void desencolar(cola_avls_t &c) {
	assert(c->primero != NULL);
	nodo *temp = c->primero;
	c->primero = c->primero->siguiente;
	if (c->primero == NULL) 
		c->ultimo = NULL;
	delete temp;
}
/* Libera la memoria asignada a `c', pero NO la de sus elementos. */
 
void liberar_cola_avls(cola_avls_t &c) {
	nodo *temp;
	while (c->primero != NULL){
		temp = c->primero;
		c->primero = c->primero->siguiente;
		delete temp;
	}delete c;
}

/* Devuelve `true' si y sólo si `c' es vacía (no tiene elementos). */
bool es_vacia_cola_avls(cola_avls_t c) {
	return c->primero == NULL;
}

/*
	Devuelve el elemento que está en el frente de `c'.
	Precondición: ! es_vacia_cola_binarios(c);
 */
avl_t frente(cola_avls_t c) {
	assert(c->primero != NULL);
	return c->primero->dato;
}