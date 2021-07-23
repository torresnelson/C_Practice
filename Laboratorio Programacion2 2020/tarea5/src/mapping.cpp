#include "../include/utils.h"
#include "../include/mapping.h"
#include "../include/info.h"
#include <assert.h>
#include <stdio.h>

struct repMap {
	TInfo *dato;
	nat *valorNat;
	nat cantidadActual;
	nat tamanio;
};

TMapping crearMap(nat M) {
	TMapping map = new repMap;
	map->tamanio = M;
	nat MAXNAT = (map->tamanio + 1) * 10;
	map->dato = new TInfo[MAXNAT];
	map->valorNat = new nat[MAXNAT];
	map->cantidadActual = 0;
	for (nat i = 0; i < MAXNAT; i++) {
		map->dato[i] = NULL;
		map->valorNat[i] = 0;
	}
	return map;
}

TMapping asociarEnMap(nat clave, double valor, TMapping map) {
	map->cantidadActual++;
	map->dato[clave] = crearInfo(clave, valor);
	map->valorNat[clave] = map->cantidadActual;		
	return map;
}

TMapping desasociarEnMap(nat clave, TMapping map) {
	assert(existeAsociacionEnMap(clave, map));
	map->cantidadActual--;
	map->valorNat[clave] = 0;
	liberarInfo(map->dato[clave]);
	map->dato[clave] = NULL;
	return map;	
}

bool existeAsociacionEnMap(nat clave, TMapping map){ return map->valorNat[clave] != 0; }

double valorEnMap(nat clave, TMapping map) {
	return realInfo(map->dato[clave]);
}

bool estaLlenoMap(TMapping map) { return map->tamanio == map->cantidadActual; }

void liberarMap(TMapping map) {
	nat MAXNAT = (map->tamanio + 1) * 10;
	for (nat i = 0; i < MAXNAT; i++) 
		liberarInfo(map->dato[i]);
	delete[] map->dato;
	delete[] map->valorNat;
	delete map;	
}