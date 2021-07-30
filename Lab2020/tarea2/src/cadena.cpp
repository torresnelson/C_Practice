#include "../include/cadena.h"
#include "../include/utils.h"
#include "../include/info.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct nodo {
	TInfo dato;
	TLocalizador anterior;
	TLocalizador siguiente;
};

struct repCadena {
	TLocalizador inicio;
	TLocalizador final;
};

bool esLocalizador(TLocalizador loc) { return loc != NULL; }

TCadena crearCadena(){
	TCadena res = new repCadena;
	res->inicio = res->final = NULL;
	return res;
}

void liberarCadena(TCadena cad){
	nodo *a_borrar;
	while (cad->inicio != NULL){
		a_borrar = cad->inicio;
		cad->inicio = cad->inicio->siguiente;
		liberarInfo(a_borrar->dato);
		delete (a_borrar);
	}
	delete cad;
}

bool esVaciaCadena(TCadena cad){
	return  ((cad->inicio == NULL) && (cad->final == NULL));
}

TLocalizador inicioCadena(TCadena cad){
	TLocalizador res;
	if (esVaciaCadena(cad)) {
		res = NULL;
	} else {
		res = cad->inicio;
	}
	return res;
}

TLocalizador finalCadena(TCadena cad) {
	TLocalizador res;
	if (esVaciaCadena(cad)) {
		res = NULL;
	} else {
		res = cad->final;
	}
	return res;
}

TInfo infoCadena(TLocalizador loc, TCadena cad){
	assert(localizadorEnCadena(loc, cad));
	TInfo res = loc->dato;
	return res;
}

TLocalizador siguiente(TLocalizador loc, TCadena cad){
	assert(localizadorEnCadena(loc, cad));
	TLocalizador res;
	if (esFinalCadena(loc, cad)) {
		res = NULL;
	} else {
		res = loc->siguiente;
	}
	return res;
}

TLocalizador anterior(TLocalizador loc, TCadena cad){
	assert(localizadorEnCadena(loc, cad));
	TLocalizador res;
	if (esInicioCadena(loc, cad)) {
		res = NULL;
	} else {
		res = loc->anterior;
	}
	return res;
}

bool esFinalCadena(TLocalizador loc, TCadena cad){
	return !esVaciaCadena(cad) && cad->final == loc;
}

bool esInicioCadena(TLocalizador loc, TCadena cad){
return !esVaciaCadena(cad) && cad->inicio == loc;
}

TCadena insertarAlFinal(TInfo i, TCadena cad){
	nodo *nuevo = new nodo;
	nuevo->dato = i;
	nuevo->siguiente = NULL;
	nuevo->anterior = cad->final;
	if (cad->final == NULL){
		if (cad->inicio == NULL){
		assert(cad->inicio == NULL);
		cad->inicio = nuevo;
		}
	}else {
		if (cad->inicio != NULL){
		assert(cad->inicio != NULL);
		cad->final->siguiente = nuevo;
		}
	}
	cad->final = nuevo;
	return cad;
}

TCadena insertarAntes(TInfo i, TLocalizador loc, TCadena cad){ 
	assert(localizadorEnCadena(loc, cad));
	nodo *nuevo = new nodo;
	nuevo->dato = i;
	TLocalizador loc_anterior = anterior(loc, cad);
	if (!esInicioCadena(loc, cad)){
		nuevo->siguiente = loc;
		nuevo->anterior = loc_anterior;
		loc_anterior->siguiente = nuevo;
		loc->anterior = nuevo;
	} else {
		cad->inicio = nuevo;
		nuevo->anterior = cad->inicio;
		nuevo->siguiente = loc;
		loc->anterior = nuevo;
	}
	return cad;
}

TCadena removerDeCadena(TLocalizador loc, TCadena cad){
	assert(localizadorEnCadena(loc, cad));
	TLocalizador ant_aux = loc->anterior;
	TLocalizador sig_aux = loc->siguiente;
	if(!esInicioCadena(loc, cad)) {
		ant_aux->siguiente = loc->siguiente;
	} else {
		cad->inicio = sig_aux;
	}
	if(esFinalCadena(loc,cad)) {
		cad->final = ant_aux;
	}
	liberarInfo(loc->dato);
	delete loc;
	if(esLocalizador(sig_aux)) {
		sig_aux->anterior = ant_aux;
	}
	return cad;
}

void imprimirCadena(TCadena cad){
	TLocalizador cursor = cad->inicio;
	while (esLocalizador(cursor)){
		char *infostr = infoATexto(cursor->dato);
		printf("%s", infostr);
		cursor = siguiente(cursor, cad);
		delete[] infostr;
	}
	if (!esLocalizador(cursor)){
		printf("\n");
	}
}

TLocalizador kesimo(nat k, TCadena cad){
	if(k <= 0)
		return NULL;
	nat i = 1;
	TLocalizador loc = inicioCadena(cad);
	while (esLocalizador(loc) && (i < k)){
		loc = loc->siguiente;
		i++;
	}
	return loc; 
}

bool localizadorEnCadena(TLocalizador loc, TCadena cad){
	TLocalizador cursor = inicioCadena(cad);
	while (esLocalizador(cursor) && (cursor != loc)) 
		cursor = siguiente(cursor, cad);
	return esLocalizador(cursor);
}

bool precedeEnCadena(TLocalizador loc1, TLocalizador loc2, TCadena cad){
	bool res;
	res = localizadorEnCadena(loc1, cad);
	if (res) {
		TLocalizador cursor = loc1;
		while (esLocalizador(cursor) && (cursor != loc2))
			cursor = siguiente(cursor, cad);
		res = esLocalizador(cursor);
		assert(!res || localizadorEnCadena(loc2, cad));
	}
	return res;
}

TCadena insertarSegmentoDespues(TCadena sgm, TLocalizador loc, TCadena cad){
	assert(esVaciaCadena(cad) || localizadorEnCadena(loc, cad));
	if (esVaciaCadena(cad)){
		cad->inicio = sgm->inicio;
		cad->final = sgm->final;
	} else {
		if (!esVaciaCadena(sgm)){
			sgm->inicio->anterior = loc;
			sgm->final->siguiente = loc->siguiente;
			if (esFinalCadena(loc, cad))
				cad->final = sgm->final;
			else
				loc->siguiente->anterior = sgm->final;
			loc->siguiente = sgm->inicio;
		}
	}
	sgm->inicio = sgm->final = NULL;
	liberarCadena(sgm);
	return cad;
}

TCadena copiarSegmento(TLocalizador desde, TLocalizador hasta, TCadena cad){
	assert(esVaciaCadena(cad) || precedeEnCadena(desde, hasta, cad));
	TCadena res = crearCadena();
	if (!esVaciaCadena(cad)){
		TLocalizador loc = desde;
		while (loc != siguiente(hasta, cad)){
			//ERROR: compartiria memoria
			//info_t info = loc->dato;
			TInfo info = copiaInfo((loc->dato));
			insertarAlFinal(info, res);
			loc = siguiente(loc, cad);
		}
	}
	return res;
}

TCadena borrarSegmento(TLocalizador desde, TLocalizador hasta, TCadena cad){
	if (!esVaciaCadena(cad)){ 
		assert(precedeEnCadena(desde, hasta, cad));
		TLocalizador cursor = desde;
		while (esLocalizador(cursor) && precedeEnCadena(cursor, hasta, cad)){
			TLocalizador aux = cursor->siguiente;
			cad = removerDeCadena(cursor, cad);
			cursor = aux;
		}
	}
	return cad;
}

TCadena cambiarEnCadena(TInfo i, TLocalizador loc, TCadena cad){
	assert(localizadorEnCadena(loc, cad));
	loc->dato = i;
	return cad;
}

TCadena intercambiar(TLocalizador loc1, TLocalizador loc2, TCadena cad){
	assert(localizadorEnCadena(loc1, cad) && localizadorEnCadena(loc2, cad));
	TInfo info_loc1;
	info_loc1 = loc1->dato;
	loc1->dato = loc2->dato;
	loc2->dato = info_loc1;
	return cad;
}

TLocalizador siguienteClave(nat clave, TLocalizador loc, TCadena cad){
	assert(esVaciaCadena(cad) || localizadorEnCadena(loc, cad));
	TLocalizador res = loc;
	if (esVaciaCadena(cad))
		res = NULL;
	else{
		while (esLocalizador(res) && natInfo(infoCadena(res, cad)) != clave)
			res = siguiente(res, cad);
	}
	return res;
}

TLocalizador anteriorClave(nat clave, TLocalizador loc, TCadena cad){
	assert(esVaciaCadena(cad) || localizadorEnCadena(loc, cad));
	TLocalizador res = loc;
	if (esVaciaCadena(cad))
		res = NULL;
	else{
		while (esLocalizador(res) && natInfo(infoCadena(res, cad)) != clave)
			res = anterior(res, cad);
	}
	return res;
}

TLocalizador menorEnCadena(TLocalizador loc, TCadena cad){
	assert(localizadorEnCadena(loc, cad));
	TLocalizador res = loc;
	while (esLocalizador(siguiente(loc, cad))){
		loc = siguiente(loc, cad);
		if (natInfo(infoCadena(loc, cad)) < natInfo(infoCadena(res, cad)))
			res = loc;
	}
	return res;
}