#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

struct DatosBienes{
    int codigo;
    char tipo; // T:terreno, C:comun, M:mueble
    double valor;
};

class Bienes{
    public:
    Bienes(int cod, char tipo, double valor) : codigo(cod), tipo(tipo), valor(valor) {}
    Bienes(DatosBienes datos) : codigo(datos.codigo), tipo(datos.tipo), valor(datos.valor) {}

    virtual void amortizacion() = 0;

    int getCodigo() { return codigo; }
    char getTipo() { return tipo; }
    double getValor() { return valor; }

    protected:
    int codigo;
    char tipo;
    double valor;
};

class Terrenos : public Bienes{
    public:
    Terrenos(int cod, char tipo, double valor) : Bienes(cod, tipo, valor) {}
    Terrenos(DatosBienes datos) : Bienes(datos) {}

    // Sin amortizacion
    void amortizacion() {}
};

class Comunes : public Bienes{
    public:
    Comunes(int cod, char tipo, double valor) : Bienes(cod, tipo, valor) {}
    Comunes(DatosBienes datos) : Bienes(datos) {}

    // 5% menos
    void amortizacion(){
        this->valor *= 0.95;
    }
};

class Muebles : public Bienes{
    public:
    Muebles(int cod, char tipo, double valor) : Bienes(cod, tipo, valor) {}
    Muebles(DatosBienes datos) : Bienes(datos) {}

    // O un 2% o un 2.2%
    void amortizacion(){
        if (this->valor < 10000) this->valor *= 0.98;
        else this->valor *= 0.978;
    }
};

class GestorBienes{
    public:
    GestorBienes(const string &archivo){
        ifstream in(archivo, ios::binary);
        if (!in.is_open()) return;

        DatosBienes dat;
        while(in.read((char*)&dat, sizeof(DatosBienes))){
            switch (dat.tipo){
                case 'T': bienes.push_back(new Terrenos(dat));
                break;
                case 'C': bienes.push_back(new Comunes(dat));
                break;
                case 'M': bienes.push_back(new Muebles(dat));
                break;
            }
        }
        in.close();
    }
    
    Bienes* getBien(int pos);
    void amortizarBienes(){
        for (auto &b : bienes) b->amortizacion();
    }

    bool guardarBienes(const string &archivo){
        ofstream out(archivo);
        if (!out.is_open()) return false;

        this->amortizarBienes();
        for (auto &b : bienes) out<< "Codigo: "<< b->getCodigo()<< " | valor: "<< b->getValor()<< endl;

        out.close();
        return true;
    }

    vector<Bienes*> getBienesMayorValor(){
        sort(bienes.begin(), bienes.end(), [](Bienes* a, Bienes* b){
            return (a->getValor()) > (b->getValor());
        });
        
        vector<Bienes*> bienesMayorValor;
        double mayorValor = bienes[0]->getValor();
        copy_if(bienes.begin(), bienes.end(), back_inserter(bienesMayorValor), [mayorValor](Bienes* b){
            return b->getValor() == mayorValor; 
        });
        return bienesMayorValor;
    }

    map<char, double> getTotalPorTipo(){
        map<char, double> totalBienes;
        totalBienes['T'] = 0.0;
        totalBienes['C'] = 0.0;
        totalBienes['M'] = 0.0;

        for (auto &b : bienes) totalBienes[b->getTipo()] += b->getValor();

        return totalBienes;
    }

    private:
    vector<Bienes*> bienes;
};

int main() {
    
    return 0;
}