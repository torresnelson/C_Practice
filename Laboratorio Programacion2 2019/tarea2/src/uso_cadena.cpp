#include "info.h"
#include "cadena.h"

#include "stddef.h"
#include "stdio.h"
#include "assert.h"
#include <string.h>

/*
	Devuelve `true' si y sólo si en `cad' hay un elemento cuyo dato numérico es
	`i'.
 */
bool pertenece(int i, cadena_t cad){
	localizador_t loc = inicio_cadena(cad);
	if (localizador_en_cadena(loc, cad) && numero_info(info_cadena(loc, cad)) != i){
		loc = siguiente_clave(i, loc, cad);
	}
	return es_localizador(loc);

}

/*  Devuelve la cantidad de elementos de `cad'. */
nat longitud(cadena_t cad){
	nat i = 0;
	localizador_t loc = inicio_cadena(cad);
	while (!es_vacia_cadena(cad) && loc != NULL){
		++i;
		loc = siguiente(loc, cad);
	}
	return i;
}

/*
	Devuelve `true' si y sólo si `cad' está ordenada de forma no decreciente
	(= creciente de manera no estricta) según los datos numéricos de sus
	elementos.
	Si es_vacia_cadena (cad) devuelve `true'.
 */
bool esta_ordenada(cadena_t cad){
	if (es_vacia_cadena(cad)){
		return true;
	}else{
		bool cmp = true;
		localizador_t loc = inicio_cadena(cad);
		while(es_localizador(siguiente(loc, cad)) && cmp) {
			cmp &= numero_info(info_cadena(loc, cad)) <= numero_info(info_cadena(siguiente(loc, cad), cad));
			loc = siguiente(loc, cad);
		}
		return cmp && es_final_cadena(loc, cad);
	}
}
/*
	Devuelve el resultado de concatenar `c2' después de `c1'.
	La cadena_t resultado no comparte memoria ni con `c1' ni con `c2'.
 */
cadena_t concatenar(cadena_t c1, cadena_t c2){
	cadena_t copia_c1, copia_c2, res;
	copia_c1 = copiar_segmento(inicio_cadena(c1), final_cadena(c1), c1);
	copia_c2 = copiar_segmento(inicio_cadena(c2), final_cadena(c2), c2);
	res = insertar_segmento_despues(copia_c2, final_cadena(copia_c1), copia_c1);
	liberar_cadena(copia_c2);
	return res;
}

/*
	Se ordena `cad' de manera creciente según los datos numéricos de sus
	elementos.
	Devuelve `cad'.
	No se debe obtener ni devolver memoria de manera dinámica.
	Si es_vacia_cadena (cad) no hace nada.
 */
cadena_t ordenar(cadena_t cad){
	if (es_vacia_cadena(cad)){
		return cad;
	}
	localizador_t cursor = inicio_cadena(cad);
	while (es_localizador(cursor)){
		localizador_t menor = menor_en_cadena(cursor, cad);
		cad = intercambiar(menor, cursor, cad);
		cursor = siguiente(cursor, cad);
	}
	return cad;
}

/*
	Devuelve una cadena_t con todos los elementos de `c1' y `c2' ordenada de
	manera  creciente segun sus datos numéricos.
	Si hay elementos cuyos datos numéricos son iguales los elementos de `c1'
	quedan antes de los de `c2'.
	La cadena_t resultado no comparte memoria ni con `c1' ni con `c2'.
	Precondición: esta_ordenada (c1) y esta_ordenada (c2).
 */
cadena_t mezcla(cadena_t c1, cadena_t c2){
	assert(esta_ordenada(c1) && esta_ordenada(c2));
	cadena_t res = crear_cadena();
	localizador_t iter1 = inicio_cadena(c1);
	localizador_t iter2 = inicio_cadena(c2);
	while(es_localizador(iter1) || es_localizador(iter2)) {
		if(es_localizador(iter1) && es_localizador(iter2)) {
			info_t info1 = info_cadena(iter1, c1);
			info_t info2 = info_cadena(iter2, c2);
			while(es_localizador(iter1) && 
				numero_info(info1) <= numero_info(info2)) {
				info1 = info_cadena(iter1, c1);
				//printf("%d: %d <= %d \n", numero_info(info1) <= numero_info(info2), numero_info(info1), numero_info(info2));
				insertar_al_final(copia_info(info1), res);
				//imprimir_cadena(res);
				iter1 = siguiente(iter1, c1);
				if(es_localizador(iter1))
					info1 = info_cadena(iter1, c1);
			}
			insertar_al_final(copia_info(info2), res);
			iter2 = siguiente(iter2, c2);
		} else {
			while(es_localizador(iter1)) {
				info_t info1 = info_cadena(iter1, c1);
				insertar_al_final(copia_info(info1), res);
				iter1 = siguiente(iter1, c1);
			}
			while(es_localizador(iter2)) {
				info_t info2 = info_cadena(iter2, c2);
				insertar_al_final(copia_info(info2), res);
				iter2 = siguiente(iter2, c2);
			}
		}
	}
	return res;
}

/*
	Cambia todas las ocurrencias de `original' por `nuevo' en los elementos
	 de `cad'. No debe quedar memoria inaccesible.
	Devuelve `cad'.
 */
cadena_t cambiar_todos(int original, int nuevo, cadena_t cad){
	localizador_t loc = inicio_cadena(cad);
	while (es_localizador(loc)){
		if (numero_info(info_cadena(loc, cad)) == original){
			info_t old = info_cadena(loc,cad);
			char* nfrase = new char[strlen(frase_info(old))+ 1];
			strcpy(nfrase, frase_info(old));
			info_t aux = crear_info(nuevo, nfrase);
			info_t orig = info_cadena(loc, cad);
			liberar_info(orig);
			cambiar_en_cadena(aux, loc, cad);
		} 
		loc = siguiente(loc, cad);
	}
	return cad;
} 