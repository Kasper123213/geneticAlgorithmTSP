//
// Created by radom on 19.01.2024.
//

#include "GeneticAlg.h"
#include "../time/Time.h"
#include <iostream>
#include <algorithm>

using namespace std;
GeneticAlg::GeneticAlg(int **matrix, int matrixSize, long maxTime, int populationSize, double mutationPoss,
                       double crossingPoss, int mutationMethod, int crossingMethod) {
    this->matrix = matrix;
    this->matrixSize = matrixSize;
    this->maxTime = maxTime;
    this->populationSize = populationSize;
    this->mutationPoss = mutationPoss;
    this->crossingPoss = crossingPoss;
    this->mutationMethod = mutationMethod;
    this->crossingMethod = crossingMethod;
}

//metoda rozpoczynający cały algorytm
void GeneticAlg::start(){
    Time* time = new Time();
    time->start();

    //tworzymy losową populacje
    generatePopulation(populationSize);
    //przyjmujemy pierwszego osobnika za najlepszego. potem zostanie to zweryfkowane i zapewne zmienione
    bestSolution = population[0];

    //Pętla główna programu
    do{
        pokazPopulacje();
        //znajdowanie najlepszego chromosomu i sprawdzenie czy jest najlepszym rozwiązaniem
        bestInIteration = findBestChromosome(population);
        //turniej
//        doTournament();

        //krzyżowanie
//        for(int i=0;i<populationSize-1;i+=2) {
//            double randomValue = uniform_real_distribution<double>(0.0, 1.0)(generator);
//            if(randomValue < crossingPoss) crossing(chromosome);
//        }


        //mutacje
        for(Chromosome &chromosome:population) {
            double randomValue = uniform_real_distribution<double>(0.0, 1.0)(generator);
            if(randomValue < mutationPoss) mutation(chromosome);
        }

    }while(time->getTime()<=maxTime);

    delete time;
}

//funkcja zwraca losową populację size-elementową
void GeneticAlg::generatePopulation(int size) {
    for(int i=0; i<size; i++) {
        Chromosome* chromosome = new Chromosome();

        for (int j = 0; j < matrixSize; j++) {
            chromosome->path.push_back(j);
        }
        chromosome->path.push_back(0);

        shuffle(chromosome->path.begin() + 1, chromosome->path.end() - 1, generator);

        chromosome->calcCost(matrix, matrixSize);

        population.push_back(*chromosome);
    }
}



Chromosome GeneticAlg::findBestChromosome(vector<Chromosome> chromosomes){
    Chromosome best = chromosomes[0];
    for(Chromosome chromosome:chromosomes){
        if(chromosome.cost<best.cost){
            best = chromosome;
        }
    }

    if(best.cost<bestSolution.cost){
        bestSolution = best;
    }

    return best;
}


void GeneticAlg::doTournament(){
    int tournamentSize = 2; //todo do ustalenia
    int chromosomeIndex;

    vector<Chromosome> newPopulation;
    vector<Chromosome> tournmentGroup;
    Chromosome chromosome;

    newPopulation.push_back(bestInIteration);

    //wypełniamy vektor newPopulation nowymi chromosomami
    for(int i=0; i<populationSize-1;i++){
        tournmentGroup.clear();
        //bierzemy określoną liczbę chromowomów. losujemy je ze zwracaniem. wybieramy najlepszych z wylosowanej puli
        for(int i=0; i<tournamentSize; i++) {
            chromosomeIndex = uniform_int_distribution<int>(0, int(population.size()) - 1)(generator);
            chromosome = population[chromosomeIndex];
            tournmentGroup.push_back(chromosome);
        }
        chromosome = findBestChromosome(tournmentGroup);
        newPopulation.push_back(chromosome);

    }

    population = newPopulation;

}

GeneticAlg::~GeneticAlg(){
}

void GeneticAlg::pokazPopulacje() {
    //todo usunąć
    cout<<"Populacja:"<<endl;
    for(auto ch:population){
        for(auto i:ch.path){
            cout<<i<<", ";
        }
//        ch.calcCost(matrix, matrixSize);
        cout<<" Koszt:"<<ch.cost<<endl;
    }
    cout<<endl<<endl;
    //todo usunąć
}



void GeneticAlg::crossing(Chromosome& chromosome) {
    switch(crossingMethod){
        case 1: //todo typy krzyżowania
            break;
    }
}


void GeneticAlg::mutation(Chromosome& chromosome){
    switch(mutationMethod){
        case 1://todo typy mutacji
            insertMutation(&chromosome);
            break;
    }
}

void GeneticAlg::insertMutation(Chromosome* chromosome) {
    int index1;
    int index2;

    do {
        index1 = uniform_int_distribution<int>(1, chromosome->path.size() - 2)(generator);
        index2 = uniform_int_distribution<int>(1, chromosome->path.size() - 2)(generator);
    }while(index1 == index2);

    int vertex = chromosome->path[index1];
    chromosome->path.erase(chromosome->path.begin()+index1);
    chromosome->path.insert(chromosome->path.begin()+index2, vertex);

    chromosome->calcCost(matrix, matrixSize);

}
