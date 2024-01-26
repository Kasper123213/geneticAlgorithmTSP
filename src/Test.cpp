#include <fstream>
#include <sstream>
#include "Test.h"
#include "algorithm/GeneticAlg.h"


Test::Test() {

    //zmienna przechowująca decyzje użytkownika o uruchomieniu testu automatycznegp
    char choice;

    choice = 'N';

    while (choice == 'n' or choice == 'N') {
        startTest();

        cout << "Czy zamknąć program T/N" <<endl;
        cout<<">>";
        cin >> choice;
        cout<<endl;


    }
}

//destruktor klasy Test służy do zwolnienia miejsca w pamięci zajmowanej przez macierz sąsiedstwa grafu
Test::~Test() {
    deleteMatrix();
}


void Test::showData(){
    if(fileName!="")cout<<"Nazwa Pliku: "<<fileName<<endl;
    if(populationSize!=0)cout<<"Wielkość populacji początkowej: "<<populationSize<<endl;
    if(mutationPoss!=0)cout<<"Prawdopodobieństwo mutacji: "<<mutationPoss<<endl;

    cout<<"Typ mutacji: ";
    if(mutationMethod==1)cout<<"Insert Mutation"<<endl;
    else cout<<"Exchange Mutation"<<endl;

    if(crossingPoss!=0)cout<<"Prawdopodobieństwo krzyżowania: "<<crossingPoss<<endl;

    cout<<"Metoda krzyżowania: PMX"<<endl;
    cout<<"Kryterium stopu: "<<maxTime/1000<<"s"<<endl;


    cout<<"_____________________________________________________________________________________"<<endl;
}

//uruchomienie testów algorytmu
void Test::startTest(){
    while (true){
        showData();

        cout << "Co chcesz zrobić?\n"
                "1.Wczytanie grafu z pliku i wyświetlenie wczytanych danych\n"
                "2. Wprowadzenie kryterium stopu\n"
                "3. Ustawienie wielkości populacji początkowej\n"
                "4. Ustawienie współczynnika mutacji\n"
                "5. Ustawienia współczynnika krzyżowania\n"
                "6. Wybór metody metody mutacji (opcjonalnie)\n"
                "7. Wybór metody metody krzyżowania (opcjonalnie)\n"
                "8. Uruchomianie algorytmu dla wczytanych danych i ustawionych parametrów i wyświetlenie wyników\n"
                "0. Wyjście\n"
                <<endl;
        //wybór czynności przez użytkownika
        int choice;
        cout<<">>";
        cin >> choice;
        cout<<endl;

        string path;

        int slashPoss;
        switch (choice) {
            case 1:
                cout<<"Podaj ścieżkę do pliku zawierającego reprezentację grafu:\n>>";
                cin>>path;

                if(readMatrix(path)) {
                    slashPoss = path.rfind('/');
                    if (slashPoss == string::npos)slashPoss = path.rfind('\\');
                    fileName = path.substr(slashPoss + 1);
                    showMatrix();
                }else{
                    cout<<"Nie można wczytać ścieżki"<<endl;
                }
                break;
            case 2:
                cout<<"Podaj kryterium stopu (czas w sekundach):\n>>";
                cin>>maxTime;
                maxTime*=1000;
                break;
            case 3:
                cout<<"Podaj wielkość populacji początkowej:\n>>";
                cin>>populationSize;
                break;
            case 4:
                cout<<"Podaj współczynnik mutacji:\n>>";
                cin>>mutationPoss;
                break;
            case 5:
                cout<<"Podaj współczynnik krzyżowania:\n>>";
                cin>>crossingPoss;
                break;
            case 6:
                cout<<"Wybierz metodę mutacji"<<endl;
                cout<<"\t1. Insert Mutation"<<endl
                <<"\t2. Exchange Mutation\n>>";
                cin>>mutationMethod;
                break;
            case 7:
                cout<<"Niestety, została zaimplementowana wyłącznie jedna metoda krzyżowania"<<endl;
                break;
            case 8:
                if(matrixSize<=0)cout<<"Nie ustawiono grafu"<<endl;
                else if(maxTime<=0)cout<<"Nie podano kryterium stopu"<<endl;
                else if(populationSize<=0)cout<<"Nie podano wielkości populacji"<<endl;
                else if(mutationMethod<=0)cout<<"Nie podano kryterium stopu"<<endl; //todo robic cos w stulu jesli to nie jest rowne 1,2 lub 3 to...
                else if(crossingMethod<=0)cout<<"Nie podano kryterium stopu"<<endl;//todo tu też
                else {
                    startAlgorithm();
                    //todo wypisywanie danych
                }
                break;
            default:
                return;
        }
    }
}

//Wyswietlanie wczytanych danych
void Test::showMatrix() {
    for(int i = 0;i<matrixSize;i++){
        for(int j = 0;j<matrixSize;j++){
            cout<<matrix[i][j]<<"\t";
        }
        cout<<endl;
    }
    cout<<"__________________________________________"<<endl;
    cout<<endl<<endl;
}


//usuwanie macierzy sąsiedstwa z pamięci
void  Test::deleteMatrix(){
    if(matrixSize!=0 ) {
        for (int i = 0; i < matrixSize; i++) {
            delete[] matrix[i];
        }
        matrixSize = 0;
    }
    delete[] matrix;
}

//rozpoczęcie algorytmu
void Test::startAlgorithm() {
    GeneticAlg* geneticAlg = new GeneticAlg(matrix, matrixSize, maxTime, populationSize, mutationPoss,
                                            crossingPoss,mutationMethod,crossingMethod);

    geneticAlg->start();

    cout<<"_____________________ROZWIĄZANIE_____________________"<<endl;
    cout<<"Najlepsze rozwiązanie: "<<endl;
    for(auto i:geneticAlg->bestSolution.path){
        cout<<i<<", ";
    }
    cout<<endl;
    cout<<"Jego długość: "<<geneticAlg->bestSolution.cost<<endl;
    cout<<"____________________________________________________"<<endl<<endl;

    delete geneticAlg;
}


//wczytanie macierzy sąsiedstwa z pliku
bool Test::readMatrix(string path){
    if(matrixSize!=0) deleteMatrix();

    int size{};
    // Otwórz plik
    ifstream file(path);

    // Sprawdź, czy udało się otworzyć plik
    if (!file.is_open()) {
        return false;
    }

    // Zmienna przechowująca wartość wczytaną z pliku
    string word;

    // Wyczyść flagi błędów i pozycję odczytu pliku
    file.clear();
    file.seekg(0, ios::beg);

    bool startReading = false;
    bool readSize = false;

    int x = 0;
    int y = 0;
    while (file >> word) {
        if (word.find("DIMENSION:") != string::npos) {
            readSize = true;
            continue;
        }
        if (readSize) {
            istringstream iss(word);
            iss >> size;
            matrix = new int *[size];
            for(int i=0; i<size;i++){
                matrix[i] = new int[size];
            }
            readSize = false;
            matrixSize = size;
        }

        if (word.find("EDGE_WEIGHT_SECTION") != string::npos) {
            startReading = true;
            continue;
        }
        if (startReading) {
            istringstream iss(word);
            float weight{};
            while (iss >> weight) {
                matrix[y][x] = weight;
                x++;
                if(x==size){
                    x=0;
                    y++;
                }
            }
        }

    }
    file.close();
    return true;
}
