#include "../include/avl.h"
#include "../include/grafo.h"
#include "../include/cadena.h"
#include "../include/conjunto.h"
#include "../include/info.h"
#include "../include/iterador.h"
#include "../include/usoTads.h"
#include <float.h>
#include <assert.h>
#include <stdio.h>



// nueva
/*
  Devuelve un arreglo que indica a cuales vértices se puede llegar desde 'v'.
  El arreglo devuelto tiene tamaño N+1. La coordenada 0 no se tiene en cuenta.
  La coordenada 'i' (1 <= i <= N) es 'true' si y solo si el vértice 'i' es
  accesible desde 'v'.
  Precondición: 1 <= v <= N.

  Seudocódigo:
   - Inicializar cada coordenada del arreglo a devolver con 'false'
     (se considera cada vértice no accesible).
   - Pasar 'v' como parámetro a una función auxiliar. Esta función
     -- marca el parámetro como accesible desde 'v'.
     -- para cada vécino de su parámetro que todavía sea no accesible
        desde 'v' hace una llamada recursiva.
   - Devolver el arreglo.
 */
void accesiblesRecursive(nat v, TGrafo g, ArregloBools res) {
	res[v] = true;
	TIterador aux = vecinos(v, g);
	while (estaDefinidaActual(aux)) {
		nat actual = actualEnIterador(aux);
		if (res[actual] != true)
			accesiblesRecursive(actual, g, res);
		aux = avanzarIterador(aux);
	}
	liberarIterador(aux);
}
ArregloBools accesibles(nat v, TGrafo g) {
	assert(1 <= v && v <= cantidadVertices(g));
	ArregloBools nuevo = new bool[cantidadVertices(g) + 1];
	for (nat i = 0; i < cantidadVertices(g) + 1; ++i)
		nuevo[i] =  false;
	accesiblesRecursive(v, g, nuevo);
	return nuevo;
}

// nueva

/*
  Devuelve un arreglo con las longitudes de los caminos más cortos desde 'v'
  hasta cada vértice del grafo.
  El arreglo devuelto tiene tamaño N+1. La coordenada 0 no se tiene en cuenta.
  En la coordenada 'i' (1 <= i <= N) se devuelve la longitud del camino más
  corto desde 'v' hasta 'i'. Si 'i' no es accesible desde 'v' el valor de esa
  coordenada es DBL_MAX (definido en float.h)
  Precondición: 1 <= v <= N.

  Seudocódigo:
   - Crear colecciones 'C' y 'S', inicialmente vacías, de pares (u,du)
     donde 'u' representa un vértice y 'du' es la longitud del camino más
     corto desde 'v' hasta 'u'. El valor de 'du' en 'C' es provisorio
     mientras que en 'S' es definitivo.
   - Insertar (v,0) en 'C'.
   . Mientras 'C' no es vacía:
     -- se obtiene y remueve de 'C' el par (u, du) tal que 'du' es mínimo
        entre todos los pares de 'C'.
     -- se inserta ese par en 'S'.
     -- para cada vecino 'w' de 'u' que no está en S sea dw' = du + d(u,w),
        donde d(u,v) es la distancia entre 'u' y 'w'.
        Si 'w' no está en 'C' se inserta (w,dw') en 'C'.
        Si 'w' está en 'C' en el par (w,dw) y dw' < dw entoces se actualiza
        'C' con el par con (w,dw') en lugar de (w,dw).
   - Para cada vétice 'u' que no pertenece a 'S' se inserta en 'S' el par
     (u, infinito).
   - Devolver 'S'.

 */
void insertarOrdenadoCadena(TInfo info, TCadena cad){
	TLocalizador loc = inicioCadena(cad);
	if (esLocalizador(loc)){
		while (realInfo(info) < realInfo(infoCadena(loc, cad))){
			loc = siguiente(loc, cad);
		}
		cad = insertarAntes(info, loc, cad);
	} else {
		cad = insertarAlFinal(info, cad);
	}
}
ArregloDoubles longitudesCaminosMasCortos(nat v, TGrafo g) {
	ArregloDoubles s = new double[cantidadVertices(g) + 1];
	for (nat i = 1; i < cantidadVertices(g) + 1; ++i) {
		s[i] = DBL_MAX;
	}
	TCadena c = crearCadena();
	insertarOrdenadoCadena(crearInfo(v, 0), c);
	TLocalizador loc = inicioCadena(c);
	while (!esVaciaCadena(c) && esLocalizador(loc)) {
		loc = inicioCadena(c);
		nat v = natInfo(infoCadena(loc, c));
		s[v] = realInfo(infoCadena(loc, c));
		c = removerDeCadena(loc, c);
		TIterador vecino = vecinos(v, g);
		while (esLocalizador(loc) && estaDefinidaActual(vecino)) {
			nat actual = actualEnIterador(vecino);
			if (s[actual] == DBL_MAX){
				TLocalizador aux = siguienteClave(actual, inicioCadena(c), c);
				double dist = distancia(v, actual, g);
				if (aux && realInfo(infoCadena(aux, c)) < dist) {
					TInfo caja = crearInfo(actual, dist);
					c = removerDeCadena(aux, c);
					insertarOrdenadoCadena(caja, c);
				} else {
					insertarOrdenadoCadena(crearInfo(actual, dist), c);
				}
			}
			vecino = avanzarIterador(vecino);
		}
		liberarIterador(vecino);
	}
	liberarCadena(c);
	return s;
}


TConjunto interseccionDeConjuntos(TConjunto c1, TConjunto c2) {
	TConjunto conjuntoResultado = crearConjunto();
	TIterador iterC1, iterC2;
	iterC1 = iteradorDeConjunto(c1);
	iterC2 = iteradorDeConjunto(c2);
	bool definido = estaDefinidaActual(iterC1) && estaDefinidaActual(iterC2);
	if (definido) {
		while (definido) {
			if (definido && actualEnIterador(iterC1) < actualEnIterador(iterC2)) {
				iterC1 = avanzarIterador(iterC1);
			} else if (definido && actualEnIterador(iterC1) > actualEnIterador(iterC2)) {
				iterC2 = avanzarIterador(iterC2);
			} else if (definido && actualEnIterador(iterC1) == actualEnIterador(iterC2)) {
				TConjunto conjuntoResultadoAux = singleton(actualEnIterador(iterC1));
				conjuntoResultado = unionDeConjuntos(conjuntoResultadoAux, conjuntoResultado);
				iterC1 = avanzarIterador(iterC1);
				iterC2 = avanzarIterador(iterC2);
				liberarConjunto(conjuntoResultadoAux);
				conjuntoResultadoAux = NULL;
			}  
			definido = estaDefinidaActual(iterC1) && estaDefinidaActual(iterC2);
		}
	}
	liberarIterador(iterC1);
	liberarIterador(iterC2);
	return conjuntoResultado;
}

static nat nivelcad = 1;
static TCadena cadenaNivel(nat l, TBinario b, TCadena cad){
	if (!esVacioBinario(b)){
		if (!esVacioBinario(izquierdo(b))){
			nivelcad++;
			cad = cadenaNivel(l, izquierdo(b), cad);
			nivelcad--;
		}
		if (l == nivelcad){
			TInfo data = copiaInfo(raiz(b));
			cad = insertarAlFinal(data, cad);	
		}
		if (!esVacioBinario(derecho(b))){
			nivelcad++;
			cad = cadenaNivel(l, derecho(b), cad);
			nivelcad--;
		}
	}
	return cad;
}

TCadena nivelEnBinario(nat l, TBinario b) {
	TCadena cad = crearCadena();
	cad = cadenaNivel(l, b, cad);
	return cad;
}

nat longitud(TCadena cad){
	nat i = 0;
	TLocalizador loc = inicioCadena(cad);
	while (!esVaciaCadena(cad) && loc != NULL){
		++i;
		loc = siguiente(loc, cad);
	}
	return i;
}

bool esCamino(TCadena c, TBinario b){
	if ((longitud(c) > 0) && (b != 0) && (natInfo(infoCadena(inicioCadena(c), c)) == natInfo(raiz(b)))){
		if((longitud(c) == 1) && (alturaBinario(b) == 1) && (natInfo(infoCadena(inicioCadena(c), c)) == natInfo(raiz(b)))) 
			return true;
		
		TLocalizador loc = inicioCadena(c);
		c = removerDeCadena(loc, c);

		if ((longitud(c) > 0) && (natInfo(infoCadena(inicioCadena(c), c)) < natInfo(raiz(b))))
			return esCamino(c, izquierdo(b));
		else
			return esCamino(c, derecho(b));
	} else 
		return false;
}

bool pertenece(nat elem, TCadena cad) {
	TLocalizador loc = inicioCadena(cad);
	if (localizadorEnCadena(loc, cad) && natInfo(infoCadena(loc, cad)) != elem){
		loc = siguienteClave(elem, loc, cad);
	}
	return esLocalizador(loc);
}

bool estaOrdenadaPorNaturales(TCadena cad) {
	if (esVaciaCadena(cad)){
		return true;
	}else{
		bool cmp = true;
		TLocalizador loc = inicioCadena(cad);
		while(esLocalizador(siguiente(loc, cad)) && cmp) {
			cmp &= natInfo(infoCadena(loc, cad)) <= natInfo(infoCadena(siguiente(loc, cad), cad));
			loc = siguiente(loc, cad);
		}
		return cmp && esFinalCadena(loc, cad);
	}
}

bool hayNatsRepetidos(TCadena cad) {
	if (esVaciaCadena(cad) && longitud(cad) <= 1)
		return false;
	TLocalizador loc = inicioCadena(cad);
	bool cmp = esLocalizador(loc);
	while (!esFinalCadena(loc, cad)){
		TLocalizador cursor = siguiente(loc, cad);
		if (esFinalCadena(cursor, cad))
				cmp &= natInfo(infoCadena(loc, cad)) != natInfo(infoCadena(cursor, cad));
		while(cmp && !esFinalCadena(cursor, cad)){
			cmp &= natInfo(infoCadena(loc, cad)) != natInfo(infoCadena(cursor, cad));
			cursor = siguiente(cursor, cad);
			if (esFinalCadena(cursor, cad))
				cmp &= natInfo(infoCadena(loc, cad)) != natInfo(infoCadena(cursor, cad));
		}
		if (!cmp)
			return !cmp;
		loc = siguiente(loc, cad);
	}
	return !cmp;
}

bool sonIgualesCadena(TCadena c1, TCadena c2) {
	if (esVaciaCadena(c1) && esVaciaCadena(c2)){
		return true;
	}else{
		if (!esVaciaCadena(c1) && !esVaciaCadena(c2)){
			bool cmp = true;
			TLocalizador loc1 = inicioCadena(c1);
			TLocalizador loc2 = inicioCadena(c2);
			while(esLocalizador(loc1) && esLocalizador(loc2) && cmp) {
				cmp &= natInfo(infoCadena(loc1, c1)) ==  natInfo(infoCadena(loc2, c2)) && 
					  realInfo(infoCadena(loc1, c1)) == realInfo(infoCadena(loc2, c2));
				loc1 = siguiente(loc1, c1);
				loc2 = siguiente(loc2, c2);
			}
			return cmp && !esFinalCadena(loc1, c1) && !esFinalCadena(loc2, c2);
		}else 
			return false;
	}
}

TCadena concatenar(TCadena c1, TCadena c2) {
	TCadena copia_c1, copia_c2, res;
	copia_c1 = copiarSegmento(inicioCadena(c1), finalCadena(c1), c1);
	copia_c2 = copiarSegmento(inicioCadena(c2), finalCadena(c2), c2);
	res = insertarSegmentoDespues(copia_c2, finalCadena(copia_c1), copia_c1);
	return res;
}

TCadena ordenar(TCadena cad) {
	if (esVaciaCadena(cad)){
		return cad;
	}
	TLocalizador cursor = inicioCadena(cad);
	while (esLocalizador(cursor)){
		TLocalizador menor = menorEnCadena(cursor, cad);
		cad = intercambiar(menor, cursor, cad);
		cursor = siguiente(cursor, cad);
	}
	return cad;
}

TCadena cambiarTodos(nat original, nat nuevo, TCadena cad) {
	TLocalizador loc = inicioCadena(cad);
	while (esLocalizador(loc)){
		if (natInfo(infoCadena(loc, cad)) == original){
			double nreal = realInfo(infoCadena(loc, cad));
			TInfo aux = crearInfo(nuevo, nreal);
			TInfo orig = infoCadena(loc, cad);
			liberarInfo(orig);
			cambiarEnCadena(aux, loc, cad);
		} 
		loc = siguiente(loc, cad);
	}
	return cad;
}

TCadena subCadena(nat menor, nat mayor, TCadena cad) {
	assert(estaOrdenadaPorNaturales(cad) && menor <= mayor && pertenece(menor, cad) && pertenece(mayor, cad));
	TCadena res;
	TLocalizador locI = inicioCadena(cad);
	TLocalizador locF = finalCadena(cad);
	locI = siguienteClave(menor, locI, cad);
	locF = anteriorClave(mayor, locF, cad); 
	res = copiarSegmento(locI, locF, cad);
	return res;
}