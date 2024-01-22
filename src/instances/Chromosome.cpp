
#include "Chromosome.h"

void Chromosome::calcCost(int **matrix, int matrixSize) {
    cost = 0;
    for(int i = 0; i< matrixSize; i++){
        cost += matrix[path[i]][path[i+1]];
    }

}


