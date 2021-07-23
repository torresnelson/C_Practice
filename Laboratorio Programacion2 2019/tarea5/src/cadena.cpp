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
/*
  Se inserta `i' como último elemento de `cad'.
  Devuelve `cad'.
  Si es_vacia_cadena (cad) `i' se inserta como único elemento de `cad'.
 */
cadena_t insertar_al_final(info_t i, cadena_t cad){
	nodo *nuevo = new nodo;
	nuevo->dato = i;
	nuevo->siguiente = NULL;
	nuevo->anterior = cad->final;
	if (cad->final == NULL){
		assert(cad->inicio == NULL);
		cad->inicio = nuevo;
	}else{
		assert(cad->inicio != NULL);
		cad->final->siguiente = nuevo;
	}
	cad->final = nuevo;
	return cad;
}
/*
  Se inserta `i' como un nuevo elemento inmediatamente antes de `loc'.
  Devuelve `cad'.
  Precondición: localizador_en_cadena(loc, cad).
 */
cadena_t insertar_antes(info_t i, localizador_t loc, cadena_t cad){ 
	assert(localizador_en_cadena(loc, cad));
	nodo *nuevo = new nodo;
	nuevo->dato = i;
	localizador_t loc_anterior = anterior(loc, cad);
	if (!es_inicio_cadena(loc, cad)){
		nuevo->siguiente = loc;
		nuevo->anterior = loc_anterior;
		loc_anterior->siguiente = nuevo;
		loc->anterior = nuevo;
	}else{
		cad->inicio = nuevo;
		nuevo->anterior = cad->inicio;
		nuevo->siguiente = loc;
		loc->anterior = nuevo;
	}
	return cad;
}

/*
  Se inserta la cadena_t `sgm' inmediatamente después de `loc' en `cad',
  manteniendo los elementos originales y el orden relativo entre ellos.
  Devuelve `cad'.
  No se debe obtener memoria (los nodos de `sgm' pasan a ser parte de `cad').
  Al terminar, `sgm' queda vacía.
  Si es_vacia_cadena(cad) `loc' es ignorado y el segmento queda insertado.
  Precondición: es_vacia_cadena(cad) o localizador_en_cadena(loc, cad).
 */
cadena_t insertar_segmento_despues(cadena_t &sgm, localizador_t loc, cadena_t cad){
	assert(es_vacia_cadena(cad) || localizador_en_cadena(loc, cad));
	if (es_vacia_cadena(cad)){
		cad->inicio = sgm->inicio;
		cad->final = sgm->final;
	}else{
		if (!es_vacia_cadena(sgm)){
			sgm->inicio->anterior = loc;
			sgm->final->siguiente = loc->siguiente;
			if (es_final_cadena(loc, cad))
				cad->final = sgm->final;
			else
				loc->siguiente->anterior = sgm->final;

			loc->siguiente = sgm->inicio;
		}
	}
	sgm->inicio = sgm->final = NULL;
	return cad;
}
/*
  Devuelve una cadena_t con los elementos de `cad' que se encuentran entre
  `desde' y `hasta', incluidos.
  La cadena_t resultado no comparte memoria con `cad'.
  Si es_vacia_cadena(cad) devuelve la cadena_t vacia.
  Precondición: es_vacia_cadena(cad) o precede_en_cadena(desde, hasta, cad).
 */
cadena_t copiar_segmento(localizador_t desde, localizador_t hasta, cadena_t cad){
	assert(es_vacia_cadena(cad) || precede_en_cadena(desde, hasta, cad));
	cadena_t res = crear_cadena();
	if (!es_vacia_cadena(cad)){
		localizador_t loc = desde;
		while (loc != siguiente(hasta, cad)){
			//ERROR: compartiria memoria
			//info_t info = loc->dato;
			info_t info = copia_info((loc->dato));
			insertar_al_final(info, res);
			loc = siguiente(loc, cad);
		}
	}
	return res;
}
/*
  Remueve de `cad' los elementos que se encuentran  entre `desde' y `hasta',
  incluidos y libera la memoria que tenían asignada y la de sus nodos.
  Devuelve `cad'.
  Si es_vacia_cadena(cad) devuelve la cadena_t vacía.
  Precondición: es_vacia_cadena(cad) o precede_en_cadena(desde, hasta, cad).
 */
cadena_t cortar_segmento(localizador_t desde, localizador_t hasta, cadena_t cad){
	if (!es_vacia_cadena(cad)){
		assert(precede_en_cadena(desde, hasta, cad));
		localizador_t cursor = desde;
		while (es_localizador(cursor) && precede_en_cadena(cursor, hasta, cad)){
			localizador_t aux = cursor->siguiente;
			cad = remover_de_cadena(cursor, cad);
			cursor = aux;
		}
	}
	return cad;
}
/*
  Se quita el elemento al que se accede desde `loc' y se libera la memoria 
  asignada al mismo y al nodo apuntado por el localizador.
  Devuelve `cad'.
  El valor de `loc' queda indeterminado.
  Precondición: localizador_en_cadena(loc, cad).
 */
cadena_t remover_de_cadena(localizador_t &loc, cadena_t cad){
	assert(localizador_en_cadena(loc, cad));
	localizador_t ant_aux = loc->anterior;
	localizador_t sig_aux = loc->siguiente;
	if(!es_inicio_cadena(loc, cad)) {
		ant_aux->siguiente = loc->siguiente;
	} else {
		cad->inicio = sig_aux;
	}
	if(es_final_cadena(loc,cad)) {
		cad->final = ant_aux;
	}
	liberar_info(loc->dato);
	delete loc;
	if(es_localizador(sig_aux)) {
		sig_aux->anterior = ant_aux;
	}
	return cad;
}

cadena_t liberar_cadena(cadena_t cad){
	nodo *a_borrar;
	while (cad->inicio != NULL){
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
/*
  Devuelve `true' si y sólo si con `loc' se accede al último elemento de `cad'.
  Si es_vacia_cadena (cad) devuelve `false'.
 */
bool es_final_cadena(localizador_t loc, cadena_t cad){ 
	return !es_vacia_cadena(cad) && loc == cad->final;
}
/*
  Devuelve `true' si y sólo si con `loc' se accede al primer elemento de `cad'.
  Si es_vacia_cadena (cad) devuelve `false'.
 */
bool es_inicio_cadena(localizador_t loc, cadena_t cad){
	return !es_vacia_cadena(cad) && cad->inicio == loc;
}
/*
  Devuelve `true' si y sólo si con `loc' se accede a un elemento de `cad',
  (o sea, si apunta a un nodo de `cad').
  Si es_vacia_cadena (cad) devuelve `false'.
 */
bool localizador_en_cadena(localizador_t loc, cadena_t cad){
	localizador_t cursor = inicio_cadena(cad);
	while (es_localizador(cursor) && (cursor != loc))
		cursor = siguiente(cursor, cad);
	return es_localizador(cursor);
} 
/*
  Devuelve `true' si y sólo si `loc1' es igual o precede a `loc2` en `cad`.
  Si es_vacia_cadena (cad) devuelve `false'.
 */
bool precede_en_cadena(localizador_t loc1, localizador_t loc2, cadena_t cad){ 
	localizador_t cursor = loc1;
	while(es_localizador(cursor) && (cursor != loc2))
		cursor = cursor->siguiente;
	return es_localizador(cursor);
}
/*
  Devuelve el localizador_t con el que se accede al inicio de `cad`.
  Si es_vacia_cadena(cad) devuelve un localizador_t no válido.
 */
localizador_t inicio_cadena(cadena_t cad){
	localizador_t res;
 	if (es_vacia_cadena(cad)){
 		res = NULL;
 	}else{
 		res = cad->inicio;
 	}
 	return res;
 }

localizador_t final_cadena(cadena_t cad){ 
	localizador_t res;
 	if (es_vacia_cadena(cad)){
	 	assert(es_vacia_cadena(cad));
 		res = NULL;
 	}else{
 		res = cad->final;
 	}
 	return res;
 }

/*
  Devuelve el localizador_t con el que se accede al k-esimo elemento de `cad'.
  Si `k' es 0 o mayor a la cantidad de elementos de `cad' devuelve un localizdor
  no válido.
 */
localizador_t kesimo(nat k, cadena_t cad){
	if(k <= 0)
		return NULL;
	nat i = 1;
	localizador_t loc = inicio_cadena(cad);
	while (es_localizador(loc) && (i < k)){
		loc = loc->siguiente;
		i++;
	}
	return loc; 
}
/*
  Devuelve el localizador_t con el que se accede al elemento de `cad'
  inmediatamente siguiente a `loc'.
  Si es_final_cadena(loc, cad) devuelve un localizador_t no válido.
  Precondición: localizador_en_cadena(loc, cad).
 */
localizador_t siguiente(localizador_t loc, cadena_t cad){
	assert(localizador_en_cadena(loc, cad));
	return loc->siguiente;
}
/*
  Devuelve el localizador_t con el que se accede al elemento de `cad'
  inmediatamente anterior a `loc'.
  Si es_inicio_cadena(loc, cad) devuelve un localizador_t no válido.
  Precondición: localizador_en_cadena(loc, cad).
 */
localizador_t anterior(localizador_t loc, cadena_t cad){
	assert(localizador_en_cadena(loc, cad));
	if (cad->inicio == loc){
		return loc = NULL;
	}else{
		return loc = loc->anterior;
	}
}
/*
  Devuelve el localizador_t con el que se accede al elemento cuyo dato numérico
  es el menor en el segmento que va desde `loc' hasta el final_cadena(cad).
  Si hay más de un elemento cuyo valor es el menor el resultado accede al que
  precede a los otros.
  Precondición: localizador_en_cadena(loc, cad).
 */
localizador_t menor_en_cadena(localizador_t loc, cadena_t cad){
	assert(localizador_en_cadena(loc, cad));
	localizador_t res = loc;
	while (es_localizador(siguiente(loc, cad))){
		loc = siguiente(loc, cad);
		if (numero_info(info_cadena(loc, cad)) < numero_info(info_cadena(res, cad)))
			res = loc;
	}
	return res;
}
/*
  Devuelve el primer localizador_t con el que se accede a un elemento cuyo dato
  numérico es igual a `clave', buscando desde `loc' (inclusive) hacia el final
  de `cad'.
  Si no se encuentra o `cad' es vacía devuelve un localizador_t no válido.
  Precondición: es_vacia_cadena(cad) o localizador_en_cadena(loc, cad).
 */
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

/*
  Devuelve el elemento de `cad' al que se accede con `loc'.
  Precondición: localizador_en_cadena(loc, cad).
 */
info_t info_cadena(localizador_t loc, cadena_t cad){ 
	assert(localizador_en_cadena(loc, cad));
	info_t info = loc->dato;
	return info; 
}

/*
  Sustituye con `i' el elemento de `cad' al que se accede con `loc'.
  Devuelve `cad'.
  No destruye el elemento al que antes se accedía con `loc'.
  Precondición: localizador_en_cadena(loc, cad).
 */
cadena_t cambiar_en_cadena(info_t i, localizador_t loc, cadena_t cad){
	assert(localizador_en_cadena(loc, cad));
	loc->dato = i;
	return cad;
}

/*
  Intercambia los elementos a los que se accede con `loc1' y `loc2'.
  `loc1' y `loc2' mantienen su relación de precedencia.
  Devuelve `cad'.
  Precondición:
    localizador_en_cadena (loc1, cad)
  y localizador_en_cadena (loc2, cad).
 */
cadena_t intercambiar(localizador_t loc1, localizador_t loc2, cadena_t cad){
	assert(localizador_en_cadena(loc1, cad) && localizador_en_cadena(loc2, cad));
	info_t info_loc1;
	info_loc1 = loc1->dato;
	loc1->dato = loc2->dato;
	loc2->dato = info_loc1;
	return cad;
}
/*
  Imprime los elementos de `cad` de la siguiente forma:
  (dn de pos1,fr de pos1)(dn de pos2,fr de pos2) ...
  donce `dn` es el dato numérico y `fr` es la frase.
  Antes de terminar, se debe imprimir un fin de linea.
  Si es_vacia_cadena(cad) sólo se imprime el fin de línea.
 */
void imprimir_cadena(cadena_t cad){
	localizador_t cursor = cad->inicio;
	while (es_localizador(cursor)){
		char *infostr = info_a_texto(cursor->dato);
		printf("%s", infostr);
		cursor = siguiente(cursor, cad);
		delete[] infostr;
	}
	if (!es_localizador(cursor)){
		printf("\n");
	}
}