#include "../include/colaInfo.h"
#include "../include/listaInfo.h"
#include "../include/utils.h"
#include <assert.h>
#include <stdlib.h>
// Representación de 'TCola'.
// Se debe definir en colaInfo.cpp.
// struct repCola;
// Declaración del tipo 'TCola'.
struct repCola {
	TLista lst;
};
/*	Operaciones de TCola
	Devuelve un elemento de tipo 'TCola' vacío (sin elementos).
 */
TCola crearCola() {
/* 
	Se obtiene memoria para la repr de la cola
	(*resultado) es un registro de tipo repCola.
	Su unico campo es lst, que debe ser inicializado.  
	De manera alternativa se puede usar el operador '->': resultado->lst = crearLista();
*/
	TCola resultado = new repCola;
	resultado->lst = crearLista();
	return resultado;
}
/*
	Devuelve 'true' si y solo si 'cola' no tiene elementos.
 */
bool esVaciaCola(TCola cola) {
	return longitud(cola->lst) == 0;
}
/*
	Si en 'cola' hay menos de MAX (definido en utils.h) elementos encola
	'nuevo' en 'cola'.
	Devulve 'cola'.
 */
TCola encolar(info_t nuevo, TCola cola) {
	assert(longitud(cola->lst) < MAX + 1);
	cola->lst = insertar(longitud(cola->lst) + 1, nuevo, cola->lst);
	return cola;
}
	/*
	Devuelve el frente de 'cola'.
	Precondición: 'cola' tiene elementos.
 */
info_t frente(TCola cola) {
	assert(!esVaciaCola(cola));
	return infoLista(1, cola->lst);
}
/*
	Si 'cola' tiene elementos desapila el frente de 'cola'.
	Devulve 'cola'.
 */
TCola desencolar(TCola cola) {
	cola->lst = remover(1, cola->lst);
	return cola;
}