#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../include/utils.h"
#include "../include/info.h"
#include "../include/iterador.h"

typedef struct nodoIt* PNodoIt;

struct nodoIt {
	nat elem;
	PNodoIt sig;
};

struct repIterador {
	PNodoIt inicio, fin, actual;
	bool bloqueado;
};

TIterador crearIterador() { 
	TIterador res = new repIterador;
	res->actual = res->inicio = res->fin = NULL;
	res->bloqueado = false;
	return res;
}

TIterador agregarAIterador(nat elem, TIterador iter) {
	if (!iter->bloqueado) {
		PNodoIt nuevo;
		nuevo = new nodoIt;
		nuevo->elem = elem;
		nuevo->sig = NULL;
		if (iter->fin != NULL)
			iter->fin->sig = nuevo;
		else
			iter->inicio = nuevo;
		iter->fin = nuevo;
	}
	return iter;
}

TIterador reiniciarIterador(TIterador iter){
	if (iter->inicio != NULL){	
		iter->actual = iter->inicio;
	}
	return iter;
}

TIterador avanzarIterador(TIterador iter){
	if (estaDefinidaActual(iter)){
		if(iter->actual == iter->fin)
			iter->actual = NULL;
		else
			iter->actual = iter->actual->sig;
	}
	return iter;
}

nat actualEnIterador(TIterador iter){
	assert(estaDefinidaActual(iter));
	return iter->actual->elem;
}

bool estaDefinidaActual(TIterador iter) { return iter->actual != NULL; }

void liberarIterador(TIterador iter){
	if (iter != NULL){
		iter->actual = iter->inicio;
		while (iter->actual != NULL) {
			PNodoIt aBorrar = iter->actual;
			iter->actual = iter->actual->sig;
			delete aBorrar;
		}		
		delete iter;
	}
}