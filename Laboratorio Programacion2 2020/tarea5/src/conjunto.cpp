#include "../include/avl.h"
#include "../include/cadena.h"
#include "../include/conjunto.h"
#include "../include/info.h"
#include <stdio.h>

struct repConjunto {
	TAvl arbol;
};

TConjunto crearConjunto() {
	TConjunto res = new repConjunto;
	res->arbol = crearAvl();
	return res;
}

TConjunto singleton(nat elem) {
	TConjunto res = crearConjunto();
  res->arbol = insertarEnAvl(elem, res->arbol);
	return res;
}

bool perteneceConjunto(nat elem, TCadena cad) {
  TLocalizador loc = inicioCadena(cad);
  if (localizadorEnCadena(loc, cad) && natInfo(infoCadena(loc, cad)) != elem){
    loc = siguienteClave(elem, loc, cad);
  }
  return esLocalizador(loc);
}

TConjunto unionDeConjuntos(TConjunto c1, TConjunto c2) {
  TConjunto conjuntoResultado = crearConjunto();
  TIterador iterC1, iterC2;
  iterC1 = enOrdenAvl(c1->arbol);
  iterC2 = enOrdenAvl(c2->arbol);
  while (estaDefinidaActual(iterC1) || estaDefinidaActual(iterC2)) {
    bool definido = estaDefinidaActual(iterC1) && estaDefinidaActual(iterC2);
    if (definido && actualEnIterador(iterC1) < actualEnIterador(iterC2)) {
      conjuntoResultado->arbol = insertarEnAvl(actualEnIterador(iterC1), conjuntoResultado->arbol);
      iterC1 = avanzarIterador(iterC1);
    } else if (definido && actualEnIterador(iterC1) > actualEnIterador(iterC2)) {
      conjuntoResultado->arbol = insertarEnAvl(actualEnIterador(iterC2), conjuntoResultado->arbol);
      iterC2 = avanzarIterador(iterC2);
    } else if (definido && actualEnIterador(iterC1) == actualEnIterador(iterC2)) {
      conjuntoResultado->arbol = insertarEnAvl(actualEnIterador(iterC1), conjuntoResultado->arbol);
      iterC1 = avanzarIterador(iterC1);
      iterC2 = avanzarIterador(iterC2);
    } else if (!definido) {
      while (estaDefinidaActual(iterC1)) {
        conjuntoResultado->arbol = insertarEnAvl(actualEnIterador(iterC1), conjuntoResultado->arbol);
        iterC1 = avanzarIterador(iterC1);
      }
      while (estaDefinidaActual(iterC2)) {
        conjuntoResultado->arbol = insertarEnAvl(actualEnIterador(iterC2), conjuntoResultado->arbol);
        iterC2 = avanzarIterador(iterC2);
      }
    }
  }
  liberarIterador(iterC1);
  liberarIterador(iterC2);
  return conjuntoResultado;
}

TConjunto diferenciaDeConjuntos(TConjunto c1, TConjunto c2) {
	TConjunto res = crearConjunto();
  TAvl resultado = crearAvl();
  TIterador iterC1, iterC2;
  iterC1 = enOrdenAvl(c1->arbol);
  iterC2 = enOrdenAvl(c2->arbol);
  while (estaDefinidaActual(iterC1)) {
    if (estaDefinidaActual(iterC2) && actualEnIterador(iterC1) < actualEnIterador(iterC2)) {
      resultado = insertarEnAvl(actualEnIterador(iterC1), resultado);
      iterC1 = avanzarIterador(iterC1);
    } else if (estaDefinidaActual(iterC2) && actualEnIterador(iterC1) > actualEnIterador(iterC2)) {
      iterC2 = avanzarIterador(iterC2);        
    } else if (estaDefinidaActual(iterC2) && actualEnIterador(iterC1) == actualEnIterador(iterC2)) {
      iterC1 = avanzarIterador(iterC1);
      iterC2 = avanzarIterador(iterC2);
    } else if (!estaDefinidaActual(iterC2)) {
      while (estaDefinidaActual(iterC1)) {
        resultado = insertarEnAvl(actualEnIterador(iterC1), resultado);
        iterC1 = avanzarIterador(iterC1);
      }
    }
  }
  liberarIterador(iterC1);
  liberarIterador(iterC2);
  res->arbol = resultado;
	return res;
}

bool perteneceAConjunto(nat elem, TConjunto c) { 
	TAvl nodo = buscarEnAvl(elem, c->arbol);
	return nodo && (raizAvl(nodo) == elem);
}

bool estaVacioConjunto(TConjunto c) { return estaVacioAvl(c->arbol); }

nat cardinalidad(TConjunto c) { return cantidadEnAvl(c->arbol); }

TConjunto arregloAConjunto(nat *elems, nat n){
	TConjunto res = crearConjunto();
  res->arbol = arregloAAvl(elems, n);
	return res;
}

TIterador iteradorDeConjunto(TConjunto c) {
	TIterador res;
  res = enOrdenAvl(c->arbol);
	return res;
}

void liberarConjunto(TConjunto c)  {
	if (c == NULL) 
		return;
  else {
    liberarAvl(c->arbol);
    c->arbol = NULL;
  }
	delete c;
}