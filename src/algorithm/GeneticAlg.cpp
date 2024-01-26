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

    vector<Chromosome> parents;

    //tworzymy losową populacje
    population = generatePopulation(populationSize);
    //przyjmujemy pierwszego osobnika za najlepszego. potem zostanie to zweryfkowane i zapewne zmienione
    bestSolution = population[0];
    bestSolution.calcCost(matrix, matrixSize);
    daneDoWykresowCzas.push_back(time->getTime());
    daneDoWykresowBest.push_back(bestSolution.cost);

    //Pętla główna programu
    do{
        //ocena populacji
        ratePopulation();


        //znajdowanie najlepszego chromosomu i sprawdzenie czy jest najlepszym rozwiązaniem
        bestInIteration = findBestChromosome(population);

        if(bestInIteration.cost<bestSolution.cost){
            bestSolution = bestInIteration;
            daneDoWykresowCzas.push_back(time->getTime());
            daneDoWykresowBest.push_back(bestSolution.cost);
//            cout<<bestSolution.cost<<" Time: "<<time->getTime()<<endl;
        }



        //sukcesja
        population = succession(population);
//        pokazPopulacje();

        //turniej
        parents = doTournament(population);

//        krzyżowanie
        for(int i=0;i<parents.size()-1;i+=2) {
            double randomValue = uniform_real_distribution<double>(0.0, 1.0)(generator);
            if(randomValue < crossingPoss){
                pair<Chromosome, Chromosome> children = crossing(parents[i], parents[i+1]);
                population.push_back(children.first);
                population.push_back(children.second);
            }
        }



        //mutacje
        for(Chromosome &chromosome:population) {
            double randomValue = uniform_real_distribution<double>(0.0, 1.0)(generator);
            if(randomValue < mutationPoss){
                mutation(chromosome);
            }
        }

    }while(time->getTime()<=maxTime);

    delete time;
}

//funkcja zwraca losową populację size-elementową
vector<Chromosome> GeneticAlg::generatePopulation(int size) {
    vector<Chromosome> population;
    for(int i=0; i<size; i++) {
        Chromosome* chromosome = new Chromosome();
        //wypelniamy chromosom wierzchołkami grafu
        for (int j = 0; j < matrixSize; j++) {
            chromosome->path.push_back(j);
        }
        //dodajemy wierzchołek ostatni aby powstał cykl
        chromosome->path.push_back(0);
        //losowo mieszamy kolejność wierzchołków rozwiązania
        shuffle(chromosome->path.begin() + 1, chromosome->path.end() - 1, generator);

        //dodajemy rozwiązanie do populacji
        population.push_back(*chromosome);
    }
    return population;
}


//metoda znajduje w populacji chromosom o najmniejszym koszcie
Chromosome GeneticAlg::findBestChromosome(vector<Chromosome> chromosomes){
    Chromosome best = chromosomes[0];
    for(Chromosome chromosome:chromosomes){
        if(chromosome.cost<best.cost){
            best = chromosome;
        }
    }

    return best;
}

//metoda turniejowa
vector<Chromosome> GeneticAlg::doTournament(vector<Chromosome> population){
    int tournamentSize = int(matrixSize * 0.08);
    if(tournamentSize<2)tournamentSize=2;
    int winnersSize = population.size();
    int chromosomeIndex;

    vector<Chromosome> parents;
    vector<Chromosome> tournmentGroup;
    Chromosome chromosome;

    parents.push_back(bestInIteration);

    //wypełniamy vektor parents nowymi chromosomami
    for(int i=0; i<winnersSize;i++){
        tournmentGroup.clear();
        //bierzemy określoną liczbę chromowomów. losujemy je ze zwracaniem. wybieramy najlepszych z wylosowanej puli
        for(int i=0; i<tournamentSize; i++) {
            chromosomeIndex = uniform_int_distribution<int>(0, int(population.size()) - 1)(generator);
            chromosome = population[chromosomeIndex];
            tournmentGroup.push_back(chromosome);
        }
        chromosome = findBestChromosome(tournmentGroup);
        parents.push_back(chromosome);

    }

    return parents;

}

GeneticAlg::~GeneticAlg(){
}



//wybór typu krzyżowania
pair<Chromosome, Chromosome> GeneticAlg::crossing(Chromosome& parent1, Chromosome& parent2) {
    switch(crossingMethod){
        case 1: //todo typy krzyżowania
            return pmxCrossover(parent1, parent2);
    }
}

//wybór typu mutacji
void GeneticAlg::mutation(Chromosome& chromosome){
    switch(mutationMethod){
        case 1://todo typy mutacji
            insertMutation(&chromosome);
            break;
        case 2:
            exchangeMutation(&chromosome);
    }
}

//mutacja typu insert
void GeneticAlg::insertMutation(Chromosome* chromosome) {
    int index1;
    int index2;
    //losowanie punkców do mutacji
    index1 = uniform_int_distribution<int>(1, chromosome->path.size() - 2)(generator);
    do {
        index2 = uniform_int_distribution<int>(1, chromosome->path.size() - 2)(generator);
    }while(index1 == index2);

    //zapamietanie usuwanego wierzchołka
    int vertex = chromosome->path[index1];
    //usuwanie wierzchołka
    chromosome->path.erase(chromosome->path.begin()+index1);
    //dodawanie wierzchołka na wybrany indeks
    chromosome->path.insert(chromosome->path.begin()+index2, vertex);


}



//mutacja typu exchange
void GeneticAlg::exchangeMutation(Chromosome* chromosome) {
    int index1;
    int index2;

    index1 = uniform_int_distribution<int>(1, chromosome->path.size() - 2)(generator);
    do {
        index2 = uniform_int_distribution<int>(1, chromosome->path.size() - 2)(generator);
    }while(index1 == index2);

    swap(chromosome->path[index1],chromosome->path[index2]);


}

pair<Chromosome, Chromosome> GeneticAlg::pmxCrossover(Chromosome &parent1, Chromosome &parent2) {
    Chromosome child1;
    Chromosome child2;
    int index1;
    int index2;

    do {
        index1 = uniform_int_distribution<int>(1, parent1.path.size() - 2)(generator);
        index2 = uniform_int_distribution<int>(1, parent1.path.size() - 2)(generator);
    }while(index1 >= index2);

    //przypisywanie fragmentu chromosomu ktory nie zostanie zmieniony
    vector<int> fromFirst;
    for(int i = index1; i<=index2;i++){
        fromFirst.push_back(parent1.path[i]);
    }

    vector<int> fromSecond;
    for(int i = index1; i<=index2;i++){
        fromSecond.push_back(parent2.path[i]);
    }


    int value;
    for(int i=0; i<parent1.path.size()-1; i++){
        //dodawanie sekcji dopasowań do potomków
        if(index2>=i and i>=index1){
            child1.path.push_back(fromSecond[i-index1]);
            child2.path.push_back(fromFirst[i-index1]);
            continue;
        }


        //pierwszy potomek
        value = parent1.path[i];
        //dopuki wartosc jest na sekcji dopasowań szukamy nowej wartości
        while(find(fromSecond.begin(), fromSecond.end(), value) != fromSecond.end()){
            //szukamy indeksu na liscie dopasowań
            auto iterator = find(fromSecond.begin(), fromSecond.end(),value);
            int matchListIndex = distance(fromSecond.begin() , iterator);
            value = fromFirst[matchListIndex];
        }
        child1.path.push_back(value);


        //drugi potomek
        value = parent2.path[i];
        //dopuki wartosc jest na sekcji dopasowań szukamy nowej wartości
        while(find(fromFirst.begin(), fromFirst.end(), value) != fromFirst.end()){
            //szukamy indeksu na liscie dopasowań
            auto iterator = find(fromFirst.begin(), fromFirst.end(),value);
            int matchListIndex = distance(fromFirst.begin() , iterator);
            value = fromSecond[matchListIndex];
        }
        child2.path.push_back(value);

    }

    child1.path.push_back(0);
    child2.path.push_back(0);


    return make_pair(child1, child2);
}

vector<Chromosome> GeneticAlg::succession(vector<Chromosome> population) {//sukcesja elitarna
    vector<Chromosome> newPopulation;

    //sortowanie wektora
    std::sort(population.begin(), population.end(),
              [](const Chromosome& a, const Chromosome& b) {
                  return a.cost < b.cost;
              });

    newPopulation.assign(population.begin(), population.begin() + populationSize);

    return newPopulation;
}

//wyliczanie kosztu kazdego rozwiązania
void GeneticAlg::ratePopulation() {
    for(Chromosome & chromosome : population){
        chromosome.calcCost(matrix, matrixSize);
    }
}
