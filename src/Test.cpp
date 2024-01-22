#include <fstream>
#include <sstream>
#include "Test.h"
#include "algorithm/GeneticAlg.h"
//#include "algorithms/Sa.h"
//#include "algorithms/Ts.h"

//konstruktor klasy
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
    //todo dodac metody mutacji
    if(crossingPoss!=0)cout<<"Prawdopodobieństwo krzyżowania: "<<crossingPoss<<endl;
    //todo dodac metody krzyzowania
    cout<<"Kryterium stopu: "<<maxTime<<endl;


    cout<<"_____________________________________________________________________________________"<<endl;
}

//uruchomienie testów algorytmu
void Test::startTest(){
    while (true){
        //todo usunąć
        readMatrix(R"(C:\Users\radom\OneDrive\Pulpit\PEA\geneticTSP\z2_z3\ATSP\tsp_6_1.txt)");
//        readMatrix(R"(C:\Users\radom\OneDrive\Pulpit\PEA\geneticTSP\z2_z3\ATSP\br17.atsp)");
        maxTime = 5;
        populationSize = 3;
        mutationPoss = 0.1;
        crossingPoss = 0.3;
        mutationMethod = 1;
        crossingPoss = 1;
        startAlgorithm();
        return;
        //todo

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
                    showData();
                }else{
                    cout<<"Nie można wczytać ścieżki"<<endl;
                }
                break;
            case 2:
                cout<<"Podaj kryterium stopu (czas w sekundach):\n>>";
                cin>>maxTime;
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
                cout<<"Wybierz metodę mutacji\n>>"; //todo wypisywanie metod mutacji
                cin>>mutationMethod;
                break;
            case 7:
                cout<<"Wybierz metodę krzyżowania\n>>"; //todo wypisywanie metod krzyżowania
                cin>>crossingMethod;
                break;
            case 8:
                if(maxTime<=0)cout<<"Nie podano kryterium stopu"<<endl;
                if(populationSize<=0)cout<<"Nie podano wielkości populacji"<<endl;
                if(mutationMethod<=0)cout<<"Nie podano kryterium stopu"<<endl; //todo robic cos w stulu jesli to nie jest rowne 1,2 lub 3 to...
                if(crossingMethod<=0)cout<<"Nie podano kryterium stopu"<<endl;//todo tu też

                startAlgorithm();
                //todo wypisywanie danych
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
        cout<<"__________________________________________"<<endl;
    }
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



void Test::runAutoTests(){
//    readMatrix("C:\\Users\\radom\\OneDrive\\Pulpit\\PEA_Z2\\z2_z3\\ATSP\\ftv55.atsp");
//    cout<<"odpalam ftv55, cooling type = 1"<<endl;
//    for(int i=0;i<10;i++) {
//        cout<<i<<endl;
//
//        string nazwa = "SA_ftv55_1_" + to_string(i) + ".csv";
//        ofstream excelFile(nazwa);
//
//        Sa* simAnneling = new Sa(matrix, matrixSize, 0.9989, 2*60, 1);
//        simAnneling->start();
//
//        for(int x:simAnneling->wykresBesty){
//            excelFile<<x<<";";
//        }
//        excelFile<<endl;
//        for(int x:simAnneling->wykresCzasy){
//            excelFile<<x<<";";
//        }
//        excelFile<<endl;
//        for(int x:simAnneling->bestPath){
//            excelFile<<x<<";";
//        }
//
//        excelFile.close();
//
//        delete simAnneling;
//    }
}