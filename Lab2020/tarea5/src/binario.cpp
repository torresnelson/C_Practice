#include "../include/binario.h"
#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/utils.h"
#include <assert.h>
#include <stdio.h>

struct repBinario {
	TInfo dato;
	TBinario izq;
	TBinario der;
};

TBinario crearBinario(){ return NULL; }

int natcmp(nat n1, nat n2){
	int res;
	if (n1 == n2)
		res = 0;
	else if (n1 > n2)
		res = 1;
	else if (n1 < n2)
		res = -1;
	return res;
}
TBinario insertarEnBinario(TInfo i, TBinario b){
	assert(esVacioBinario(buscarSubarbol(natInfo(i), b)));
	if (b == NULL) {
		TBinario nuevo;
		nuevo = new repBinario;
		nuevo->dato = i;
		nuevo->izq = NULL;
		nuevo->der = NULL;
		b = nuevo;
	}else{
		int cmp;
		cmp = natcmp(natInfo(i), natInfo(b->dato));
		if (cmp < 0)
			b->izq = insertarEnBinario(i, b->izq);
		else if (cmp > 0)
			b->der = insertarEnBinario(i, b->der);
	}
	return b;
}

TInfo mayor(TBinario b){
	assert(!esVacioBinario(b));
	if (b->der == NULL)
		return b->dato;
	else 
		return mayor(b->der);
}

TBinario removerMayor(TBinario b){
	assert(!esVacioBinario(b));
	if (b->der == NULL){
		TBinario izq = b->izq;
		delete b;
		b = izq;
	}else{
		b->der = removerMayor(b->der);
	}
	return b;
}

TBinario removerDeBinario(nat elem, TBinario b){
	assert(!esVacioBinario(buscarSubarbol(elem, b)));

	int comp = natcmp(elem, natInfo(b->dato));
	if(b == NULL) return b;
	else if (comp < 0) 
		b->izq = removerDeBinario(elem, b->izq);
	else if (comp > 0)
		b->der = removerDeBinario(elem, b->der);
	else{
		if(b->der == NULL && b->izq == NULL){ 	//No child
			liberarInfo(b->dato);
			delete b;
			b = NULL;
		}else if (b->der == NULL){ 				//One child 
			TBinario temp = b;
			b = b->izq;
			liberarInfo(temp->dato);
			delete temp;
			temp = NULL;
		}else if (b->izq == NULL){
			TBinario temp = b;
			b= b->der;
			liberarInfo(temp->dato);
			delete temp;
			temp = NULL;
		}else{ 									//two child
			TBinario maxIzq = b->izq;
			while(maxIzq->der != NULL)
				maxIzq = maxIzq->der;
			liberarInfo(b->dato);
			b->dato = copiaInfo(maxIzq->dato);
			b->izq = removerDeBinario(natInfo(maxIzq->dato), b->izq);
		}
	}
	return b;
}

void liberarBinario(TBinario b){
	if (b != NULL){
		liberarBinario(izquierdo(b));
		liberarBinario(derecho(b));
		liberarInfo(raiz(b));
		delete b;
	}
}

bool esVacioBinario(TBinario b){ return b == NULL; }

static int absolute(int n) { return (n >= 0) ? (n) : (-n); } //Absolute value, if n >=0 then (n) else (-n) version golf
static nat maximo(nat n1, nat n2){ return (n1 >= n2) ? n1 : n2; } //Max value between to nats
static int recursiveEsAvl(TBinario nodo){
	if (nodo == NULL) return 0;

	int left = recursiveEsAvl(nodo->izq);
	if (left == -1) return -1;

	int right = recursiveEsAvl(nodo->der);
	if (right == -1) return -1;

	if (absolute(left - right) > 1) return -1;
	else return (1 + maximo(left, right));
}
bool esAvl(TBinario b){ return recursiveEsAvl(b) != -1; }

TInfo raiz(TBinario b){
	assert(!esVacioBinario(b));
	return b->dato;
}

TBinario izquierdo(TBinario b){
	assert(!esVacioBinario(b));
	return b->izq;
}

TBinario derecho(TBinario b){
	assert(!esVacioBinario(b));
	return b->der;
}

TBinario buscarSubarbol(nat elem, TBinario b){
	TBinario res;
	if (b == NULL)
		res = crearBinario();
	else {
		int cmp = natcmp(elem, natInfo(b->dato));
		if (cmp < 0)
			res = buscarSubarbol(elem, b->izq);
		else if (cmp > 0)
			res = buscarSubarbol(elem, b->der);
		else
			res = b;
	}
	return res;
}

nat alturaBinario(TBinario b){
	if (b == NULL)
		return 0;
	else
		return (1 + maximo(alturaBinario(b->izq), alturaBinario(b->der)));
}

nat cantidadBinario(TBinario b){
	if (b == NULL)
		return 0;
	else
		return (1 + cantidadBinario(b->izq) + cantidadBinario(b->der));
}

static double recursiveSumaUltimosPositivos(nat &i, TBinario b) {
	if(b) {
		double sum = 0;
		if(i > 0)
			sum += recursiveSumaUltimosPositivos(i, derecho(b));
		if(i > 0)
			if(realInfo(raiz(b)) > 0) {
				sum += realInfo(raiz(b));
				i--;
			}
		if(i > 0)
			sum += recursiveSumaUltimosPositivos(i, izquierdo(b));
		return sum;
	} else
		return 0;
}
double sumaUltimosPositivos(nat i, TBinario b){
	return recursiveSumaUltimosPositivos(i, b);
}

TCadena cadenaInOrder(TBinario b, TCadena res){
	if(b){
		if(b->izq)
			res = cadenaInOrder(b->izq, res);
		
		TInfo data = copiaInfo(b->dato);
		res = insertarAlFinal(data, res);

		if(b->der)
			res = cadenaInOrder(b->der, res);
	}
	return res;
}
TCadena linealizacion(TBinario b){
	TCadena res = crearCadena();
	res = cadenaInOrder(b, res);
	return res;
}

TBinario menores(double cota, TBinario b){
	if(b) {

		TBinario nb = NULL;
		TBinario left = menores(cota, b->izq);
		TBinario right = menores(cota, b->der);
		if(realInfo(raiz(b)) < cota) {
			nb = new repBinario;
			nb->dato = copiaInfo(b->dato);
			nb->izq = left;
			nb->der = right;
			return nb;
		}
		if (alturaBinario(b) > alturaBinario(left) && !right)
			return left;
		if(left){
			nb = new repBinario;
			nb->dato = mayor(left);
			nb->izq = removerMayor(left);
			nb->der = right;
			return nb;
		} else
			return right;
	} else
		return NULL;
}

static int nivel = 0;
void imprimirBinario(TBinario b){
	if (b){
		if (!nivel)
			printf("\n");
		if (b->der){
			nivel++;
			imprimirBinario(b->der);
			nivel--;
		}

		char *infostr = infoATexto(b->dato);
		for (int i = 1; i <= nivel; i++)
			printf("-");
		printf("%s\n", infostr);
		delete[] infostr;

		if (b->izq){
			nivel++;
			imprimirBinario(b->izq);
			nivel--;
		}
	}else
		printf("\n");
}