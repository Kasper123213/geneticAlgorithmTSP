
#ifndef GENETICTSP_CHROMOSOME_H
#define GENETICTSP_CHROMOSOME_H

#include <vector>

using namespace std;
class Chromosome {
public:
    vector<int> path;
    int cost;

    void calcCost(int **matrix, int matrixSize);
};


#endif //GENETICTSP_CHROMOSOME_H
