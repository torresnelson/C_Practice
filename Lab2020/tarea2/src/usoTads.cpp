#include "../include/cadena.h"
#include "../include/utils.h"
#include "../include/info.h"
#include <assert.h>
#include <stdlib.h>

bool pertenece(nat elem, TCadena cad) {
	TLocalizador loc = inicioCadena(cad);
	if (localizadorEnCadena(loc, cad) && natInfo(infoCadena(loc, cad)) != elem){
		loc = siguienteClave(elem, loc, cad);
	}
	return esLocalizador(loc);
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

bool estaOrdenadaPorNaturales(TCadena cad){
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

bool hayNatsRepetidos(TCadena cad){
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

bool sonIgualesCadena(TCadena c1, TCadena c2){
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

TCadena concatenar(TCadena c1, TCadena c2){
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