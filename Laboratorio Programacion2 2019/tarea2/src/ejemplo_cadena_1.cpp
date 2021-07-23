#include "../include/cadena.h"
#include "../include/info.h"

#include "stddef.h"
#include "stdio.h"
#include "assert.h"

struct nodo{
	info_t dato;
	nodo *anterior;
	nodo *siguiente;
};

struct rep_cadena{
	nodo *inicio;
	nodo *final;
	
};

cadena_t crear_cadena(){
	cadena_t res = new rep_cadena;
	res->inicio = res->final = NULL;
	return res;
}

cadena_t insertar_al_final(info_t i, cadena_t cad){
	nodo *nuevo = new nodo;
	nuevo->dato = i;
	nuevo-> siguiente = NULL;
	nuevo->anterior = cad->final;
	if (cad->inicio == NULL){
		assert(cad->inicio == NULL);
		cad->inicio = nuevo;
	}else{
		assert(cad->inicio != NULL);
		cad->final->siguiente = nuevo;
	}
	cad->final = nuevo;
	return cad;
}

cadena_t insertar_segmento_despues(cadena_t &sgm, localizador_t loc, 
								   cadena_t cad){
	assert(es_vacia_cadena(cad) || localizador_en_cadena(loc, cad));
	if (es_vacia_cadena(cad)){
		cad->inicio = sgm->inicio;
		cad->final = sgm-> final;
	}else{
		if (!es_vacia_cadena(sgm)){
			sgm->inicio->anterior = loc;
			sgm->final->siguiente == loc->siguiente;
			if (es_final(loc, cad))
				cad->final = sgm->final;
			else
				loc->siguiente->anterior = sgm->final;

			loc->siguiente = sgm->inicio;
		}
	}
	sgm->inicio = sgm->final = NULL;
	return cad;
}

cadena_t liberar_cadena(cadena_t cad){
	nodo *a_borrar;
	while (cad->final != NULL){
		a_borrar = cad->inicio;
		cad->inicio = cad->inicio->siguiente;
		liberar_info(a_borrar->dato);
		delete (a_borrar);
	}
	delete cad;
	return cad;
}

bool es_localizador(localizador_t loc){ return loc != NULL; }

bool es_vacia_cadena(cadena_t cad){
	return (cad->inicio == NULL) && (cad->final == NULL);
}

localizador_t inicio_cadena(cadena_t cad){
	localizador_t res;
	if (es_vacia_cadena(cad)){
		res = NULL;
	}else{
		res = cad->inicio;
	}
	return res;
}

bool localizador_en_cadena(localizador_t loc, cadena_t cad){
	localizador_t cursor = inicio_cadena(cad);
	while (es_localizador(cursor) && (cursor != loc))
		cursor = siguiente(cursor, cad);
	return es_localizador(cursor);
}

localizador_t menor_en_cadena(localizador_t loc, cadena_t cad){
	assert(localizador_en_cadena(loc, cad));
	localizador_t res = loc;
	while (es_localizador(siguiente, cad)){
		loc = siguiente(loc, cad);
		if (numero_info(info_cadena(loc, cad)) < numero_info(info_cadena(res, cad)))
			res = loc;
	}
	return res;
}

localizador_t siguiente_clave(int clave, localizador_t loc, cadena_t cad){
	assert(es_vacia_cadena(cad) || localizador_en_cadena(loc, cad));
	localizador_t res = loc;
	if (es_vacia_cadena(cad))
		res = NULL;
	else{
		while (es_localizador(res) && numero_info(info_cadena(res, cad)) != clave)
			res = siguiente(res, cad);
	}
	return res;
}

cadena_t cambiar_en_cadena(info_t i, localizador_t loc, cadena_t cad){
	assert(localizador_en_cadena(loc, cad));
	loc->dato = i;
	return cad;
}
