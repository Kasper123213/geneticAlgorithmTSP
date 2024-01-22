#ifndef GENETICTSP_TEST_H
#define GENETICTSP_TEST_H

#include<iostream>
#include <vector>
#include "time/Time.h"

using namespace std;

class Test {
public:
    string fileName="";
    long maxTime=0;
    double mutationPoss = 0;
    double crossingPoss = 0;
    int crossingMethod = 1;
    int mutationMethod = 1;
    int populationSize = 0;

    int matrixSize=0;
    int **matrix{};
    int bestSolutionLen = INT_MAX;
    vector<int> bestSolutionPath;

    void showData();
    void startTest();
    void deleteMatrix();
    void startAlgorithm();
    bool readMatrix(string path);
    void runAutoTests();
    void showMatrix();

    Test();
    ~Test();

};


#endif //GENETICTSP_TEST_H
