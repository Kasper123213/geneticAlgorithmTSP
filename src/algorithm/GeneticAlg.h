//
// Created by radom on 19.01.2024.
//

#ifndef GENETICTSP_GENETICALG_H
#define GENETICTSP_GENETICALG_H

#include <vector>
#include <random>
#include "../instances/Chromosome.h"

using namespace std;

class GeneticAlg {
public:
    int **matrix;
    int matrixSize;
    long maxTime;
    int populationSize;
    double mutationPoss;
    double crossingPoss;
    int mutationMethod;
    int crossingMethod;

    mt19937 generator{random_device{}()};

    vector<Chromosome> population ={};
    Chromosome bestInIteration;
    Chromosome bestSolution;

    void start();
    void generatePopulation(int size);
    Chromosome findBestChromosome(vector<Chromosome> chromosomes);
    void doTournament();
    void crossing(Chromosome& chromosome);
    void mutation(Chromosome& chromosome);
    void insertMutation(Chromosome* chromosome);

    GeneticAlg(int **matrix, int matrixSize, long maxTime, int populationSize, double mutationPoss,
               double crossingPoss, int mutationMethod, int crossingMethod);

    ~GeneticAlg();

    void pokazPopulacje();
};


#endif //GENETICTSP_GENETICALG_H
