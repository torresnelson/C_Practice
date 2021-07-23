/*
	Módulo de definición de 'TMapping'.

	'TMapping' es un mapping de asociaciones de elementos de
	tipo nat con elementos de tipo double.

	Laboratorio de Programación 2.
	InCo-FIng-UDELAR
*/
#include "../include/listaInfo.h"
#include "../include/utils.h"
#include "../include/mapping.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
// Representación de 'TMapping'.
// Se debe definir en mappingInfo.cpp.
// struct repMap;
// Declaración del tipo 'TMapping'.
/*
	Estructura del "paquete" o Bundle donde va la .clave y el .valor
*/
struct bundle {
	double valor;
	nat clave;
};
/*
	Representacion de el mappig, que vendria siendo 
	el "casillero" donde van calzando los "paquetes"
	Estas casillas se llaman m->data[i] y cada m->data[i] tiene .valor y .clave
	m->data[i].valor, m->data[i].clave
*/
struct repMap {
	// el rango del arreglo es desde 0 hasta CAPACIDAD - 1
	bundle datos[MAX];
	// cantidad indica la cantidad de elementos
	//                = posición donde se insertaria el nuevo
	int cantidad;
};
/*  Operaciones de TMapping */
/*
	Devuelve un elemento de tipo 'TMapping' vacío (sin elementos).
*/
TMapping crearMapping(){
	TMapping m = new repMap;
	m->cantidad = 0;
	return m;
}
/* Función auxiliar. La palabra reservada 'static' establece que el ámbito de
	 la función es solo este archivo. */
/* Devuelve el indice en (*m).datos donde está clave, o -1 si no está. */
static int buscador(nat clave, TMapping m) {
	int i = m->cantidad - 1;
	while (i >= 0) {
		if (m->datos[i].clave != clave)
			i--;
		else
			break;
	}
	return i;
}
/*
	Devuelve 'true' si y solo si 'clave' tiene un valor asociado en 'map'.
*/
bool esClave(nat clave, TMapping map){ return buscador(clave, map) > -1; }
/*
	Si en 'map' hay menos de MAX (definido en utils.h) asociaciones y 'clave'
	no tiene un valor asociado en 'map' agrega en 'map' la asociación
	(clave,valor)
	Devuelve 'map'.
*/
TMapping asociar(nat clave, double valor, TMapping map) {
	if ((map->cantidad < MAX) && !esClave(clave, map)) {
//		assert(!esClave(clave, map) && (map->cantidad <= MAX));
		map->datos[map->cantidad].clave = clave;
		map->datos[map->cantidad].valor = valor;
		map->cantidad++;
	}
/*	
	printf("                       Asociar: [%u]\n", clave);
//	printf("                            Valor: [%.2f]\n", map->datos[buscador(clave, map)].valor);
	printf("                         Cantidad: %d\n", map->cantidad);
	for (int i = 0; i < map->cantidad; ++i){
		printf("[%u , %.2f] " , map->datos[i].clave, map->datos[i].valor);
	}
	printf("\n");
	for (int i = 0; i < map->cantidad; ++i){
		printf("-------------");
	}
	printf("\n");
*/	
	return map;
}
/*
	Devuelve el valor que en 'map' está asociado a 'clave'.
	Precondición: esClave(clave, map)
*/
double valor(nat clave, TMapping map){
	assert(esClave(clave, map));
	return map->datos[buscador(clave, map)].valor;
}
/*
	Si 'clave' tiene un valor asociado en 'map' remueve de `map' la asociación de
	'clave'.
	Devuelve 'map'.
*/
TMapping desasociar(nat clave, TMapping map){
//	printf("                     Desasociar: %u\n", clave);
	assert(esClave(clave, map));
	for (int i = buscador(clave, map); i < map->cantidad - 1; ++i)
		map->datos[i] = map->datos[i + 1];	
	map->cantidad--;
/*	printf("                            Valor: [%.2f]\n", map->datos[buscador(clave, map)].valor);
	printf("                         Cantidad: %d\n", map->cantidad);

	for (int i = 0; i < map->cantidad; ++i){
		printf("[%u , %.2f] " , map->datos[i].clave, map->datos[i].valor);
	}
	printf("\n");
	for (int i = 0; i < map->cantidad; ++i){
		printf("-------------");
	}
	printf("\n");
*/
	return map;	
}
