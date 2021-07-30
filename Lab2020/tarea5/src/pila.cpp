#include "../include/pila.h"
#include <assert.h>
#include <stdio.h>

struct repPila {
	nat *datos;
	nat tope;
	nat cota;
};

TPila crearPila(nat tamanio) {
	assert(tamanio > 0);
	TPila p = new repPila;
	p->cota = tamanio;
	p->datos =  new nat[tamanio];
	p->tope = 0;
	return p;
}

bool estaLlenaPila(TPila p) { return p->cota == p->tope; }

bool estaVaciaPila(TPila p) { return p->tope == 0; }

void liberarPila(TPila p) { 
	delete[] p->datos;
	delete p; 
};

TPila apilar(nat num, TPila p) {
	if (!estaLlenaPila(p)) {
		p->datos[p->tope] = num;
		p->tope++;
	}
	return p;
}

TPila desapilar(TPila p) {
	if (!estaVaciaPila(p)) {
  		p->tope--;
  	}
  	return p;
}

nat cima(TPila p) {
  assert(!estaVaciaPila(p));
  return p->datos[p->tope - 1];
}