
#include "../include/hash.h"
#include <stdio.h>
#include <assert.h>

//Lista de textos en conflicto
struct rep_nodo{
	char *texto;
	int clave;
	struct rep_nodo *siguiente;
};
typedef rep_nodo *nodo_lista_texto;

// Representación de `hash_t'.
// Se debe definir en hash.cpp.
struct rep_hash{
	nodo_lista_texto *texto;	
	nat  capacidad;
	nat tamanio_act;
};
// Declaración del tipo `hash_t'.

nat formula_hash(int clave, hash_t h){
	nat res = clave%h->capacidad; 
	return res;
}

nodo_lista_texto crear_nodo(int clave, char *valor){
	nodo_lista_texto res = new rep_nodo;
	res->texto = valor;
	res->clave = clave;
	res->siguiente = NULL;
	return res;
}

void liberar_nodo(nodo_lista_texto &n){
	delete[] n->texto;
	delete n;
}

/*
 Crea un hash_t de asociaciones numero->texto.
 Podrá haber hasta `tamanio' asociaciones.
 */
hash_t crear_hash(nat tamanio){
	hash_t h = new rep_hash;
	h->texto = new nodo_lista_texto[tamanio];
	h->capacidad = tamanio;
	h->tamanio_act = 0;
	for (nat i = 0; i < h->capacidad; i++){
		h->texto[i] = NULL;
	}
	return h;
}

void imprimir_hash(hash_t h){
	nat i;
	printf("\n");
	for (i = 0; i < h->capacidad; i++){
		nodo_lista_texto a_imprimir, aux;
		aux = h->texto[i];
		printf("[%d]: ", i);
		while (aux != NULL){
			a_imprimir = aux;
			aux = aux->siguiente;
			printf("[%d] ", a_imprimir->clave );
		}
		printf("\n");
	}
}
/*
	Inserta en `h' la asociación entre `clave' y `valor'.
	Precondición: !esta_lleno_hash(t) y !existe_asociacion(clave, h).
	El tiempo de ejecución es O(1).
 */
void asociar_en_hash(int clave, char *valor, hash_t &h) {
	assert(!esta_lleno_hash(h) && !existe_asociacion(clave, h));
	nodo_lista_texto aux = h->texto[formula_hash(clave, h)];
	if (aux == NULL) {
		h->texto[formula_hash(clave, h)] = crear_nodo(clave, valor);
		h->tamanio_act++;
		return;
	} else {
		while (aux->siguiente != NULL) {
			aux = aux->siguiente;
		}
		aux->siguiente = crear_nodo(clave, valor);
		h->tamanio_act++;
		return;
	}
}
	
/*
	Sustituye en `h' el anterior valor asociado a `clave' por `valor'.
	Precondición: existe_asociacion(clave, h).
	El tiempo de ejecución es O(1).
 */
void actualizar_hash(int clave, char *valor, hash_t &h){
	assert(existe_asociacion(clave, h));
	nodo_lista_texto aux = h->texto[formula_hash(clave, h)];
	while (aux->clave != clave) {
		aux = aux->siguiente;
	}
	delete[] aux->texto;
	aux->texto = valor;

}

/*
  Elimina de `h' la asociación entre `clave' y algún valor y libera la menoria
  asignada a mantener esa asociación.
  Precondición: existe_asociacion(clave, h).
  El tiempo de ejecución es O(1) en promedio.
 */

void eliminar_de_hash(int clave, hash_t &h) {
	assert(existe_asociacion(clave, h));
	nodo_lista_texto aux = h->texto[formula_hash(clave, h)];
//	imprimir_hash(h);
//	printf("-----before Eliminar---------\n");

	if (aux->clave == clave) {
		nodo_lista_texto aux_primero = aux->siguiente;
		liberar_nodo(aux);
		h->texto[formula_hash(clave, h)] = aux_primero;
		h->tamanio_act--;
	} else {
		nodo_lista_texto aux_prev = NULL;
		while (aux != NULL && (aux->clave != clave )) {
			aux_prev = aux;
			aux = aux->siguiente;
		}
		aux_prev->siguiente = aux->siguiente;
		liberar_nodo(aux);
		h->tamanio_act--;
	}
//	imprimir_hash(h);
//	printf("-----after Eliminar---------\n");
}

/*
	Libera la memoria asignada a `h' y todos sus elementos.
 */

void liberar_hash(hash_t &h) {
	nat i;
	for (i = 0; i < h->capacidad; i++){
		nodo_lista_texto a_borrar, aux;
		aux = h->texto[i];
		while (aux != NULL){
			a_borrar = aux;
			aux = aux->siguiente;
			liberar_nodo(a_borrar);
		}
	}
	delete[] h->texto;
	delete h;
}



/*
	Devuelve `true' si y sólo si en `h' hay una asociación entre `clave'
	y algún valor.
	El tiempo de ejecución es O(1) en promedio.
 */
bool existe_asociacion(int clave, hash_t h){
	nodo_lista_texto aux = h->texto[formula_hash(clave, h)];
	while (aux != NULL && aux->clave != clave) {
		aux = aux->siguiente;
	}
	return aux != NULL && aux->clave == clave;
}

/*
	Devuelve el valor correspondiente a la asociacion de `clave' en `h'.
	Precondición: existe_asociacion(clave, h).
	El tiempo de ejecución es O(1) en promedio.
 */
char *valor_en_hash(int clave, hash_t h){
	assert(existe_asociacion(clave, h));
	nodo_lista_texto aux = h->texto[formula_hash(clave, h)];
	while (aux != NULL && aux->clave != clave) {
		aux = aux->siguiente;
	}
	return aux->texto;
}

/*
	Devuelve `true' si y sólo si `h' tiene `tamanio' elementos.
	El tiempo de ejecución es O(1).
 */
bool esta_lleno_hash(hash_t h){
	return (h->tamanio_act == h->capacidad);
}