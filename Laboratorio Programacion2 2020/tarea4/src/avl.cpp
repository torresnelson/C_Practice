#include "../include/iterador.h"
#include "../include/utils.h"
#include "../include/avl.h"
#include <assert.h>
#include <stdio.h>

struct repAvl {
	nat dato;
	nat altura;
	nat cantidad;
	TAvl izq, der;
};

TAvl crearAvl() { 
	return NULL; 
}

bool estaVacioAvl(TAvl avl) { 
	return avl == NULL; 
}

nat raizAvl(TAvl avl) { 
	assert(!estaVacioAvl(avl));
	return avl->dato; 
}

TAvl izqAvl(TAvl avl) { 
	assert(!estaVacioAvl(avl));
	return avl->izq; 
}

TAvl derAvl(TAvl avl) { 
	assert(!estaVacioAvl(avl));
	return avl->der; 
}

nat cantidadEnAvl(TAvl avl) {
	if (avl == NULL) {
	 	return 0;
	} else {
		return avl->cantidad; 
	}
}

nat alturaDeAvl(TAvl avl) { 
	if (avl == NULL) {
	 	return 0;
	} else {
		return avl->altura; 
	}
}

TAvl buscarEnAvl(nat elem, TAvl avl) {
	TAvl res;
	if (estaVacioAvl(avl))
		res = crearAvl();
	else {
		if (elem < avl->dato)
			res = buscarEnAvl(elem, avl->izq);
		else if (elem > avl->dato)
			res = buscarEnAvl(elem, avl->der);
		else
			res = avl;
	}
	return res;
}

nat maximo(nat n1, nat n2){ return (n1 >= n2) ? n1 : n2; }
TAvl derRotate(TAvl y) { 
	TAvl x = y->izq; 
	TAvl T2 = x->der; 
	x->der = y; 
	y->izq = T2; 
	y->altura = maximo(alturaDeAvl(y->izq), alturaDeAvl(y->der)) + 1; 
	y->cantidad = (cantidadEnAvl(y->izq) + cantidadEnAvl(y->der)) + 1;
	x->altura = maximo(alturaDeAvl(x->izq), alturaDeAvl(x->der)) + 1;
	x->cantidad = (cantidadEnAvl(x->izq) + cantidadEnAvl(x->der)) + 1;
	return x; 
}
TAvl izqRotate(TAvl x) { 
	TAvl y = x->der; 
	TAvl T2 = y->izq; 
	y->izq = x; 
	x->der = T2; 
	x->altura = maximo(alturaDeAvl(x->izq), alturaDeAvl(x->der)) + 1; 
	x->cantidad = (cantidadEnAvl(x->izq) + cantidadEnAvl(x->der)) + 1;				 
	y->altura = maximo(alturaDeAvl(y->izq), alturaDeAvl(y->der)) + 1;
	y->cantidad = (cantidadEnAvl(y->izq) + cantidadEnAvl(y->der)) + 1;
	return y; 
} 
int getBalance(TAvl avl) {  
	if (avl == NULL)  
		return 0; 
	else
		return alturaDeAvl(avl->izq) - alturaDeAvl(avl->der);  
} 
void insertarEnAvlRecursive(nat elem, TAvl &avl) {
	if (avl == NULL) {
		avl = new repAvl;
		avl->dato = elem;
		avl->der = avl->izq = NULL;
		avl->altura = 1;
		avl->cantidad = 1;
		return;
	}
	if (elem < avl->dato) 
		insertarEnAvlRecursive(elem, avl->izq); 
	else if (elem > avl->dato) 
		insertarEnAvlRecursive(elem, avl->der);
	

	avl->altura = (1 + maximo(alturaDeAvl(avl->izq), alturaDeAvl(avl->der)));
	avl->cantidad = (1 + cantidadEnAvl(avl->izq) + cantidadEnAvl(avl->der));
	int balance = getBalance(avl); 

	// Left Left Case
	if (balance > 1 && elem < avl->izq->dato) {
		avl = derRotate(avl); 
		return;
	}
	// Right Right Case
	if (balance < -1 && elem > avl->der->dato) {
		avl = izqRotate(avl); 
		return;
	}
	// Left Right Case
	if (balance > 1 && elem > avl->izq->dato) {
		avl->izq = izqRotate(avl->izq); 
		avl = derRotate(avl); 
		return;
	} 
	// Right Left Case
	if (balance < -1 && elem < avl->der->dato) {
		avl->der = derRotate(avl->der); 
		avl = izqRotate(avl); 
		return;
	} 
}
TAvl insertarEnAvl(nat elem, TAvl avl) {
	assert(estaVacioAvl(buscarEnAvl(elem, avl)));
	insertarEnAvlRecursive(elem, avl);
	return avl;
}

static TIterador enOrdenRecursive(TIterador res, TAvl avl) {
	if (!estaVacioAvl(avl)) {
		res = enOrdenRecursive(res, izqAvl(avl));
		res = agregarAIterador(raizAvl(avl), res);
		res = enOrdenRecursive(res, derAvl(avl));
	}

	return res;
}
TIterador enOrdenAvl(TAvl avl) {
	TIterador res;
	if (estaVacioAvl(avl))
		res  = crearIterador();
	else {
		res = crearIterador();
		res = enOrdenRecursive(res, avl);
		res = reiniciarIterador(res);
	}
	return res;
}

static TAvl a2AvlRecursive(nat *elems, int inf, int sup) {
	TAvl res;
	if (inf > sup)
		res = NULL;
	else {
		nat medio = (inf + sup) / 2;
		res = new repAvl;
		res->dato = elems[medio];
		res->izq = a2AvlRecursive(elems, inf, medio - 1);
		res->der = a2AvlRecursive(elems, medio + 1, sup);
		// ajustar res->altura y res->cantidad
		res->altura = (1 + maximo(alturaDeAvl(res->izq), alturaDeAvl(res->der)));
		res->cantidad = (1 + cantidadEnAvl(res->izq) + cantidadEnAvl(res->der));
	
	}
	return res;
}
TAvl arregloAAvl(nat *elems, nat n) {
	return a2AvlRecursive(elems, 0, n - 1);
}

struct avlUltimo {
	TAvl avl;
	int ultimo;
};
static avlUltimo avlMinRecursive(nat h, nat primero) {
	avlUltimo res;
	if (h == 0) {
		res.avl = NULL;
		res.ultimo = primero - 1;
	} else if (h == 1) {
		res.avl = new repAvl;
		res.ultimo = primero;
		nat hoja = primero;
		res.avl->dato = hoja;
		res.avl->cantidad = 1;
		res.avl->altura = 1;
		res.avl->izq = NULL;
		res.avl->der = NULL;
	} else {
		avlUltimo auxDer, auxIzq;
		
		auxIzq = avlMinRecursive(h - 1, primero);
	    auxDer = avlMinRecursive(h - 2, auxIzq.ultimo + 2);
		
		res.avl = new repAvl;
		res.avl->izq = auxIzq.avl;
		res.avl->der = auxDer.avl;

		res.avl->cantidad = (1 + cantidadEnAvl(auxIzq.avl) + cantidadEnAvl(auxDer.avl));
		res.avl->altura = h;

		res.avl->dato = auxIzq.ultimo + 1;

		res.ultimo = auxDer.ultimo;
	}
	return res;
}
TAvl avlMin(nat h) {
	avlUltimo res = avlMinRecursive(h, 1);
	return res.avl;
}

static void imprimirNivelAvl(TAvl avl, nat level) { 
	if (avl == NULL) 
		return; 
	if (level == 1) 
		printf("%d ", avl->dato); 
	else if (level > 1) { 
		imprimirNivelAvl(avl->izq, level - 1); 
		imprimirNivelAvl(avl->der, level - 1); 
	} 
} 
void imprimirAvl(TAvl avl) { 
	nat h = alturaDeAvl(avl); 
	for (int i = h; i >= 1; i--) { 
		imprimirNivelAvl(avl, i); 
		printf("\n"); 
	} 
}

void liberarAvl(TAvl avl) {
	if (avl != NULL) {
		liberarAvl(avl->izq);
		liberarAvl(avl->der);
		delete avl;
	}
}