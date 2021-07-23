#include "../include/binario.h"
#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/uso_cadena.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rep_binario {
	info_t dato;
	rep_binario *izq;
	rep_binario *der;	
};
/*
  Devuelve un binario_t vacío (sin elementos).
  El tiempo de ejecución es O(1).
 */
binario_t crear_binario(){ return NULL; }
/*
  Inserta `i' en `b' respetando la propiedad de orden definida.
  Devuelve `b'.
  Precondición: es_vacio_binario(buscar_subarbol(frase_info(i), b).
  El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de
  elementos de `b'.
 */
binario_t insertar_en_binario(info_t i, binario_t b){ 
	assert(es_vacio_binario(buscar_subarbol(frase_info(i, b))));
	if (b == NULL){
		binario_t nuevo;
		nuevo = new rep_binario;
		nuevo->dato = i;
		nuevo->izq = NULL;
		nuevo->der = NULL;
		b = nuevo;
	}else {
		int comp = strcmp(frase_info(i), frase_info(b->dato));
		if (comp < 0)
			b->izq = insertar_en_binario(i, b->izq);
		else if (comp > 0)
			b->der = insertar_en_binario(i, b->der);
	}
	return b;
}

/*
  Devuelve el elemento mayor (según la propiedad de orden definida) de `b'.
  Precondición: ! es_vacio_binario(b).
  El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de
  elementos de `b'.
 */
info_t mayor(binario_t b){ 
	assert(!es_vacio_binario(b));
	 if (b->der == NULL)
		return b->dato;
	else
		return mayor(b->der);
}

/*
  Remueve el nodo en el que se localiza el elemento mayor de `b'
  (libera la memoria  asignada al nodo, pero no la del elemento).
  Devuelve `b'.
  Precondición: ! es_vacio_binario(b).
  El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de
  elementos de `b'.
 ejemplo_binario_cpp.png*/
binario_t remover_mayor(binario_t b){
	assert(!es_vacio_binario(b));
	if (b->der == NULL){
		binario_t izq = b->izq;
		delete b;
		b = izq;
	}else {
		b->der = remover_mayor(b->der);
	}
	return b;
}
/*
  Remueve de `b' el nodo en el que el dato de texto de su elemento es `t'.
  Si los dos subárboles del nodo a remover son no vacíos, en sustitución del
  elemento removido debe quedar el que es el mayor (segun la propiedad de orden
  definida) en el subárbol izquierdo.
  Devuelve `b'.
  Precondición: !es_vacio_binario(buscar_subarbol(frase_info(i), b).
  Libera la memoria del nodo y del elemento.
  El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de
  elementos de `b'.
 */

binario_t remover_de_binario(const char *t, binario_t b){

	int comp = strcmp(t, frase_info(b->dato));
	if(b == NULL) return b;
	else if(comp < 0) 
		b->izq = remover_de_binario(t, b->izq);
	else if (comp > 0)
		b->der = remover_de_binario(t, b->der);
	else{
	
		if(b->der == NULL && b->izq == NULL){ 	//No child
			liberar_info(b->dato);
			delete b;
			b = NULL;
	
		}else if (b->der == NULL){ 				//One child 
			binario_t temp = b;
			b = b->izq;
			liberar_info(temp->dato);
			delete temp;
			temp = NULL;

		}else if (b->izq == NULL){
			binario_t temp = b;
			b= b->der;
			liberar_info(temp->dato);
			delete temp;
			temp = NULL;
	
		}else{ 									//two child
			binario_t maxIzq = b->izq;
			while(maxIzq->der != NULL)
				maxIzq = maxIzq->der;
			liberar_info(b->dato);
			b->dato = copia_info(maxIzq->dato);
			b->izq = remover_de_binario(frase_info(maxIzq->dato), b->izq);
		
		}
	}
	return b;
}

/*
  Libera la memoria asociada a `b' y todos sus elementos.
  Devuelve `b'.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
 ejemplo_binario_cpp.png */
binario_t liberar_binario(binario_t b){
	if (b != NULL){
		b->izq = liberar_binario(b->izq);
		b->der = liberar_binario(b->der);
		liberar_info(b->dato);
		delete b;
		b = NULL;
	}
	return b;
}

/*
  Devuelve `true' si y sólo si `b' es vacío (no tiene elementos).
  El tiempo de ejecución es O(1).
 ejemplo_binario_cpp.png */
bool es_vacio_binario(binario_t b) { return b == NULL; }

/*
  Devuelve `true' si y sólo si cada nodo de `b' cumple la condición de balanceo
  AVL. El árbol vacío cumple la condición.
  Un nodo cumple la condición de balanceo AVL si el valor absoluto de la
  diferencia de las alturas de sus subárboles izquierdo y derecho en menor o
  igual a 1.
  Cada nodo se puede visitar una sola vez.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
 */
static int absoluto(int n){ return (n >= 0) ? (n) : (-n); }
static nat maximo(nat n1, nat n2){ return (n1 >= n2) ? n1 : n2; }
static int recursive_es_AVL(binario_t node){
    if(node == NULL) return 0;
    int left = recursive_es_AVL(node->izq);

    if(left == -1) return -1;

    int right = recursive_es_AVL(node->der);

    if(right == -1) return -1;

    if(absoluto(left - right) > 1){
        return -1;
    }else{
        return 1 + maximo(left, right);
    }
}
bool es_AVL(binario_t b){
    return recursive_es_AVL(b) != -1;
}
/*
  Devuelve el elemento asociado a la raíz de `b'.
  Precondición: ! es_vacio_binario(b).
  El tiempo de ejecución es O(1).
 */
info_t raiz(binario_t b){ 
	assert(!es_vacio_binario(b));
	return b->dato;
}

/*
  Devuelve el subárbol izquierdo de `b'.
  Precondición: ! es_vacio_binario(b).
  El tiempo de ejecución es O(1).
 */
binario_t izquierdo(binario_t b){ 
	assert(!es_vacio_binario(b));
	return b->izq;
}

/*
  Devuelve el subárbol derecho de `b'.
  Precondición: ! es_vacio_binario(b).
  El tiempo de ejecución es O(1).
 */
binario_t derecho(binario_t b){ 
	assert(!es_vacio_binario(b));
	return b->der;
}

/*
  Devuelve el subárbol que tiene como raíz al nodo con el elemento cuyo dato de
  texto es `t'.
  Si `t' no pertenece a `b', devuelve el árbol vacío.
  El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de
  elementos de `b'.
 ejemplo_binario_cpp.png */
binario_t buscar_subarbol(const char *t, binario_t b){
	binario_t res;
	if (es_vacio_binario(b))
		res = crear_binario();
	else {
		int comp = strcmp(t, frase_info(b->dato));
		if (comp < 0)
			res = buscar_subarbol(t, b->izq);
		else if (comp > 0)
			res = buscar_subarbol(t, b->der);
		else
			res = b;
	}
	return res;
}

/*
  Devuelve la altura de `b'.
  La altura de un árbol vacío es 0.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
 */

nat altura_binario(binario_t b){
	if (b == NULL)
		return 0;
	else 
		return (1 + maximo(altura_binario(b->izq), altura_binario(b->der)));
}

/*
  Devuelve la cantidad de elementos de `b'.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
 */
nat cantidad_binario(binario_t b){ 
	if (b == NULL)
		return 0;
	else 
		return (1 + cantidad_binario(b->izq) + cantidad_binario(b->der));	
}

/*
  Devuelve la suma de los datos numéricos de los últimos `i' elementos
  (considerados según la propiedad de orden de los árboles binario_t)
  de `b' cuyos datos numéericos sean pares.
  Si en `b' hay menos de `i' elementos con dato numérico par devuelve la
  suma de todos los datos numéricos pares de `b'.
  No se deben crear estructuras auxiliares.
  No se deben visitar nuevos nodos después que se hayan encontrado los
  `i' elementos.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
 */

static int recursive_suma_ultimos_pares(nat &i, binario_t b) {
	if(b) {
		int sum = 0;

		if(i > 0)
			sum += recursive_suma_ultimos_pares(i, b->der);

		if(i > 0)
			if(numero_info(b->dato) % 2 == 0) {
				sum += numero_info(b->dato);
				i--;
			}

		if(i > 0)
			sum += recursive_suma_ultimos_pares(i, b->izq);

		return sum;
	} else
		return 0;
}
int suma_ultimos_pares(nat i, binario_t b){
	//imprimir_binario(b);
	return recursive_suma_ultimos_pares(i, b);
}
/*
  Devuelve una cadena_t con los elementos de `b' en orden lexicográfico
  creciente según sus datos de texto.
  La cadena_t devuelta no comparte memoria con `b'.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
 */
static cadena_t cadena_inorder(binario_t b, cadena_t cad){
	if (b){
		if (b->izq)
			cad = cadena_inorder(b->izq, cad);
		
		info_t data = copia_info(b->dato);
		cad = insertar_al_final(data, cad);	
		
		if (b->der)
			cad = cadena_inorder(b->der, cad);
	}
	return cad;
}
cadena_t linealizacion(binario_t b){
	cadena_t cad = crear_cadena();
	cad = cadena_inorder(b, cad);
	return cad;	
}
/*
  Devuelve un árbol balanceado cuyos elementos son los que están contenidos en
  `cad'.
  En esta función se dice que un árbol está balanceado si
  a) es vacío;
  o
  b)
	b1) el sub-árbol izquierdo tiene la misma cantidad de elementos o uno más
	  que el  subárbol derecho;
	y
	b2) los subárboles izquierdo y derecho están balanceados.

  DEFINICIÓN ALTERNATIVA DE ÁRBOL BALANCEADO
  En esta función se dice que un árbol esta balanceado si en cada nodo, la
  cantidad de elementos de su subárbol izquierdo es igual a, o 1 más que, la
  cantidad de elementos de su subárbol derecho.

  Precondición: los elementos de `cad' están en orden lexicográfico creciente
  estricto según sus datos de texto.
  El árbol devuelto no comparte memoria con `cad'.
  El tiempo de ejecución es O(n . log n), siendo `n` la cantidad de elementos
  de `cad'.
 */

binario_t recursive_cadena_a_binario(cadena_t cad, int n) {  

    if (n <= 0)  
        return NULL;

    binario_t left = recursive_cadena_a_binario(cad, n / 2);  

    localizador_t head = inicio_cadena(cad);

    binario_t b = new rep_binario;
    b->dato = copia_info(info_cadena(head, cad));
    b->izq = left;

    cad = cortar_segmento(head, head, cad);

    b->der = recursive_cadena_a_binario(cad, n - n / 2 - 1);  
  
    return b;  
} 

binario_t cadena_a_binario(cadena_t cad){
	
	return recursive_cadena_a_binario(cad, longitud(cad));

}

/*
  Devuelve un árbol con copias de los elementos de `b' que cumplen la condición
  "numero_info(elemento) < clave".
  La estructura del árbol resultado debe ser análoga a la de `b'. Esto
  significa que dados dos nodos `U' y `V' de `b' en los que se cumple la
  condición y tal que `U' es ancestro de `V', en el árbol resultado la copia de
  `U' debe ser ancestro de la copia de `V' cuando sea posible. Esto no siempre
  se puede lograr y al mismo tiempo mantener la propiedad de orden del árbol
  en el caso en el que en un nodo `V' no se cumple la condición y en sus dos
  subárboles hay nodos en los que se cumple. En este caso, la copia del nodo
  cuyo elemento es el mayor (según la propiedad de orden definida) de los que
  cumplen la condición en el subárbol izquierdo de `V' deberá ser ancestro de
  las copias de todos los descendientes de `V' que cumplen la condición.
  (Ver ejemplos en LetraTarea3.pdf).
  El árbol resultado no comparte memoria con `b'. *)
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
 */
binario_t menores(int clave, binario_t b){

	if(b) {

		binario_t nb = NULL;

		binario_t left = menores(clave, b->izq);
		binario_t right = menores(clave, b->der);

		if(numero_info(b->dato) < clave) {
			nb = new rep_binario;
			nb->dato = copia_info(b->dato);
			nb->izq = left;
			nb->der = right;
			return nb;
		}

		if(left) {
			nb = new rep_binario;
			nb->dato = mayor(left);
			nb->izq = remover_mayor(left);
			nb->der = right;
			return nb;
		} else
			return right;
	} else
		return NULL;

}


/*
  Devuelve `true' si y solo si en `b' hay un camino desde la raiz hasta una
  hoja que sea compatible con `c'.
  Decimos que una cadena y un camino son compatibles si tienen la misma
  longitud y al recorrer la cadena desde el inicio hasta el final y el camino
  desde la raíz hasta la hoja las frases de los respectivos elementos son
  iguales.
  El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de
  elementos de `b'.
 */
bool es_camino(cadena_t c, binario_t b){
	if ((longitud(c) > 0) && (b != 0) && (strcmp(frase_info(info_cadena(inicio_cadena(c), c)), frase_info(b->dato)) == 0)){

		if((longitud(c) == 1) && (altura_binario(b) == 1) && (strcmp(frase_info(info_cadena(inicio_cadena(c), c)), frase_info(b->dato)) == 0)) 
			return true;
		
		localizador_t loc = inicio_cadena(c);
		c = remover_de_cadena(loc, c);

		if ((longitud(c) > 0) && strcmp(frase_info(info_cadena(inicio_cadena(c), c)), frase_info(b->dato)) < 0)
			return es_camino(c, b->izq);
		else
			return es_camino(c, b->der);
	} else 
		return false;
}

/*
  Devuelve una cadena_t con los elementos del nivel `l' de 'b'.
  La raíz es el nivel 1.
  La cadena_t resultado debe estar ordenada de manera creciente según las
  frases de sus elementos.
  La cadena_t devuelta no comparte memoria con `b'.
 */

static nat nivelcad = 1;
static cadena_t cadena_nivel(nat l, binario_t b, cadena_t cad){
	if (b){
		if (b->izq){
			nivelcad++;
			cad = cadena_nivel(l, b->izq, cad);
			nivelcad--;
		}
		if (l == nivelcad){
			info_t data = copia_info(b->dato);
			cad = insertar_al_final(data, cad);	
		}
		if (b->der){
			nivelcad++;
			cad = cadena_nivel(l, b->der, cad);
			nivelcad--;
		}
	}
	return cad;
}
cadena_t nivel_en_binario(nat l, binario_t b){
	cadena_t cad = crear_cadena();
	cad = cadena_nivel(l, b, cad);
	return cad;
}

/*
  Imprime los elementos de `b', uno por línea, en orden descendente de la
  propiedad de orden de los árboles `binario_t'.
  Antes del elemento imprime una cantidad de guiones igual a su nivel.
  El elemento se imprime según la especificación dada en `info_a_texto'.
  El nivel de la raíz es 0.
  Antes de terminar, se debe imprimir un fin de linea.
  Si es_vacio_binario(b) sólo se imprime el fin de línea.
  El tiempo de ejecución es O(n . log n) en promedio, donde `n' es la cantidad
  de elementos de `b'.
 */
static int nivel = 0;
void imprimir_binario(binario_t b){
	if (b){
		if (!nivel)
			printf("\n");
		if (b->der){
			nivel++;
			imprimir_binario(b->der);
			nivel--;
		}	
		char *infostr = info_a_texto(b->dato);
		for (int i = 1; i <= nivel; i++)
			printf("-");
		printf("%s\n",infostr);		
		delete[] infostr;
		if (b->izq){
			nivel++;
			imprimir_binario(b->izq);
			nivel--;
		}
	}else
	printf("\n");
}
