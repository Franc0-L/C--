#include <iostream>
#include <fstream>
#include <vector>

int main() {
    std::vector<int> nums(5);
    std::cout << "Ingrese 5 numeros enteros:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "Numero " << i+1 << ": ";
        std::cin >> nums[i];
    }

    // Guardar en archivo (texto)
    std::ofstream out("numeros.txt");
    if (!out) {
        std::cerr << "Error al abrir numeros.txt para escribir\n";
        return 1;
    }
    for (int n : nums) out << n << '\n';
    out.close();

    // Leer y sumar
    std::ifstream in("numeros.txt");
    if (!in) {
        std::cerr << "Error al abrir numeros.txt para leer\n";
        return 1;
    }
    int suma = 0, val;
    while (in >> val) suma += val;
    in.close();

    std::cout << "La suma de los 5 numeros es: " << suma << '\n';
    return 0;
}