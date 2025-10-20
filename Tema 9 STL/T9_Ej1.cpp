#include <iostream>
#include <algorithm>
#include <vector>
/*
Realice un programa que genere 200 números enteros, los imprima y luego ordene dicha 
lista sin tener en cuenta los 10 primeros, ni los últimos 50 y los vuelva a imprimir. Por 
ultimo ordene de forma descendiente e imprima la lista.
*/
using namespace std;

void generarEnteros(vector<int> &v, int cantidad);
void mostrarNumeros(vector<int> &v);
void ordenarNumeros(vector<int> &v);
void ordenarDesc(vector<int> &v);

int main() {
    srand(time(nullptr));

    vector<int> numeros;
    int cantidad;
    cout<< "Cantidad de numeros a generar: "; cin>> cantidad;
    generarEnteros(numeros, cantidad);

    cout<< "\n Lista Original \n";
    mostrarNumeros(numeros);

    ordenarNumeros(numeros);
    cout<< endl<< "\n Ordenado segun consigna \n";
    mostrarNumeros(numeros);

    ordenarDesc(numeros);
    cout<< endl<< "\n Reordenado segun consigna \n";
    mostrarNumeros(numeros);

    
    return 0;
}

void generarEnteros(vector<int> &v, int cantidad){
    v.resize(cantidad);
    for (int &n : v) n = rand() % 1000;
}

void mostrarNumeros(vector<int> &v){
    for (int n : v) cout<< n<< " ";
    cout<< "\n\n";
}

void ordenarNumeros(vector<int> &v){
    sort (v.begin() + 10, v.end() - 50);
}

void ordenarDesc(vector<int> &v){
    sort(v.begin(), v.end(), greater<int>());
}
