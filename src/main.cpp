#include <iostream>
#include <vector>
#include <algorithm>
#include <windows.h>
#include "Test.h"

using namespace std;
int main() {
    SetConsoleOutputCP(CP_UTF8);    //ustawianie polskich znaków

    srand(time(NULL));

    cout << "Autor: Kasper Radom 264023" <<endl<< endl;

    //inicjowanie pamięci na włączenie testów
    Test *test = new Test;

    //zwolnienie pamięci po obiekcie test i wywołanie jego destruktora
    delete test;
}







