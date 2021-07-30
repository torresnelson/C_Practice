#include "../include/avl.h"
#include "../include/info.h"
#include "../include/binario.h"
#include <cassert>
#include <stdio.h>
#include <cstddef>

// Representación de `avl_t'.
// Se debe definir en avl.cpp.
struct rep_avl {
	info_t dato;
	nat altura;
	nat cantidad;
	avl_t izq, der;
};

/*
	Devuelve un avl_t vacío (sin elementos).
	El tiempo de ejecución es O(1).
 */
avl_t crear_avl() {
	return NULL;  
}

/*
	Devuelve `true' si y sólo si `avl' es vacío (no tiene elementos).
	El tiempo de ejecución es O(1).
 */
bool es_vacio_avl(avl_t avl){
	return avl == NULL;
}

/*
	Devuelve el elemento asociado a la raíz de `avl'.
	Precondición: ! es_vacio_avl(b).
	El tiempo de ejecución es O(1).
 */
info_t raiz_avl(avl_t avl) {
	assert(!es_vacio_avl(avl));
	return avl->dato;
}

/*
	Devuelve el subárbol izquierdo de `avl'.
	Precondición: ! es_vacio_avl(avl).
	El tiempo de ejecución es O(1).
 */
avl_t izq_avl(avl_t avl) {
	assert(!es_vacio_avl(avl));
	return avl->izq;
}

/*
	Devuelve el subárbol derecho de `avl'.
	Precondición: ! es_vacio_avl(avl).
	El tiempo de ejecución es O(1).
 */
avl_t der_avl(avl_t avl) {
	assert(!es_vacio_avl(avl));
	return avl->der;
}

/*
	Devuelve la cantidad de elementos en `avl'.
	El tiempo de ejecución es O(1).
 */
nat cantidad_en_avl(avl_t avl) {
	if (avl == NULL)
		return 0;
	else 
		return avl->cantidad;	
}

nat altura_de_avl(avl_t avl) {
	if (avl == NULL)
		return 0;
	else 
		return avl->altura;
}

nat maximo(nat n1, nat n2){ return (n1 >= n2) ? n1 : n2; }
avl_t derRotate(avl_t y) { 
	avl_t x = y->izq; 
	avl_t T2 = x->der; 
	// Perform rotation 
	x->der = y; 
	y->izq = T2; 
	// Update alturas 
	y->altura = maximo(altura_de_avl(y->izq), altura_de_avl(y->der)) + 1; 
	y->cantidad = (cantidad_en_avl(y->izq) + cantidad_en_avl(y->der)) + 1;
	x->altura = maximo(altura_de_avl(x->izq), altura_de_avl(x->der)) + 1;
	x->cantidad = (cantidad_en_avl(x->izq) + cantidad_en_avl(x->der)) + 1;
	// Return new root 
	return x; 
} 

avl_t izqRotate(avl_t x) { 
	avl_t y = x->der; 
	avl_t T2 = y->izq; 
	// Perform rotation 
	y->izq = x; 
	x->der = T2; 
	// Update alturas 
	x->altura = maximo(altura_de_avl(x->izq), altura_de_avl(x->der)) + 1; 
	x->cantidad = (cantidad_en_avl(x->izq) + cantidad_en_avl(x->der)) + 1;				 
	y->cantidad = (cantidad_en_avl(y->izq) + cantidad_en_avl(y->der)) + 1;
	y->altura = maximo(altura_de_avl(y->izq), altura_de_avl(y->der)) + 1;
	// Return new root 
	return y; 
} 
int getBalance(avl_t avl) {  
	if (avl == NULL)  
		return 0;  
	return altura_de_avl(avl->izq) - altura_de_avl(avl->der);  
}  
 
void insertar_en_avl(info_t i, avl_t &avl) {
	assert(es_vacio_avl(buscar_en_avl(numero_info(i), avl)));
	assert(numero_info(i) != INT_MAX);
	if (avl == NULL) {
		avl = new rep_avl;
		avl->dato = i;
		avl->der = NULL;
		avl->izq = NULL;
		avl->altura = 1;
		avl->cantidad = 1;
		return;
	}
	if (numero_info(i) < numero_info(avl->dato)) 
		insertar_en_avl(i, avl->izq); 
	else if (numero_info(i) > numero_info(avl->dato)) 
		insertar_en_avl(i, avl->der);
	

	avl->altura = (1 + maximo(altura_de_avl(avl->izq), altura_de_avl(avl->der)));
	avl->cantidad = (1 + cantidad_en_avl(avl->izq) + cantidad_en_avl(avl->der));
	int balance = getBalance(avl); 

	// Left Left Case
	if (balance > 1 && numero_info(i) < numero_info(avl->izq->dato)) {
		avl = derRotate(avl); 
		return;
	}
	// Right Right Case
	if (balance < -1 && numero_info(i) > numero_info(avl->der->dato)) {
		avl = izqRotate(avl); 
		return;
	}
	// Left Right Case
	if (balance > 1 && numero_info(i) > numero_info(avl->izq->dato)) {
		avl->izq = izqRotate(avl->izq); 
		avl = derRotate(avl); 
		return;
	} 
	// Right Left Case
	if (balance < -1 && numero_info(i) < numero_info(avl->der->dato)) {
		avl->der = derRotate(avl->der); 
		avl = izqRotate(avl); 
		return;
	} 
}

/*
	Devuelve el subárbol que tiene como raíz al nodo con el elemento cuyo dato
	numérico es `clave'.
	Si ningún nodo cumple esa condición devuelve el árbol vacío.
	El tiempo de ejecución es O(log n), donde `n' es la cantidad de elementos de
	`avl'.
 */

avl_t buscar_en_avl(int clave, avl_t avl){
	avl_t res;
	if (es_vacio_avl(avl))
		res = crear_avl();
	else {
		if (clave < numero_info(avl->dato))
			res = buscar_en_avl(clave, avl->izq);
		else if (clave > numero_info(avl->dato))
			res = buscar_en_avl(clave, avl->der);
		else
			res = avl;
	}
	return res;
}



/*
	Devuelve un arreglo con los elementos de `avl'.
	Los elementos en el arreglo deben estar en orden creciente según los datos
	numericos.
 */
static info_t *en_orden_rec(info_t *res, nat &tope, avl_t avl) {
	if (!es_vacio_avl(avl)) {
		res = en_orden_rec(res, tope, izq_avl(avl));
		res[tope] = raiz_avl(avl);
		tope++;
		res = en_orden_rec(res, tope, der_avl(avl));
	}
	return res;
}

info_t *en_orden_avl(avl_t avl) {
	info_t *res;
	if (es_vacio_avl(avl))
		res = NULL;
	else {
		res = new info_t[cantidad_en_avl(avl)];
		nat tope = 0;
		res = en_orden_rec(res, tope, avl);
	}
	return res;
}
/*
	Devuelve un avl_t con los `n' elementos que están en el rango [0 .. n - 1]
	del arreglo `infos'.
	Los elementos están ordenados de manera creciente estricto (creciente y sin
	repetidos) según los datos numércos.
	En cada nodo del árbol resultado la cantidad de elementos de su subárbol
	izquierdo es igual a, o 1 menos que, la cantidad de elementos de su subárbol
	derecho.
	El tiempo de ejecución es O(n).
 */
static avl_t a2avl_rec(info_t *infos, int inf, int sup) {
	avl_t res;
	if (inf > sup)
		res = NULL;
	else {
		nat medio = (inf + sup) / 2;
		res = new rep_avl;
		res->dato = infos[medio];
		res->izq = a2avl_rec(infos, inf, medio -1);
		res->der = a2avl_rec(infos, medio + 1, sup);
		// ajustar res->altura y res->cantidad
		res->altura = (1 + maximo(altura_de_avl(res->izq), altura_de_avl(res->der)));
		res->cantidad = (1 + cantidad_en_avl(res->izq) + cantidad_en_avl(res->der));
	
	}
	return res;
}
avl_t arreglo_a_avl(info_t *infos, nat n) {
	return a2avl_rec(infos, 0, n - 1);
}
/*
	Devuelve un avl_t de altura `h' con `n' nodos, siendo `n' la mínima cantidad
	de elementos que puede tener un avl de altura `h'.
	Los datos numéricos de los elementos van desde 1 hasta `n'.
	El dato de texto de cada elemento es la cadena vacia.
	En ningún nodo puede ocurrir que el subárbol derecho tenga más nodos que el
	subárbol izquierdo.
	El tiempo de ejecución es O(n).
	Ver ejemplos en la letra y en el caso 408.
 */
struct avl_ultimo {
	avl_t avl;
	int ultimo;
};

info_t crear_info_avl_min(int num){
	char *nulo = new char[1];
	nulo[0] = '\0';
	info_t nueva = crear_info(num, nulo);
	return nueva;
}

static avl_ultimo avl_min_rec(nat h, nat primero) {
	avl_ultimo res;
	if (h == 0) {
		res.avl = NULL;
		res.ultimo = primero -1;
	} else if (h == 1) {
		res.avl = new rep_avl;
		res.ultimo = primero;
		info_t hoja = crear_info_avl_min(primero);
		res.avl->dato = hoja;
		res.avl->cantidad = 1;
		res.avl->altura = 1;
		res.avl->izq = NULL;
		res.avl->der = NULL;
	} else {
		avl_ultimo aux_der, aux_izq;
		aux_izq = avl_min_rec(h - 1, primero);
	    aux_der = avl_min_rec(h - 2, aux_izq.ultimo + 2);
		
		res.avl = new rep_avl;
		res.avl->izq = aux_izq.avl;
		res.avl->der = aux_der.avl;

		res.avl->cantidad = (1 + cantidad_en_avl(aux_izq.avl) + cantidad_en_avl(aux_der.avl));
		res.avl->altura = h;

		res.avl->dato = crear_info_avl_min(aux_izq.ultimo + 1);

		res.ultimo = aux_der.ultimo;
	}
	return res;
}
avl_t avl_min(nat h) {
	avl_ultimo res = avl_min_rec(h, 1);
	return res.avl;
}
/*
	Imprime los datos numéricos de los nodos de cada nivel de `avl'.
	Imprime una línea por nivel, primero el más profundo y al final el que
	corresponde a la raíz.
	Cada nivel se imprime de izquierda a derecha. Se debe dejar un espacio en
	blanco después de cada número.
	El tiempo de ejecución es O(n), siendo `n' la cantidad de nodos de `avl'.
	Ver ejemplos en la letra y en el caso 404.
 */

/* Function to line by line print level order traversal a tree*/
/* Print nodes at a given level */
static void imprimir_nivel_avl(avl_t root, nat level) { 
	if (root == NULL) 
		return; 
	if (level == 1) 
		printf("%d ", numero_info(root->dato)); 
	else if (level > 1) { 
		imprimir_nivel_avl(root->izq, level - 1); 
		imprimir_nivel_avl(root->der, level - 1); 
	} 
} 
void imprimir_avl(avl_t root) { 
	nat h = altura_de_avl(root); 
	for (int i = h; i>=1; i--) { 
		imprimir_nivel_avl(root, i); 
		printf("\n"); 
	} 
} 

/*
	Libera la memoria asignada a `avl' y la de sus elementos.
	El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de
	`avl'.
 */
void liberar_avl(avl_t &avl){
	if (avl != NULL){
		liberar_info(avl->dato);
		liberar_avl(avl->izq);
		liberar_avl(avl->der);
		delete avl;
	}
}