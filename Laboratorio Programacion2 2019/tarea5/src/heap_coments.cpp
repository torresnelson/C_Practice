#include "../include/heap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
// Representación de `heap_t'
// Se debe definir en heap.cpp
struct rep_heap {
	info_t *dato;
	nat tamanio;
	nat max_valor;
	nat cantidad_actual;
	nat *valor_nat;
};

/*
	Devuelve un `heap' que puede contener hasta `tamanio' elementos.
	Los datos numéricos pueden estar en el rango 0 .. max_valor.
 */
heap_t crear_heap(nat tamanio, nat max_valor){
	heap_t nuevo = new rep_heap;
	nuevo->dato = new info_t[tamanio + 1];
	nuevo->valor_nat = new nat[max_valor + 1];
	nuevo->tamanio = tamanio;
	nuevo->max_valor = max_valor;
	nuevo->cantidad_actual = 0;
	for (nat i = 0; i < nuevo->tamanio + 1; i++){
		nuevo->dato[i] = NULL;
	}
	for (nat i = 0; i <= nuevo->max_valor; i++){
		nuevo->valor_nat[i] = 0;
	}
	return nuevo;
}

nat hijo_izquierdo(nat i){
	return (2*i);
}

nat hijo_derecho(nat i){
	return ((2*i) + 1);
}
nat padre(nat i){
	return (i/2);
}


/*
	Si inserta el elemento `i'.
	Precondición: !es_lleno_heap(h) y !hay_valor(v,h) y v <= max_valor(h),
	donde v = numero_info(i).
	El tiempo de ejecución es O(log tamanio).
 */


void intercambiar_nats(nat x, nat y, heap_t h){
	info_t aux;
	nat aux_indices_valor_nat;

	nat val1 = numero_info(h->dato[x]);
	nat val2 = numero_info(h->dato[y]);

	aux = h->dato[x];
	h->dato[x] = h->dato[y];
	h->dato[y] = aux;

	aux_indices_valor_nat = h->valor_nat[val1];
	h->valor_nat[val1] = h->valor_nat[val2];
	h->valor_nat[val2] = aux_indices_valor_nat;

}

bool comparar(nat nuevo, nat padre, heap_t h){
	return numero_info(h->dato[nuevo]) < numero_info(h->dato[padre]);
}
// void imprimir_heap(heap_t h){
// 	for (nat i = 1; i < h->tamanio + 1; i++)
// 	{
// 		if (h->dato[i])
// 			printf("[%u]",numero_info(h->dato[i]));
// 		else
// 			printf("[//]");
// 	}
// }
void insertar_en_heap(info_t i, heap_t &h) {
	nat v = numero_info(i);
	nat indice_nuevo =  h->cantidad_actual + 1;
	assert(!es_lleno_heap(h) && !hay_valor(v,h) && v <= max_valor(h));

	h->valor_nat[v] = indice_nuevo;
	if (h->cantidad_actual > 0){
		nat indice_padre = padre(indice_nuevo);
		h->dato[indice_nuevo] = i;
		while (indice_padre > 0 && comparar(indice_nuevo, indice_padre, h)){
			intercambiar_nats(indice_padre, indice_nuevo, h);
			indice_nuevo = indice_padre;
			indice_padre = padre(indice_nuevo);
		}
	} else
		h->dato[indice_nuevo] = i;
		
	h->cantidad_actual++;

	// printf("insertado en el heap: %u valor_nat: %u \n ", v, h->valor_nat[v]);
	// imprimir_heap(h);
	// printf("\n");
	
	// printf("\nvalores nat: ");
	// for (int i = 0; i < 20; ++i) {
	// 	printf("%d, ", h->valor_nat[i]);
	// }
	// printf("\n");
}

/*
	Reduce a v/2 el valor del elemento cuyo valor actual es `v'.
	Precondición: hay_valor(v, h) y !hay_valor(v/2, h).
	No debe quedar memoria inaccesible.
	El tiempo de ejecución es O(log tamanio).
 */

void reducir(nat v, heap_t &h) {
	assert(hay_valor(v, h) && !hay_valor(v/2, h));

	// printf("\nvalores nat: ");
	// for (int i = 0; i < 20; ++i) {
	// 	printf("%d, ", h->valor_nat[i]);
	// }
	// printf("\n");

	nat indice = h->valor_nat[v];

	// printf("reducir before: %u indice: %u \n ", v, h->valor_nat[v]);
	// imprimir_heap(h);
	// printf("-----\n");

 	intercambiar_nats(indice, h->cantidad_actual, h);
	indice = h->cantidad_actual;
 	
 	char *copia = new char[strlen(frase_info(h->dato[indice])) + 1]; 
	strcpy(copia, frase_info(h->dato[indice]));
	
	info_t modificado = crear_info(v/2, copia);
	liberar_info(h->dato[indice]);
	h->dato[indice] = modificado;//
	nat indice_padre = padre(indice);
	
	h->valor_nat[v] = 0;
	h->valor_nat[v/2] = indice;
	while (indice_padre > 0 && comparar(indice, indice_padre, h)){
		intercambiar_nats(indice_padre, indice, h);
		indice = indice_padre;
		indice_padre = padre(indice);
	}
	

	// printf("reducir after: %u indice: %u \n ", v/2, h->valor_nat[v/2]);
	// imprimir_heap(h);
	// printf("-----\n");
	// printf("\nvalores nat: ");
	// for (int i = 0; i < 20; ++i)
	// {
	// 	printf("%d, ", h->valor_nat[i]);
	// }
	// printf("\n");
}

/*
	Elimina de 'h' el elemento de menor valor y libera la memoria que tiene
	asignada.
	Precondición: ! es_vacia_heap(h).
	El tiempo de ejecución es O(log tamanio).
 */
void eliminar_menor(heap_t &h) {
	assert(!es_vacia_heap(h));
	nat aux = 1;
	nat indice_ultimo = h->cantidad_actual;

	intercambiar_nats(aux, indice_ultimo, h);
	h->valor_nat[numero_info(h->dato[indice_ultimo])] = 0;
	liberar_info(h->dato[indice_ultimo]);

	h->dato[indice_ultimo] = NULL;


	nat saltar = -1;
	while (saltar != 0 && aux < indice_ultimo) {
		if (hijo_izquierdo(aux) < h->cantidad_actual) 
			if (hijo_derecho(aux) < h->cantidad_actual)
				if (comparar(hijo_derecho(aux), hijo_izquierdo(aux), h))
					saltar = hijo_derecho(aux);
				else
					saltar = hijo_izquierdo(aux);
			else
				if (comparar(hijo_izquierdo(aux), aux, h))
					saltar = hijo_izquierdo(aux);
				else
					saltar = 0;
		else
			if (hijo_derecho(aux) <= h->cantidad_actual)
				if (comparar(hijo_derecho(aux), aux, h))
					saltar = hijo_derecho(aux);
				else
					saltar = 0;
			else 
				saltar = 0;
		if (saltar != 0)
			intercambiar_nats(saltar, aux, h);
		aux = saltar;	
	}

	h->cantidad_actual--;
	// printf("\nvalor_nat luego de eliminar_menor: ");
	// for (int i = 0; i < 20; ++i) {
	// 	printf("%d, ", h->valor_nat[i]);
	// }
	// printf("\n");
}

void liberar_heap(heap_t &h) {
	for (nat i = 1; i < h->cantidad_actual + 1; i++) {
		liberar_info(h->dato[i]);		
	}
	delete[] h->dato;
	delete[] h->valor_nat;
	delete h;
}

bool es_vacio_heap(heap_t h){
	return h->cantidad_actual == 0;
}

bool es_lleno_heap(heap_t h){
	return h->tamanio == h->cantidad_actual;
}

bool hay_valor(nat v, heap_t h){
	// printf("\nValor a buscar: %u\n", v );
	// printf("\nhay_valor -> valores nat: ");
	// for (int i = 0; i < 20; ++i)
	// {
	// 	printf("%d, ", h->valor_nat[i]);
	// }
	// printf("\n");
	return h->valor_nat[v] != 0;
}

info_t menor(heap_t h){
	assert(!es_vacio_h(h));	
	return h->dato[1];
}

nat max_valor(heap_t h){
	return h->max_valor;
}
