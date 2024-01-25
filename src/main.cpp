#include <iostream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include "Test.h"
#include "algorithm/GeneticAlg.h"

using namespace std;
int main() {
    SetConsoleOutputCP(CP_UTF8);    //ustawianie polskich znaków

    srand(time(NULL));

    cout << "Autor: Kasper Radom 264023" <<endl<< endl;

    //inicjowanie pamięci na włączenie testów
    Test *test = new Test;

    //zwolnienie pamięci po obiekcie test i wywołanie jego destruktora
    delete test;

//    vector<int> x;
//    x.push_back(1);
//    x.push_back(2);
//    x.push_back(3);
//
//    auto d = std::find(x.begin(), x.end(),2);
//    cout<<distance(x.begin() , d);
}







