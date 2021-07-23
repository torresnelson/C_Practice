#include "../include/colaDePrioridad.h"
#include "../include/info.h"
#include <assert.h>
#include <stdio.h>

struct repCP {
	TInfo *dato;
	nat *valorNat;
	nat tamanio;
	nat maxValor;
	nat cantidadActual;
};

TColaDePrioridad crearCP(nat N) {
	TColaDePrioridad nuevo = new repCP;
	nuevo->dato = new TInfo[N + 1];
	nuevo->valorNat = new nat[N + 1];
	nuevo->tamanio = N;
	nuevo->maxValor = N;
	nuevo->cantidadActual = 0;
	for (nat i = 0; i < nuevo->tamanio + 1; i++){
		nuevo->dato[i] = NULL;
		nuevo->valorNat[i] = 0;
	}
	return nuevo;
}

bool estaVaciaCP(TColaDePrioridad cp) { return cp->cantidadActual == 0; }

bool estaLlenaCP(TColaDePrioridad cp){ return cp->tamanio == cp->cantidadActual; }

nat rangoCP(TColaDePrioridad cp) { return cp->maxValor; }

nat hijoIzquierdo(nat i){ return (2*i); }

nat hijoDerecho(nat i){ return ((2*i) + 1); }

nat padre(nat i){ return (i/2); }

bool comparar(nat dato1, nat dato2, TColaDePrioridad cp){ return realInfo(cp->dato[dato1]) < realInfo(cp->dato[dato2]); }

void intercambiarNats(nat x, nat y, TColaDePrioridad cp){
	TInfo aux;
	nat auxIndicesValorNat;
	nat val1 = natInfo(cp->dato[x]);
	nat val2 = natInfo(cp->dato[y]);
	aux = cp->dato[x];
	cp->dato[x] = cp->dato[y];
	cp->dato[y] = aux;
	auxIndicesValorNat = cp->valorNat[val1];
	cp->valorNat[val1] = cp->valorNat[val2];
	cp->valorNat[val2] = auxIndicesValorNat;
}

TColaDePrioridad insertarEnCP(nat elem, double valor, TColaDePrioridad cp) {
	assert(!estaLlenaCP(cp) && !estaEnCP(elem,cp) && (elem <= rangoCP(cp)));
	TInfo nuevo = crearInfo(elem, valor);
	nat indiceNuevo =  cp->cantidadActual + 1;
	cp->valorNat[elem] = indiceNuevo;

	if (cp->cantidadActual > 0){
		nat indicePadre = padre(indiceNuevo);
		cp->dato[indiceNuevo] = nuevo;
		while (indicePadre > 0 && comparar(indiceNuevo, indicePadre, cp)){
			intercambiarNats(indicePadre, indiceNuevo, cp);
			indiceNuevo = indicePadre;
			indicePadre = padre(indiceNuevo);
		}
	} else
		cp->dato[indiceNuevo] = nuevo;
		
	cp->cantidadActual++;

	return cp;
}

nat prioritario(TColaDePrioridad cp) {
	assert(!estaVaciaCP(cp)); 
	return natInfo(cp->dato[1]);
}

TColaDePrioridad eliminarPrioritario(TColaDePrioridad cp) {
	assert(!estaVaciaCP(cp));
	nat aux = 1;
	nat indiceUltimo = cp->cantidadActual;

	intercambiarNats(aux, indiceUltimo, cp);
	cp->valorNat[natInfo(cp->dato[indiceUltimo])] = 0;
	liberarInfo(cp->dato[indiceUltimo]);

	cp->dato[indiceUltimo] = NULL;

	nat saltar = -1;
	while (saltar != 0 && aux < indiceUltimo) {
		if (hijoIzquierdo(aux) < cp->cantidadActual) 
			if (hijoDerecho(aux) < cp->cantidadActual)
				if (comparar(hijoDerecho(aux), hijoIzquierdo(aux), cp))
					saltar = hijoDerecho(aux);
				else
					saltar = hijoIzquierdo(aux);
			else
				if (comparar(hijoIzquierdo(aux), aux, cp))
					saltar = hijoIzquierdo(aux);
				else
					saltar = 0;
		else
			if (hijoDerecho(aux) <= cp->cantidadActual)
				if (comparar(hijoDerecho(aux), aux, cp))
					saltar = hijoDerecho(aux);
				else
					saltar = 0;
			else 
				saltar = 0;
		if (saltar != 0)
			intercambiarNats(saltar, aux, cp);
		aux = saltar; 
	}

	cp->cantidadActual--;
	return cp;
}

bool estaEnCP(nat elem, TColaDePrioridad cp) {
	return cp->valorNat[elem] != 0;
}

double prioridad(nat elem, TColaDePrioridad cp) {
	assert(estaEnCP(elem, cp));
	return realInfo(cp->dato[cp->valorNat[elem]]);
}

TColaDePrioridad actualizarEnCP(nat elem, double valor, TColaDePrioridad cp) {
	assert(estaEnCP(elem, cp));
	nat i = 1;
	
	while (natInfo(cp->dato[i]) != elem)
		i++;

	if (i == 1){
		cp = eliminarPrioritario(cp);
		cp = insertarEnCP(elem, valor, cp);
	} else {
		cp->valorNat[natInfo(cp->dato[i])] = 0;
		TInfo nuevo = crearInfo(natInfo(cp->dato[cp->cantidadActual]), realInfo(cp->dato[cp->cantidadActual]));
		liberarInfo(cp->dato[i]);
		cp->dato[i] = nuevo;
		liberarInfo(cp->dato[cp->cantidadActual]);
		cp->cantidadActual--;		
		cp = insertarEnCP(elem, valor, cp);

	}
	return cp;
}

void liberarCP(TColaDePrioridad cp) {
	for (nat i = 1; i < cp->cantidadActual + 1; i++) {
		liberarInfo(cp->dato[i]);   
	}
	delete[] cp->dato;
	delete[] cp->valorNat;
	delete cp;
}