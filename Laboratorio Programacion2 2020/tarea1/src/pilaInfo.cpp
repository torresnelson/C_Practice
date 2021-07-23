#include "../include/pilaInfo.h"
#include "../include/listaInfo.h"
#include "../include/utils.h"

#include <assert.h>
#include <stdlib.h>

//Se define la representacion de TPila.
//Se usan TLista y sus operaciones.
struct repPila {
	TLista lst;
};

/* Se consider que la cima de la pila es el ultimo elemento de la losta con la 
que se implementa, o sea, el que esta en la posicion longitud de lista.
Esa es la posicion pasada como parametro al implementar 'cima' y 'desacoplar'
en las llamadas a las operaciones de listaInfo 'infoLista' y 'remover'.
En 'apilar' se pasa la posicion longitud + 1 como parametro de la operacion
'insertar'
*/

/*
  Devuelve un elemento de tipo 'TPila' vacío (sin elementos).
 */

TPila crearPila(){
	//Se obtiene memoria para la repr de la pila
	TPila resultado = new repPila; //C++
	// (*resultado) es un registro de tipo repPila.
	// Su unico campo es lst, que debe ser inicializado. 
	(*resultado).lst = crearLista();
	// De manera alternativa se puede usar el operador '->':
	// 							resultado->lst = crearLista();
	return resultado;
}

/*
  Devuelve 'true' si y solo si 'pila' no tiene elementos.
 */
bool esVaciaPila(TPila p) {
	return longitud((*p).lst) == 0;
}

/*
  Si en 'pila' hay menos de MAX (definido en utils.h) elementos apila
  'nuevo' en 'pila'.
  Devuelve 'pila'.
 */
TPila apilar(info_t nuevo, TPila p) {
	(*p).lst = insertar(longitud((*p).lst) + 1, nuevo, (*p).lst);
	return p;
}

/*
  Devuelve el tope de 'pila'.
  Precondición: 'pila' tiene elementos.
 */
info_t cima(TPila p) {
	assert(!esVaciaPila(p));
	return infoLista(longitud((*p).lst), (*p).lst);
}

/*
  Si 'pila' tiene elementos desapila el tope de 'pila'.
  Devuelve 'pila'.
 */
TPila desapilar(TPila p) {
	(*p).lst = remover(longitud((*p).lst), (*p).lst);
	return p;
}