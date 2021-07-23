#include "../include/pila.h"
#include <assert.h>
#include <cstddef>


// Representación de `pila_t'.
// Se debe definir en pila.cpp.
struct rep_pila {
	int *datos;
	int tope;
	int cota;
};

/*
  Devuelve una pila_t vacía (sin elementos) que puede tener hasta `tamanio'
  elementos.
 */
pila_t crear_pila(int tamanio) {
	pila_t p = new rep_pila;
	p->cota = tamanio;
	p->datos =  new int [tamanio];
	p->tope = 0;
	return p;
}
/*
	Devuelve `true' si y sólo si la cantidad de elementos en `p' es `tamanio'
	(siendo `tamanio' el valor del parámetro con que fue creada `p').
 */
bool es_llena_pila(pila_t p) { return p->cota == p->tope; }

/* Devuelve `true' si y sólo si `p' es vacía (no tiene elementos). */

bool es_vacia_pila(pila_t p) { return p->tope == 0; }

/* Libera la memoria asignada a `p'. */
void liberar_pila(pila_t &p) { 
	delete[] p->datos;
	delete p; 
};
                                                                               
/*
  Apila `num' en `p'.
  Si es_llena_pila(p) no hace nada.
 */
void apilar(int num, pila_t &p) {
	if (!es_llena_pila(p)) {
		p->datos[p->tope] = num;
		p->tope++;
	}
}
/*
	Remueve de `p' el elemento que está en la cima.
	Si es_vacia_pila(p) no hace nada.
 */
void desapilar(pila_t &p) {
	if (!es_vacia_pila(p)) {
  		p->tope--;
  	}
}

/*
	Devuelve el elemento que está en la cima de `p'.
	Precondición: ! es_vacia_pila(p);
 */

int cima(pila_t p){
  assert(!es_vacia_pila(p));
  return p->datos[p->tope - 1];
}