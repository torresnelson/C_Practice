/*Ejercicio 2 Primos
Escriba un procedimiento que calcule e imprima en pantalla todos los números primos entre dos enteros
positivos A y B cualesquiera.*/
#include "stdio.h"
bool primo(int num){
	int aux = 2;
	while (num % aux != 0 )
		aux++;
	return aux == num;
}
int main(){
	int n1, n2;
	printf("Ingrese dos numero separados.\n");
	scanf("%i" "%i",&n1, &n2);	
	for (int i = n1; i <= n2; i++)
		if (primo(i))
			printf("%i ", i);
	
	printf("\n");
}