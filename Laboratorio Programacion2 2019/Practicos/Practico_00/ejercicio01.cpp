#include "stdio.h"
#include "math.h"

float PromClase(float notas[]) {
		float sum = 0;
		for (int i = 0; i < 5; i++){
			sum = notas[i] + sum;
		}
		float prom = sum/5;
		return prom;
	}

int main(){
	float notas[5] = {};
	for (int i = 0; i < 5; i++){
		printf("Nota %d: ", i+1);
		scanf("%f", &notas[i]);
		printf("%2.2f pts.\n", notas[i]);
	}
	printf("%2.2f pts promedio.\n", PromClase(notas));
	return 0;
}