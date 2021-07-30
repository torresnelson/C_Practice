#include "../include/avl.h"
#include "../include/conjunto.h"
#include "../include/info.h"
#include <stdio.h>


// Declaración del tipo `conjunto_t'
// struct rep_conjunto es la representación de `conjunto_t',
struct rep_conjunto {
	avl_t arbol;
};

/* Constructoras */

/*
	Devuelve un conjunto_t vacío (sin elementos).
	El tiempo de ejecución es O(1).
 */
conjunto_t crear_conjunto() {
	conjunto_t res = new rep_conjunto;
	res->arbol = crear_avl();
	return res;
}

/*
	Devuelve un conjunto_t cuyo único elemento es `i'.
	El tiempo de ejecución es O(1).
 */
conjunto_t singleton(info_t i) {
	conjunto_t res = crear_conjunto();
	insertar_en_avl(i, res->arbol);
	return res;
}

/*`
	Devuelve un conjunto_t con los elementos que pertenecen a  `c1' o `c2'.
	Si en ambos conjuntos hay un elemento con el mismo dato  numérico en el
	conjunto_t devuelto se debe incluir el que pertenece a `c1'.
	El tiempo de ejecucion es O(n1 + n2 + n.log n), siendo `n1' y `n2' la
	cantidad de elementos de `c1' y `c2' respectivamente y `n' la del conjunto_t
	resultado.
	El conjunto_t devuelto no comparte memoria ni con `c1' no con `c2'.
 */
conjunto_t union_conjunto(conjunto_t c1, conjunto_t c2){
	conjunto_t res = crear_conjunto();
	info_t *conjunto1, *conjunto2;
	nat tope_conjunto1 = 0;
 	nat tope_conjunto2 = 0;
	if (c1) {
		conjunto1 = en_orden_avl(c1->arbol);
		tope_conjunto1 = cantidad_en_avl(c1->arbol);
	}
	if (c2) {
		conjunto2 = en_orden_avl(c2->arbol);
		tope_conjunto2 = cantidad_en_avl(c2->arbol);
	}
	nat i, j;
	i = 0;
	j = 0;
	while ((i < tope_conjunto1) || (j < tope_conjunto2)){
		if (i < tope_conjunto1 && j < tope_conjunto2) {
			if (numero_info(conjunto1[i]) < numero_info(conjunto2[j])){
				insertar_en_avl(copia_info(conjunto1[i]), res->arbol);
				i++;
			} else if (numero_info(conjunto1[i]) > numero_info(conjunto2[j])){
				insertar_en_avl(copia_info(conjunto2[j]), res->arbol);
				j++;
			} else {
				insertar_en_avl(copia_info(conjunto1[i]), res->arbol);
				i++;
				j++;
			}
		} else if (i < tope_conjunto1){
			insertar_en_avl(copia_info(conjunto1[i]), res->arbol);
			i++;
		} else if (j < tope_conjunto2) {
			insertar_en_avl(copia_info(conjunto2[j]), res->arbol);
			j++;
		}
	}
	delete[] conjunto1;
	delete[] conjunto2; 
	return res;
}

/*
	Devuelve un conjunto_t con los elementos de `c1' cuyos datos numéricos no
	pertenecen a `c2'.
	El tiempo de ejecucion es O(n1 + n2 + n.log n), siendo `n1' y `n2' la
	cantidad de elementos de `c1' y `c2' respectivamente y `n' la del conjunto_t
	resultado.
	El conjunto_t devuelto no comparte memoria ni con `c1' no con `c2'.
 */
conjunto_t diferencia(conjunto_t c1, conjunto_t c2){
	conjunto_t res = crear_conjunto();
	info_t *conjunto1, *conjunto2;
	nat tope_conjunto1 = 0;
 	nat tope_conjunto2 = 0;
	if (c1) {
		conjunto1 = en_orden_avl(c1->arbol);
		tope_conjunto1 = cantidad_en_avl(c1->arbol);
	}
	if (c2) {
		conjunto2 = en_orden_avl(c2->arbol);
		tope_conjunto2 = cantidad_en_avl(c2->arbol);
	}
	nat i, j;
	i = 0;
	j = 0;
	while (i < tope_conjunto1) {
		if (i < tope_conjunto1 && j < tope_conjunto2) {
			if (numero_info(conjunto1[i]) < numero_info(conjunto2[j])){
				insertar_en_avl(copia_info(conjunto1[i]), res->arbol);
				i++;
			} else if (numero_info(conjunto1[i]) > numero_info(conjunto2[j])){
				//insertar_en_avl(copia_info(conjunto2[j]), res->arbol);
				j++;
			} else {
				//insertar_en_avl(copia_info(conjunto1[i]), res->arbol);
				i++;
				j++;
			}
		} else if (i < tope_conjunto1){
			insertar_en_avl(copia_info(conjunto1[i]), res->arbol);
			i++;
		} 
	}
	delete[] conjunto1;
	delete[] conjunto2; 
	return res;
}


/*
	Libera la memoria asignada a `c' y la de todos sus elementos.
	El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos de `c'.
 */
void liberar_conjunto(conjunto_t &c){
	if (c == NULL) return;
	liberar_avl(c->arbol);
	delete c;
}

/*
	Devuelve `true' si y sólo si `info' es un elemento de `c'.
	El tiempo de ejecución es O(log n), siendo `n' es la cantidad de
	elementos de `c'.
 */

bool pertenece_conjunto(info_t info, conjunto_t s) { 
	avl_t nodo = buscar_en_avl(numero_info(info),s->arbol);
	return nodo && son_iguales(raiz_avl(nodo), info);
}

/*
	Devuelve `true' si y sólo `c' es vacío (no tiene elementos).
	El tiempo de ejecución es O(1).
 */
bool es_vacio_conjunto(conjunto_t c){
	return (c->arbol == NULL);
}

/*
	Devuelve un conjunto_t con los `n' elementos que están en en el rango
	[0 .. n - 1] del arreglo `infos'.
	Los elementos están ordenados de manera creciente estricto (creciente y sin
	repetidos) según los datos numércos.
	El tiempo de ejecución es O(n).
 */
conjunto_t arreglo_a_conjunto(info_t *infos, nat n){
	conjunto_t res = crear_conjunto();
	for (nat i = 0; i < n; i++){
		insertar_en_avl(infos[i], res->arbol);
	}
	return res;
}

/*
	Devuelve un iterador_t de los elementos de `c'.
	En la recorrida del iterador devuelto los datos numéricos aparecerán en orden
	creciente.
	El tiempo de ejecución es O(n), siendo `n' es la cantidad de elementos de `c'.
	El iterador_t resultado NO comparte memoria con `c'.
 */
iterador_t iterador_conjunto(conjunto_t  c) {
	iterador_t res = crear_iterador();
	info_t *infos = en_orden_avl(c->arbol);
	for (nat i = 0; i < cantidad_en_avl(c->arbol); i++)
		agregar_a_iterador(infos[i], res);
	bloquear_iterador(res);
	delete[] infos;
	return res;
}