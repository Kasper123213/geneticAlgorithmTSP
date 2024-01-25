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
    vector<Chromosome> generatePopulation(int size);
    Chromosome findBestChromosome(vector<Chromosome> chromosomes);
    vector<Chromosome> doTournament(vector<Chromosome> population);
    pair<Chromosome, Chromosome> crossing(Chromosome& parent1, Chromosome& parent2);
    void mutation(Chromosome& chromosome);
    void insertMutation(Chromosome* chromosome);
    void exchangeMutation(Chromosome* chromosome);

    GeneticAlg(int **matrix, int matrixSize, long maxTime, int populationSize, double mutationPoss,
               double crossingPoss, int mutationMethod, int crossingMethod);

    ~GeneticAlg();

    void pokazPopulacje();

    pair<Chromosome, Chromosome> pmxCrossover(Chromosome &chromosome, Chromosome &chromosome1);

    vector<Chromosome> succession(vector<Chromosome> vector1);

    void ratePopulation();

    bool compareChromosomes(const Chromosome &a, const Chromosome &b);
};


#endif //GENETICTSP_GENETICALG_H
