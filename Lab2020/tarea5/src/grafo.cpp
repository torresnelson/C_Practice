#include "../include/binario.h"
#include "../include/cadena.h"
#include "../include/grafo.h"
#include "../include/info.h"
#include <assert.h>
#include <stdio.h>


// Representación de 'TGrafo'.
// Se debe definir en grafo.cpp.
// struct repGrafo;
// Declaración del tipo 'TGrafo'.
//typedef struct repGrafo *TGrafo;

struct nodo {
	ArregloBools vecinos;
	TBinario distancias;
};

struct repGrafo {
	nodo** nodos;
	nat cantidadAristas;
	nat cantidadVertices;
	nat cantidadActualAristas;
};

/*
	Devuelve un 'TGrafo' con 'N' vértices identificados desde 1 hasta 'N'
	sin parejas de vecinos establecidos.
	Podŕa haber hasta 'M' parejas de vecinos.
	El tiempo de ejecución en peor caso es O(max{N,M}).
 */
TGrafo crearGrafo(nat N, nat M) {
	TGrafo nuevo = new repGrafo;
	nuevo->cantidadAristas = M;
	nuevo->cantidadVertices = N;
	nuevo->nodos = new nodo*[N + 1];
	for (nat i = 0; i < N + 1; ++i) {
		nuevo->nodos[i] = new nodo;
		nuevo->nodos[i]->vecinos = new bool[N + 1];
		for (nat j = 0; j < N + 1; ++j)	
			nuevo->nodos[i]->vecinos[j] = false;
		nuevo->nodos[i]->distancias = crearBinario();
	}
	nuevo->cantidadActualAristas = 0;
	return nuevo;
}


/*
	Devuelve la cantidad de vértices de 'g'.
	El tiempo de ejecución en peor caso es O(1).
 */
nat cantidadVertices(TGrafo g) {
	return g->cantidadVertices;
}

/*
	Devuelve 'true' si y solo si en 'g' ya hay M parejas de vecinos,
	siendo M el segundo parámetro pasado en crearGrafo.
	El tiempo de ejecución en peor caso es O(1).
 */
bool hayMParejas(TGrafo g) {
	return g->cantidadActualAristas == g->cantidadAristas;
}

/*
	Establece que 'v1' y 'v2' son vecinos en 'g' y la distancia entre ambos es
	'd'.
	Precondición: 1 <= v1, v2 <= N.
	Precondición: v1 != v2
	Precondición: !sonVecinos(v1, v2, g).
	Precondición: !hayMParejas(g).
	Precondición: !hayMParejas(g).
	Devuelve 'g'.
	El tiempo de ejecución es en peor caso O(max{log n1, log n2}), siendo 'n1' y
	'n2' la cantidad de vecinos de 'v1' y 'v2' respectivamente.
 */
TGrafo hacerVecinos(nat v1, nat v2, double d, TGrafo g) {
	assert(1 <= v1 && v1 <= g->cantidadVertices);
	assert(1 <= v2 && v2 <= g->cantidadVertices);
	assert(!sonVecinos(v1, v2, g));
	assert(!hayMParejas(g));
	assert(v1 != v2);	
	g->nodos[v1]->vecinos[v2] = true;
	g->nodos[v2]->vecinos[v1] = true;
	g->nodos[v1]->distancias = insertarEnBinario(crearInfo(v2, d) ,g->nodos[v1]->distancias);	
	g->nodos[v2]->distancias = insertarEnBinario(crearInfo(v1, d) ,g->nodos[v2]->distancias);
	g->cantidadActualAristas++;	
	return g;
}

/*
	Devuelve 'true' si y solo si 'v1' y 'v2' son vecinos.
	Precondición: 1 <= v1, v2 <= N.
	El tiempo de ejecución es O(1) en promedio.
 */
bool sonVecinos(nat v1, nat v2, TGrafo g) {
	return g->nodos[v1] != NULL && g->nodos[v1]->vecinos[v2];	
}

/*
	Devuelve la distancia entre 'v1' y 'v2'.
	Precondición: sonVecinos(v1, v2, g).
	Precondición: 1 <= v1, v2 <= N.
	El tiempo de ejecución es O(1) en promedio.
 */

nat minimo(nat v1, nat v2,TGrafo g) {
	nat resultado;
	if (v1 < v2)
		resultado = (v1)*g->cantidadVertices + (v2);
	else
		resultado = (v2)*g->cantidadVertices + (v1);
	return resultado;
}
double distancia(nat v1, nat v2, TGrafo g) {
	assert(1 <= v1 && v1 <= g->cantidadVertices);
	assert(1 <= v2 && v2 <= g->cantidadVertices) ;
	assert(sonVecinos(v1, v2, g));
	return minimo(v1, v2, g);
}
	

/*
	Devuelve un 'TIterador' con los vecinos de 'v' ordenados de manera creciente.
	Precondición: 1 <= v <= N.
	El tiempo de ejecución en peor caso es O(n), siendo 'n' la cantidad de
	vecinos de 'v'.
 */
TIterador vecinos(nat v, TGrafo g) {
	assert(1 <= v && v <= g->cantidadVertices);
	TIterador nuevoIter = crearIterador();
	TCadena cadena = linealizacion(g->nodos[v]->distancias);
	TLocalizador actualLoc = inicioCadena(cadena);
	while (esLocalizador(actualLoc)) {
		nuevoIter = agregarAIterador(natInfo(infoCadena(actualLoc, cadena)), nuevoIter);
		actualLoc = siguiente(actualLoc, cadena);
	}
	nuevoIter = reiniciarIterador(nuevoIter);
	liberarCadena(cadena);
	return nuevoIter;
}

/*
	Libera la memoria asignada a 'g'.
	El tiempo de ejecuión en el peor caso en O(N*N + M), siende 'N' y 'M' los
	parámetros pasados en crearGrafo.
 */
void liberarGrafo(TGrafo g) {
	for (nat i = 0; i < g->cantidadVertices + 1; ++i) {
		liberarBinario(g->nodos[i]->distancias);
		delete[] g->nodos[i]->vecinos;
		delete g->nodos[i];
	}
	delete[] g->nodos;
	delete g;
}
