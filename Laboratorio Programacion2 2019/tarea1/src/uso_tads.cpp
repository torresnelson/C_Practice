#include "../include/uso_tads.h"
#include "../include/pila.h"
#include "../include/mapping.h"
#include "cstdio"

mapping_t parejas;
pila_t stack;

static void construc_map(){
	parejas = crear_map();
	parejas = asociar('<', '>', parejas);
	parejas = asociar('(', ')', parejas);
	parejas = asociar('{', '}', parejas);
	parejas = asociar('[', ']', parejas);
}

char apertura[4] = {'<', '(', '{', '['};
char cierre[4] = {'>', ')', '}', ']'};

int es_apertura(char caracter){
	for (int i = 0 ; i < 4 ; i++){
		if (caracter == apertura[i]){
			return i;
		}
	}
	return -1;
}

int es_cierre(char caracter){
	for (int i = 0 ; i < 4 ; i++){
		if (caracter == cierre[i]){
			return i;
		}
	}
	return -1;
}

bool es_char(char caracter){
	if (es_apertura(caracter) == -1 && es_cierre(caracter) == -1){
		return true;
	}
	return false;	
}

bool es_balanceado(char expresion[MAX_EXP], int n){
	int i = 0;
	construc_map();
	stack = crear_pila();
	while (i < n){
		if (es_char(expresion[i])){
			i++;
		}
		if (es_apertura(expresion[i]) != -1){ 
			apilar(expresion[i],stack); // si el char es un simbolo de apertura, apila en el stack
		}
		if (es_cierre(expresion[i]) != -1 && es_vacia_pila(stack)){ 
			liberar_map(parejas);
			liberar_pila(stack);
			return false; // si el char es un simbolo de cierre y la pila esta vacia, retorna false 
		}
		if (es_cierre(expresion[i]) != -1 && !es_vacia_pila(stack) && (valor(tope(stack), parejas) == cierre[es_cierre(expresion[i])])){ 
			desapilar(stack); //si el char es un cierre y el stack tiene elementos y el cierre es el correspondiente al tope del stack, entonces desapila el tope del stack.
		}
		i++;
	}
	if ((i >= n) && es_vacia_pila(stack)){
		liberar_map(parejas);
		liberar_pila(stack);
		return true;
	}
	liberar_pila(stack);
	liberar_map(parejas);
	return false;
}