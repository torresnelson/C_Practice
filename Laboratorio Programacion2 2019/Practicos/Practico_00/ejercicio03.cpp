/*Ejercicio 3 Ocurrencias
Se quiere implementar una función que cuente la cantidad de veces que una letra aparece en una frase.
La frase se representa como un arreglo de caracteres, y dado que se conoce que el largo máximo de una
frase es de 100 caracteres, la frase se implementa como char frase[100] .
Usando esta representación escriba una función Ocurrencias que recibe una frase, un natural llamado
largo que representa el número de caracteres en la frase, y el caracter a buscar (almacenado en la variable
letra), y devuelve el número de ocurrencias del carácter letra en el arreglo frase.*/

#include "stdio.h"

#define MAXCHR 100

bool ocurrencia(char entrada, char entrada_en_frase){
	if (entrada == entrada_en_frase){
		return true;
	}
	return false;
}

int main(){
	char letter;
	int counter, i;
	counter = 0;
	printf("ingrese la letra a buscar. \n");
	scanf("%c", &letter);
	char frase[MAXCHR] = {'f','r','a','s','e',' ','s','e',' ','r','e','p','r','e','s','e','n','t','a',' ','c','o','m','o',' ','u','n',' ','a','r','r','e','g','l','o',' ','d','e',' ','c','a','r','a','c','t','e','r','e','s'};
	printf("Letter: %c \n", letter);
	for (i = 0; i <= MAXCHR; i++){
		if (ocurrencia(letter,frase[i])){
			counter++;
		}
	}
	printf("---> %d numero de ocurrencias\n", counter);
}